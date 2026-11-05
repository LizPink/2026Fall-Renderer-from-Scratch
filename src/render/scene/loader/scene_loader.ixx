export module scene_loader;

import std;
import scene;

// 加载器只在读取文件期间存在，加载后的场景由调用者独立管理。
export class SceneLoader
{
public:
    // XML 中的 Fov、Rotation 和聚光灯锥角使用度，加载时转为弧度。
    // 按示例格式读取 Camera、Materials、Objects 和可选的 Lights。
    // 材质使用 <Material><Name>名称</Name><Type>Lambert</Type><Albedo>r, g, b</Albedo></Material>。
    // 每个 Object 必须通过 <Material>名称</Material> 引用 Materials 中已定义的材质。
    std::unique_ptr<Scene> loadSceneFromXML(const char* filepath, int width, int height) const;
};
