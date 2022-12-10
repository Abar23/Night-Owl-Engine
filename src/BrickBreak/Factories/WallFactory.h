#pragma once

#include "NightOwl/Core/Application/Scene.h"
#include "NightOwl/Graphics/Interfaces/ITexture2D.h"

namespace BrickBreak
{
	enum class WallType
	{
		LeftWall,
		RightWall,
		TopWall,
		BottomWall
	};

	class WallFactory
	{
	public:
		WallFactory(NightOwl::Core::Scene& brickScene, const std::shared_ptr<NightOwl::Graphics::ITexture2D>& textureAtlas);

		NightOwl::GameObject::GameObject& CreateWall(WallType type) const;

	private:
		NightOwl::Core::Scene& scene;

		const std::shared_ptr<NightOwl::Graphics::ITexture2D>& textureAtlas;
	};
}
