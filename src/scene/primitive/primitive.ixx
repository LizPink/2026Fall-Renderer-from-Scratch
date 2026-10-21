module;

export module primitive;
import ray;

export class Primitive
{
public:
	virtual bool intersect(Ray& ray, Intersection& intersect) const = 0;
};