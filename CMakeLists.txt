#[[ 指定项目构建（编译-链接）的具体规则
]]
cmake_minimum_required(VERSION 3.30)
project(								# 声明整个项目的信息
	Render_from_Scratch					# 1.整个CMake项目的名称
	VERSION 0.0.1
	LANGUAGES CXX
)
set(CMAKE_CXX_STANDARD 23)				# 默认使用使用C++23标准进行编译
set(CMAKE_CXX_STANDARD_REQUIRED ON)		# 强制默认使用C++23标准
set(CMAKE_CONFIGURATION_TYPES "Debug;Release"
    CACHE STRING "Available build configurations" FORCE)


#[[ 收集项目的头文件和源文件、模块文件
	GLOB_RECURSE: 		递归地寻找这些源文件（包括src的子文件夹）
	APP_SOURCES: 		找到的文件路径存入名为APP_SOURCES的变量中
	CONFIGURE_DEPENDS:	构建时检查APP_SOURCES中的文件变化，重新生成构建规则
]]
file(
	GLOB_RECURSE APP_SOURCES CONFIGURE_DEPENDS
	"${CMAKE_SOURCE_DIR}/src/*.cpp"
	"${CMAKE_SOURCE_DIR}/src/*.h"
	"${CMAKE_SOURCE_DIR}/src/*.hpp"
)
file(
	GLOB_RECURSE APP_MODULES CONFIGURE_DEPENDS
	"${CMAKE_SOURCE_DIR}/src/*.ixx"
)


#[[ 引导第三方库构建
	第三方库MiniFB在使用前需要经过构建，这里声明子构建的CMakeLists文件所在的目录，引导CMake执行lib/minifib/CmakeLists.txt中的指令
]]
add_subdirectory(
	lib/minifb
)


#[[ 生成构建目标：可执行文件
	利用APP_SOURCES中收集到的文件，进行编译和链接，生成可执行文件
]]
add_executable(
	${PROJECT_NAME}
	${APP_SOURCES}
)


#[[ 为构建目标配置：模块文件
	由于模块文件属于新功能，需要单独设置一个构建规则
]]
target_sources(
	${PROJECT_NAME} PRIVATE
    FILE_SET CXX_MODULES
    FILES ${APP_MODULES}
)


# 按 src 的实际目录结构生成 Visual Studio 筛选器
source_group(
    TREE "${CMAKE_CURRENT_SOURCE_DIR}/src"
    PREFIX "Source Files"
    FILES ${APP_SOURCES} ${APP_MODULES}
)


#[[ 为构建目标配置：第三方头文件
	由于引用了第三方库，可能不在默认的头文件搜索路径中，这里我们需要手动将该路径加入到头文件搜索路径中
]]
target_include_directories(
	${PROJECT_NAME} PRIVATE
	"${CMAKE_SOURCE_DIR}/lib/minifb/include"
	"${CMAKE_SOURCE_DIR}/lib/glm"
)


#[[ 为构建目标配置：第三方源文件
	由于引入了第三方库，在程序链接时需要将第三方库的源文件引入链接
]]
target_link_libraries(
	${PROJECT_NAME} PRIVATE
	minifb
)
if (WIN32)
target_link_libraries(
	${PROJECT_NAME} PRIVATE
	minifb
	opengl32
	winmm
	kernel32
	user32
	gdi32
	winspool
	shell32
	ole32
	oleaut32
	uuid
	comdlg32
	advapi32
)
endif()


# 构建时如果使用VisualStudio生成，则指定默认的启动项目
if(CMAKE_GENERATOR MATCHES "^Visual Studio")
    set_property(DIRECTORY PROPERTY VS_STARTUP_PROJECT ${PROJECT_NAME})
endif()
