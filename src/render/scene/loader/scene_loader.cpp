module;
#include <tinyxml2.h>
#include <glm/glm.hpp>

module scene_loader;

import std;
import utils;
import camera;
import scene_object;
import sphere;
import triangle;
import circle;
import light;

namespace
{
    using tinyxml2::XMLElement;
    constexpr float DEG_TO_RAD = PI / 180.0f;

    // 必填节点缺失时给出提示；可选节点直接使用 FirstChildElement。
    const XMLElement* requiredChild(const XMLElement* parent, const char* name)
    {
        const auto* element = parent->FirstChildElement(name);
        if (!element)
            throw std::runtime_error(std::string("Missing <") + name + ">");
        return element;
    }

    float readFloat(const XMLElement* parent, const char* name, float defaultValue)
    {
        const auto* element = parent->FirstChildElement(name);
        if (!element)
            return defaultValue;

        float value;
        if (element->QueryFloatText(&value) != tinyxml2::XML_SUCCESS)
            throw std::runtime_error(std::string("Expected a number in <") + name + ">");
        return value;
    }

    // XML 中向量和颜色都写成 x, y, z。
    Vector3f readVector(const XMLElement* element)
    {
        std::istringstream stream(element->GetText() ? element->GetText() : "");
        Vector3f value{ 0.0f };
        char comma1, comma2;
        if (!(stream >> value.x >> comma1 >> value.y >> comma2 >> value.z)
            || comma1 != ',' || comma2 != ',')
            throw std::runtime_error(std::string("Expected x, y, z in <") + element->Name() + ">");
        return value;
    }

    void loadCamera(Scene& scene, const XMLElement* element, int width, int height)
    {
        const auto position = readVector(requiredChild(element, "Position"));
        const auto target = readVector(requiredChild(element, "Target"));
        const auto up = readVector(requiredChild(element, "Up"));
        const float nearZ = readFloat(element, "NearZ", 0.1f);
        const float farZ = readFloat(element, "FarZ", 1000.0f);
        const float fov = readFloat(element, "Fov", 45.0f) * DEG_TO_RAD;

        Camera camera;
        camera.initialize(position, target, up, fov, nearZ, farZ, width, height);
        scene.setCamera(camera);
    }

    void loadPrimitive(SceneObject& object, const XMLElement* element)
    {
        const std::string_view type = element->Name();
        // 球和圆盘以局部原点为中心，位置和朝向由物体变换决定。
        if (type == "Sphere")
        {
            const float radius = readFloat(element, "Radius", 1.0f);
            object.createPrimitive<Sphere>(Vector3f{ 0.0f }, radius);
        }
        else if (type == "Circle" || type == "Disk")
        {
            const float radius = readFloat(element, "Radius", 1.0f);
            object.createPrimitive<Circle>(Vector3f{ 0.0f }, Vector3f{ 0.0f }, radius);
        }
        else if (type == "Triangle")
        {
            Vector3f vertices[3];
            const auto* vertex = element->FirstChildElement("Vertex");
            for (auto& position : vertices)
            {
                if (!vertex)
                    throw std::runtime_error("Triangle requires three <Vertex> elements");
                position = readVector(vertex);
                vertex = vertex->NextSiblingElement("Vertex");
            }
            object.createPrimitive<Triangle>(vertices[0], vertices[1], vertices[2]);
        }
        else
            throw std::runtime_error("Unknown primitive: " + std::string(type));
    }

    void loadObjects(Scene& scene, const XMLElement* elements)
    {
        for (const auto* element = elements->FirstChildElement("SceneObject"); element;
            element = element->NextSiblingElement("SceneObject"))
        {
            Vector3f position{ 0.0f }, rotation{ 0.0f };
            float scale = 1.0f;
            if (const auto* transform = element->FirstChildElement("Transform"))
            {
                if (const auto* value = transform->FirstChildElement("Position"))
                    position = readVector(value);
                if (const auto* value = transform->FirstChildElement("Rotation"))
                    rotation = readVector(value) * DEG_TO_RAD;
                scale = readFloat(transform, "Scale", 1.0f);
            }

            auto* object = scene.createSceneObject(position, rotation, scale);
            const auto* primitives = requiredChild(element, "Primitives");
            for (const auto* primitive = primitives->FirstChildElement(); primitive;
                primitive = primitive->NextSiblingElement())
                loadPrimitive(*object, primitive);
        }
    }

    void loadLights(Scene& scene, const XMLElement* elements)
    {
        for (const auto* element = elements->FirstChildElement(); element;
            element = element->NextSiblingElement())
        {
            const std::string_view type = element->Name();
            if (type == "DirectionalLight")
            {
                const auto direction = readVector(requiredChild(element, "Direction"));
                const auto radiance = readVector(requiredChild(element, "Radiance"));
                scene.createSceneLight<DirectionalLight>(direction, radiance);
            }
            else if (type == "PointLight" || type == "SpotLight")
            {
                const auto position = readVector(requiredChild(element, "Position"));
                const auto intensity = readVector(requiredChild(element, "Intensity"));
                const auto attenuations = readVector(requiredChild(element, "Attenuations"));
                if (type == "PointLight")
                    scene.createSceneLight<PointLight>(position, intensity, attenuations);
                else
                {
                    const auto direction = readVector(requiredChild(element, "Direction"));
                    const float innerAngle = readFloat(element, "InnerAngle", 15.0f) * DEG_TO_RAD;
                    const float outerAngle = readFloat(element, "OuterAngle", 30.0f) * DEG_TO_RAD;
                    scene.createSceneLight<SpotLight>(position, direction, intensity,
                        attenuations, innerAngle, outerAngle);
                }
            }
            else
                throw std::runtime_error("Unknown light: " + std::string(type));
        }
    }
}

std::unique_ptr<Scene> SceneLoader::loadSceneFromXML(const char* filepath, int width, int height) const
{
    tinyxml2::XMLDocument document;
    if (document.LoadFile(filepath) != tinyxml2::XML_SUCCESS)
        throw std::runtime_error(std::string(filepath) + ": " + document.ErrorStr());

    const auto* root = document.FirstChildElement("Scene");
    if (!root)
        throw std::runtime_error("Missing <Scene> root");

    auto scene = std::make_unique<Scene>();
    loadCamera(*scene, requiredChild(root, "Camera"), width, height);
    loadObjects(*scene, requiredChild(root, "SceneObjects"));
    if (const auto* lights = root->FirstChildElement("Lights"))
        loadLights(*scene, lights);
    return scene;
}
