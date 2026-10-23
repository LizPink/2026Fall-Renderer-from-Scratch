module;

export module sphere;
import utils;
import ray;
import scene_object;

export
{
	class Sphere: public Primitive
	{
	public:
		Sphere() = delete;
		/**
		*	@brief 继承自Primitive图元类。球面几何体的构造函数，用于生成一个球面.
		*	@param pSceneObject	传递给基类构造函数，用于构造基类部分
		*	@param center 球面的球心位置（世界坐标系下）.
		*/
		Sphere(SceneObject* pSceneObject, const Vector3f& center, float R);

		/**
		*	@brief 判断给定光线是否与几何体相交
		*	@param ray 射线（世界空间坐标系）
		*	@return 如果不相交返回false, 否则返回true, 并且修改交点intersection的信息（世界坐标）.
		*/
		virtual bool intersect(Ray& ray, Intersection& intersection) const override;

	private:
		Vector3f myCenter;			// 坐标原点（世界坐标）
		float myRadius;
	};
}