/// Template Implementation for scene.ixx

template<typename T, typename... Args>
LightBase* Scene::createSceneLight(Args&&... args)
{
	LightBase* light = new T(std::forward<Args>(args)...);
	myLights.push_back(light);

	return light;
}


template<typename T, typename ...Args>
auto Scene::createSceneMaterial(const std::string& name, Args&& ...args) -> std::weak_ptr<MaterialBase>
{
	/// 创建智能指针
	auto material = std::make_shared<T>(std::forward<Args>(args)...);
	
	/// 保存共享指针给类对象
	myMaterials.insert({name, material}); 

	/// 返回指针地址供外部使用
	return std::weak_ptr<MaterialBase>{material};
}
