#pragma once

#include "OmniLight.h"

using namespace Rorn::Max;

OmniLight::OmniLight(INode* node)
{
	Object* object = node->GetObjectRef();
	LightObject* lightObject = dynamic_cast<LightObject*>(object);
	colour_ = lightObject->GetRGBColor(TimeValue(0));
	intensity_ = lightObject->GetIntensity(TimeValue(0));
}
