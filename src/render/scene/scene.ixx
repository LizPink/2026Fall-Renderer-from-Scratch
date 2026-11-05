module;

export module scene;
import scene_common;

export class Scene;

/****************************************************************************************************
 * @brief 场景类
****************************************************************************************************/
class Scene
{
private:
	Camera													myCamera;
	std::vector<Object*>									myObjects;
	std::vector<LightBase*>									myLights;
	std::map<std::string, std::shared_ptr<MaterialBase>>	myMaterials;


public:
	Scene()				= default;
	Scene(Scene&)		= delete;
	Scene(const Scene&) = delete;
	Scene(Scene&&)		= delete;

	~Scene();

	void setCamera(const Camera& camera);
	Camera getCamera() const;


	/**************************************************
	 * 场景物体
	**************************************************/
	/// 创建场景中的物体
	Object* createSceneObject(const Vector3f& position, const Vector3f& euler, const float scaler);

	/// 判断光线与场景物体相交性
	Object* intersect(Ray ray, Intersection& intersect) const;


	/**************************************************
	 * 场景光源
	**************************************************/
	/// 创建场景中的光源
	template<typename T, typename... Args>
	LightBase* createSceneLight(Args&&... args);

	/// 获取场景中的全部光源
	const std::vector<LightBase*>& getLights() const;


	/**************************************************
	 * 场景材质
	**************************************************/
	/// 创建场景中的材质
	template<typename T, typename... Args>
	std::weak_ptr<MaterialBase> createSceneMaterial(const std::string& name, Args&& ...args);
};


#include "scene.tpp"	///< 模板成员的实现
