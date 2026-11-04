/// Template Implementation for scene.ixx

template<typename T, typename... Args>
LightBase* Scene::createSceneLight(Args&&... args)
{
	LightBase* light = new T(std::forward<Args>(args)...);
	mySceneLights.push_back(light);

	return light;
}