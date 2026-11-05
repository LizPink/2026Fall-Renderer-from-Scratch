# Render from Scratch

这是一个从零开始实现C++渲染器项目。主要使用 C++23 模块组织项目文件，通过 XML 加载场景，在CPU上计算光线与图元的交点，用MiniFB显示渲染结果。

## 项目结构

```text
├── CMakeLists.txt          # 构建配置
├── scenes/
│   ├── scene01.xml         # 两个三角形组成的正方形
│   ├── scene02.xml         # 犹他茶壶
│   └── scene03.xml         # 康奈尔盒子
├── src/
│   ├── main.cpp           # 程序入口
│   ├── render/            # 加载场景、计算像素并显示
│   ├── scene/
│   │   ├── scene    	   # 管理相机、场景物体和光源
│   │   ├── loader/        # 读取XML并创建Scene对象
│   │   ├── camera/        # 相机和光线
│   │   ├── light/         # 平行光、点光源和聚光灯
│   │   └── object/        # 场景物体和图元
│   └── utils/             # 常用数学工具
├── lib/
│   ├── minifb/            # 窗口与像素缓冲区显示
│   ├── glm/               # 向量、矩阵计算
│   └── tinyxml2/          # 场景XML解析
├── build/                 # 当前构建的生成文件和编译产物，不提交Git
└── archive/               # 本地历史验证资料与场景备份
```

## 构建目录

只使用项目根目录下的 `build/` 作为当前构建目录，保留 Debug、Release 两种配置。由 CMake 管理其中的文件位置，不手动移动生成的工程或中间文件。

| 路径 | 内容 |
| --- | --- |
| `build/Render_from_Scratch.slnx` | Visual Studio 解决方案入口 |
| `build/*.vcxproj`、`*.filters` | 渲染器、构建检查和安装目标的生成工程 |
| `build/CMakeCache.txt`、`CMakeFiles/` | 当前构建配置、检测结果和生成记录 |
| `build/Debug/`、`Release/` | 对应配置的程序和调试信息 |
| `build/Render_from_Scratch.dir/`、其他 `*.dir/`、`x64/` | 编译中间文件与构建记录 |
| `build/lib/minifb/`、`build/lib/tinyxml2/` | 第三方库的生成工程、静态库和中间文件 |

根目录的 `lib/` 是第三方源码，需要保留；`build/lib/` 是构建产物。当前关闭第三方库的示例和测试目标。

2026-10-29 清理时，历史验证源码、脚本、场景备份和预览图已移至 `archive/build-cleanup-2026-10-29/`，其中的说明和 `cleanup-manifest.json` 记录归档内容。该本地归档不参与当前构建，也不提交 Git。`build/` 此后只存放当前构建的生成文件；Visual Studio 打开工程时可能重新创建 `.vs/` 缓存。

## 场景与渲染

| 类型 | 职责 |
| --- | --- |
| `Scene` | 保存相机，管理物体和光源，查找光线最近命中的物体 |
| `Object` | 保存物体变换，管理物体内的多个图元 |
| `Primitive` | 定义图元的求交接口，由 `Sphere`、`Circle`、`Triangle` 实现 |
| `Camera` | 根据位置、朝向和投影参数，将屏幕采样点转换为世界空间光线 |
| `Light` | 定义光源接口，由 `DirectionalLight`、`PointLight`、`SpotLight` 实现 |
| `SceneLoader` | 解析 XML，创建相机、物体、图元和光源，返回 `std::unique_ptr<Scene>` |
| `Renderer` | 持有场景，计算像素颜色，并通过 MiniFB 显示缓冲区 |

场景由 Renderer 持有；Scene 释放其管理的物体和光源，Object 释放其管理的图元。加载器遇到文件读取失败、缺少必填节点或无法读取数值时抛出 `std::runtime_error`。

加载流程按 `loadCamera`、`loadObjects`、`loadLights` 分开，物体中的图元交给 `loadPrimitive`。这是教学演示项目，XML 按示例填写；参数范围和几何关系由场景作者保证。

当前主程序使用 1920×1080 分辨率。工作线程通过原子索引领取像素，主线程刷新窗口；每个像素在像素范围内随机采样 100 次并取平均，实现 SSAA 抗锯齿。

## XML 场景

| 文件 | 内容 |
| --- | --- |
| `scenes/scene01.xml` | 一个物体、两个三角形组成的正方形，用于检查相机、变换和三角形求交 |
| `scenes/scene02.xml` | 完整犹他茶壶，包含壶身、壶底、壶盖、壶钮、壶嘴和把手；曲面离散为 248 个三角形，文件内有参数和部件注释 |
| `scenes/scene03.xml` | 两个球，以及由 10 个三角形组成的地面、天花板和三面墙 |

XML 以 `Scene` 为根，包含 `Camera`、`Objects` 和材质定义 `Materials`、可选的 `Lights`。每个 `Object` 必须包含 `Material` 和 `Primitives`，`Transform` 可省略；一个物体可以包含多个图元。物体格式参考 `scene01.xml`，材质和光源字段见下文。

