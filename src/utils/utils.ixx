module;
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <glm/geometric.hpp>

export module utils;
import std;

export
{
	/* 数字 */
	using uint32 = uint32_t;

	/* 分数向量 */
	using Vector2f = glm::vec2;
	using Vector3f = glm::vec3;
	using Vector4f = glm::vec4;

	/* 整数向量 */
	using Vector2i = glm::ivec2;
	using Vector3i = glm::ivec3;
	using Vector4i = glm::ivec4;

	/* 分数矩阵 */
	using Matrix3x3f = glm::mat3;
	using Matrix4x4f = glm::mat4;

	/* 常量 */
	constexpr float PI		= glm::pi<float>();
	constexpr float INV_PI	= 1 / glm::pi<float>();
	constexpr float FLOAT_MAX = std::numeric_limits<float>::max();

	/* 颜色 */
	using Color = Vector3f;


	/**
	*	@brief 生成平移变换矩阵.
	*	@param position 指定平移的方向，朝该方向进行平移
	*/
	Matrix4x4f makeTranslation(const Vector3f& position);


	/**
	*	@brief 生成旋转变换矩阵. 采用左手系建系，并采用左手定则进行变换. 
	*	@param eulerAngle 指定旋转变换的欧拉角（分别绕x,y,z轴旋转的弧度角）。约定顺序为：先绕x轴、再绕y轴、最后绕z轴旋转.
	*/
	Matrix4x4f makeRotation(const Vector3f& eulerAngle);


	/**
	*	@brief 生成缩放变换矩阵.
	*/
	Matrix4x4f makeScaling(float s);

	/**
	*	@brief 综合平移、旋转、伸缩变换的世界变换（局部坐标系 -> 世界坐标系）矩阵。约定物体从局部坐标系进入世界坐标系时，遵循如下的变换顺序:（1）先缩放；（2）再旋转；（3）最后平移
	*	@param direction	平移的方向
	*	@param rotation		旋转的欧拉角
	*	@param scaler		缩放系数，默认等比例缩放。
	*/
	Matrix4x4f makeWorldTransformation(const Vector3f& direction, const Vector3f& rotation, float scaler);

	/*************************
	* @brief 以给定方向为 z 轴，构建一个局部坐标系，返回对应的世界变换（局部坐标系 -> 世界坐标系）.
	* @param w （单位向量）
	* @return 返回的是表示单纯的正交旋转变换矩阵.
	* @note 生成的坐标系在世界空间中不唯一. 在计算 BRDF 的时候适合用于各向同性的材质，对于各项异性的材质需要结合 mesh 进一步计算.
	*************************/
	auto makeCoordinateSystem(const Vector3f& w) -> Matrix3x3f;
}


