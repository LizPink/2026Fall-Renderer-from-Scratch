module;
#include <glm/glm.hpp>

module circle;
import std;
import ray;


/**
*	@brief 构造一个圆盘对象.
*	@param center	圆盘的位置（世界坐标）
*	@param euler	圆盘的朝向（世界坐标）
*	@param radius	圆盘的半径
*/
Circle::Circle(SceneObject* pSceneObject, const Vector3f& center, const Vector3f& euler, float radius)
	: Primitive(pSceneObject),
	  myCenter(center), myRadius(radius)
{}


bool Circle::intersect(Ray& ray, Intersection& intersect) const
{
	// 将光线ray变换到局部坐标中
	Ray local_ray = Primitive::myPtrSceneObject->getWorldToObject() * ray;

	// 判断是否与圆盘所在平面相交
	if (std::fabsf(local_ray.d.z) < 1e-6f)	// 平行于该圆盘
		return false;

	auto solve_t = -local_ray.o.z / local_ray.d.z;
	if (solve_t < local_ray.minT or solve_t > local_ray.maxT)	// 射线够不到交点
		return false;

	auto intersect_point = local_ray.o + solve_t * local_ray.d;
	if (glm::dot(intersect_point, intersect_point) > glm::dot(this->myRadius, this->myRadius))	// 交点不在圆盘内
		return false;

	// 记录相交信息
	intersect.t = solve_t;
	intersect.position = Vector3f(Primitive::myPtrSceneObject->getObjectToWorld() * Vector4f{ intersect_point, 1.0f });
	intersect.normal = Vector3f(glm::normalize(Primitive::myPtrSceneObject->getObjectToWorld() * Vector4f{ 0.0f, 0.0f, 1.0f, 0.0f }));

	return true;
}