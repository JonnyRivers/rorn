#pragma once

#include "BoxPrimitive.h"

using namespace Rorn::Max;

BoxPrimitive::BoxPrimitive(INode* node)
{
	Object* object = node->GetObjectRef();
	IParamArray* paramArray = object->GetParamBlock();

	paramArray->GetValue( object->GetParamBlockIndex( BOXOBJ_LENGTH ), TimeValue(0), length_, Interval() );
	paramArray->GetValue( object->GetParamBlockIndex( BOXOBJ_WIDTH ), TimeValue(0), width_, Interval() );
	paramArray->GetValue( object->GetParamBlockIndex( BOXOBJ_HEIGHT ), TimeValue(0), height_, Interval() );
}
