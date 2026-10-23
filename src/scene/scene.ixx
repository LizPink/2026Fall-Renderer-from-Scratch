module;

export module scene;
import std;
import utils;
import camera;
import ray;
import scene_object;


export class Scene;

class Scene
{
public:
	Scene() = default;
	Scene(const Scene&) = delete;
	Scene(Scene&&)		= delete;

	~Scene();

	void setCamera(const Camera& camera);
	Camera getCamera() const;

	SceneObject* createSceneObject(const Vector3f& position, const Vector3f& euler, const float scaler);
	SceneObject* intersect(Ray& ray, Intersection& intersect) const;

private:
	Camera myCamera;
	std::vector<SceneObject*> mySceneObjects;
};