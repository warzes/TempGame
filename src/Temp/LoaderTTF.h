#pragma once

#include "Font.h"
#include "Loader.h"

class LoaderTTF final : public Loader<Font> {
public:
	~LoaderTTF() override = default;

	std::shared_ptr<Font> loadFromFile(const std::string&, const std::map<std::string, std::string, std::less<>>&) override;
};