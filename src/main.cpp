/* 渲染器主程序 */

import std;
import renderer;


int main(int argc, char* argv[]) {
    try
    {
        const char* sceneFilepath = argc > 1 ? argv[1] : RENDERER_DEFAULT_SCENE_PATH;
        Renderer renderer{1920, 1080, "../scenes/scene02.xml"};
        renderer.run();
        return 0;
    }
    catch (const std::exception& error)
    {
        std::cerr << error.what() << '\n';
        return 1;
    }
}
