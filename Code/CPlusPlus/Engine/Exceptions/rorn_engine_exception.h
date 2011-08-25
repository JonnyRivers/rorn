#pragma once

#include <exception>

namespace Rorn
{
	namespace Engine
	{
		class rorn_engine_exception : public std::exception
		{
		public:
			rorn_engine_exception(const char* const& what) : std::exception(what) {}
		};
	}
}