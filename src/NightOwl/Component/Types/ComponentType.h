#pragma once

#include <string>

namespace NightOwl::Component
{
	enum class ComponentType
	{
		None,
		Transform,
		MeshRenderer,
		Camera,
		OwlBehavior
	};

	inline const std::string& ComponentTypeToString(ComponentType type)
	{
		switch (type)
		{
		case ComponentType::Transform:
			return std::string{"Transform"};

		case ComponentType::MeshRenderer:
			return std::string{ "MeshRenderer" };

		case ComponentType::Camera:
			return std::string{ "Camera" };

		case ComponentType::OwlBehavior:
			return std::string{ "OwlBehavior" };

		default:
			return std::string{ "Unknown Component" };
		}
	}
}
