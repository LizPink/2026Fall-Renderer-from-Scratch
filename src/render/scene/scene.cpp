module;

module scene;


/// 释放场景拥有的物体和光源.
Scene::~Scene()
{
	for (auto* object : myObjects)
		delete object;
	for (auto* light : myLights)
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


Object* Scene::createSceneObject(const Vector3f& position, const Vector3f& euler, const float scaler)
{
	Object* pObject = new Object{ position, euler, scaler };
	myObjects.push_back(pObject);

	return pObject;
}


Object* Scene::intersect(Ray ray, Intersection& intersect) const
{
	Object* pHitObject = nullptr;
	for (auto& object : myObjects)
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
	return myLights;
}
