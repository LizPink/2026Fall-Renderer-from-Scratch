module;

export module circle;
import utils;
import ray;

/**
*	@brief 位于Z轴上的圆盘（局部坐标）
*/
export class Circle
{
public:
	Circle() = delete;

	/**
	*	@brief 构造一个圆盘对象.
	*	@param center	圆盘的位置（世界坐标）
	*	@param euler	圆盘的朝向（世界坐标）
	*	@param radius	圆盘的半径
	*/
	Circle(const Vector3f& center, const Vector3f& euler, float radius);

	/**
	*	@param ray	光线（世界坐标系）
	*	@param intersection	相交信息记录对象
	*/
	bool intersect(const Ray& ray, Intersection& intersect) const;

private:
	Vector3f myCenter;				/// 圆心坐标（世界坐标系）
	Vector3f myRadius;				/// 半径
	Matrix4x4f myObjectToWorld;
	Matrix4x4f myWorldToObject;
};