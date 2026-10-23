export module scene_loader;

import std;
import scene;

// 加载器只在读取文件期间存在，加载后的场景由调用者独立管理。
export class SceneLoader
{
public:
    // XML 中的 Fov 和 Rotation 使用角度；加载失败时抛出 std::runtime_error。
    std::unique_ptr<Scene> loadSceneFromXML(const char* filepath, int width, int height) const;
};
