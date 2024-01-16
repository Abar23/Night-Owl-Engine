#pragma once

#include <string>

namespace NightOwl
{
	enum class ComponentType
	{
		None,
		Transform,
		MeshRenderer,
		SkinnedMeshRenderer,
		Camera,
		RigidBody2D,
		AABBBoxCollider2D,
		AudioListener,
		AudioSource,
		OwlBehavior,
		Animator,
		CatmullRomSpline,
		ChainIK,
		PlanarCloth
	};

	inline std::string ComponentTypeToString(ComponentType type)
	{
		switch (type)
		{
		case ComponentType::Transform:
			return std::string{"Transform"};

		case ComponentType::MeshRenderer:
			return std::string{ "MeshRenderer" };

		case ComponentType::SkinnedMeshRenderer:
			return std::string{ "SkinnedMeshRenderer" };

		case ComponentType::Camera:
			return std::string{ "Camera" };

		case ComponentType::RigidBody2D:
			return std::string{ "RigidBody2D" };

		case ComponentType::AABBBoxCollider2D:
			return std::string{ "AABBBoxCollider2D" };

		case ComponentType::OwlBehavior:
			return std::string{ "OwlBehavior" };

		case ComponentType::Animator:
			return std::string{ "Animator" };

		case ComponentType::CatmullRomSpline:
			return std::string{ "CatmullRomSpline" };

		case ComponentType::ChainIK:
			return std::string{ "ChainIK" };

		case ComponentType::PlanarCloth:
			return std::string{ "PlanarCloth" };

		default:
			return std::string{ "Unknown Component" };
		}
	}
}
