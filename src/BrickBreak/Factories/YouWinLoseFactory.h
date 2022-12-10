#pragma once

#include "NightOwl/Core/Application/Scene.h"
#include "NightOwl/Graphics/Interfaces/ITexture2D.h"

namespace BrickBreak
{
	class YouWinLoseFactory
	{
	public:
		YouWinLoseFactory(NightOwl::Core::Scene& brickScene, 
						  const std::shared_ptr<NightOwl::Graphics::ITexture2D>& youWinTexture,
						  const std::shared_ptr<NightOwl::Graphics::ITexture2D>& youLoseTexture);

		NightOwl::GameObject::GameObject& CreateLose() const;

		NightOwl::GameObject::GameObject& CreateWin() const;

	private:
		NightOwl::Core::Scene& scene;

		const std::shared_ptr<NightOwl::Graphics::ITexture2D>& youWinTexture;

		const std::shared_ptr<NightOwl::Graphics::ITexture2D>& youLoseTexture;
	};
}
