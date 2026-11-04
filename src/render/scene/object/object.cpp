module;
#include <glm/glm.hpp>

module scene_object;
import utils;

Primitive::Primitive(SceneObject* pSceneObject)
	: myPtrSceneObject(pSceneObject)
{}


SceneObject::SceneObject(const Vector3f& position, const Vector3f& euler, const float scaler)
	: myObjectToWorld(makeWorldTransformation(position, euler, scaler)), myWorldToObject(glm::inverse(this->myObjectToWorld))
{}


SceneObject::~SceneObject()
{
    for (auto& primitive : myPrimitives)
    {
        if (primitive)
        {
            delete primitive;
            primitive = nullptr;
        }
    }
}


void SceneObject::addPrimitive(Primitive* primitive)
{
    this->myPrimitives.push_back(primitive);
}


bool SceneObject::intersect(Ray& ray, Intersection& intersect)
{
    bool hit_flag{false};

    for (const auto& primitive : this->myPrimitives)
    {
        if (primitive->intersect(ray, intersect))
        {
            hit_flag = true;
            ray.maxT = intersect.t;
        }
    }

    return hit_flag;
}


Matrix4x4f SceneObject::getObjectToWorld()
{
	return this->myObjectToWorld;
}

Matrix4x4f SceneObject::getWorldToObject()
{
	return this->myWorldToObject;
}
