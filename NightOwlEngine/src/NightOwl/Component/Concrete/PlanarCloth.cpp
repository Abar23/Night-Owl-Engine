#include "NightOwlPch.h"

#include "PlanarCloth.h"
#include "MeshRenderer.h"
#include "NightOwl/Component/Structures/ClothParticle.h"
#include "NightOwl/Component/Structures/ClothSpring.h"
#include "NightOwl/Component/Structures/Mesh.h"
#include "NightOwl/Core/Locator/Locator.h"
#include "NightOwl/Core/Time/Time.h"
#include "NightOwl/GameObject/GameObject.h"
#include "NightOwl/Graphics/Debugging/DebugSystem.h"
#include "NightOwl/Physics/ClothSimSystem.h"
#include <execution>


namespace NightOwl
{
	PlanarCloth::PlanarCloth()
		: Component(ComponentType::PlanarCloth),
		  gravity(-9.8f),
		  structuralSpringConstant(100),
		  shearSpringConstant(100),
		  flexionSpringConstant(3000),
		  springStretchTolerance(1.1f),
		  structuralDampConstant(1.0f),
		  shearDampConstant(1.0f),
		  flexionDampConstant(1.0f),
		  mass(1.0f),
		  planeDimension(0),
		  sphereCollider(nullptr, -1.0f)
	{
		ClothSimSystemLocator::Get()->AddClothSimComponent(this);
	}

	void PlanarCloth::Start()
	{
		planeMesh = gameObject->GetComponent<MeshRenderer>()->GetMesh();
	}

	void PlanarCloth::FixedUpdate()
	{
		ApplyForces();

		VerletIntegration();

		ApplyConstraints();

		SolveCollision();
	}

	void PlanarCloth::Update()
	{
		UpdateVertexData();
	}

	void PlanarCloth::DebugRender()
	{
		DebugSystem* debugSystem = DebugSystemLocator::Get();
		for (const auto& clothSpring : springs)
		{
			Vec3F color = { 1.0f, 0.0f, 1.0f };
			if (clothSpring.type == SpringType::Shear)
			{
				color = { 1.0f, 0.64f, 0.0f };
			}
			else if (clothSpring.type == SpringType::Flexion)
			{
				color = { 0.0f, 1.0f, 1.0f };
			}

			debugSystem->DrawLine(*clothSpring.particle->position, *clothSpring.otherParticle->position, color);
		}

		for (const auto& corner : corners)
		{
			if (corner->isFixed)
			{
				debugSystem->DrawPoint(*corner->position, { 1.0f, 1.0f, 1.0f });
			}
		}
	}

	void PlanarCloth::Reset()
	{
		planeVertices.resize(planeDimension * planeDimension);
		InitializeVertices(planeVertices);

		planeMesh->SetVertices(planeVertices);
		planeMesh->UploadMeshData(false);

		for (int particleIndex = 0; particleIndex < particles.size(); ++particleIndex)
		{
			ClothParticle* clothParticle = particles[particleIndex];
			clothParticle->position = &planeVertices[particleIndex];
			clothParticle->previousPosition = *clothParticle->position;
		}

		// Reset default values
		gravity = -9.8f;
		structuralSpringConstant = 100;
		shearSpringConstant = 100;
		flexionSpringConstant = 3000;
		springStretchTolerance = 1.1f;
		structuralDampConstant = 1.0f;
		shearDampConstant = 1.0f;
		flexionDampConstant = 1.0f;
		mass = 1.0f;

		corners[0]->isFixed = true;
		corners[1]->isFixed = true;
		corners[2]->isFixed = true;
		corners[3]->isFixed = true;
	}

