module;

module scene;


/// 释放场景拥有的物体和光源.
Scene::~Scene()
{
	for (auto* object : mySceneObjects)
		delete object;
	for (auto* light : mySceneLights)
		delete light;
}


void Scene::setCamera(const Camera& camera)
{
	this->myCamera = camera;
}


Camera Scene::getCamera() const
{
	return this->myCamera;
}


SceneObject* Scene::createSceneObject(const Vector3f& position, const Vector3f& euler, const float scaler)
{
	SceneObject* pSceneObject = new SceneObject{ position, euler, scaler };
	mySceneObjects.push_back(pSceneObject);

	return pSceneObject;
}


SceneObject* Scene::intersect(Ray& ray, Intersection& intersect) const
{
	SceneObject* pHitObject = nullptr;
	for (auto& object : mySceneObjects)
	{
		if (object->intersect(ray, intersect))
		{
			ray.maxT = intersect.t;
			pHitObject = object;
		}
	}
	return pHitObject;
}


const std::vector<LightBase*>& Scene::getLights() const
{
	return mySceneLights;
}
