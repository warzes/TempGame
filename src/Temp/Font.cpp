#include "stdafx.h"
#include "Font.h"

Font::Font(unsigned int fontSize, const Vector3<float>& fontColor, Texture2DRef alphabetTexture, std::array<Glyph, UnicodeUtil::NUM_CHARACTERS> glyph,
	unsigned int spaceBetweenCharacters, unsigned int spaceBetweenLines, unsigned int height) :
	fontSize(fontSize),
	fontColor(fontColor),
	alphabetTexture(std::move(alphabetTexture)),
	glyph(std::move(glyph)),
	spaceBetweenCharacters(spaceBetweenCharacters),
	spaceBetweenLines(spaceBetweenLines),
	height(height) {

}

unsigned int Font::getFontSize() const {
	return fontSize;
}

Vector3<float> Font::getFontColor() const {
	return fontColor;
}

const Glyph& Font::getGlyph(char32_t unicodeCharacter) const {
	if (!UnicodeUtil::isCharacterManaged(unicodeCharacter)) {
		throw std::out_of_range("Unicode character " + std::to_string(unicodeCharacter) + " cannot be retrieved (not implemented)");
	}
	return glyph[unicodeCharacter];
}

Texture2DRef Font::getTexture() const {
	return alphabetTexture;
}

unsigned int Font::getDimensionTexture() const {
	return alphabetTexture->width;
}

unsigned int Font::getSpaceBetweenCharacters() const {
	return spaceBetweenCharacters;
}

unsigned int Font::getSpaceBetweenLines() const {
	return spaceBetweenLines;
}

unsigned int Font::getHeight() const {
	return height;
}