	void PlanarCloth::ConstructClothWithDimension(int dimension)
	{
		planeDimension = dimension; 
		const int numberOfVertices = planeDimension * planeDimension;
		planeNormals.resize(numberOfVertices);
		planeVertices.resize(numberOfVertices);
		InitializeVertices(planeVertices);

		std::vector<Vec3UI> triangles((planeDimension - 1) * (planeDimension - 1) * 2);
		int triangleIndex = 0;
		for (int zIndex = 0; zIndex < planeDimension - 1; ++zIndex)
		{
			for (int xIndex = 0; xIndex < planeDimension - 1; ++xIndex)
			{
				// upper triangle
				Vec3UI upperTriangle;
				upperTriangle.x = xIndex + zIndex * planeDimension;
				upperTriangle.y = xIndex + (zIndex + 1) * planeDimension;
				upperTriangle.z = (xIndex + 1) + zIndex * planeDimension;

				// lower triangle
				Vec3UI lowerTriangle;
				lowerTriangle.x = (xIndex + 1) + zIndex * planeDimension;
				lowerTriangle.y = xIndex + (zIndex + 1) * planeDimension;
				lowerTriangle.z = (xIndex + 1) + (zIndex + 1) * planeDimension;

				triangles[triangleIndex] = upperTriangle;
				triangles[triangleIndex + 1] = lowerTriangle;
				triangleIndex += 2;
			}
		}

		int indexCount = triangles.size() * 3;
		int vertexCount = planeVertices.size();
		std::vector<SubMeshData> subMeshData = {
			{ 0, indexCount, 0, vertexCount, 0}
		};

		planeMesh->SetVertices(planeVertices);
		planeMesh->SetTriangles(triangles);
		planeMesh->SetSubMeshes(subMeshData);
		planeMesh->UploadMeshData(false);

		grid.resize(numberOfVertices);
		for (int zIndex = 0; zIndex < planeDimension; ++zIndex)
		{
			for (int xIndex = 0; xIndex < planeDimension; ++xIndex)
			{
				int gridIndex = zIndex * planeDimension + xIndex;

				grid[gridIndex] = ClothParticle();

				particles.push_back(&grid[gridIndex]);
			}
		}

		springs.clear();
		constrainedSprings.clear();
		const float structuralEdgeLength = 2.0f / static_cast<float>(planeDimension - 1);
		const float flexionEdgeLength = 2.0f * structuralEdgeLength;
		const float shearEdgeLength = std::sqrtf(structuralEdgeLength * structuralEdgeLength * 2);
		for (int zIndex = 0; zIndex < planeDimension; ++zIndex)
		{
			for (int xIndex = 0; xIndex < planeDimension; ++xIndex)
			{
				// Indices of the vertices in the quad
				const int particleIndex = zIndex * planeDimension + xIndex;

				// direct neighbors (up, down, left, right) - forms structural springs
				if (zIndex - 1 >= 0)
				{
					// up
					const int up = (zIndex - 1) * planeDimension + xIndex;
					springs.emplace_back(&grid[particleIndex], &grid[up], structuralEdgeLength, SpringType::Structural);
				}
				if (zIndex + 1 < planeDimension) 
				{
					// down
					const int down = (zIndex + 1) * planeDimension + xIndex;
					springs.emplace_back(&grid[particleIndex], &grid[down], structuralEdgeLength, SpringType::Structural);
				}
				if (xIndex - 1 >= 0) 
				{
					// left
					const int left = zIndex * planeDimension + (xIndex - 1);
					springs.emplace_back(&grid[particleIndex], &grid[left], structuralEdgeLength, SpringType::Structural);
				}
				if (xIndex + 1 < planeDimension) 
				{
					// right
					const int right = zIndex * planeDimension + (xIndex + 1);
					springs.emplace_back(&grid[particleIndex], &grid[right], structuralEdgeLength, SpringType::Structural);
				}

				// diagonal neighbors - forms shear springs
				if (zIndex - 1 >= 0 && xIndex - 1 >= 0) 
				{
					// top-left
					const int topLeft = (zIndex - 1) * planeDimension + (xIndex - 1);
					springs.emplace_back(&grid[particleIndex], &grid[topLeft], shearEdgeLength, SpringType::Shear);
				}
				if (zIndex - 1 >= 0 && xIndex + 1 < planeDimension) 
				{
					// top-right
					const int topRight = (zIndex - 1) * planeDimension + (xIndex + 1);
					springs.emplace_back(&grid[particleIndex], &grid[topRight], shearEdgeLength, SpringType::Shear);
				}
				if (zIndex + 1 < planeDimension && xIndex - 1 >= 0) 
				{
					// bottom-left
					const int bottomLeft = (zIndex + 1) * planeDimension + (xIndex - 1);
					springs.emplace_back(&grid[particleIndex], &grid[bottomLeft], shearEdgeLength, SpringType::Shear);
				}
				if (zIndex + 1 < planeDimension && xIndex + 1 < planeDimension) 
				{
					// bottom-right
					const int bottomRight = (zIndex + 1) * planeDimension + (xIndex + 1);
					springs.emplace_back(&grid[particleIndex], &grid[bottomRight], shearEdgeLength, SpringType::Shear);
				}

				// flexion neighbors (up+1, down+1, left+1, right+1) - forms flexion springs
				if (zIndex - 2 >= 0) 
				{
					// up+1
					const int up = (zIndex - 2) * planeDimension + xIndex;
					springs.emplace_back(&grid[particleIndex], &grid[up], flexionEdgeLength, SpringType::Flexion);
				}
				if (zIndex + 2 < planeDimension) 
				{ 
					// down+1
					const int down = (zIndex + 2) * planeDimension + xIndex;
					springs.emplace_back(&grid[particleIndex], &grid[down], flexionEdgeLength, SpringType::Flexion);
				}
				if (xIndex - 2 >= 0) 
				{ 
					// left+1
					const int left = zIndex * planeDimension + (xIndex - 2);
					springs.emplace_back(&grid[particleIndex], &grid[left], flexionEdgeLength, SpringType::Flexion);
				}
				if (xIndex + 2 < planeDimension) 
				{ 
					// right+1
					const int right = zIndex * planeDimension + (xIndex + 2);
					springs.emplace_back(&grid[particleIndex], &grid[right], flexionEdgeLength, SpringType::Flexion);
				}

				Vec3F& position = planeVertices.at(zIndex * planeDimension + xIndex);
				grid[zIndex * planeDimension + xIndex].position = &position;
				grid[zIndex * planeDimension + xIndex].previousPosition = position;
			}
		}

		for (auto& clothSpring : springs)
		{
			if (clothSpring.type == SpringType::Flexion)
			{
				continue;
			}

			constrainedSprings.push_back(&clothSpring);
		}

		// get corners
		corners.resize(4);
		corners[0] = &grid.front();
		corners[1] = &grid[planeDimension - 1];
		corners[2] = &grid[(planeDimension - 1) * planeDimension];
		corners[3] = &grid.back();

		corners[0]->isFixed = true;
		corners[1]->isFixed = true;
		corners[2]->isFixed = true;
		corners[3]->isFixed = true;
	}

