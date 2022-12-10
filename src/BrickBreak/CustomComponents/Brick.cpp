#include "Brick.h"

#include "Ball.h"
#include "NightOwl/Component/Concrete/MeshRenderer.h"
#include "NightOwl/Component/Concrete/RigidBody2D.h"
#include "NightOwl/Core/Application/Scene.h"
#include "NightOwl/GameObject/GameObject.h"

namespace BrickBreak
{
	void Brick::Awake()
	{

	}

	void Brick::Start()
	{
		brickManager = gameObject->GetScene()->FindGameObject("Brick Manager")->GetComponent<BrickManager>();
		brickManager->RegisterBrick(gameObject);
	}

	void Brick::Update()
	{

	}

	void Brick::OnEnable()
	{

	}

	void Brick::OnDisable()
	{

	}

	void Brick::OnCollisionEnter2D(const NightOwl::Physics::Collision2D& collision2D)
	{

	}

	void Brick::ReactivateBrick()
	{
		gameObject->SetActive(true);
	}
}
