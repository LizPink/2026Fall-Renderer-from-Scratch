module;
#include <MiniFB.h>
module renderer;

void Renderer::run()
/* 让渲染器对象执行渲染操作 */
{
    struct mfb_window* window = mfb_open_ex("Hello Render", 800, 600, MFB_WF_RESIZABLE);
    if (window == NULL)
        return;

    uint32_t* buffer = (uint32_t*)malloc(800 * 600 * 4);
    mfb_update_state state;
    do {
        // TODO: add some fancy rendering to the buffer


        state = mfb_update_ex(window, buffer, 800, 600);
        if (state != MFB_STATE_OK)
            break;

    } while (mfb_wait_sync(window));

    free(buffer);
    buffer = NULL;
    window = NULL;
}