module;
#include <glm/glm.hpp>
#include <glm/geometric.hpp>
#include <glm/ext/scalar_constants.hpp>
export module utils;
import std;

export{
	/* 分数向量 */
	using Vector2f = glm::vec2;
	using Vector3f = glm::vec3;
	using Vector4f = glm::vec4;

	/* 整数向量 */
	using Vector2i = glm::ivec2;
	using Vector3i = glm::ivec3;
	using Vector4i = glm::ivec4;

	/* 分数矩阵 */
	using Matrix3x3 = glm::mat3;
	using Matrix4x4 = glm::mat4;

	/* 常量 */
	constexpr float PI = glm::pi<float>();
	constexpr float FLOAT_MAX = std::numeric_limits<float>::max();

	/**
	*	@brief 生成一个4x4的平移变换矩阵.
	*	@param position [Vector3f] 输入方向，指定平移的方向
	*/
	Matrix4x4 Translation(const Vector3f& position);
}
