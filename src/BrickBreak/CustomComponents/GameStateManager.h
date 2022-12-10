#pragma once

#include "NightOwl/Behavior/OwlBehavior.h"

namespace BrickBreak
{
	class GameStateManager : public NightOwl::Behavior::OwlBehavior
	{
	public:
		void Awake() override;

		void Start() override;

		void Update() override;

		void OnEnable() override;

		void OnDisable() override;

		void OnCollisionEnter2D(const NightOwl::Physics::Collision2D& collision2D) override;

		void PlayerWins();

	private:
		NightOwl::GameObject::GameObject* ball;

		NightOwl::GameObject::GameObject* paddle;

		NightOwl::GameObject::GameObject* bottomWall;

		NightOwl::GameObject::GameObject* youLose;

		NightOwl::GameObject::GameObject* youWin;

		int numberOfLives;

		bool needToSetBallPosition;

		bool playerHasWon;
	};
}

