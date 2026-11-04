module;

export module triangle;

import utils;
import ray;
import scene_object;

// 前向声明
export class Triangle;


/**
	@brief 三角形类Triangle，继承自Primitive图元类，统一由ObjectScene场景物体类管理.
*/
class Triangle: public Primitive
{
public:
	Triangle() = delete;
	Triangle(Triangle&)			= delete;
	Triangle(const Triangle&)	= delete;
	Triangle(Triangle&&)		= delete;
	/* @brief 构造函数 */
	Triangle(SceneObject* pSceneObject, const Vector3f& v0, const Vector3f& v1, const Vector3f& v2);

	~Triangle() = default;

	/**
	*	@brief		光线检测
	*	@details	将三角形的顶点（世界坐标）和光线（世界坐标）进行光线检测. 由于三角形比较规则，直接在世界坐标中检测光线即可.
	*/
	virtual bool intersect(const Ray& ray, Intersection& intersect) const override;

private:
	Vector3f myVertices[3];		///	三角形的顶点（世界坐标系）
	Vector3f myNormal;			///	三角形的法线（世界坐标系）
};
