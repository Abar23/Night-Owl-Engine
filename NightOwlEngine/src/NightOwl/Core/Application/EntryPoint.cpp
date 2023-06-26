#include "NightOwlPch.h"

#include "NightOwlEngine.h"
#include "IApplication.h"
#include <windows.h>

int main()
{
	#ifdef DEBUG
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	#endif

	NightOwl::NightOwlEngine engine;

	engine.SetApplication(NightOwl::CreateApplication());

	engine.Init();

	engine.Update();

	engine.Shutdown();

	return 0;
}
