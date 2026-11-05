module;
#include <glm/glm.hpp>

module utils;

Matrix4x4f makeTranslation(const Vector3f& position)
{
	return Matrix4x4f(
		1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		position.x, position.y, position.z, 1.0f
	);
}

Matrix4x4f makeRotation(const Vector3f& eulerAngle)
{
	auto alpha = eulerAngle.x;
	auto beta  = eulerAngle.y;
	auto gamma = eulerAngle.z;

	auto sin_x = std::sin(alpha);
	auto sin_y = std::sin(beta);
	auto sin_z = std::sin(gamma);
	auto cos_x = std::cos(alpha);
	auto cos_y = std::cos(beta);
	auto cos_z = std::cos(gamma);

	Matrix4x4f rotationX{
		1.0f,	0.0f,	0.0f,	0.0f,
		0.0f,	cos_x,	sin_x,	0.0f,
		0.0f,	-sin_x,	cos_x,	0.0f,
		0.0f,	0.0f,	0.0f,	1.0f
	};

	Matrix4x4f rotationY{
		cos_y,	0.0f,	-sin_y,	0.0f,
		0.0f,	1.0f,	0.0f,	0.0f,
		sin_y,	0.0f,	cos_y,	0.0f,
		0.0f,	0.0f,	0.0f,	1.0f
	};

	Matrix4x4f rotationZ{
		cos_z,	sin_z,	0.0f,	0.0f,
		-sin_z,	cos_z,	0.0f,	0.0f,
		0.0f,	0.0f,	1.0f,	0.0f,
		0.0f,	0.0f,	0.0f,	1.0f
	};

	return rotationZ * rotationY * rotationX;
}


Matrix4x4f makeScaling(float s)
{
	return Matrix4x4f{
		s,		0.0f,	0.0f,	0.0f,
		0.0f,	s,		0.0f,	0.0f,
		0.0f,	0.0f,	s,		0.0f,
		0.0f,	0.0f,	0.0f,	1.0f
	};
}


Matrix4x4f makeWorldTransformation(const Vector3f& direction, const Vector3f& angle, float scaler)
{
	auto transition = makeTranslation(direction);
	auto rotation = makeRotation(angle);
	auto scaling = makeScaling(scaler);
	// Default Order: TRS
	return transition * rotation * scaling;
}


auto makeCoordinateSystem(const Vector3f& w) -> Matrix3x3f
{
	/// 尝试世界坐标中的 (1,0,0) 作为 u 方向.
	Vector3f u{1, 0, 0};

	if (std::abs(glm::dot(u, w)) > 0.99f)
		u = Vector3f{0, 1, 0};

	Vector3f v = glm::cross(w, u);
	u = glm::cross(v, w);

	/// 归一化：确保改变换是单纯的旋转变换（剔除缩放变换的影响）
	u = glm::normalize(u);
	v = glm::normalize(v);

	/// 返回世界变换矩阵
	return Matrix3x3f{u, v, w};
}
