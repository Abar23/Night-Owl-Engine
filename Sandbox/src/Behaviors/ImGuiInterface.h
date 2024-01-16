#pragma once

#include "IkTargetController.h"
#include "NightOwl/Behavior/OwlBehavior.h"
#include "NightOwl/Component/Concrete/MeshRenderer.h"
#include "NightOwl/Component/Concrete/PlanarCloth.h"

class ImGuiInterface : public NightOwl::OwlBehavior
{
public:
	~ImGuiInterface() override;

	void Awake() override;

	void Start() override;

	void Update() override;

private:
	bool showWindow = true;

	bool debugRender = false;

	std::array<bool, 4> fixCornerData;

	float structuralSpringConstant;

	float shearSpringConstant;

	float flexionSpringConstant;

	float springStretchTolerance;

	float structuralDampConstant;

	float shearDampConstant;

	float flexionDampConstant;

	float clothMass;

	float gravity;

	NightOwl::PlanarCloth* planarCloth;

	NightOwl::MeshRenderer* planeRenderer;

	IkTargetController* targetController;

	void InitImGui();

	void TerminateImGui();

	void BeginFrame();

	void EndFrame();

	void Reset();

	void GetClothPropertyValues();

	void UnpinAllCorners();

	void PinAllCorners();
};
