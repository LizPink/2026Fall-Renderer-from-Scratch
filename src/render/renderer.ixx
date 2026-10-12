module;
#include <glm/glm.hpp>
export module renderer;

using Color = glm::vec3;

export class Renderer
/* 渲染器类: 承担一个渲染器的角色 */
{
public:
	Renderer() = default;
	Renderer(int width, int height);
	virtual ~Renderer() {};

	void run();

private:
	int myViewPortWidth = 800;
	int myViewPortHeight = 600;

	Color renderPixel(int x, int y);
};