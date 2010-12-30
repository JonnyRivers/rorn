#pragma once

#include "RenderCommand.h"

#include "../FileManager/FileReader.h"

namespace Rorn
{
	namespace Engine
	{
		class UntexturedRenderCommand : public RenderCommand
		{
		public:
			UntexturedRenderCommand(void);
			~UntexturedRenderCommand(void);

			void LoadFromFile(FileReader& fileReader);
		};
	}
}

