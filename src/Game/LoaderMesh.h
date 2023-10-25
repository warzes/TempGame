#pragma once

#include "Loader.h"
#include "ConstMeshes.h"

class LoaderUrchinMesh final : public Loader<ConstMeshes> {
public:
	~LoaderUrchinMesh() override = default;

	std::shared_ptr<ConstMeshes> loadFromFile(const std::string&, const std::map<std::string, std::string, std::less<>>&) override;
};