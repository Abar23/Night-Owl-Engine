#pragma once

#include <map>
#include <string>

namespace NightOwl
{
	class Animation;

	class AnimationCollection
	{
	public:
		std::map<std::string, Animation*> animationsMap;
	};
}
