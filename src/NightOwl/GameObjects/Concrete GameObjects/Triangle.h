#pragma once

#include "NightOwl/GameObjects/GameObject.h"

namespace NightOwl::GameObjects
{
	class Triangle : public GameObject
	{
	public:
		void Start() override;

		void Update() override;

		void OnEnable() override;

		void OnDisable() override;
	};
}
