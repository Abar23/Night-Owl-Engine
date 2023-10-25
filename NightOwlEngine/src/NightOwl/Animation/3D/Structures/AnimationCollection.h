#pragma once

#include <map>
#include <string>

namespace NightOwl
{
	class Motion;

	class AnimationCollection
	{
	public:
		std::map<std::string, Motion*> motionsMap;
	};
}
