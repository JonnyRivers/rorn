#pragma once

#include <string>
#include <vector>

#include <Max.h>

class SceneExporter
{
public:
	SceneExporter(Interface* maxInterface);
	~SceneExporter(void);

	void ExportScene(const std::string& filename);
};

