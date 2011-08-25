#pragma once

#include "rorn_engine_exception.h"

namespace Rorn
{
	namespace Engine
	{
		class input_exception : public rorn_engine_exception
		{
		public:
			input_exception(const char* const& what) : rorn_engine_exception(what) {}
		};
	}
}