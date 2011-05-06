#include "FontManager.h"

#include <cassert>
#include <sstream>

#include "../DiagnosticsManager/DiagnosticsManager.h"

using namespace Rorn::Engine;

/*static*/ FontManager& FontManager::instance_ = FontManager();// init static instance

/*static*/ FontManager& FontManager::GetInstance()
{
	return instance_;
}

FontManager::FontManager(void)
{
}

HRESULT FontManager::Startup()
{
	DiagnosticsManager::GetInstance().GetLoggingStream() << "The Font Manager is starting up." << std::endl;

	std::wstring debugFontPathname(L"debug.font");
	debugTextFont_ = LoadOrGetFont(debugFontPathname.c_str());
	if( debugTextFont_ == NULL )
	{
		std::wstringstream errorStream;
		errorStream << "Unable to load debug text font from '" << debugFontPathname << "'";
		DiagnosticsManager::GetInstance().ReportError(errorStream.str().c_str());
		return E_FAIL;
	}

	DiagnosticsManager::GetInstance().GetLoggingStream() << "The Font Manager started up successfully." << std::endl;

	return S_OK;
}

void FontManager::Shutdown()
{
	DiagnosticsManager::GetInstance().GetLoggingStream() << "The Font Manager is shutting down." << std::endl;

	std::list<std::unique_ptr<Font>>::iterator fontIter;
	for(fontIter = fonts_.begin() ; fontIter != fonts_ .end() ; ++fontIter)
		(*fontIter)->Release();
	fonts_.clear();

	DiagnosticsManager::GetInstance().GetLoggingStream() << "The Font Manager shut down successfully." << std::endl;
}

void FontManager::Step()
{
}

Font* FontManager::LoadOrGetFont(const wchar_t* fontPathname)
{
	Rorn::Engine::Font* newFont = new Rorn::Engine::Font();
	if( newFont->Load(fontPathname) )
	{
		fonts_.push_back( std::unique_ptr<Font>(newFont) );

		return newFont;
	}

	return NULL;
}