	std::shared_ptr<Component> PlanarCloth::Clone()
	{
		return Component::Clone();
	}

	float PlanarCloth::GetGravity() const
	{
		return gravity;
	}

	void PlanarCloth::SetGravity(const float gravity)
	{
		this->gravity = gravity;
	}

	float PlanarCloth::GetStructuralSpringConstant() const
	{
		return structuralSpringConstant;
	}

	void PlanarCloth::SetStructuralSpringConstant(const float springConstant)
	{
		this->structuralSpringConstant = springConstant;
	}

	float PlanarCloth::GetShearSpringConstant() const
	{
		return shearSpringConstant;
	}

	void PlanarCloth::SetShearSpringConstant(float shearSpringConstant)
	{
		this->shearSpringConstant = shearSpringConstant;
	}

	float PlanarCloth::GetFlexionSpringConstant() const
	{
		return flexionSpringConstant;
	}

	void PlanarCloth::SetFlexionSpringConstant(float flexionSpringConstant)
	{
		this->flexionSpringConstant = flexionSpringConstant;
	}

	float PlanarCloth::GetSpringStretchTolerance() const
	{
		return springStretchTolerance;
	}

	void PlanarCloth::SetSpringStretchTolerance(float springStretchTolerance)
	{
		this->springStretchTolerance = springStretchTolerance;
	}

