module;
#include <cassert>
#include <glm/glm.hpp>

module sphere;
import utils;

Sphere::Sphere(const Vector3f& center, float R)
	: myCenter(center), myRadius(R)
{
	// 球体可以省略旋转和缩放信息
	this->myObjecToWorld = makeWorldTransformation(center, Vector3f(0.0f, 0.0f, 0.0f), 1.0f);
	this->myWorldToObject = glm::inverse(this->myObjecToWorld);
}


bool Sphere::intersect(Ray& ray, Intersection& intersection) const
{
	// 将射线ray转移至局部坐标系表示下
	Ray local_ray = this->myWorldToObject * ray;

	// 计算局部坐标系下，光线是否与球体相交
	auto A = glm::dot(local_ray.d, local_ray.d);
	auto B = 2 * glm::dot(local_ray.o, local_ray.d);
	auto C = glm::dot(local_ray.o, local_ray.o) - this->myRadius*this->myRadius;	// ||o'||^2-r^2

	auto delta = B*B - 4*A*C;	// B^2-4AC

	if (delta < 0)
		return false;

	auto t1 = (-B - std::sqrt(delta)) / 2*A;	// Root t1
	auto t2 = (-B + std::sqrt(delta)) / 2*A;	// Root t2
	assert(t1 <= t2);
	
	// 计算所得根是否在光线所及的范围内
	if (t1 > local_ray.maxT or t2 < local_ray.minT)
		return false;

	if (t1 < local_ray.minT and t2 > local_ray.maxT)
		return false;

	if (t1 >= local_ray.minT)
	{
		intersection.t = t1;
	}
	else
	{
		intersection.t = t2;
	}

	// 如果相交，更新相交信息
	Vector3f intersect_point	= local_ray.o + intersection.t * local_ray.d;	// 交点（局部坐标）
	Vector3f intersect_normal	= glm::normalize(intersect_point);				// 法线方向（局部坐标）
	intersection.position		= Vector3f(this->myObjecToWorld * Vector4f{ intersect_point, 1.0f });					// 交点（世界坐标）
	intersection.normal			= glm::normalize(Vector3f(this->myObjecToWorld * Vector4f{ intersect_normal, 0.0f }));	// 法线方向（世界坐标）
	
	return true;
}
