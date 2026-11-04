module;

export module scene_object;
import std;
import utils;
import ray;

// 前向声明
export class Primitive;
export class SceneObject;


/**
*	@brief	抽象类 Primitive 图元类，负责统一各类真实的基本几何物体，例如圆、球、三角形。
*/
class Primitive
{
public:
	Primitive() = delete;
	Primitive(Primitive&)		= delete;
	Primitive(const Primitive&)	= delete;
	Primitive(Primitive&&)		= delete;
	Primitive(SceneObject* pSceneObject);

	virtual ~Primitive() = default;

	virtual bool intersect(const Ray& ray, Intersection& intersect) const = 0;

protected:
	SceneObject* myPtrSceneObject = nullptr;
};



/**
*	@brief	场景物体类，代表场景中的一个物体对象，用于管理场景中的Primitives图元对象。
*/
class SceneObject
{
public:
	SceneObject() = delete;
	SceneObject(SceneObject&)		= delete;
	SceneObject(const SceneObject&) = delete;
	SceneObject(SceneObject&&)		= delete;
	/* @brief	构造函数：通过传入缩放、旋转、平移参数，初始化世界变换矩阵和其逆矩阵。 */
	SceneObject(const Vector3f& position, const Vector3f& euler, const float scaler);

	~SceneObject();

	template<typename T, typename...Args>
	T* createPrimitive(Args&&...);

	/* @brief 向场景物体中添加图元 */
	void addPrimitive(Primitive* primitive);

	/* @brief 判断场景物体与给定光线是否相交 */
	bool intersect(Ray& ray, Intersection& intersect);

	/* @brief 获取世界变换矩阵 */
	Matrix4x4f getObjectToWorld();
	/* @brief 获取局部变换矩阵 */
	Matrix4x4f getWorldToObject();


private:
	Matrix4x4f myObjectToWorld;
	Matrix4x4f myWorldToObject;
	std::vector<Primitive*> myPrimitives;
};


#include "object.tpp"
