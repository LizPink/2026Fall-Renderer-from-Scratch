# Render from Scratch

一个从基础开始学习和实现渲染的 C++ 项目。使用 C++23 模块组织相机、场景和图元，通过 XML 加载场景，在 CPU 上计算光线与图元的交点，用 MiniFB 显示渲染结果。

## 项目结构

```text
├── CMakeLists.txt          # 构建配置
├── scenes/
│   ├── scene01.xml         # 两个三角形组成的正方形
│   ├── scene02.xml         # 犹他茶壶，当前调试场景
│   └── scene03.xml         # 两个球与房间
├── src/
│   ├── main.cpp           # 程序入口
│   ├── render/            # Renderer：加载场景、计算像素并显示
│   ├── scene/
│   │   ├── scene.*        # Scene：管理相机和场景物体
│   │   ├── loader/        # SceneLoader：读取 XML 并创建 Scene
│   │   ├── camera/        # 相机和光线
│   │   └── scene-object/  # 场景物体和图元
│   └── utils/             # 向量、矩阵及变换工具
└── lib/
    ├── minifb/            # 窗口与像素缓冲区显示
    ├── glm/               # 向量、矩阵等数学计算
    └── tinyxml2/          # XML 解析
```

## 场景与渲染

| 类型 | 职责 |
| --- | --- |
| `Scene` | 保存相机，管理多个 `SceneObject`，查找光线最近命中的物体 |
| `SceneObject` | 保存物体变换，管理物体内的多个图元 |
| `Primitive` | 定义图元的求交接口，由 `Sphere`、`Circle`、`Triangle` 实现 |
| `Camera` | 根据位置、朝向和投影参数，将屏幕采样点转换为世界空间光线 |
| `SceneLoader` | 解析 XML，创建相机、物体和图元，返回 `std::unique_ptr<Scene>` |
| `Renderer` | 持有场景，计算像素颜色，并通过 MiniFB 显示缓冲区 |

场景由 Renderer 持有；Scene 释放其管理的场景物体，SceneObject 释放其管理的图元。加载失败时，主程序输出包含文件、行号和原因的错误信息并退出。

当前主程序使用 1920×1080 分辨率。工作线程通过原子索引领取像素，主线程刷新窗口；每个像素在像素范围内随机采样 100 次并取平均，实现 SSAA 抗锯齿。

命中点的世界空间法线通过 `normal * 0.5 + 0.5` 映射为 RGB，未命中的采样点为黑色。当前画面用于观察几何与法线，三角形曲面会显示彩色分面。求交时逐个遍历物体和图元，茶壶的绘制时间比正方形和两球场景长。

## XML 场景

| 文件 | 内容 |
| --- | --- |
| `scenes/scene01.xml` | 一个物体、两个三角形组成的正方形，用于检查相机、变换和三角形求交 |
| `scenes/scene02.xml` | 完整犹他茶壶，包含壶身、壶底、壶盖、壶钮、壶嘴和把手；曲面离散为 248 个三角形，文件内有参数和部件注释 |
| `scenes/scene03.xml` | 两个球，以及由 10 个三角形组成的地面、天花板和三面墙 |

XML 以 `Scene` 为根，包含 `Camera` 和 `SceneObjects`。每个 `SceneObject` 包含 `Transform` 和 `Primitives`；一个物体可以包含多个图元。完整格式可参考 `scene01.xml`。

| 元素 | 格式与含义 |
| --- | --- |
| `Camera/Position`、`Target`、`Up` | `x, y, z`：相机位置、观察目标和上方向；示例使用 Y 轴向上 |
| `Camera/NearZ`、`FarZ` | 投影矩阵的近、远平面参数，满足 `0 < NearZ < FarZ` |
| `Camera/Fov` | 视野角，单位为度，范围为 `0 < Fov < 180` |
| `Transform/Position` | 物体平移，格式为 `x, y, z` |
| `Transform/Rotation` | 绕 X、Y、Z 轴的旋转角，格式为 `x, y, z`，单位为度 |
| `Transform/Scale` | 一个正数，表示统一缩放比例 |
| `Sphere/Radius` | 球半径，必须为正数 |
| `Disk/Radius` 或 `Circle/Radius` | 圆盘半径，必须为正数；两种标签均创建 `Circle` |
| `Triangle/Vertex` | 三个局部坐标顶点，每个顶点为 `x, y, z`，顶点不得重合或共线 |

球和圆盘以物体局部原点为中心，圆盘位于局部 XY 平面；位置和朝向通过 `Transform` 设置。三角形的顶点顺序决定法线方向。XML 注释使用 `<!-- 注释内容 -->`，放在元素之间。

## 开发环境

- Windows + Visual Studio 2026，安装“使用 C++ 的桌面开发”工作负载及 Windows SDK。
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

## 构建与运行

在项目根目录打开 PowerShell，依次执行：

```powershell
cmake -S . -B build -G "Visual Studio 18 2026" -DMINIFB_BUILD_EXAMPLES=OFF -DMINIFB_BUILD_TESTS=OFF
cmake --build build --config Release --target Render_from_Scratch
Set-Location build
.\Release\Render_from_Scratch.exe
```

当前 `main.cpp` 的调试入口为：

```cpp
Renderer renderer{1920, 1080, "../scenes/scene02.xml"};
```

因此运行时的工作目录需要设为项目的 `build/`。在 Visual Studio 中打开 `build/` 下的解决方案，选择 `Render_from_Scratch` 和 Release，并确认调试工作目录为 `build/`。

切换场景时，修改 `src/main.cpp` 中 Renderer 构造函数的第三个参数，例如 `"../scenes/scene03.xml"`，然后重新编译。当前入口使用这条固定路径，命令行场景参数尚未接入实际构造过程。

## 后续方向

继续完善图元求交与渲染流程，增加材质和光照，并引入空间加速结构以支持更复杂的模型。
