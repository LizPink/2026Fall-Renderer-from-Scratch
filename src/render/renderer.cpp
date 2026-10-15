module;
#include <cassert>
#include <thread>
#include <vector>
#include <MiniFB.h>
#include <glm/glm.hpp>
module renderer;

import utils;

Renderer::Renderer(int width, int height):

	myViewPortWidth(width),
	myViewPortHeight(height)
{
    myCamera.initialize(
        Vector3f(0.0f, 0.0f, 5.0f), 
        Vector3f(0.0f, 0.0f, 0.0f),
        Vector3f(0.0f, 1.0f, 0.0f),
        glm::radians(45.0f),
        0.1f, 1000.0f,
        width, height
    );
}


void Renderer::run()
/*  让渲染器对象执行渲染操作 */
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
        renderThreads[i].detach();                                          // 分离主线程与子线程
    }

    // Presenting the ViewPort
    do 
    {
        mfb_update_state state = mfb_update_ex(window, myBuffer, myViewPortWidth, myViewPortHeight);
        if (state != MFB_STATE_OK)
            break;
    } while (mfb_wait_sync(window));

    // Recycling the ViewPort
    free(myBuffer);
    myBuffer = NULL;
    window = NULL;
}


Color Renderer::renderPixel(int x, int y)
{   
    // 生成光线
    auto ray = myCamera.getRay(x, y);

    auto r = (float)x / myViewPortWidth;
    auto g = (float)y / myViewPortHeight;
	return Color(r, g, 0.0f);
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
