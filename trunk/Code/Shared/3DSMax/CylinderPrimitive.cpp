#pragma once

#include "CylinderPrimitive.h"

using namespace Rorn::Max;

CylinderPrimitive::CylinderPrimitive(INode* node)
{
	Object* object = node->GetObjectRef();
	IParamArray* paramArray = object->GetParamBlock();

	paramArray->GetValue( object->GetParamBlockIndex( CYLINDER_RADIUS ), TimeValue(0), radius_, Interval() );
	paramArray->GetValue( object->GetParamBlockIndex( CYLINDER_HEIGHT ), TimeValue(0), height_, Interval() );
}
