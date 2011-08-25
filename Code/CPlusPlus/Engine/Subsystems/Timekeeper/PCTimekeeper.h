#pragma once

#include <deque>

#include <windows.h>

#include "../../Interfaces/IDiagnostics.h"

namespace Rorn
{
	namespace Engine
	{
		class PCTimekeeper
		{
		public:
			PCTimekeeper(IDiagnostics* diagnostics);
			~PCTimekeeper();

			float GetTimeElapsedSinceLastRequest();
			float GetFramerate() const;
			void SleepTillNextFrame() const;
		private:
			PCTimekeeper(PCTimekeeper&);
			PCTimekeeper& operator=(PCTimekeeper&);

			IDiagnostics* diagnostics_;

			bool firstStep_;
			LARGE_INTEGER timerFrequency_;
			LARGE_INTEGER timeAtStart_;
			LARGE_INTEGER timeAtLastStep_;
			
			std::deque<float> previousFrameTimes_;
		};
	}
}