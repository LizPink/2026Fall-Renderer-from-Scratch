module;

export module triangle;
import utils;
import ray;

/**
	@brief 三角形类
*/
export class Triangle
{
public:
	Triangle() = delete;

	/**
	*	@brief	构造一个三角形对象
	*	@param	v0, v1, v2		三角形的三个顶点（局部坐标系）(有顺序)
	*	@param	objectToWorld	世界变换矩阵（将点变换到世界坐标中）
	*/
	Triangle(const Vector3f& v0, const Vector3f& v1, const Vector3f& v2, const Matrix4x4f& objectToWorld);

	/**
	*	@brief		光线检测
	*	@details	将三角形的顶点（世界坐标）和光线（世界坐标）进行光线检测. 由于三角形比较规则，直接在世界坐标中检测光线即可.
	*/
	bool intersect(Ray& ray, Intersection& intersect) const;

private:
	Vector3f myVertices[3];		///	三角形的顶点（世界坐标系）
	Vector3f myNormal;			///	三角形的法线（世界坐标系）
};