	float PlanarCloth::GetStructuralDampConstant() const
	{
		return structuralDampConstant;
	}

	void PlanarCloth::SetStructuralDampConstant(float structuralDampConstant)
	{
		this->structuralDampConstant = structuralDampConstant;
	}

	float PlanarCloth::GetShearDampConstant() const
	{
		return shearDampConstant;
	}

	void PlanarCloth::SetShearDampConstant(float shearDampConstant)
	{
		this->shearDampConstant = shearDampConstant;
	}

	float PlanarCloth::GetFlexionDampConstant() const
	{
		return flexionDampConstant;
	}

	void PlanarCloth::SetFlexionDampConstant(float flexionDampConstant)
	{
		this->flexionDampConstant = flexionDampConstant;
	}

	void PlanarCloth::EnableFixedCorner(int cornerIndex, bool enable) const
	{
		ENGINE_ASSERT(cornerIndex < 4 && cornerIndex >= 0, "Invalid corner index used for unpinning corner.");

		corners[cornerIndex]->isFixed = enable;
	}

	void PlanarCloth::SetSphereCollider(Transform* colliderTransform, float radius)
	{
		sphereCollider.first = colliderTransform;
		sphereCollider.second = radius;
	}

	float PlanarCloth::GetMass() const
	{
		return mass;
	}

	void PlanarCloth::SetMass(float mass)
	{
		this->mass = mass;
	}

	void PlanarCloth::ApplyForces() const
	{
		// Apply gravity
		const Vec3F gravityAcceleration = Vec3F(0.0f, gravity, 0.0f) * mass;
		for (const auto& clothParticle : particles)
		{
			clothParticle->totalForce = gravityAcceleration;
		}

		// Apply spring forces
		for (const auto& clothSpring : springs)
		{
			Vec3F particlePositionDifference = *clothSpring.otherParticle->position - *clothSpring.particle->position;
		
			float springLength = particlePositionDifference.Magnitude();
		
			Vec3F springDirection = particlePositionDifference.GetNormalize();
		
			float springConstant = structuralSpringConstant;
			float dampConstant = structuralDampConstant;
			if (clothSpring.type == SpringType::Shear)
			{
				springConstant = shearSpringConstant;
				dampConstant = shearDampConstant;
			}
			else if (clothSpring.type == SpringType::Flexion)
			{
				springConstant = flexionSpringConstant;
				dampConstant = flexionDampConstant;
			}
		
			const float springStiffnessTerm = springConstant * (springLength - clothSpring.restLength);
			const float springDampTerm = dampConstant * Vec3F::Dot(particlePositionDifference, springDirection);
			const Vec3F springForce = springDirection * (springStiffnessTerm - springDampTerm);
		
			clothSpring.particle->totalForce += springForce;
			clothSpring.otherParticle->totalForce -= springForce;
		}

		// Remove force from fixed corners
		for (const auto& corner : corners)
		{
			if (corner->isFixed)
			{
				corner->totalForce = 0.0f;
			}
		}
	}

	void PlanarCloth::VerletIntegration()
	{
		const float fixedTimeStepSquared = Time::GetFixedDeltaTime() * Time::GetFixedDeltaTime();
		for (const auto & clothParticle : particles)
		{
			if (clothParticle->isFixed)
			{
				continue;
			}

			// Verlet
			const Vec3F velocity = (*clothParticle->position - clothParticle->previousPosition);
			clothParticle->previousPosition = *clothParticle->position;
			*clothParticle->position = *clothParticle->position + velocity + (clothParticle->totalForce / mass) * fixedTimeStepSquared;
		}
	}

