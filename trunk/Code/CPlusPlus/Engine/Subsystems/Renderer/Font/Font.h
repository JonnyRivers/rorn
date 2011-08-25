#pragma once

#include <map>

#include "Glyph.h"

namespace Rorn
{
	namespace Engine
	{
		class Font
		{
		public:
			Font(const std::map<unsigned int, Glyph>& glyphMap, unsigned int textureId);
			~Font();

			const Glyph& GetGlyph(unsigned int characterCode) const;
		private:
			Font(Font&);
			Font& operator=(Font&);

			std::map<unsigned int, Glyph> glyphMap_;
			unsigned int textureId_;
		};
	}
}