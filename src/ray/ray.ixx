module;
export module ray;

import utils;


export struct Ray {
public:
	Vector3f o;
	Vector3f d;

	float minT = 0.0f;
	float maxT = FLOAT_MAX;
};