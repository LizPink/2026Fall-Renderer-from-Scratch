module;

export module light;
import std;
import utils;
import ray;

export class LightBase;
export class DirectionalLight;
export class PointLight;
export class SpotLight;


enum class LightType
{ DistantLight, PointLight, SpotLight };

/****************************************************************************************************
 * @brief 光源的基类
****************************************************************************************************/
class LightBase
{
public:
	LightBase() = default;
	virtual ~LightBase() = default;

	/// 对于空间中的物体点 point，求解光源到该点方向上的光照亮度 Radiance，返回 Color, sourcePoint.
	virtual Color getRadiance(const Vector3f& position, Vector3f& sourcePoint) = 0;
};


/****************************************************************************************************
 * @brief 平行光.
****************************************************************************************************/
class DirectionalLight : public LightBase
{
public:
	DirectionalLight()							= delete;
	DirectionalLight(DirectionalLight&)			= delete;
	DirectionalLight(const DirectionalLight&)	= delete;
	DirectionalLight(DirectionalLight&&)		= delete;
	DirectionalLight(const Vector3f& direction, const Color& radiance);

	Color getRadiance(const Vector3f& point, Vector3f& sourcePoint) override;

private:
	Vector3f	myDirection;	///< 光线方向（单位向量）（世界坐标）
	Color		myRadiance;		///< 光线亮度
};


/****************************************************************************************************
 *	@brief 点光源.
****************************************************************************************************/
class PointLight : public LightBase
{
private:
	Vector3f	myPosition;		///< 光源位置（世界坐标系）
	Color		myIntensity;	///< 光照方向强度
	Vector3f	myAttenuations;	///< 光照衰减系数

public:
	PointLight() = delete;
	PointLight(PointLight&) = delete;
	PointLight(const PointLight&) = delete;
	PointLight(PointLight&&) = delete;
	PointLight(const Vector3f& position, const Color& intensity, const Vector3f& attenuations);

	Color getRadiance(const Vector3f& point, Vector3f& sourcePoint) override;
};


/****************************************************************************************************
 *	@brief 聚光灯
****************************************************************************************************/
class SpotLight: public LightBase
{
private:
	Vector3f	myPosition;		///< 光源位置（世界坐标系）
	Vector3f	myDirection;	///< 光源方向
	Color		myIntensity;	///< 光照方向强度
	Vector3f	myAttenuations;	///< 光照衰减系数

	float myInnerAngle;			///< 内锥角（弧度）
	float myOuterAngle;			///< 外锥角（弧度）
	float myCosInnerAngle;
	float myCosOuterAngle;

public:
	SpotLight()					= delete;
	SpotLight(SpotLight&)		= delete;
	SpotLight(const SpotLight&) = delete;
	SpotLight(SpotLight&&)		= delete;
	SpotLight(const Vector3f& position, const Vector3f direction, const Vector3f& intensity, const Vector3f& attenuations, const float& innerAngle, const float& outerAngle);

	Color getRadiance(const Vector3f& point, Vector3f& sourcePoint) override;
};
