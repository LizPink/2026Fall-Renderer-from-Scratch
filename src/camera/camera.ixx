module;
export module camera;

import utils;
import ray;

export class Camera {
public:
	Camera() = default;
	/**
	*	@brief 初始化 Camera 对象
	*	@param p Position of the camera.
	*	@param target Target point of the camera.
	*/
	void initialize(const Vector3f& p, const Vector3f& target, const Vector3f& up, float fov, float n, float f, int W, int H);

	/**
	*	@brief 给定屏幕上的像素位置,获取光线
	*/
	Ray getRay(int x, int y) const;

private:
	Vector3f myPosition;
	Matrix4x4f myCombineMatrix;
	Matrix4x4f myInverseMatrix;
};