module;
#include <cassert>
#include <thread>
#include <MiniFB.h>
#include <glm/glm.hpp>
module renderer;

Renderer::Renderer(int width, int height):
/*  构造函数，用于初始化一个渲染器对象 */
	myViewPortWidth(width),
	myViewPortHeight(height)
{}


void Renderer::run()
/*  让渲染器对象执行渲染操作 */
{
    // Creating Window and RenderBuffer
    struct mfb_window* window = mfb_open_ex("Hello Render", myViewPortWidth, myViewPortHeight, MFB_WF_RESIZABLE);
    if (window == NULL)
        return;
    myBuffer = (uint32_t*)malloc(myViewPortWidth * myViewPortHeight * 4);

    // Rendering the ViewPort
    // 采用子线程进行渲染工作
    std::thread renderThread{&Renderer::runRenderThread, this}; // 创建子线程对象（入口函数 + 调用对象）
    renderThread.detach();  // 分离主线程与子线程

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
    auto r = (float)x / myViewPortWidth;
    auto g = (float)y / myViewPortHeight;
    std::this_thread::sleep_for(std::chrono::milliseconds(1));
	return Color(r, g, 0.0f);
}


void Renderer::runRenderThread()
/*  运行渲染线程
*   作为渲染线程的入口函数，开辟一个子线程用于渲染工作，和主线程共享缓冲区，主线程只读取缓冲区中的数据，然后更新到视口中。
*/
{
    for (int y = 0; y < myViewPortHeight; y++)
    {
        for (int x = 0; x < myViewPortWidth; x++)
        {
            // 计算像素
            auto color = renderPixel(x, y);
            uint32_t r = glm::clamp((uint32_t)std::round(color.r * 255.0f), 0u, 255u);
            uint32_t g = glm::clamp((uint32_t)std::round(color.g * 255.0f), 0u, 255u);
            uint32_t b = glm::clamp((uint32_t)std::round(color.b * 255.0f), 0u, 255u);

            // 填充像素缓冲区
            assert(myBuffer != NULL);
            myBuffer[y * myViewPortWidth + x] = (r << 16) | (g << 8) | (b);
        }
    }
}
