#pragma once

#include "NightOwl/Core/Application/Scene.h"
#include "NightOwl/Graphics/Interfaces/ITexture2D.h"

namespace BrickBreak
{
	class PaddleAndBallFactory
	{
	public:
		PaddleAndBallFactory(NightOwl::Core::Scene& brickScene, const std::shared_ptr<NightOwl::Graphics::ITexture2D>& textureAtlas);

		NightOwl::GameObject::GameObject& CreateBall() const;

		NightOwl::GameObject::GameObject& CreatePaddle() const;

	private:
		NightOwl::Core::Scene& scene;

		const std::shared_ptr<NightOwl::Graphics::ITexture2D>& textureAtlas;
	};
}
