module;
#include <glm/glm.hpp>

module triangle;
import std;


/**
*	@brief	构造一个三角形对象
*	@param	v0->v1->v2		三角形的三个顺序顶点（局部坐标系）
*	@param	objectToWorld	世界变换矩阵（局部坐标系 -> 世界坐标）
*/
Triangle::Triangle(SceneObject* pSceneObject, const Vector3f& v0, const Vector3f& v1, const Vector3f& v2)
	: Primitive(pSceneObject)
{
	Matrix4x4f objectToWorld = Primitive::myPtrSceneObject->getObjectToWorld();

	// 计算三个顶点的世界坐标
	this->myVertices[0] = Vector3f(objectToWorld * Vector4f{v0,1.0f});
	this->myVertices[1] = Vector3f(objectToWorld * Vector4f{v1,1.0f});
	this->myVertices[2] = Vector3f(objectToWorld * Vector4f{v2,1.0f});

	// 计算三角形对象的法线（世界坐标）
	auto edge1 = this->myVertices[1] - this->myVertices[0];
	auto edge2 = this->myVertices[2] - this->myVertices[0];
	this->myNormal = glm::normalize(glm::cross(edge1, edge2));
}


bool Triangle::intersect(const Ray& ray, Intersection& intersect) const
{
	Vector3f p0 = this->myVertices[0];
	Vector3f p1 = this->myVertices[1];
	Vector3f p2 = this->myVertices[2];

	auto o = ray.o;
	auto d = ray.d;
	auto s	= o - p0;
	auto e1 = p1 - p0;
	auto e2 = p2 - p0;

	// 提前计算公共部分
	auto s1	= glm::cross(d, e2);
	auto s2	= glm::cross(s, e1);
	auto denominator = glm::dot(s1, e1);

	// 如果光线和三角形近乎平行就不想交，否则有可能相交
	if (std::fabs(glm::dot(d, this->myNormal)) < 1e-6f)
		return false;

	// 计算相交方程组的解
	auto b = glm::dot(s1, s)  / denominator;
	auto c = glm::dot(s2, d)  / denominator;
	auto t = glm::dot(s2, e2) / denominator;

	// 判断解是否满足相交条件
	if (t < ray.minT || t > ray.maxT)	// 光线无法到达
		return false;

	auto a = 1 - b - c;
	if (a<0.0f || b<0.0f || c<0.0f)		// 交点落在三角形外
		return false;

	// 光线与三角形相交，更新相交信息
	Vector3f intersect_point = ray.o + t*ray.d;
	intersect.normal = this->myNormal;
	intersect.t = t;
	intersect.position = intersect_point;

	return true;
}


