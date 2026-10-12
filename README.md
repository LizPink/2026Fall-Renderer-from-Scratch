# Render from Scratch

一个从基础开始学习和实现渲染的 C++ 项目。目前已搭建构建环境并接入第三方库，主程序用于验证 C++ 模块调用，尚未实现渲染功能。

## 项目结构

```text
├── CMakeLists.txt          # 构建配置
├── src/
│   ├── main.cpp           # 程序入口
│   └── module_files/
│       └── Module.ixx     # 自定义 C++ 模块
└── lib/
    ├── minifb/            # 窗口与像素缓冲区显示
    └── glm/               # 向量、矩阵等数学计算
```

## 开发环境

- Windows + Visual Studio 2026，安装“使用 C++ 的桌面开发”工作负载及 Windows SDK。
- CMake 4.4.4。
- C++23，工具链需支持 C++ 模块和 `import std;`。

## 第三方库

库文件源码在 `lib/`文件夹中，无需单独下载。

| 库 | 当前源码版本 | 用途 |
| --- | --- | --- |
| [MiniFB](https://github.com/emoon/minifb) | 0.15.0 | 创建窗口、显示像素缓冲区 |
| [GLM](https://github.com/g-truc/glm) | 1.1.0 | 向量和矩阵计算 |

版本号以项目内源码声明为准；上游最新源码与正式发布版本可能不同。MiniFB 随项目编译，GLM 通过头文件使用。

## 构建与运行

在项目根目录打开 PowerShell，依次执行：

```powershell
cmake -S . -B build -G "Visual Studio 18 2026" -DMINIFB_BUILD_EXAMPLES=OFF -DMINIFB_BUILD_TESTS=OFF
cmake --build build --config Debug --target Render_from_Scratch
.\build\Debug\Render_from_Scratch.exe
```

也可以在 Visual Studio 中打开 `build/` 下生成的解决方案，选择 `Render_from_Scratch` 运行。

当前程序输出：

```text
hello world
Hello from MyFunc!
```
