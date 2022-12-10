#pragma once

#include "NightOwl/Core/Application/Scene.h"
#include "NightOwl/GameObject/GameObject.h"
#include "NightOwl/Graphics/Interfaces/ITexture2D.h"
#include <memory>

namespace BrickBreak
{
	enum class BrickType
	{
		RED,
		BLUE,
		GREEN,
		PURPLE,
		YELLOW,
		ORANGE
	};

	class BrickFactory
	{
	public:
		BrickFactory(NightOwl::Core::Scene& brickScene, const std::shared_ptr<NightOwl::Graphics::ITexture2D>& textureAtlas);

		NightOwl::GameObject::GameObject& CreateBrick(BrickType type) const;

	private:
		NightOwl::Core::Scene& scene;

		const std::shared_ptr<NightOwl::Graphics::ITexture2D>& textureAtlas;
	};
}
