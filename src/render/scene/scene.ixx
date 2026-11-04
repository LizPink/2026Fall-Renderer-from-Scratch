module;

export module scene;
import std;
import utils;
import camera;
import light;
import ray;
import scene_object;


export class Scene;

class Scene
{
public:
	Scene()				= default;
	Scene(Scene&)		= delete;
	Scene(const Scene&) = delete;
	Scene(Scene&&)		= delete;

	~Scene();

	void setCamera(const Camera& camera);
	Camera getCamera() const;


	/*************************
	* 场景物体
	*************************/
	/// 创建场景中的物体
	SceneObject* createSceneObject(const Vector3f& position, const Vector3f& euler, const float scaler);

	/// 判断光线与场景物体相交性
	SceneObject* intersect(Ray& ray, Intersection& intersect) const;


	/*************************
	* 场景光源
	*************************/
	/// 创建场景中的光源
	template<typename T, typename... Args>
	LightBase* createSceneLight(Args&&... args);

	/// 获取场景中的全部光源
	const std::vector<LightBase*>& getLights() const;


private:
	Camera						myCamera;
	std::vector<SceneObject*>	mySceneObjects;
	std::vector<LightBase*>		mySceneLights;
};


#include "scene.tpp"