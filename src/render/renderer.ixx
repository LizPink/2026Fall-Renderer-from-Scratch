module;
#include <atomic>
#include <glm/glm.hpp>

export module renderer;
import camera;
import scene_object;

using Color = glm::vec3;

/* @brief 渲染器类: 承担一个渲染器的角色 */
export class Renderer
{
public:
	/* @brief 构造函数 */
	Renderer() = delete;
	Renderer(Renderer&)			= delete;
	Renderer(const Renderer&)	= delete;
	Renderer(Renderer &&)		= delete;
	Renderer(int width, int height);

	virtual ~Renderer();

	/* @brief 渲染器渲染入口 */
	void run();

private:
	int myViewPortWidth = 800;
	int myViewPortHeight = 600;
	uint32_t* myBuffer = nullptr;

	// 渲染缓冲区像素
	Color renderPixel(int x, int y);
	Color renderSubPixel(float px, float py);

	// 缓冲区内存索引（供多线程使用）
	std::atomic<int> myCurrentPixelIndex = 0;
	// 多线程渲染的执行函数
	void runRenderThread();

	// 场景相机
	Camera myCamera;

	// 场景物体
	SceneObject* mySceneObject = nullptr;
};