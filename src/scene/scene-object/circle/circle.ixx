module;

export module circle;
import utils;
import ray;
import scene_object;

/**
*	@brief 位于Z轴上的圆盘（局部坐标）
*/
export class Circle: public Primitive
{
public:
	Circle() = delete;
	Circle(Circle&)			= delete;
	Circle(const Circle&)	= delete;
	Circle(Circle&&)		= delete;
	Circle(SceneObject* pSceneObject, const Vector3f& center, const Vector3f& euler, float radius);

	/**
	*	@param ray	光线（世界坐标系）
	*	@param intersection	相交信息记录对象
	*/
	virtual bool intersect(Ray& ray, Intersection& intersect) const override;

private:
	Vector3f myCenter;				/// 圆心坐标（世界坐标系）
	Vector3f myRadius;				/// 半径
};