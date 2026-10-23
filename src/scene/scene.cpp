module;

module scene;


/**
*	@brief 析构函数
*	Step1.遍历引用每一个SceneObject对象.
*	Step2.释放对应的资源，并且将指针置空.
*/
Scene::~Scene()
{
	for (auto& pSceneObject : mySceneObjects)
	{
		if (pSceneObject)
		{
			delete pSceneObject;
			pSceneObject = nullptr;
		}
	}
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
	this->mySceneObjects.push_back(pSceneObject);

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