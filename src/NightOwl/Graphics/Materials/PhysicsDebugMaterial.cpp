#include "NightOwlPch.h"

#include "PhysicsDebugMaterial.h"

#include "NightOwl/Physics/2D/Collider/OBBBoxCollider2D.h"
#include "NightOwl/Core/Locator/AssetManagerLocator.h"
#include "NightOwl/Component/Concrete/RigidBody2D.h"
#include "NightOwl/Component/Concrete/Camera.h"
#include "NightOwl/GameObject/GameObject.h"
#include "NightOwl/Graphics/RenderAPI.h"


namespace NightOwl::Graphics
{
	PhysicsDebugMaterial::PhysicsDebugMaterial()
	{
		physicsDebugShader = Core::AssetManagerLocator::GetAssetManager()->LoadShader("Physics Debug Shader", "./assets/Shaders/PhysicsDebugShader.vert", "./assets/Shaders/PhysicsDebugShader.frag");
	}

	std::shared_ptr<IMaterial> PhysicsDebugMaterial::Clone()
	{
		std::shared_ptr<PhysicsDebugMaterial> clone = std::make_shared<PhysicsDebugMaterial>();

		clone->physicsDebugShader = physicsDebugShader;

		return clone;
	}

	void PhysicsDebugMaterial::SetTexture(Core::WeakPointer<ITexture2D> texture2D)
	{
	}

	Core::WeakPointer<ITexture2D> PhysicsDebugMaterial::GetTexture()
	{
		return nullptr;
	}

	void PhysicsDebugMaterial::Draw(Component::MeshRenderer& meshRenderer)
	{
	}

	void PhysicsDebugMaterial::Draw(Physics::Collider2D& collider)
	{
		auto mesh = collider.GetMesh();
		auto transform = collider.GetRigidBody()->GetGameObject().GetTransform();

		physicsDebugShader->Bind();
		if (dynamic_cast<Physics::OBBBoxCollider2D*>(&collider) != nullptr)
		{
			physicsDebugShader->SetUniformMat4F(Math::Mat4F::MakeTranslation(transform->GetPosition()) * collider.GetRigidBody()->GetGameObject().GetTransform()->GetRotation().GetRotationMatrix(), "modelMatrix");
		} else
		{
			physicsDebugShader->SetUniformMat4F(Math::Mat4F::MakeTranslation(transform->GetPosition()), "modelMatrix");
		}
		physicsDebugShader->SetUniformFloat(collider.GetRigidBody()->GetVelocity().Magnitude(), "velocity");
		physicsDebugShader->SetUniformMat4F(Component::Camera::GetMainCamera()->GetViewProjectionMatrix(), "viewProjectionMatrix");
		physicsDebugShader->SetUniformInt(collider.IsCollided(), "collided");

		mesh.Bind();
		RenderApi::GetContext()->DrawIndexed(DrawType::Triangles, mesh.GetTriangles().size() * sizeof(mesh.GetTriangles()[0]));
		mesh.Unbind();

		physicsDebugShader->Unbind();
	}

	void PhysicsDebugMaterial::SetAlpha(float alpha)
	{
	}
}
