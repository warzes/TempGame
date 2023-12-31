#pragma once

#include "Material.h"
#include "MeshScaleAxis.h"
#include "Loader.h"

class LoaderMaterial final : public Loader<nMaterial> {
public:
	~LoaderMaterial() override = default;

	std::shared_ptr<nMaterial> loadFromFile(const std::string&, const std::map<std::string, std::string, std::less<>>&) override;

private:
	UvScaleType toUvScaleType(std::string_view, const std::string&) const;
	MeshScaleAxis toMeshScaleAxis(std::string_view, const std::string&) const;

	static constexpr char UV_SCALE_NONE[] = "NONE";
	static constexpr char SCALE_ON_AXIS_ALIGNED_FACES[] = "SCALE_ON_AXIS_ALIGNED_FACES";
	static constexpr char SCALE_ON_XY_FACES[] = "SCALE_ON_XY_FACES";
	static constexpr char SCALE_ON_XZ_FACES[] = "SCALE_ON_XZ_FACES";
	static constexpr char SCALE_ON_YZ_FACES[] = "SCALE_ON_YZ_FACES";
	static constexpr char SCALE_ON_MESH_SCALE[] = "SCALE_ON_MESH_SCALE";

	static constexpr char MESH_SCALE_AXIS_X[] = "X";
	static constexpr char MESH_SCALE_AXIS_Y[] = "Y";
	static constexpr char MESH_SCALE_AXIS_Z[] = "Z";
	static constexpr char MESH_SCALE_AXIS_NONE[] = "NONE";
};