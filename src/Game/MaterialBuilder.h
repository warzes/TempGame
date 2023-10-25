#pragma once

#include "Material.h"

class MaterialBuilder : public std::enable_shared_from_this<MaterialBuilder>
{
public:
	static std::shared_ptr<MaterialBuilder> Create(std::string, Texture2DRef, bool);

	const std::string& GetMaterialName() const;

	const Texture2DRef& GetAlbedoTexture() const;
	bool HasTransparency() const;

	std::shared_ptr<MaterialBuilder> EnableRepeatTextures();
	bool RepeatTextures() const;

	std::shared_ptr<MaterialBuilder> NormalTexture(Texture2DRef);
	const Texture2DRef& GetNormalTexture() const;

	std::shared_ptr<MaterialBuilder> RoughnessTexture(Texture2DRef);
	const Texture2DRef& GetRoughnessTexture() const;

	std::shared_ptr<MaterialBuilder> MetalnessTexture(Texture2DRef);
	const Texture2DRef& GetMetalnessTexture() const;

	std::shared_ptr<MaterialBuilder> UVScale(const UvScale&);
	const UvScale& GetUvScale() const;

	std::shared_ptr<MaterialBuilder> EmissiveFactor(float);
	float GetEmissiveFactor() const;

	std::shared_ptr<MaterialBuilder> AmbientFactor(float);
	float GetAmbientFactor() const;

	std::shared_ptr<MaterialBuilder> DisableDepthTest();
	bool IsDepthTestEnabled() const;
	std::shared_ptr<MaterialBuilder> DisableDepthWrite();
	bool IsDepthWriteEnabled() const;

	std::unique_ptr<nMaterial> Build();

private:
	MaterialBuilder(std::string, Texture2DRef, bool);

	static unsigned long nextId;

	std::string m_materialName;
	Texture2DRef m_albedoTexture;
	bool m_hasTransparency;

	bool m_repeatTextures;
	UvScale m_uvScale;
	Texture2DRef m_normalTexture;
	Texture2DRef m_roughnessTexture;
	Texture2DRef m_metalnessTexture;

	float m_emissiveFactor;
	float m_ambientFactor;

	bool m_depthTestEnabled;
	bool m_depthWriteEnabled;
};