module;
#include <glm/glm.hpp>
#include <glm/geometric.hpp>
#include <glm/ext.hpp>
module camera;


void Camera::initialize(const Vector3f& p, const Vector3f& target, const Vector3f& up, float fov, float n, float f, int W, int H) {
	this->myPosition = p;

	// 左手规则: 构建单位正交系
	//Vector3f l	= glm::normalize(target - p);
	//Vector3f r	= glm::normalize(glm::cross(up, l));
	//Vector3f u	= glm::normalize(glm::cross(l, r));

	//Matrix4x4f viewMatrix = glm::transpose(
	//	Matrix4x4f(
	//		r.x, r.y, r.z, 0.0f,
	//		u.x, u.y, u.z, 0.0f,
	//		l.x, l.y, l.z, 0.0f,
	//		0.0f, 0.0f, 0.0f, 1.0f
	//	) * Translation(-p)
	//);

	// 计算取景矩阵
	Matrix4x4f viewMatrix = glm::lookAtLH(p, target, up);
	// 计算投影矩阵
	Matrix4x4f projectionMatrix = glm::perspectiveFovLH_ZO(fov, (float) W, (float) H, n, f);
	// 计算视口矩阵
	Matrix4x4f viewPortMatrix = Matrix4x4f(
		W/2.0f,	0.0f,		0.0f,	0.0f,
		0.0f,	-H/2.0f,	0.0f,	0.0f,
		0.0f,	0.0f,		1.0f,	0.0f,
		W/2.0f, H/2.0f,		0.0f,	1.0f
	);
	// 计算联合矩阵
	this->myCombineMatrix = viewPortMatrix * projectionMatrix * viewMatrix;
	this->myInverseMatrix = glm::inverse(this->myCombineMatrix);
}


Ray Camera::getRay(int x, int y) const
{
	Ray ray;
	ray.o = this->myPosition;

	Vector4f point_screen{x, y, 0.0f, 1.0f};
	Vector4f point_world = this->myInverseMatrix * point_screen;
	point_world = point_world / point_world.w;

	ray.d = glm::normalize(Vector3f(point_world) - ray.o);

	return ray;
}
