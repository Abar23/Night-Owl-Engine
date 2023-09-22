#pragma once

#include "NightOwl/Behavior/OwlBehavior.h"
#include "NightOwl/Component/Concrete/Animator.h"

class ImGuiInterface : public NightOwl::OwlBehavior
{
public:
	~ImGuiInterface() override;

	void Awake() override;

	void Start() override;

	void Update() override;

private:
	bool showWindow = true;

	NightOwl::Animator* characterAnimator;

	void InitImGui();

	void TerminateImGui();

	void BeginFrame();

	void EndFrame();
};
