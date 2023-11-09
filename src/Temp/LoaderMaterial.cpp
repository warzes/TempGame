#include "stdafx.h"
#include "LoaderMaterial.h"
#include "UdaParser.h"
#include "MaterialBuilder.h"

std::shared_ptr<nMaterial> LoaderMaterial::loadFromFile(const std::string& filename, const std::map<std::string, std::string, std::less<>>&)
{
	auto& renderSystem = GetRenderSystem();

	UdaParser udaParser(filename, UdaLoadType::LOAD_FILE);

	//albedo texture/color
	bool hasTransparency = false;
	Texture2DRef albedoTexture;
	auto albedoChunk = udaParser.getFirstChunk(true, "albedo");
	auto albedoTextureChunk = udaParser.getFirstChunk(false, "texture", UdaAttribute(), albedoChunk);
	if (albedoTextureChunk) 
	{
		albedoTexture = renderSystem.CreateTexture2D(albedoTextureChunk->getStringValue().c_str());
		hasTransparency = albedoTexture->hasTransparency;
	}

	auto albedoColorChunk = udaParser.getFirstChunk(false, "color", UdaAttribute(), albedoChunk);
	if (albedoColorChunk) 
	{
		if (albedoTexture) 
			throw std::runtime_error("Material defines an albedo while a albedo texture is defined: " + filename);

		Vector4<float> color = albedoColorChunk->getVector4Value();
		if (color.X > 1.0f || color.Y > 1.0f || color.Z > 1.0f || color.W > 1.0f
			|| color.X < 0.0f || color.Y < 0.0f || color.Z < 0.0f || color.W < 0.0f) 
		{
			throw std::runtime_error("Material albedo must be in range 0.0 - 1.0: " + filename);
		}

		std::vector<unsigned char> rgbaColor({ (unsigned char)(255.0f * color.X), (unsigned char)(255.0f * color.Y),
		(unsigned char)(255.0f * color.Z), (unsigned char)(255.0f * color.W) });
		hasTransparency = !MathFunction::isOne(color.W);

		const Texture2DCreateInfo createInfo = {
			.format = TexelsFormat::RGBA_U8,
			.width = 1,
			.height = 1,
			.pixelData = rgbaColor.data(),
			.hasTransparency = hasTransparency
		};
		const Texture2DInfo textureInfo = {};

		albedoTexture = renderSystem.CreateTexture2D(createInfo, textureInfo);
	}
	std::shared_ptr<MaterialBuilder> materialBuilder = MaterialBuilder::Create(filename, albedoTexture, hasTransparency);

	//repeat textures
	auto repeatTexturesChunk = udaParser.getFirstChunk(false, "repeatTextures");
	if (repeatTexturesChunk && repeatTexturesChunk->getBoolValue())
	{
		materialBuilder->EnableRepeatTextures();
	}

	//normal texture
	auto normalChunk = udaParser.getFirstChunk(false, "normal");
	if (normalChunk)
	{
		auto normalTextureChunk = udaParser.getFirstChunk(true, "texture", UdaAttribute(), normalChunk);
		auto normalTexture = renderSystem.CreateTexture2D(normalTextureChunk->getStringValue().c_str());
		materialBuilder->NormalTexture(normalTexture);
	}

	//roughness texture/value
	auto roughnessChunk = udaParser.getFirstChunk(false, "roughness");
	if (roughnessChunk) 
	{
		Texture2DRef roughnessTexture;

		auto roughnessTextureChunk = udaParser.getFirstChunk(false, "texture", UdaAttribute(), roughnessChunk);
		if (roughnessTextureChunk) 
		{
			roughnessTexture = renderSystem.CreateTexture2D(roughnessTextureChunk->getStringValue().c_str());
			//if (roughnessTexture->getFormat() != TextureFormat::GRAYSCALE_8_INT && roughnessTexture->getFormat() != TextureFormat::GRAYSCALE_16_FLOAT) 
			//{
			//	throw std::runtime_error("Material defines a roughness texture not in grayscale: " + filename);
			//}
		}

		auto roughnessValueChunk = udaParser.getFirstChunk(false, "value", UdaAttribute(), roughnessChunk);
		if (roughnessValueChunk)
		{
			if (roughnessTexture)
				throw std::runtime_error("Material defines a roughness value while a roughness texture is defined: " + filename);

			float roughness = roughnessValueChunk->getFloatValue();
			if (roughness > 1.0f || roughness < 0.05f)
				throw std::runtime_error("Material roughness must be in range 0.05 - 1.0: " + filename);

			std::vector<unsigned char> roughnessTextureData({ (unsigned char)(255.0f * roughness) });

			const Texture2DCreateInfo createInfo = {
				.format = TexelsFormat::R_U8,
				.width = 1,
				.height = 1,
				.pixelData = roughnessTextureData.data()
			};
			const Texture2DInfo textureInfo = {};
			roughnessTexture = renderSystem.CreateTexture2D(createInfo, textureInfo);
		}

		if (roughnessTexture) {
			materialBuilder->RoughnessTexture(roughnessTexture);
		}
	}

	//metalness texture/value
	auto metalnessChunk = udaParser.getFirstChunk(false, "metalness");
	if (metalnessChunk)
	{
		Texture2DRef metalnessTexture;

		auto metalnessTextureChunk = udaParser.getFirstChunk(false, "texture", UdaAttribute(), metalnessChunk);
		if (metalnessTextureChunk)
		{
			metalnessTexture = renderSystem.CreateTexture2D(metalnessTextureChunk->getStringValue().c_str());
			//if (metalnessTexture->GetFormat() != TextureFormat::GRAYSCALE_8_INT && metalnessTexture->getFormat() != TextureFormat::GRAYSCALE_16_FLOAT)
			//	throw std::runtime_error("Material defines a metalness texture not in grayscale: " + filename);
		}

		auto metalnessValueChunk = udaParser.getFirstChunk(false, "value", UdaAttribute(), metalnessChunk);
		if (metalnessValueChunk) {
			if (metalnessTexture) {
				throw std::runtime_error("Material defines a metalness value while a metalness texture is defined: " + filename);
			}

			float metalness = metalnessValueChunk->getFloatValue();
			if (metalness > 1.0f || metalness < 0.0f) {
				throw std::runtime_error("Material metalness must be in range 0.0 - 1.0: " + filename);
			}

			std::vector<unsigned char> metalnessTextureData({ (unsigned char)(255.0f * metalness) });

			const Texture2DCreateInfo createInfo = {
				.format = TexelsFormat::R_U8,
				.width = 1,
				.height = 1,
				.pixelData = metalnessTextureData.data()
			};
			const Texture2DInfo textureInfo = {};
			metalnessTexture = renderSystem.CreateTexture2D(createInfo, textureInfo);
		}
		if (metalnessTexture) 
		{
			materialBuilder->MetalnessTexture(metalnessTexture);
		}
	}

	//UV scale
	auto uvScaleChunk = udaParser.getFirstChunk(false, "uvScale");
	if (uvScaleChunk) {
		std::string scaleTypeValue = udaParser.getFirstChunk(true, "scaleType", UdaAttribute(), uvScaleChunk)->getStringValue();
		UvScaleType uvScaleType = toUvScaleType(scaleTypeValue, filename);

		MeshScaleAxis uMeshScaleAxis = MeshScaleAxis::NONE;
		MeshScaleAxis vMeshScaleAxis = MeshScaleAxis::NONE;
		if (uvScaleType == UvScaleType::SCALE_ON_MESH_SCALE) {
			std::string uMeshScaleAxisValue = udaParser.getFirstChunk(true, "uMeshScaleAxis", UdaAttribute(), uvScaleChunk)->getStringValue();
			uMeshScaleAxis = toMeshScaleAxis(uMeshScaleAxisValue, filename);
			std::string vScaleMeshValue = udaParser.getFirstChunk(true, "vMeshScaleAxis", UdaAttribute(), uvScaleChunk)->getStringValue();
			vMeshScaleAxis = toMeshScaleAxis(vScaleMeshValue, filename);
		}

		materialBuilder->UVScale(UvScale(uvScaleType, uMeshScaleAxis, vMeshScaleAxis));
	}

	//emissive factor
	auto emissiveFactorChunk = udaParser.getFirstChunk(false, "emissiveFactor");
	if (emissiveFactorChunk) {
		materialBuilder->EmissiveFactor(emissiveFactorChunk->getFloatValue());
	}

	//ambient factor
	auto ambientFactorChunk = udaParser.getFirstChunk(false, "ambientFactor");
	if (ambientFactorChunk) {
		materialBuilder->AmbientFactor(ambientFactorChunk->getFloatValue());
	}

	//depth test
	auto depthTestChunk = udaParser.getFirstChunk(false, "depthTest");
	if (depthTestChunk && !depthTestChunk->getBoolValue()) {
		materialBuilder->DisableDepthTest();
	}

	//depth write
	auto depthWriteChunk = udaParser.getFirstChunk(false, "depthWrite");
	if (depthWriteChunk && !depthWriteChunk->getBoolValue()) {
		materialBuilder->DisableDepthWrite();
	}

	return materialBuilder->Build();
}

