/* 渲染器主程序 */

import std;
import renderer;


int main(int argc, char* argv[]) {
    //const char* sceneFilepath = argc > 1 ? argv[1] : RENDERER_DEFAULT_SCENE_PATH;

    Renderer renderer{1920, 1080, "../scenes/scene04.xml"};
    renderer.run();

    return 0;
}
