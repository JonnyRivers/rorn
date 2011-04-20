// Max SDK Includes
#include "Max.h"
#include "MaxScrpt/MaxScrpt.h"
#include "MaxScrpt/Definsfn.h"

#include <sstream>

// Shared includes
#include "../../../Shared/Process/Process.h"
#include "../../../Shared/Text/ci_string.h"
#include "../../../Shared/Text/Path.h"
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
	Rorn::Text::ci_string compiledModelPathname = Rorn::Text::Path::ChangeExtension(exportPath, "model");
	std::stringstream compilerArgumentsStream;
	compilerArgumentsStream << "\"" << modelCompilerPathname.c_str() << "\" \"" << exportPath.c_str() << "\" \"" << compiledModelPathname.c_str() << "\"";
	std::string modelCompilerOutput;
	int compilerExitCode = Rorn::Process::RunCommandLineApplication(modelCompilerPathname.c_str(), compilerArgumentsStream.str().c_str(), modelCompilerOutput);

	// view
	std::string modelViewerPathname;
	Rorn::WindowsRegistry::GetRegistryTextValue(Rorn::WindowsRegistry::LocalMachine, "SOFTWARE\\Riversoft\\Rorn", "ModelViewerPathname", modelViewerPathname);
	Rorn::Process::LaunchApplication(modelViewerPathname.c_str(), compiledModelPathname.c_str());

	return &ok;
}