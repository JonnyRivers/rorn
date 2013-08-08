#pragma once

#include "rorn_engine_exception.h"

namespace Rorn
{
	namespace Engine
	{
		class bounds_load_exception : public rorn_engine_exception
		{
		public:
			bounds_load_exception(const char* const& what) : rorn_engine_exception(what) {}
		};
	}
}