module;
#include <tinyxml2.h>

module scene_loader;

import std;
import utils;
import camera;
import scene_object;
import sphere;
import triangle;
import circle;

namespace
{
    using tinyxml2::XMLElement;

    // 每条错误都带有文件、元素和行号，便于直接定位到 XML。
    [[noreturn]] void fail(const char* filepath, const XMLElement* element,
        const std::string& reason, int documentLine = 0)
    {
        const int line = element ? element->GetLineNum() : documentLine;
        const char* name = element ? element->Name() : "document";
        throw std::runtime_error(std::string(filepath ? filepath : "<null>")
            + ": line " + std::to_string(line) + " <" + name + ">: " + reason);
    }

    const XMLElement* child(const XMLElement* parent, const char* name,
        const char* filepath, bool required = false)
    {
        const auto* element = parent->FirstChildElement(name);
        if (!element && required)
            fail(filepath, parent, std::string("missing <") + name + ">");
        if (element && element->NextSiblingElement(name))
            fail(filepath, element, std::string("duplicate <") + name + ">");
        return element;
    }

    std::istringstream textStream(const XMLElement* element, const char* filepath)
    {
        if (!element->GetText() || element->FirstChildElement())
            fail(filepath, element, "expected a plain numeric value");
        std::istringstream stream(element->GetText());
        stream.imbue(std::locale::classic());
        return stream;
    }

    float readFloat(const XMLElement* parent, const char* name,
        float defaultValue, const char* filepath)
    {
        const auto* element = child(parent, name, filepath);
        if (!element)
            return defaultValue;

        auto stream = textStream(element, filepath);
        float value = 0.0f;
        if (!(stream >> value) || !std::isfinite(value))
            fail(filepath, element, "expected a finite floating-point number");
        stream >> std::ws;
        if (!stream.eof())
            fail(filepath, element, "unexpected characters after the number");
        return value;
    }

    Vector3f readVector(const XMLElement* element, const char* filepath)
    {
        auto stream = textStream(element, filepath);
        float x = 0.0f, y = 0.0f, z = 0.0f;
        char comma1 = 0, comma2 = 0;
        if (!(stream >> x >> comma1 >> y >> comma2 >> z)
            || comma1 != ',' || comma2 != ','
            || !std::isfinite(x) || !std::isfinite(y) || !std::isfinite(z))
            fail(filepath, element, "expected three finite numbers: x, y, z");
        stream >> std::ws;
        if (!stream.eof())
            fail(filepath, element, "expected exactly three components: x, y, z");
        return Vector3f{ x, y, z };
    }

    // 用 double 做校验，避免有限的 float 在求差或叉积时提前溢出。
    double crossLengthSquared(double ax, double ay, double az,
        double bx, double by, double bz)
    {
        const double x = ay * bz - az * by;
        const double y = az * bx - ax * bz;
        const double z = ax * by - ay * bx;
        return x * x + y * y + z * z;
    }

    void loadCamera(Scene& scene, const XMLElement* element,
        const char* filepath, int width, int height)
    {
        const auto position = readVector(child(element, "Position", filepath, true), filepath);
        const auto target = readVector(child(element, "Target", filepath, true), filepath);
        const auto up = readVector(child(element, "Up", filepath, true), filepath);
        const float nearZ = readFloat(element, "NearZ", 0.1f, filepath);
        const float farZ = readFloat(element, "FarZ", 1000.0f, filepath);
        const float fov = readFloat(element, "Fov", 45.0f, filepath);

        if (nearZ <= 0.0f || farZ <= nearZ)
            fail(filepath, element, "require 0 < NearZ < FarZ");
        if (fov <= 0.0f || fov >= 180.0f)
            fail(filepath, element, "Fov must be between 0 and 180 degrees");

        const double dx = double(target.x) - position.x;
        const double dy = double(target.y) - position.y;
        const double dz = double(target.z) - position.z;
        const double directionLength = dx * dx + dy * dy + dz * dz;
        const double upLength = double(up.x) * up.x + double(up.y) * up.y + double(up.z) * up.z;
        if (directionLength == 0.0)
            fail(filepath, element, "Position and Target must be different");
        if (upLength == 0.0
            || crossLengthSquared(dx, dy, dz, up.x, up.y, up.z)
                <= 1e-12 * directionLength * upLength)
            fail(filepath, element, "Up must be nonzero and not parallel to the viewing direction");

        Camera camera;
        camera.initialize(position, target, up, fov * (PI / 180.0f), nearZ, farZ, width, height);
        scene.setCamera(camera);
    }

