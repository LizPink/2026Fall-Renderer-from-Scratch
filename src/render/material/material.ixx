module;

export module material;
import utils;

export class MaterialBase;
export class LambertMaterial;


/****************************************************************************************************
 * CLASS MaterialBase
****************************************************************************************************/
class MaterialBase
{
public:
	MaterialBase()					= default;
	MaterialBase(MaterialBase&)		= delete;
	MaterialBase(MaterialBase&&)	= delete;
	virtual ~MaterialBase()			= default;

	virtual Color BRDF(const Vector3f& wo, const Vector3f& wi) const = 0;
};


/****************************************************************************************************
 * CLASS LambertMaterial
****************************************************************************************************/
class LambertMaterial : public MaterialBase
{
private:
	Color myAlbedo;

public:
	LambertMaterial()					= delete;
	LambertMaterial(LambertMaterial&)	= delete;
	LambertMaterial(LambertMaterial&&)	= delete;
	LambertMaterial(Color& albedo);

	/*************************
	* @brief 计算物体材质
	* @param wo 出射方向（物体局部坐标系）
	* @param wi 入射方向（物体局部坐标系）
	*************************/
	virtual Color BRDF(const Vector3f& wo, const Vector3f& wi) const override;
};