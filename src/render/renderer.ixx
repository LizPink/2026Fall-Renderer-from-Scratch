module;
#include <atomic>
#include <glm/glm.hpp>
export module renderer;

using Color = glm::vec3;

export class Renderer
/*	渲染器类: 承担一个渲染器的角色
*	Attributes
*	----------
*	myCurrentPixelIndex (PRIVATE)
*		用于Buffer中的当前像素索引，在多线程渲染中有用.
*/
{
public:
	Renderer() = default;
	Renderer(int width, int height);
	virtual ~Renderer() {};

	void run();

private:
	int myViewPortWidth = 800;
	int myViewPortHeight = 600;
	uint32_t* myBuffer = nullptr;
	std::atomic<int> myCurrentPixelIndex = 0;

	Color renderPixel(int x, int y);
	void runRenderThread();
};