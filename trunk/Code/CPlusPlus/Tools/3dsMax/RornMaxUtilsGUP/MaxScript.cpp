// Max SDK Includes
#include "Max.h"
#include "MaxScrpt/MaxScrpt.h"
#include "MaxScrpt/Definsfn.h"

// Shared includes
#include "../../../Shared/Text/ci_string.h"

// Local includes
#include "SceneExporter.h"

def_visible_primitive (RornUtils_Export, "RornUtils_Export" );
def_visible_primitive (RornUtils_ExportCompileAndView, "RornUtils_ExportCompileAndView" );

Value* RornUtils_Export_cf(Value** arg_list, int count)
{
	check_arg_count(RornUtils_Export, 1, count);

	Value* exportPathValue = arg_list[0];
	Rorn::Text::ci_string exportPath( exportPathValue->to_string() );

	SceneExporter exporter(GetCOREInterface());
	exporter.ExportScene( exportPath );

	return &ok;
}

Value* RornUtils_ExportCompileAndView_cf(Value** arg_list, int count)
{
	check_arg_count(RornUtils_ExportCompileAndView, 1, count);

	Value* exportPathValue = arg_list[0];
	Rorn::Text::ci_string exportPath( exportPathValue->to_string() );

	SceneExporter exporter(GetCOREInterface());
	exporter.ExportScene( exportPath );

	// compile

	// view

	return &ok;
}