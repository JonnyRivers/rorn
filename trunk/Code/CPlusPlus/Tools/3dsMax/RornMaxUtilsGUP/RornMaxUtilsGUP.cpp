#include "RornMaxUtilsGUP.h"

#include "SceneExporter.h"
#include "SceneValidator.h"

RornMaxUtilsGUP::RornMaxUtilsGUP() 
{
}

RornMaxUtilsGUP::~RornMaxUtilsGUP()
{
}

// GUP Methods
DWORD RornMaxUtilsGUP::Start( )
{
	return GUPRESULT_KEEP;
}

void RornMaxUtilsGUP::Stop( )
{
}

DWORD_PTR RornMaxUtilsGUP::Control(DWORD parameter)
{
	return 0;
}
		
// Loading/Saving
IOResult RornMaxUtilsGUP::Save(ISave* isave)
{
	return IO_OK;
}

IOResult RornMaxUtilsGUP::Load(ILoad* iload)
{
	return IO_OK;
}