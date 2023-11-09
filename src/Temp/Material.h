#pragma once

#include "Resource.h"
#include "UvScale.h"

class MaterialBuilder;

class nMaterial : public Resource
{
public:
	explicit nMaterial(const MaterialBuilder&);
	~nMaterial() override = default;

	static constexpr float MAX_EMISSIVE_FACTOR = 20.0f;

	const Texture2DRef& GetAlbedoTexture() const;
	bool HasTransparency() const;

	bool RepeatTextures() const;
	const UvScale& GetUvScale() const;
	const Texture2DRef& GetNormalTexture() const;
	const Texture2DRef& GetRoughnessTexture() const;
	const Texture2DRef& GetMetalnessTexture() const;

	float GetEmissiveFactor() const;
	float GetAmbientFactor() const;

	bool IsDepthTestEnabled() const;
	bool IsDepthWriteEnabled() const;

private:
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