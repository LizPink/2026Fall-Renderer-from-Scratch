module;

export module object;
import std;
import utils;
import ray;
import material;

// 前向声明
export class Primitive;
export class Object;


/****************************************************************************************************
 * @brief 基类 Primitive 图元类，负责统一各类真实的基本几何物体，例如圆、球、三角形。
****************************************************************************************************/
class Primitive
{
public:
	Primitive() = delete;
	Primitive(Primitive&)		= delete;
	Primitive(const Primitive&)	= delete;
	Primitive(Primitive&&)		= delete;
	Primitive(Object* pObject);

	virtual ~Primitive() = default;

	virtual bool intersect(const Ray& ray, Intersection& intersect) const = 0;

protected:
	Object* myPtrObject = nullptr;
};



/****************************************************************************************************
 * @brief 场景物体类，代表场景中的一个物体对象，用于管理场景中的 Primitives 图元对象
****************************************************************************************************/
class Object
{
private:
	Matrix4x4f myObjectToWorld{};
	Matrix4x4f myWorldToObject{};

	std::vector<Primitive*>		myPrimitives{};
	std::weak_ptr<MaterialBase> myMaterial{};

public:
	Object()					= delete;
	Object(Object&)		= delete;
	Object(Object&&)		= delete;
	Object(const Vector3f& position, const Vector3f& euler, const float scaler);

	~Object();


	/**************************************************
	* 场景物体图元
	**************************************************/
	template<typename T, typename...Args>
	T* createPrimitive(Args&&...);

	void addPrimitive(Primitive* primitive);


	/* @brief 判断场景物体与给定光线是否相交 */
	bool intersect(Ray ray, Intersection& intersect);

	/* @brief 获取世界变换矩阵 */
	Matrix4x4f getObjectToWorld() const;
	/* @brief 获取局部变换矩阵 */
	Matrix4x4f getWorldToObject() const;


	/**************************************************
	* 场景物体材质
	**************************************************/
	auto setMaterial(std::weak_ptr<MaterialBase> pMaterial) -> void;
	auto getMaterial() const -> std::weak_ptr<MaterialBase>;
};

#include "object.tpp"
