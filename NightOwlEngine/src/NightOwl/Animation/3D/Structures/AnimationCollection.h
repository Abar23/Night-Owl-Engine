#pragma once

#include "NightOwl/Animation/3D/Animation.h"
#include <map>
#include <string>

namespace NightOwl
{
	class AnimationCollection
	{
	public:
		std::map<std::string, Animation*> animationsMap;
	};
}
