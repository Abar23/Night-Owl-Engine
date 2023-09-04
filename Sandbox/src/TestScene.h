#pragma once

#include "NightOwl/Core/Application/Scene.h"

class TestScene : public NightOwl::Scene
{
public:
	TestScene();

	void Init() override;

	void Reset() override;

	void Load() override;

	void Shutdown() override;
};

