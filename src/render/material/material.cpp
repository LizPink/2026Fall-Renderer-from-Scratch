module;
#include<glm/glm.hpp>

module material;
import utils;


/****************************************************************************************************
 * CLASS LambertMaterial Implementation
****************************************************************************************************/
LambertMaterial::LambertMaterial(Color& albedo)
	: myAlbedo(albedo)
{}


Color LambertMaterial::BRDF(const Vector3f & wo, const Vector3f & wi) const
{
	return myAlbedo * INV_PI;
}