    void loadPrimitive(SceneObject& object, const XMLElement* element, const char* filepath)
    {
        const std::string_view name(element->Name());
        if (name == "Sphere" || name == "Disk" || name == "Circle")
        {
            const float radius = readFloat(element, "Radius", 1.0f, filepath);
            if (radius <= 0.0f)
                fail(filepath, element, "Radius must be positive");

            // 球和圆盘都以局部原点为中心，位置和朝向统一交给 SceneObject。
            if (name == "Sphere")
                object.createPrimitive<Sphere>(Vector3f{ 0.0f }, radius);
            else
                object.createPrimitive<Circle>(Vector3f{ 0.0f }, Vector3f{ 0.0f }, radius);
        }
        else if (name == "Triangle")
        {
            Vector3f vertices[3];
            int count = 0;
            for (const auto* vertex = element->FirstChildElement(); vertex;
                vertex = vertex->NextSiblingElement())
            {
                if (std::string_view(vertex->Name()) != "Vertex" || count >= 3)
                    fail(filepath, vertex, "Triangle requires exactly three <Vertex> elements");
                vertices[count++] = readVector(vertex, filepath);
            }
            if (count != 3)
                fail(filepath, element, "Triangle requires exactly three <Vertex> elements");

            const auto& a = vertices[0];
            const auto& b = vertices[1];
            const auto& c = vertices[2];
            if (crossLengthSquared(double(b.x) - a.x, double(b.y) - a.y, double(b.z) - a.z,
                double(c.x) - a.x, double(c.y) - a.y, double(c.z) - a.z) == 0.0)
                fail(filepath, element, "Triangle vertices must not be collinear or repeated");
            object.createPrimitive<Triangle>(a, b, c);
        }
        else
            fail(filepath, element, "unsupported primitive; expected Sphere, Triangle, Disk or Circle");
    }
}

std::unique_ptr<Scene> SceneLoader::loadSceneFromXML(
    const char* filepath, int width, int height) const
{
    if (!filepath || filepath[0] == '\0')
        fail(filepath, nullptr, "scene file path must not be empty");
    if (width <= 0 || height <= 0)
        fail(filepath, nullptr, "viewport width and height must be positive");

    tinyxml2::XMLDocument document;
    if (document.LoadFile(filepath) != tinyxml2::XML_SUCCESS)
        fail(filepath, nullptr, document.ErrorStr(), document.ErrorLineNum());

    const auto* root = document.RootElement();
    if (!root || std::string_view(root->Name()) != "Scene")
        fail(filepath, root, "expected <Scene> as the root element");
    if (root->NextSiblingElement())
        fail(filepath, root->NextSiblingElement(), "expected a single <Scene> root element");

    // 局部 unique_ptr 在解析中途出错时，也会释放已经创建的场景和图元。
    auto scene = std::make_unique<Scene>();
    loadCamera(*scene, child(root, "Camera", filepath, true), filepath, width, height);
    const auto* objects = child(root, "SceneObjects", filepath, true);
    for (const auto* element = objects->FirstChildElement(); element;
        element = element->NextSiblingElement())
    {
        if (std::string_view(element->Name()) != "SceneObject")
            fail(filepath, element, "expected <SceneObject>");

        Vector3f position{ 0.0f }, rotation{ 0.0f };
        float scale = 1.0f;
        if (const auto* transform = child(element, "Transform", filepath))
        {
            if (const auto* value = child(transform, "Position", filepath))
                position = readVector(value, filepath);
            if (const auto* value = child(transform, "Rotation", filepath))
                rotation = readVector(value, filepath);
            scale = readFloat(transform, "Scale", 1.0f, filepath);
            if (scale <= 0.0f)
                fail(filepath, transform, "Scale must be positive");
        }

        // XML 用角度便于编辑，项目中的变换函数使用弧度。
        rotation.x *= PI / 180.0f;
        rotation.y *= PI / 180.0f;
        rotation.z *= PI / 180.0f;
        auto* object = scene->createSceneObject(position, rotation, scale);
        const auto* primitives = child(element, "Primitives", filepath, true);
        for (const auto* primitive = primitives->FirstChildElement(); primitive;
            primitive = primitive->NextSiblingElement())
            loadPrimitive(*object, primitive, filepath);
    }
    return scene;
}
