#pragma once

#include "Resource.h"
#include "UnicodeUtil.h"
#include "Vector3.h"

struct Glyph { //glyph is a letter
	unsigned int width;
	unsigned int height;
	int shift;
	std::vector<unsigned char> buf;
};

class Font : public Resource {
public:
	Font(unsigned int, const Vector3<float>&, Texture2DRef, std::array<Glyph, UnicodeUtil::NUM_CHARACTERS>, unsigned int, unsigned int, unsigned int);
	~Font() override = default;

	unsigned int getFontSize() const;
	Vector3<float> getFontColor() const;

	const Glyph& getGlyph(char32_t) const;
	Texture2DRef getTexture() const;
	unsigned int getDimensionTexture() const;
	unsigned int getSpaceBetweenCharacters() const;
	unsigned int getSpaceBetweenLines() const;
	unsigned int getHeight() const;

private:
	unsigned int fontSize;
	Vector3<float> fontColor;

	Texture2DRef alphabetTexture;

	std::array<Glyph, UnicodeUtil::NUM_CHARACTERS> glyph;
	unsigned int spaceBetweenCharacters;
	unsigned int spaceBetweenLines;
	unsigned int height;
};