	void PlanarCloth::UpdateVertexData()
	{
		std::ranges::fill(planeNormals.begin(), planeNormals.end(), Vec3F::Zero());
		for (int zIndex = 0; zIndex < planeDimension - 1; zIndex++)
		{
			for (int xIndex = 0; xIndex < planeDimension - 1; xIndex++)
			{
				// Indices of the vertices in the quad
				const int index1 = zIndex * planeDimension + xIndex;
				const int index2 = index1 + 1;
				const int index3 = (zIndex + 1) * planeDimension + xIndex;
				const int index4 = index3 + 1;

				const Vec3F normal1 = Vec3F::Cross(*grid[index2].position - *grid[index1].position, 
										 *grid[index3].position - *grid[index1].position);
				const Vec3F normal2 = Vec3F::Cross(*grid[index4].position - *grid[index3].position,
										 *grid[index3].position - *grid[index2].position);

				// Add face normals to the vertex normals
				planeNormals[index1] += normal1;
				planeNormals[index2] += normal1 + normal2;
				planeNormals[index3] += normal1 + normal2;
				planeNormals[index4] += normal2;
			}
		}

		for (Vec3F& normal : planeNormals)
		{
			normal.Normalize().Negate();
		}

		planeMesh->SetVertices(planeVertices);
		planeMesh->SetNormals(planeNormals);
		planeMesh->UploadMeshData(false);
	}

	void PlanarCloth::ApplyConstraints() const
	{
		for (int i = 0; i < 10; ++i)
		{
			for (const auto& clothSpring : constrainedSprings)
			{
				Vec3F directionBetweenParticles = *clothSpring->otherParticle->position - *clothSpring->particle->position;
				const float distanceApartSquared = directionBetweenParticles.SquareMagnitude();
				const float maxSpringLength = springStretchTolerance * clothSpring->restLength;
				if (distanceApartSquared < maxSpringLength * maxSpringLength)
				{
					continue;
				}
		
				const float difference = std::sqrtf(distanceApartSquared) - maxSpringLength;
				const Vec3F correction = directionBetweenParticles.Normalize() * 0.5f * difference;
		
				if (clothSpring->particle->isFixed)
				{
					*clothSpring->otherParticle->position -= correction * 2.0f;
					continue;
				}
		
				if (clothSpring->otherParticle->isFixed)
				{
					*clothSpring->particle->position += correction * 2.0f;
					continue;
				}
		
				*clothSpring->particle->position += correction;
				*clothSpring->otherParticle->position -= correction;
			}
		}
	}

	void PlanarCloth::SolveCollision()
	{
		if (sphereCollider.first == nullptr)
		{
			return;
		}

		const float squareRadius = sphereCollider.second * sphereCollider.second;
		for (const auto& clothParticle : particles)
		{
			const Vec3F sphereCenterToParticle = *clothParticle->position - sphereCollider.first->GetPosition();
			if (sphereCenterToParticle.SquareMagnitude() < squareRadius)
			{
				*clothParticle->position += sphereCenterToParticle.GetNormalize() * (sphereCollider.second - sphereCenterToParticle.Magnitude());
			}
		}
	}

	void PlanarCloth::InitializeVertices(std::vector<Vec3F>& outVertices)
	{
		const float spaceBetweenVertices = 2.0f / (planeDimension - 1);
		constexpr float startingXPosition = -1.0f;
		constexpr float startingZPosition = -1.0f;
		for (int zIndex = 0; zIndex < planeDimension; ++zIndex)
		{
			float zPos = startingZPosition + spaceBetweenVertices * zIndex;
			for (int xIndex = 0; xIndex < planeDimension; ++xIndex)
			{
				float xPos = startingXPosition + spaceBetweenVertices * xIndex;
				outVertices[zIndex * planeDimension + xIndex] = { xPos, 3.0f, zPos };
			}
		}
	}

	void PlanarCloth::Remove()
	{
		ClothSimSystemLocator::Get()->RemoveClothSimComponent(this);
	}
}