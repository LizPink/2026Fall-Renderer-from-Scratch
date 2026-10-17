module;
#include <atomic>
#include <glm/glm.hpp>
export module renderer;

import camera;
import sphere;
using Color = glm::vec3;

/**
*	@brief 渲染器类: 承担一个渲染器的角色
*/
export class Renderer
{
public:
	Renderer() = delete;

	/**
	*	@brief 构造函数，用于初始化一个渲染器对象
	*/
	Renderer(int width, int height);

	/**
	*	@brief 析构函数
	*/
	virtual ~Renderer();

	/**
	*	@brief 渲染器渲染入口
	*/
	void run();

private:
	int myViewPortWidth = 800;
	int myViewPortHeight = 600;
	uint32_t* myBuffer = nullptr;

	// 渲染缓冲区像素
	Color renderPixel(int x, int y);

	// 缓冲区内存索引（供多线程使用）
	std::atomic<int> myCurrentPixelIndex = 0;
	// 多线程渲染的执行函数
	void runRenderThread();

	// 场景摄像机
	Camera myCamera;

	// 场景物体
	Sphere* mySphere;
};