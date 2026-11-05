module;
#include <glm/glm.hpp>

module object;
import utils;

Primitive::Primitive(Object* pObject)
	: myPtrObject(pObject)
{}


Object::Object(const Vector3f& position, const Vector3f& euler, const float scaler)
	: myObjectToWorld(makeWorldTransformation(position, euler, scaler)), myWorldToObject(glm::inverse(this->myObjectToWorld))
{}


Object::~Object()
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


void Object::addPrimitive(Primitive* primitive)
{
    this->myPrimitives.push_back(primitive);
}


bool Object::intersect(Ray ray, Intersection& intersect)
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


Matrix4x4f Object::getObjectToWorld() const
{
	return this->myObjectToWorld;
}


Matrix4x4f Object::getWorldToObject() const
{
	return this->myWorldToObject;
}


auto Object::setMaterial(std::weak_ptr<MaterialBase> pMaterial) -> void
{
    myMaterial = std::move(pMaterial);
}


auto Object::getMaterial() const -> std::weak_ptr<MaterialBase>
{
    return myMaterial;
}
