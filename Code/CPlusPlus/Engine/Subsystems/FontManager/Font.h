#pragma once

#include <map>

#include <windows.h>

#include <d3d11.h>
#include <d3dx11.h>

namespace Rorn
{
	namespace Engine
	{
		class Font
		{
			friend class FontManager;

		public:
			void Release();
		private:
			Font();

			bool Load(const wchar_t* fontPathname);
			// font data
		};
	}
}