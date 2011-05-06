#pragma once

#include <list>

#include <windows.h>

#include "Font.h"

namespace Rorn
{
	namespace Engine
	{
		class FontManager
		{
		public:
			static FontManager& GetInstance();

			HRESULT Startup();
			void Shutdown();

			void Step();

			Font* LoadOrGetFont(const wchar_t* fontPathname);
			const Font& GetDebugTextFont() const { return *debugTextFont_; }
		private:
			static FontManager& instance_;

			FontManager(void);

			std::list<std::unique_ptr<Font>> fonts_;
			Font* debugTextFont_;
		};
	}
}