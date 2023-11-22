#pragma once

#include "IkTargetController.h"
#include "SplineAnimator.h"
#include "NightOwl/Behavior/OwlBehavior.h"
#include "NightOwl/Component/Abstrtact/Renderer.h"
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

	NightOwl::Renderer* characterRenderer;

	NightOwl::ChainIK* chainIk;

	SplineAnimator* splineAnimator;

	IkTargetController* ikTargetController;

	void InitImGui();

	void TerminateImGui();

	void BeginFrame();

	void EndFrame();
};
