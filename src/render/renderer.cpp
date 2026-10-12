module;
#include <cassert>
#include <thread>
#include <MiniFB.h>
#include <glm/glm.hpp>
module renderer;

Renderer::Renderer(int width, int height):
	myViewPortWidth(width),
	myViewPortHeight(height)
{}


void Renderer::run()
/* 让渲染器对象执行渲染操作 */
{
    struct mfb_window* window = mfb_open_ex("Hello Render", myViewPortWidth, myViewPortHeight, MFB_WF_RESIZABLE);
    if (window == NULL)
        return;
    uint32_t* buffer = (uint32_t*)malloc(myViewPortWidth * myViewPortHeight * 4);

    // Rendering the ViewPort
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
            assert(buffer != NULL);
            buffer[y * myViewPortWidth + x] = (r << 16) | (g << 8) | (b);
        }
    }

    // Presenting the ViewPort
    do 
    {
        mfb_update_state state = mfb_update_ex(window, buffer, myViewPortWidth, myViewPortHeight);
        if (state != MFB_STATE_OK)
            break;
    } while (mfb_wait_sync(window));

    // Recycling the ViewPort
    free(buffer);
    buffer = NULL;
    window = NULL;
}


Color Renderer::renderPixel(int x, int y)
{   
    auto r = (float)x / myViewPortWidth;
    auto g = (float)y / myViewPortHeight;
	return Color(r, g, 0.0f);
}