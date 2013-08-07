#pragma once

#include "SpherePrimitive.h"

using namespace Rorn::Max;

SpherePrimitive::SpherePrimitive(INode* node)
{
	Object* object = node->GetObjectRef();
	IParamArray* paramArray = object->GetParamBlock();

	paramArray->GetValue( object->GetParamBlockIndex( SPHERE_RADIUS ), TimeValue(0), radius_, Interval() );
}
