module;
#include <glm/glm.hpp>
export module ray;

import utils;


export struct Ray {
public:
	Vector3f o;
	Vector3f d;

	float minT = 0.0f;
	float maxT = FLOAT_MAX;
};


/**
*	@brief 重载变换矩阵Matrix4x4与Ray的运算符. 用于将光线坐标进行空间变换.
*/
export inline Ray operator*(const Matrix4x4& m, const Ray& r)
{
	Ray result;
	result.o = Vector3f(m * Vector4f(r.o, 1.0f));
	result.d = Vector3f(m * Vector4f(r.d, 0.0f));
	result.minT = r.minT;
	result.maxT = r.maxT;

	return result;
}