export module scene_loader;

import std;
import scene;

// 加载器只在读取文件期间存在，加载后的场景由调用者独立管理。
export class SceneLoader
{
public:
    // XML 中的 Fov、Rotation 和聚光灯锥角使用度，加载时转为弧度。
    // 按示例格式读取 Camera、SceneObjects 和可选的 Lights。
    std::unique_ptr<Scene> loadSceneFromXML(const char* filepath, int width, int height) const;
};
