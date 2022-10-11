#pragma once

#include <string>

namespace NightOwl::Components
{
	enum class ComponentType
	{
		None,
		Transform,
		Mesh
	};

	const std::string& ComponentTypeToString(ComponentType type)
	{
		switch (type)
		{
		case ComponentType::Transform:
			return std::string{"Transform"};

		case ComponentType::Mesh:
			return std::string{ "Mesh" };

		default:
			return std::string{ "Unknown Component" };
		}
	}
}