UvScaleType LoaderMaterial::toUvScaleType(std::string_view scaleTypeValue, const std::string& filename) const
{
	if (scaleTypeValue == UV_SCALE_NONE) return UvScaleType::NONE;
	else if (scaleTypeValue == SCALE_ON_AXIS_ALIGNED_FACES) return UvScaleType::SCALE_ON_AXIS_ALIGNED_FACES;
	else if (scaleTypeValue == SCALE_ON_XY_FACES) return UvScaleType::SCALE_ON_XY_FACES;
	else if (scaleTypeValue == SCALE_ON_XZ_FACES) return UvScaleType::SCALE_ON_XZ_FACES;
	else if (scaleTypeValue == SCALE_ON_YZ_FACES) return UvScaleType::SCALE_ON_YZ_FACES;
	else if (scaleTypeValue == SCALE_ON_MESH_SCALE) return UvScaleType::SCALE_ON_MESH_SCALE;

	throw std::runtime_error("Unknown UV scale value '" + std::string(scaleTypeValue) + "' for material: " + filename);
}

MeshScaleAxis LoaderMaterial::toMeshScaleAxis(std::string_view meshScaleAxisValue, const std::string& filename) const 
{
	if (meshScaleAxisValue == MESH_SCALE_AXIS_X) return MeshScaleAxis::X;
	else if (meshScaleAxisValue == MESH_SCALE_AXIS_Y) return MeshScaleAxis::Y;
	else if (meshScaleAxisValue == MESH_SCALE_AXIS_Z) return MeshScaleAxis::Z;
	else if (meshScaleAxisValue == MESH_SCALE_AXIS_NONE) return MeshScaleAxis::NONE;

	throw std::runtime_error("Unknown mesh scale axis value '" + std::string(meshScaleAxisValue) + "' for material: " + filename);
}