module;
#include <glm/glm.hpp>
module utils;

Matrix4x4 makeTranslation(const Vector3f& position)
{
	return Matrix4x4(
		1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		position.x, position.y, position.z, 1.0f
	);
}

Matrix4x4 makeRotation(const Vector3f& eulerAngle)
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

	Matrix4x4 rotationX{
		1.0f,	0.0f,	0.0f,	0.0f,
		0.0f,	cos_x,	sin_x,	0.0f,
		0.0f,	-sin_x,	cos_x,	0.0f,
		0.0f,	0.0f,	0.0f,	1.0f
	};

	Matrix4x4 rotationY{
		cos_y,	0.0f,	-sin_y,	0.0f,
		0.0f,	1.0f,	0.0f,	0.0f,
		sin_y,	0.0f,	cos_y,	0.0f,
		0.0f,	0.0f,	0.0f,	1.0f
	};

	Matrix4x4 rotationZ{
		cos_z,	sin_z,	0.0f,	0.0f,
		-sin_z,	cos_z,	0.0f,	0.0f,
		0.0f,	0.0f,	1.0f,	0.0f,
		0.0f,	0.0f,	0.0f,	1.0f
	};

	return rotationZ * rotationY * rotationX;
}


Matrix4x4 makeScaling(float s)
{
	return Matrix4x4{
		s,		0.0f,	0.0f,	0.0f,
		0.0f,	s,		0.0f,	0.0f,
		0.0f,	0.0f,	s,		0.0f,
		0.0f,	0.0f,	0.0f,	1.0f
	};
}


Matrix4x4 makeWorldTransformation(const Vector3f& direction, const Vector3f& angle, float scaler)
{
	auto transition = makeTranslation(direction);
	auto rotation = makeRotation(angle);
	auto scaling = makeScaling(scaler);
	// Default Order: TRS
	return transition * rotation * scaling;
}