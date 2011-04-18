// Max SDK Includes
#include "Max.h"
#include "MaxScrpt/MaxScrpt.h"
#include "MaxScrpt/Definsfn.h"

// Shared includes
#include "../../../Shared/Text/ci_string.h"
#include "../../../Shared/WindowsRegistry/WindowsRegistry.h"

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
	std::string modelCompilerPathname;
	Rorn::WindowsRegistry::GetRegistryTextValue(Rorn::WindowsRegistry::LocalMachine, "SOFTWARE\\Riversoft\\Rorn", "ModelCompilerPathname", modelCompilerPathname);

	// view
	std::string modelViewerPathname;
	Rorn::WindowsRegistry::GetRegistryTextValue(Rorn::WindowsRegistry::LocalMachine, "SOFTWARE\\Riversoft\\Rorn", "ModelViewerPathname", modelViewerPathname);

	return &ok;
}