module;
#include <glm/glm.hpp>

module light;

/****************************************************************************************************
 * CLASS Directional LightBase
****************************************************************************************************/
DirectionalLight::DirectionalLight(const Vector3f& direction, const Color& radiance)
	: myDirection(glm::normalize(direction)), myRadiance(radiance)
{}


Color DirectionalLight::getRadiance(const Vector3f& point, Vector3f& sourcePoint)
{
	/// 写入光源位置: 平行光光源总是位于无穷远处.
	sourcePoint = point - this->myDirection * 1000000.0f;

	return this->myRadiance;
}


/****************************************************************************************************
 * CLASS PointLight
****************************************************************************************************/
PointLight::PointLight(const Vector3f& position, const Color& intensity, const Vector3f& attenuations)
	: myPosition(position), myIntensity(intensity), myAttenuations(attenuations)
{}


Color PointLight::getRadiance(const Vector3f & point, Vector3f & sourcePoint)
{
	sourcePoint = this->myPosition;

	float R = glm::length(sourcePoint - point);
	float attenuation = 1.0f / (myAttenuations[0]*R*R + myAttenuations[1]*R + myAttenuations[2]);
	Color radiance = myIntensity * attenuation;
	
	return radiance;
}


/****************************************************************************************************
 * CLASS SpotLight 
****************************************************************************************************/
SpotLight::SpotLight(const Vector3f& position, const Vector3f direction, const Vector3f& intensity, const Vector3f& attenuations, const float& innerAngle, const float& outerAngle)
	: myPosition(position), myDirection(glm::normalize(direction)), myIntensity(intensity), myAttenuations(attenuations), myInnerAngle(innerAngle), myOuterAngle(outerAngle), myCosInnerAngle(cos(innerAngle)), myCosOuterAngle(cos(outerAngle))
{}


Color SpotLight::getRadiance(const Vector3f& point, Vector3f& sourcePoint)
{
	sourcePoint = myPosition;

	/// 距离衰减系数 K1
	float R = glm::length(point - sourcePoint);
	float K1 = 1.0f / (myAttenuations.x * R * R + myAttenuations.y * R + myAttenuations.z);

	// 角度衰减系数 K2
	Vector3f L = glm::normalize(point - myPosition);
	float cosineTheta = glm::dot(L, myDirection);
	float K2 = glm::clamp((cosineTheta - myCosOuterAngle) / (myCosInnerAngle - myCosOuterAngle), 0.0f, 1.0f);

	/// 返回计算结果
	return myIntensity * K1 * K2;
}
