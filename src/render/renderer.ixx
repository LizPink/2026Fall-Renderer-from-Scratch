module;
#include <atomic>
#include <glm/glm.hpp>
export module renderer;

import camera;
using Color = glm::vec3;

export class Renderer
/**
*	@brief 渲染器类: 承担一个渲染器的角色
*/
{
public:
	/*  构造函数，用于初始化一个渲染器对象 */
	Renderer() = default;
	/*  构造函数，用于初始化一个渲染器对象 */
	Renderer(int width, int height);
	virtual ~Renderer() {};
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
};