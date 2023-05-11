#include "NightOwlPch.h"

#include "NightOwlEngine.h"
#include "NightOwl/Core/Application/IApplication.h"
#include <windows.h>

int main()
{
	#ifdef DEBUG
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	#endif

	NightOwl::Core::NightOwlEngine engine;

	engine.SetApplication(NightOwl::Core::CreateApplication());

	engine.Init();

	engine.Update();

	engine.Shutdown();

	return 0;
}
