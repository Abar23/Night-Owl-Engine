#pragma once

#include "WindowSnapshot.h"

namespace NightOwl::Window
{
	class MonitorProperties
	{
	public:
		int monitorWidth;
	
		int monitorHeight;
		
		int refreshRate;
		
		WindowSnapshot windowSnapshot;
	};
}