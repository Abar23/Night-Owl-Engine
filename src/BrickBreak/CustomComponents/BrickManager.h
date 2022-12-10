#pragma once

#include "NightOwl/Behavior/OwlBehavior.h"

namespace BrickBreak
{
	class BrickManager : public NightOwl::Behavior::OwlBehavior
	{
	public:
		void Awake() override;

		void Start() override;

		void Update() override;

		void TurnOnBricks();

		void RegisterBrick(NightOwl::GameObject::GameObject* brick);

		void DecreaseNumberOfActiveBricks();

	private:
		int numberOfBricks;

		int numberOfActiveBricks;
	};
}
