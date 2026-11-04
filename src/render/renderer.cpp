module;
#include <cassert>
#include <thread>
#include <vector>
#include <MiniFB.h>
#include <glm/glm.hpp>
#include <glm/ext.hpp>

module renderer;
import std;
import utils;
import scene;
import scene_loader;
import light;


/**
*	@brief 构造函数，用于初始化一个渲染器对象
*	@param width 屏幕的宽度
*	@param height 屏幕的高度
*	@param sceneFilepath 场景XML文件的路径
*/
Renderer::Renderer(int width, int height, const char* sceneFilepath) :
    myViewPortWidth(width),
    myViewPortHeight(height)
{
    // 创建场景物体
    SceneLoader loader;
    myScene = loader.loadSceneFromXML(sceneFilepath, width, height);
}


void Renderer::run()
{
    // 创建视口对象和内存缓冲区
    struct mfb_window* window = mfb_open_ex("Hello Render", myViewPortWidth, myViewPortHeight, MFB_WF_RESIZABLE);
    if (window == NULL)
        return;
    myBuffer = (uint32_t*)malloc(myViewPortWidth * myViewPortHeight * 4);

    // 渲染视口，采用多线程进行渲染
    auto numThreads = std::thread::hardware_concurrency();                  // 获取硬件（CPU）所支持的最大线程数
    std::vector<std::thread> renderThreads(numThreads);                     // 创建长度为 numThreads 的列表，用于创建与管理子线程
    for (int i = 0; i < numThreads; i++)
    {
        renderThreads[i] = std::thread(&Renderer::runRenderThread, this);   // 创建子线程对象（入口函数 + 调用对象）
    }

    // 展示主线程的视口内容
    do 
    {
        mfb_update_state state = mfb_update_ex(window, myBuffer, myViewPortWidth, myViewPortHeight);
        if (state != MFB_STATE_OK)
            break;
    } while (mfb_wait_sync(window));


    // 等待所有渲染子线程结束
    for (auto& thread : renderThreads)
    {
        thread.join();
    }

    // 回收进程（主线程）的视口资源
    free(myBuffer);
    myBuffer = NULL;
    window = NULL;
}


Color Renderer::renderPixel(int x, int y)
{   
    static const int N_SSAA = 100;    // 超级采样 Super-Sampling Anti-Aliasing 算法
    Color finalColor {0,0,0};

    for (int i = 0; i < N_SSAA; i++)
    {
        float px = x + glm::linearRand(0.0f, 1.0f);
        float py = y + glm::linearRand(0.0f, 1.0f);

        finalColor += this->renderSubPixel(px, py) / float(N_SSAA);
    }

    return finalColor;
}


/**
*   如果当前点所在光线与物体相交，则贡献渲染，否则不贡献渲染.
*   Step1. 通过渲染器的摄像机对场景进行相交性测试
*/
Color Renderer::renderSubPixel(float px, float py)
{
    // 定义屏幕该点光线方向（世界坐标）与相交信息
    auto ray = myScene->getCamera().getRay(px, py);
    Color subColor = getIrradiance(ray);
    
    return subColor;
}


/**
* @brief 计算光照照度
*/
Color Renderer::getIrradiance(Ray& ray)
{
    /// 相交性判断1: 相机光线与场景物体是否相交（决定是否渲染）
    Intersection intersect{};
    if (!myScene->intersect(ray, intersect))
        return Vector3f{ 0.0f, 0.0f, 0.0f };

    Color Irradiance{ 0.0f, 0.0f, 0.0f };
    for (auto& pLight : myScene->getLights())
    {
        /// 相交性判断2: 光源光线与场景物体是否相交（决定渲染亮度）
        Vector3f sourcePosition{};
        Color radiance = pLight->getRadiance(intersect.position, sourcePosition);

        Ray shadowRay{};
        shadowRay.o = intersect.position;
        shadowRay.d = glm::normalize(sourcePosition - intersect.position);
        shadowRay.minT = 1e-4f;                                             ///< 误差冗余，避免交点与自己所在的平面相交上，从而不进行光照渲染.
        shadowRay.maxT = glm::length(intersect.position - sourcePosition);

        Intersection intersect_shadow{};
        if (myScene->intersect(shadowRay, intersect_shadow))
            continue;
        else
        {
            Vector3f lightDirection = glm::normalize(sourcePosition - intersect.position);
            float cosTheta = glm::dot(intersect.normal, lightDirection);
            Irradiance += radiance * glm::max(cosTheta, 0.0f);              ///< 如果光线与物体外半球相交，就对光照有贡献，否则无贡献.
        }
    }
    return Irradiance;
}


void Renderer::runRenderThread()
/*  运行渲染线程
*   作为渲染线程的入口函数，开辟子线程用于渲染工作，和主线程共享缓冲区。
*/
{
    // 读取当前缓冲区的最新坐标
    while (true)
    {
        int pixelIndex = myCurrentPixelIndex.fetch_add(1);
        // 如果渲染工作已经完成，则结束渲染；否则，获取当前的像素坐标，进行后续渲染。
        if (pixelIndex >= myViewPortWidth * myViewPortHeight)
            break;
        int x = pixelIndex % myViewPortWidth;
        int y = pixelIndex / myViewPortWidth;

        // 给定像素坐标，计算颜色值
        auto color = renderPixel(x, y);
        uint32_t r = glm::clamp((uint32_t)std::round(color.r * 255.0f), 0u, 255u);
        uint32_t g = glm::clamp((uint32_t)std::round(color.g * 255.0f), 0u, 255u);
        uint32_t b = glm::clamp((uint32_t)std::round(color.b * 255.0f), 0u, 255u);

        // 给定颜色值，填充缓冲区
        assert(myBuffer != NULL);
        myBuffer[y * myViewPortWidth + x] = (r << 16) | (g << 8) | (b);
    }
}
