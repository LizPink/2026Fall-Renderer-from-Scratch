module;
#include <cassert>
#include <thread>
#include <vector>
#include <MiniFB.h>
#include <glm/glm.hpp>
#include <glm/ext.hpp>

module renderer;
import utils;
import scene_loader;


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
    // 加载器场景物体（所有权归Renderer所有）
    SceneLoader loader;
    myScene = loader.loadSceneFromXML(sceneFilepath, width, height);
}


void Renderer::run()
{
    // Creating Window and RenderBuffer
    struct mfb_window* window = mfb_open_ex("Hello Render", myViewPortWidth, myViewPortHeight, MFB_WF_RESIZABLE);
    if (window == NULL)
        return;
    myBuffer = (uint32_t*)malloc(myViewPortWidth * myViewPortHeight * 4);

    // Rendering the ViewPort
    // 采用多个子线程进行渲染工作
    auto numThreads = std::thread::hardware_concurrency();                  // 获取硬件（CPU）所支持的最大线程数
    std::vector<std::thread> renderThreads(numThreads);                     // 创建一个列表，用于创建与管理子线程
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
    auto ray = this->myScene->getCamera().getRay(px, py);
    Intersection intersect{}; Color subColor{0,0,0};

    if (this->myScene->intersect(ray, intersect))
        subColor = intersect.normal * 0.5f + 0.5f;
    
    return subColor;
}
