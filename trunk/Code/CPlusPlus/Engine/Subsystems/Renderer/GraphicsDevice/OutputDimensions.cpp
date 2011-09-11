#pragma once

#include "OutputDimensions.h"

using namespace Rorn::Engine;

OutputDimensions::OutputDimensions(HWND applicationWindowHandle)
{
	RECT rc;
    GetClientRect( applicationWindowHandle, &rc );
    Width = rc.right - rc.left;
    Height = rc.bottom - rc.top;
	AspectRatio = static_cast<FLOAT>(Width) / static_cast<FLOAT>(Height);
}
