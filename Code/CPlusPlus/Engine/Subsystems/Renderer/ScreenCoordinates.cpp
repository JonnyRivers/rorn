#pragma once

#include "../../../Shared/ErrorCodes/HResultFormatter.h"

#include "../../Exceptions/initialisation_exception.h"

#include "ScreenCoordinates.h"

using namespace Rorn::Engine;

ScreenCoordinates::ScreenCoordinates(HWND windowHandle, IDiagnostics* diagnostics)
{
	RECT rc;
    if(!::GetClientRect( windowHandle, &rc ))
	{
		diagnostics->GetLoggingStream() << "Unable to create ScreenCoordinates instance." << std::endl;
		throw initialisation_exception("Unable to create ScreenCoordinates instance.");
	}

    width_ = rc.right - rc.left;
    height_ = rc.bottom - rc.top;
	aspectRatio_ = static_cast<float>(width_) / static_cast<float>(height_);
}

unsigned int ScreenCoordinates::GetWidth() const
{
	return width_;
}

unsigned int ScreenCoordinates::GetHeight() const
{
	return height_;
}

unsigned int ScreenCoordinates::GetAspectRatio() const
{
	return aspectRatio_;
}
