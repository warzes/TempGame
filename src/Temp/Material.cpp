#include "stdafx.h"
#include "Material.h"
#include "MaterialBuilder.h"

nMaterial::nMaterial(const MaterialBuilder& materialBuilder) 
	: m_albedoTexture(materialBuilder.GetAlbedoTexture())
	, m_hasTransparency(materialBuilder.HasTransparency())
	, m_repeatTextures(materialBuilder.RepeatTextures())
	, m_uvScale(materialBuilder.GetUvScale())
	, m_normalTexture(materialBuilder.GetNormalTexture())
	, m_roughnessTexture(materialBuilder.GetRoughnessTexture())
	, m_metalnessTexture(materialBuilder.GetMetalnessTexture())
	, m_emissiveFactor(materialBuilder.GetEmissiveFactor())
	, m_ambientFactor(materialBuilder.GetAmbientFactor())
	, m_depthTestEnabled(materialBuilder.IsDepthTestEnabled())
	, m_depthWriteEnabled(materialBuilder.IsDepthWriteEnabled())
{
	if (!m_normalTexture)
	{
		// TODO: дефолтные ресурсы движка
		/*
		default normal texture:
			std::vector<unsigned char> defaultNormalColor({127, 127, 255, 255});
			return Texture::build("defaultNormal", 1, 1, TextureFormat::RGBA_8_INT, defaultNormalColor, TextureDataType::INT_8);
		*/
		//m_normalTexture = ResourceRetriever::instance().getResource<Texture>(LoaderTexture::DEFAULT_TEXTURE_FILENAME, { {"textureType", "normal"} });
	}
	if (!m_roughnessTexture) 
	{
		// TODO: дефолтные ресурсы движка
		/*
		default roughness texture:
			std::vector<unsigned char> defaultRoughnessValue({255});
			 return Texture::build("defaultRoughness", 1, 1, TextureFormat::GRAYSCALE_8_INT, defaultRoughnessValue, TextureDataType::INT_8);
		*/
		//m_roughnessTexture = ResourceRetriever::instance().getResource<Texture>(LoaderTexture::DEFAULT_TEXTURE_FILENAME, { {"textureType", "roughness"} });
	}
	if (!m_metalnessTexture) 
	{
		// TODO: дефолтные ресурсы движка
		/*
		default metalness texture:
			std::vector<unsigned char> defaultMetalnessValue({0});
			 return Texture::build("defaultMetalness", 1, 1, TextureFormat::GRAYSCALE_8_INT, defaultMetalnessValue, TextureDataType::INT_8);
		*/
		//m_metalnessTexture = ResourceRetriever::instance().getResource<Texture>(LoaderTexture::DEFAULT_TEXTURE_FILENAME, { {"textureType", "metalness"} });
	}
}

const Texture2DRef& nMaterial::GetAlbedoTexture() const
{
	return m_albedoTexture;
}

bool nMaterial::HasTransparency() const 
{
	return m_hasTransparency;
}

bool nMaterial::RepeatTextures() const 
{
	return m_repeatTextures;
}

const UvScale& nMaterial::GetUvScale() const 
{
	return m_uvScale;
}

const Texture2DRef& nMaterial::GetNormalTexture() const 
{
	return m_normalTexture;
}

const Texture2DRef& nMaterial::GetRoughnessTexture() const 
{
	return m_roughnessTexture;
}

const Texture2DRef& nMaterial::GetMetalnessTexture() const 
{
	return m_metalnessTexture;
}

float nMaterial::GetEmissiveFactor() const 
{
	return m_emissiveFactor;
}

float nMaterial::GetAmbientFactor() const 
{
	return m_ambientFactor;
}

bool nMaterial::IsDepthTestEnabled() const 
{
	return m_depthTestEnabled;
}

bool nMaterial::IsDepthWriteEnabled() const 
{
	return m_depthWriteEnabled;
}