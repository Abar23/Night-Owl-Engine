#pragma once

#include "OwlBehavior.h"

namespace NightOwl::Behavior
{
	class CustomBehavior : public OwlBehavior 
	{
	public:
		void Awake() override;

		void Start() override;

		void Update() override;

		void OnEnable() override;

		void OnDisable() override;

		void OnPreRender() override;

		void OnRender() override;

		void OnPostRender() override;

		void OnCollisionEnter2D() override;

		void OnCollisionStay2D() override;

		void OnCollisionExit2D() override;
	};
}
