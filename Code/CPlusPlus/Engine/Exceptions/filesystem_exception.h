#pragma once

#include "rorn_engine_exception.h"

namespace Rorn
{
	namespace Engine
	{
		class filesystem_exception : public rorn_engine_exception
		{
		public:
			filesystem_exception(const char* const& what) : rorn_engine_exception(what) {}
		};
	}
}