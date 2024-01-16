#pragma once

#include "NightOwl/Component/Component.h"
#include "NightOwl/Component/Structures/ClothSpring.h"
#include "NightOwl/Component/Structures/ClothParticle.h"
#include "NightOwl/Math/Vec3.h"
#include <vector>

namespace NightOwl
{
	class Mesh;

	class Transform;

	class PlanarCloth : public Component
	{
	public:
		PlanarCloth();

		void Start() override;

		void FixedUpdate();

		void Update();

		void DebugRender();

		void Reset();

		void ConstructClothWithDimension(int dimension);

		std::shared_ptr<Component> Clone() override;

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

		float GetMass() const;

		void SetMass(float mass);

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

		int planeDimension;

		std::pair<Transform*, float> sphereCollider;

		std::shared_ptr<Mesh> planeMesh;

		std::vector<ClothParticle> grid;

		std::vector<ClothParticle*> particles;

		std::vector<ClothParticle*> corners;

		std::vector<ClothSpring> springs;

		std::vector<ClothSpring*> constrainedSprings;

		std::vector<Vec3F> planeVertices;

		std::vector<Vec3F> planeNormals;

		void ApplyForces() const;

		void VerletIntegration();

		void UpdateVertexData();

		void ApplyConstraints() const;

		void SolveCollision();

		void InitializeVertices(std::vector<Vec3F>& outVertices);

		void Remove() override;
	};
}
