#pragma once

#include <exception>

namespace Rorn
{
	namespace Engine
	{
		class initialisation_exception : public std::exception
		{
		public:
			initialisation_exception(const char* const& what) : std::exception(what) {}
		};
	}
}