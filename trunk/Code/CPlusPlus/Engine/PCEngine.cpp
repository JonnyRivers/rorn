#include "PCEngine.h"

using namespace Rorn::Engine;

/*static*/ IEngine* PCEngine::Startup(HWND windowHandle)
{
	return new PCEngine(windowHandle);
}

/*static*/ void PCEngine::Shutdown(IEngine* engine)
{
	delete engine;
}

PCEngine::PCEngine(HWND windowHandle) 
	: diagnostics_(windowHandle),
	  fileSystem_(&diagnostics_),
	  timekeeper_(&diagnostics_),
	  inputManager_(windowHandle, &diagnostics_),
	  physicsSystem_(&diagnostics_),
	  renderer_(windowHandle, &diagnostics_, &fileSystem_)
{
}

PCEngine::~PCEngine()
{
}

/*virtual*/ IDiagnostics* PCEngine::GetDiagnostics()
{
	return &diagnostics_;
}

/*virtual*/ IFileSystem* PCEngine::GetFileSystem()
{
	return &fileSystem_;
}

/*virtual*/ const IKeyboard* PCEngine::GetKeyboard() const
{
	return inputManager_.GetKeyboard();
}

/*virtual*/ const IMouse* PCEngine::GetMouse() const
{
	return inputManager_.GetMouse();
}

/*virtual*/ IPhysicsSystem* PCEngine::GetPhysicsSystem()
{
	return &physicsSystem_;
}

/*virtual*/ IRenderer* PCEngine::GetRenderer()
{
	return &renderer_;
}

/*virtual*/ float PCEngine::StartFrame()
{
	float timeElpased = timekeeper_.GetTimeElapsedSinceLastRequest();
	renderer_.StartFrame();
	inputManager_.Step();

	return timeElpased;
}

/*virtual*/ void PCEngine::EndFrame()
{
	renderer_.Draw();
	timekeeper_.SleepTillNextFrame();
}

/*virtual*/ float PCEngine::GetFramerate() const
{
	return timekeeper_.GetFramerate();
}