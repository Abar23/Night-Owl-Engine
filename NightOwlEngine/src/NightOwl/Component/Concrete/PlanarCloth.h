#pragma once

#include "NightOwl/Component/Component.h"
#include "NightOwl/Math/Vec3.h"
#include <vector>

namespace NightOwl
{
	class ClothParticle;

	class ClothSpring;

	class Mesh;

	class Transform;

	class PlanarCloth : public Component
	{
	public:
		float GetMass() const;
		void SetMass(float mass);

		PlanarCloth();

		void Start() override;

		void Update();

		void DebugRender();

		void Reset();

		void ConstructClothWithDimension(int dimension);

		std::shared_ptr<Component> Clone() override;

		bool AreCornersFixed() const;

		void SetFixedCorners(const bool fixCorners);

		float GetGravity() const;

		void SetGravity(const float gravity);

		float GetStructuralSpringConstant() const;

		void SetStructuralSpringConstant(const float springConstant);

		float GetShearSpringConstant() const;

		void SetShearSpringConstant(float shearSpringConstant);

		float GetFlexionSpringConstant() const;

		void SetFlexionSpringConstant(float flexionSpringConstant);

		float GetSpringStretchTolerance() const;

		void SetSpringStretchTolerance(float springStretchTolerance);

		float GetStructuralDampConstant() const;

		void SetStructuralDampConstant(float structuralDampConstant);

		float GetShearDampConstant() const;

		void SetShearDampConstant(float shearDampConstant);

		float GetFlexionDampConstant() const;

		void SetFlexionDampConstant(float flexionDampConstant);

		void EnableFixedCorner(int cornerIndex, bool enable) const;

		void SetSphereCollider(Transform* colliderTransform, float radius);

	private:
		float gravity;

		float structuralSpringConstant;

		float shearSpringConstant;
		
		float flexionSpringConstant;

		float springStretchTolerance;

		float structuralDampConstant;

		float shearDampConstant;

		float flexionDampConstant;

		float mass;

		std::pair<Transform*, float> sphereCollider;

		std::shared_ptr<Mesh> planeMesh;

		std::vector<std::vector<std::shared_ptr<ClothParticle>>> grid;

		std::vector<std::vector<ClothParticle*>> quads;

		std::vector<ClothParticle*> particles;

		std::vector<ClothParticle*> corners;

		std::vector<std::shared_ptr<ClothSpring>> springs;

		void ClearForces();

		void ApplyForces();

		void VerletIntegration();

		void UpdateVertexData();

		void ApplyConstraints();

		void SolveCollision();

		void InitializeVertices(std::vector<Vec3F>& outVertices);

		void Remove() override;
	};
}
