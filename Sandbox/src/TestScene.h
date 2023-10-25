#pragma once

#include "NightOwl/Animation/3D/BlendTree.h"
#include "NightOwl/Core/Application/Scene.h"

class TestScene : public NightOwl::Scene
{
public:
	TestScene();

	void Init() override;

	void Reset() override;

	void Load() override;

	void Shutdown() override;

private:
	std::shared_ptr<NightOwl::BlendTree> simple1DBlendTree;
};

