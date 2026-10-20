module;

export module sphere;
import utils;
import ray;

export
{
	class Sphere
	{
	public:
		Sphere() = delete;

		/**
		*	@brief 球面几何体的构造函数，用于生成一个球面.
		*	@param center 球面的球心位置（世界坐标系下）.
		*/
		Sphere(const Vector3f& center, float R);

		/**
		*	@brief 判断给定光线是否与几何体相交
		*	@param ray 射线（世界空间坐标系）
		*	@return 如果不相交返回false, 否则返回true, 并且修改交点intersection的信息（世界坐标）.
		*/
		bool intersect(Ray& ray, Intersection& intersection) const;

	private:
		Vector3f myCenter;			// 坐标原点（世界坐标）
		float myRadius;
		Matrix4x4f myObjecToWorld;	// 对象空间到世界空间的变换矩阵
		Matrix4x4f myWorldToObject;	// 世界空间到对象空间的变换矩阵
	};
}