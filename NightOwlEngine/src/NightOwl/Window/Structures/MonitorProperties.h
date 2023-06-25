#pragma once

#include "WindowSnapshot.h"

namespace NightOwl
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