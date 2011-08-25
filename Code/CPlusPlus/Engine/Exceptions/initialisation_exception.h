#pragma once

#include "rorn_engine_exception.h"

namespace Rorn
{
	namespace Engine
	{
		class initialisation_exception : public rorn_engine_exception
		{
		public:
			initialisation_exception(const char* const& what) : rorn_engine_exception(what) {}
		};
	}
}