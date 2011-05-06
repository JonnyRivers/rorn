#include "Font.h"

#include "../FileManager/FileReader.h"
#include "../TextureManager/TextureManager.h"

using namespace Rorn::Engine;

Font::Font()
{
}

bool Font::Load(const wchar_t* fontPathname)
{
	FileReader fileReader(fontPathname);

	if( !fileReader.IsFileHandleValid() )
		return false;// better error handling/logging required

	char fileIdentifierBuffer[9];
	fileReader.ReadData(fileIdentifierBuffer, 8);
	fileIdentifierBuffer[8] = '\0';

	if( strcmp(fileIdentifierBuffer, "RORNFONT") != 0 )
		return false;// better error handling/logging required

	int versionNumber = fileReader.ReadInt();

	if( versionNumber != 1 )
		return false;// better error handling/logging required

	unsigned int numGlyphs = fileReader.ReadUInt();
	for(unsigned int glyphIndex = 0 ; glyphIndex != numGlyphs ; ++glyphIndex)
	{
		unsigned int characterCode = fileReader.ReadUInt();
		unsigned int x = fileReader.ReadUInt();
		unsigned int y = fileReader.ReadUInt();
		unsigned int width = fileReader.ReadUInt();
		unsigned int height = fileReader.ReadUInt();
		float startU = fileReader.ReadFloat();
		float startV = fileReader.ReadFloat();
		float endU = fileReader.ReadFloat();
		float endV = fileReader.ReadFloat();

		glyphs_.insert(
			std::make_pair<unsigned int, Glyph>(
				characterCode, 
				Glyph(x, y, width, height, startU, startV, endU, endV)));
	}

	unsigned int textureDataSize = fileReader.ReadUInt();

	char* textureData = new char[textureDataSize];
	fileReader.ReadData(textureData, textureDataSize);
	textureId_ = TextureManager::GetInstance().CreateTexture(textureData, textureDataSize);
	delete [] textureData;

	return true;
}

void Font::Release()
{
}

const Glyph* Font::GetGlyph(unsigned int characterCode) const
{
	std::map<unsigned int, Glyph>::const_iterator findGlyphIter = glyphs_.find(characterCode);

	if( findGlyphIter != glyphs_.end() )
		return &findGlyphIter->second;

	return NULL;
}