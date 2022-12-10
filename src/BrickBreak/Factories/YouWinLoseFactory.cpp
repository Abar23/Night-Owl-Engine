#include "YouWinLoseFactory.h"
#include "BrickBreak/BrickBreakData/Constants.h"
#include "BrickBreak/CustomComponents/Ball.h"
#include "BrickBreak/CustomComponents/PaddleController.h"
#include "NightOwl/Component/Concrete/MeshRenderer.h"
#include "NightOwl/Component/Concrete/RigidBody2D.h"
#include "NightOwl/Component/Materials/IMaterial.h"

namespace BrickBreak
{
	YouWinLoseFactory::YouWinLoseFactory(NightOwl::Core::Scene& brickScene,
		const std::shared_ptr<NightOwl::Graphics::ITexture2D>& youWinTexture,
		const std::shared_ptr<NightOwl::Graphics::ITexture2D>& youLoseTexture)
		:	scene(brickScene),
			youWinTexture(youWinTexture),
			youLoseTexture(youLoseTexture)
	{
	}

	NightOwl::GameObject::GameObject& YouWinLoseFactory::CreateLose() const
	{
		auto& youLose = scene.AddGameObject("You Lose");

		auto* youLoseRenderer = youLose.AddComponent<NightOwl::Component::MeshRenderer>();
		youLoseRenderer->GetMaterial()->SetTexture(youLoseTexture);

		auto* youLoseMesh = youLoseRenderer->GetMesh();
		youLoseMesh->SetVertices(YOU_WIN_LOSE_VERTICES);
		youLoseMesh->SetUVs(YOU_WIN_LOSE_UVS);
		youLoseMesh->SetTriangles(QUAD_TRIANGLES);

		youLose.GetTransform()->Translate(0, 0, 0.1, NightOwl::Component::Space::World);

		return youLose;
	}

	NightOwl::GameObject::GameObject& YouWinLoseFactory::CreateWin() const
	{
		auto& youWin = scene.AddGameObject("You Win");

		auto* youWinRenderer = youWin.AddComponent<NightOwl::Component::MeshRenderer>();
		youWinRenderer->GetMaterial()->SetTexture(youWinTexture);

		auto* youWinMesh = youWinRenderer->GetMesh();
		youWinMesh->SetVertices(YOU_WIN_LOSE_VERTICES);
		youWinMesh->SetUVs(YOU_WIN_LOSE_UVS);
		youWinMesh->SetTriangles(QUAD_TRIANGLES);

		youWin.GetTransform()->Translate(0, 0, 0.1, NightOwl::Component::Space::World);

		return youWin;
	}
}
