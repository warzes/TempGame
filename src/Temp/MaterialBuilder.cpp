#include "stdafx.h"
#include "MaterialBuilder.h"

//static
unsigned long MaterialBuilder::nextId = 0;

MaterialBuilder::MaterialBuilder(std::string materialName, Texture2DRef albedoTexture, bool hasTransparency) 
	: m_materialName(std::move(materialName))
	, m_albedoTexture(std::move(albedoTexture))
	, m_hasTransparency(hasTransparency)
	, m_repeatTextures(false)
	, m_uvScale(UvScale(UvScaleType::NONE))
	, m_emissiveFactor(0.0f)
	, m_ambientFactor(0.5f)
	, m_depthTestEnabled(true)
	, m_depthWriteEnabled(true)
{
}

std::shared_ptr<MaterialBuilder> MaterialBuilder::Create(std::string materialName, Texture2DRef albedoTexture, bool hasTransparency)
{
	if (!albedoTexture) 
		throw std::runtime_error("Albedo texture is mandatory to build a material");

	return std::shared_ptr<MaterialBuilder>(new MaterialBuilder(std::move(materialName), std::move(albedoTexture), hasTransparency));
}

const std::string& MaterialBuilder::GetMaterialName() const 
{
	return m_materialName;
}

const Texture2DRef& MaterialBuilder::GetAlbedoTexture() const {
	return m_albedoTexture;
}

bool MaterialBuilder::HasTransparency() const {
	return m_hasTransparency;
}

std::shared_ptr<MaterialBuilder> MaterialBuilder::EnableRepeatTextures()
{
	m_repeatTextures = true;
	return shared_from_this();
}

bool MaterialBuilder::RepeatTextures() const
{
	return m_repeatTextures;
}

std::shared_ptr<MaterialBuilder> MaterialBuilder::NormalTexture(Texture2DRef normalTexture)
{
	m_normalTexture = std::move(normalTexture);
	return shared_from_this();
}

const Texture2DRef& MaterialBuilder::GetNormalTexture() const
{
	return m_normalTexture;
}

std::shared_ptr<MaterialBuilder> MaterialBuilder::RoughnessTexture(Texture2DRef roughnessTexture)
{
	m_roughnessTexture = std::move(roughnessTexture);
	return shared_from_this();
}

const Texture2DRef& MaterialBuilder::GetRoughnessTexture() const
{
	return m_roughnessTexture;
}

std::shared_ptr<MaterialBuilder> MaterialBuilder::MetalnessTexture(Texture2DRef metalnessTexture)
{
	m_metalnessTexture = std::move(metalnessTexture);
	return shared_from_this();
}

const Texture2DRef& MaterialBuilder::GetMetalnessTexture() const
{
	return m_metalnessTexture;
}

std::shared_ptr<MaterialBuilder> MaterialBuilder::UVScale(const UvScale& uvScale)
{
	m_uvScale = uvScale;
	return shared_from_this();
}

const UvScale& MaterialBuilder::GetUvScale() const
{
	return m_uvScale;
}

std::shared_ptr<MaterialBuilder> MaterialBuilder::EmissiveFactor(float emissiveFactor)
{
	if (emissiveFactor > nMaterial::MAX_EMISSIVE_FACTOR) {
		throw std::runtime_error("Emissive factor (" + std::to_string(emissiveFactor) + ") is above the maximum (" + std::to_string(nMaterial::MAX_EMISSIVE_FACTOR) + ")");
	}

	m_emissiveFactor = emissiveFactor;
	return shared_from_this();
}

float MaterialBuilder::GetEmissiveFactor() const
{
	return m_emissiveFactor;
}

std::shared_ptr<MaterialBuilder> MaterialBuilder::AmbientFactor(float ambientFactor)
{
	m_ambientFactor = ambientFactor;
	return shared_from_this();
}

float MaterialBuilder::GetAmbientFactor() const
{
	return m_ambientFactor;
}

std::shared_ptr<MaterialBuilder> MaterialBuilder::DisableDepthTest()
{
	m_depthTestEnabled = false;
	return shared_from_this();
}

bool MaterialBuilder::IsDepthTestEnabled() const
{
	return m_depthTestEnabled;
}

std::shared_ptr<MaterialBuilder> MaterialBuilder::DisableDepthWrite()
{
	m_depthWriteEnabled = false;
	return shared_from_this();
}

bool MaterialBuilder::IsDepthWriteEnabled() const
{
	return m_depthWriteEnabled;
}

std::unique_ptr<nMaterial> MaterialBuilder::Build()
{
	auto material = std::make_unique<nMaterial>(*this);
	material->setId("_" + std::to_string(nextId++));
	material->setName(m_materialName);
	return material;
}