| 元素 | 格式与含义 |
| --- | --- |
| `Camera/Position`、`Target`、`Up` | `x, y, z`：相机位置、观察目标和上方向；示例使用 Y 轴向上 |
| `Camera/NearZ`、`FarZ` | 投影矩阵的近、远平面参数，满足 `0 < NearZ < FarZ` |
| `Camera/Fov` | 视野角，单位为度，范围为 `0 < Fov < 180` |
| `Transform/Position` | 物体平移，格式为 `x, y, z` |
| `Transform/Rotation` | 绕 X、Y、Z 轴的旋转角，格式为 `x, y, z`，单位为度 |
| `Transform/Scale` | 一个正数，表示统一缩放比例 |
| `Sphere/Radius` | 球半径，必须为正数 |
| `Circle/Radius` | 圆盘半径，必须为正数；两种标签均创建 `Circle` |
| `Triangle/Vertex` | 三个局部坐标顶点，每个顶点为 `x, y, z`，顶点不得重合或共线 |

球和圆盘以物体局部原点为中心，圆盘位于局部 XY 平面；位置和朝向通过 `Transform` 设置。三角形的顶点顺序决定法线方向。

省略 `Transform` 时，位置和旋转为零、缩放为 1；其中的字段也可以单独省略。`Radius` 默认 1，`NearZ`、`FarZ`、`Fov` 默认分别为 0.1、1000、45。

`Materials` 中使用 `Material` 节点定义材质，`Name` 指定名称，`Type` 指定类型。目前支持 `Lambert`（对应 `LambertMaterial`），必填的 `Albedo` 按 `r, g, b` 填写，示例见 `scene05.xml`：

```xml
<Materials>
    <Material>
        <Name>red</Name>
        <Type>Lambert</Type>
        <Albedo>0.8, 0.1, 0.1</Albedo>
    </Material>
</Materials>
```

`Name`、`Type` 必填且不能为空，读取时会去除两端空白。材质节点必须为 `Material`，`Type` 目前只接受 `Lambert`。

每个 `Object` 都必须添加 `<Material>red</Material>` 这样的材质引用，与 `Transform`、`Primitives` 同级。多个物体可以引用同一材质；材质定义的名称必须非空且唯一，引用的材质必须存在。加载器先读取全部材质，再绑定物体，因此 `Materials` 在 XML 中的先后位置不影响引用。

物体缺少 `Material`、引用为空或引用的材质未定义时，加载器直接报错，确保加载成功的场景中每个物体都已绑定材质。材质由 `Scene` 持有，通过 `Object::getMaterial().lock()` 获取。

`Lights` 中直接放置光源节点，每个节点创建一个光源：

| 光源节点 | 字段 |
| --- | --- |
| `DirectionalLight` | `Direction`、`Radiance` |
| `PointLight` | `Position`、`Intensity`、`Attenuations` |
| `SpotLight` | `Position`、`Direction`、`Intensity`、`Attenuations`、`InnerAngle`、`OuterAngle` |

位置和方向使用世界坐标，`Direction` 表示光线传播方向，需为非零向量，构造光源时会归一化。颜色按 `r, g, b` 填写。`Attenuations` 按二次项、一次项、常数项填写，即距离衰减为 `1 / (a*r*r + b*r + c)`；`1, 0, 0` 表示平方反比衰减。

聚光灯的锥角是光轴与锥边界的夹角，XML 中以度填写，满足 `0 <= InnerAngle < OuterAngle < 180`；默认分别为 15 和 30 度。加载后通过 `Scene::getLights()` 取得光源。目前渲染器显示法线颜色，光照着色尚未接入。

## 开发环境

- Windows + Visual Studio 2026，安装“使用 C++ 的桌面开发”工作负载及Windows SDK。
- CMake 4.4.4。
- C++23，工具链需支持 C++ 模块和 `import std;`。

## 第三方库

库文件源码在 `lib/` 文件夹中，无需单独下载。

| 库 | 当前源码版本 | 用途 |
| --- | --- | --- |
| [MiniFB](https://github.com/emoon/minifb) | 0.15.0 | 创建窗口、显示像素缓冲区 |
| [GLM](https://github.com/g-truc/glm) | 1.1.0 | 向量和矩阵计算 |
| [TinyXML2](https://github.com/leethomason/tinyxml2) | 11.0.0 | 读取 XML 场景文件 |

版本号以项目内源码声明为准。MiniFB、TinyXML2 随项目编译，GLM 通过头文件使用。

## 构建运行

在项目根目录打开 PowerShell，依次执行：

```powershell
cmake -S . -B build -G "Visual Studio 18 2026" -DMINIFB_BUILD_EXAMPLES=OFF -DMINIFB_BUILD_TESTS=OFF
cmake --build build --config Release --target Render_from_Scratch
Set-Location build
.\Release\Render_from_Scratch.exe
```

当前 `main.cpp` 的调试入口为：

```cpp
Renderer renderer{1920, 1080, "../scenes/scene03.xml"};
```

因此运行时的工作目录需要设为项目的 `build/`。在 Visual Studio 中打开 `build/` 下的解决方案，选择 `Render_from_Scratch` 和 Release，并确认调试工作目录为 `build/`。切换场景时，修改 `src/main.cpp` 中 Renderer 构造函数的第三个参数，例如 `"../scenes/scene03.xml"`，然后重新编译。

## 后续方向

接入光照计算与材质。
