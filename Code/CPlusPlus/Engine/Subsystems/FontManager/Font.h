#pragma once

#include <map>

#include "Glyph.h"

namespace Rorn
{
	namespace Engine
	{
		class Font
		{
			friend class FontManager;

		public:
			void Release();

			int GetTextureId() const { return textureId_; }
			const Glyph* GetGlyph(unsigned int characterCode) const;
		private:
			Font();// Only FontManager has access

			bool Load(const wchar_t* fontPathname);// Only FontManager has access

			std::map<unsigned int, Glyph> glyphs_;
			int textureId_;
		};
	}
}