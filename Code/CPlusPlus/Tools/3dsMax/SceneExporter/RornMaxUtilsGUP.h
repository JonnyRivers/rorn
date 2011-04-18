#pragma once

#include <string>
#include <vector>

#include <Max.h>
#include "istdplug.h"
#include "iparamb2.h"
#include "iparamm2.h"
#include <guplib.h>

// This class simply satisfies the Max SDK requirement to derive from SceneExport.
// All of the real work is farmed out to SceneValidator and SceneExporter.
class RornMaxUtilsGUP : public GUP
{
public:
	RornMaxUtilsGUP();
	~RornMaxUtilsGUP();

	// GUP Methods
	DWORD Start( );
	void Stop( );
	DWORD_PTR Control(DWORD parameter);
		
	// Loading/Saving
	IOResult Save(ISave* isave);
	IOResult Load(ILoad* iload);
};

