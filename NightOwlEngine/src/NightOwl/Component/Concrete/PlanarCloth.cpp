#include "NightOwlPch.h"

#include "PlanarCloth.h"
#include "MeshRenderer.h"
#include "NightOwl/Component/Structures/ClothParticle.h"
#include "NightOwl/Component/Structures/ClothSpring.h"
#include "NightOwl/Component/Structures/Mesh.h"
#include "NightOwl/Core/Locator/ClothSimSystemLocator.h"
#include "NightOwl/Core/Locator/DebugSystemLocator.h"
#include "NightOwl/Core/Time/Time.h"
#include "NightOwl/GameObject/GameObject.h"
#include "NightOwl/Graphics/Debugging/DebugSystem.h"
#include "NightOwl/Physics/ClothSimSystem.h"

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
		  sphereCollider(nullptr, -1.0f)
	{
		ClothSimSystemLocator::GetClothSimSystem()->AddClothSimComponent(this);
	}

	void PlanarCloth::Start()
	{
		planeMesh = gameObject->GetComponent<MeshRenderer>()->GetMesh();
	}

	void PlanarCloth::Update()
	{
		ClearForces();

		ApplyForces();

		VerletIntegration();

		ApplyConstraints();

		SolveCollision();

		UpdateVertexData();
	}

	void PlanarCloth::DebugRender()
	{
		DebugSystem* debugSystem = DebugSystemLocator::GetDebugSystem();
		for (const auto& clothSpring : springs)
		{
			Vec3F color = { 1.0f, 0.0f, 1.0f };
			if (clothSpring->type == SpringType::Shear)
			{
				color = { 1.0f, 0.64f, 0.0f };
			}
			else if (clothSpring->type == SpringType::Flexion)
			{
				color = { 0.0f, 1.0f, 1.0f };
			}

			debugSystem->DrawLine(clothSpring->particle->position, clothSpring->otherParticle->position, color);
		}

		for (const auto& corner : corners)
		{
			debugSystem->DrawPoint(corner->position, { 1.0f, 1.0f, 1.0f });
		}
	}

	void PlanarCloth::Reset()
	{
		std::vector<Vec3F> vertices(particles.size());
		InitializeVertices(vertices);
		planeMesh->SetVertices(vertices);

		for (int particleIndex = 0; particleIndex < particles.size(); ++particleIndex)
		{
			ClothParticle* clothParticle = particles[particleIndex];
			clothParticle->position = vertices[particleIndex];
			clothParticle->previousPosition = clothParticle->position;
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
		std::vector<Vec3F> vertices(dimension * dimension);
		InitializeVertices(vertices);

		std::vector<Vec3UI> triangles((dimension - 1) * (dimension - 1) * 2);
		int triangleIndex = 0;
		for (int zIndex = 0; zIndex < dimension - 1; ++zIndex)
		{
			for (int xIndex = 0; xIndex < dimension - 1; ++xIndex)
			{
				// upper triangle
				Vec3UI upperTriangle;
				upperTriangle.x = xIndex + zIndex * dimension;
				upperTriangle.y = xIndex + (zIndex + 1) * dimension;
				upperTriangle.z = (xIndex + 1) + zIndex * dimension;

				// lower triangle
				Vec3UI lowerTriangle;
				lowerTriangle.x = (xIndex + 1) + zIndex * dimension;
				lowerTriangle.y = xIndex + (zIndex + 1) * dimension;
				lowerTriangle.z = (xIndex + 1) + (zIndex + 1) * dimension;

				triangles[triangleIndex] = upperTriangle;
				triangles[triangleIndex + 1] = lowerTriangle;
				triangleIndex += 2;
			}
		}

		int indexCount = triangles.size() * 3;
		int vertexCount = vertices.size();
		std::vector<SubMeshData> subMeshData = {
			{ 0, indexCount, 0, vertexCount, 0}
		};

		planeMesh->SetVertices(vertices);
		planeMesh->SetTriangles(triangles);
		planeMesh->SetSubMeshes(subMeshData);
		planeMesh->UploadMeshData();

		grid.resize(dimension, std::vector<std::shared_ptr<ClothParticle>>(dimension));
		for (int zIndex = 0; zIndex < dimension; ++zIndex)
		{
			for (int xIndex = 0; xIndex < dimension; ++xIndex)
			{
				std::shared_ptr<ClothParticle> particle = std::make_shared<ClothParticle>();
				particle->row = zIndex;
				particle->column = xIndex;
				particle->isCorner = false;
				grid[zIndex][xIndex] = particle;

				particles.push_back(particle.get());
			}
		}

		const float structuralEdgeLength = 2.0f / static_cast<float>(dimension - 1);
		const float flexionEdgeLength = 2.0f * structuralEdgeLength;
		const float shearEdgeLength = std::sqrtf(structuralEdgeLength * structuralEdgeLength * 2);
		for (int zIndex = 0; zIndex < dimension; ++zIndex)
		{
			for (int xIndex = 0; xIndex < dimension; ++xIndex)
			{
				// direct neighbors (up, down, left, right) - forms structural springs
				if (zIndex - 1 >= 0) { // up
					auto spring = std::make_shared<ClothSpring>(grid[zIndex][xIndex].get(), grid[zIndex - 1][xIndex].get(), structuralEdgeLength, SpringType::Structural);
					springs.emplace_back(spring);
				}
				if (zIndex + 1 < dimension) { // down
					auto spring = std::make_shared<ClothSpring>(grid[zIndex][xIndex].get(), grid[zIndex + 1][xIndex].get(), structuralEdgeLength, SpringType::Structural);
					springs.emplace_back(spring);
				}
				if (xIndex - 1 >= 0) { // left
					auto spring = std::make_shared<ClothSpring>(grid[zIndex][xIndex].get(), grid[zIndex][xIndex - 1].get(), structuralEdgeLength, SpringType::Structural);
					springs.emplace_back(spring);
				}
				if (xIndex + 1 < dimension) { // right
					auto spring = std::make_shared<ClothSpring>(grid[zIndex][xIndex].get(), grid[zIndex][xIndex + 1].get(), structuralEdgeLength, SpringType::Structural);
					springs.push_back(spring);
				}

				// diagonal neighbors - forms shear springs
				if (zIndex - 1 >= 0 && xIndex - 1 >= 0) { // top-left
					auto spring = std::make_shared<ClothSpring>(grid[zIndex][xIndex].get(), grid[zIndex - 1][xIndex - 1].get(), shearEdgeLength, SpringType::Shear);
					springs.push_back(spring);
				}
				if (zIndex - 1 >= 0 && xIndex + 1 < dimension) { // top-right
					auto spring = std::make_shared<ClothSpring>(grid[zIndex][xIndex].get(), grid[zIndex - 1][xIndex + 1].get(), shearEdgeLength, SpringType::Shear);
					springs.push_back(spring);
				}
				if (zIndex + 1 < dimension && xIndex - 1 >= 0) { // bottom-left
					auto spring = std::make_shared<ClothSpring>(grid[zIndex][xIndex].get(), grid[zIndex + 1][xIndex - 1].get(), shearEdgeLength, SpringType::Shear);
					springs.push_back(spring);
				}
				if (zIndex + 1 < dimension && xIndex + 1 < dimension) { // bottom-right
					auto spring = std::make_shared<ClothSpring>(grid[zIndex][xIndex].get(), grid[zIndex + 1][xIndex + 1].get(), shearEdgeLength, SpringType::Shear);
					springs.push_back(spring);
				}

				// flexion neighbors (up+1, down+1, left+1, right+1) - forms flexion springs
				if (zIndex - 2 >= 0) { // up+1
					auto spring = std::make_shared<ClothSpring>(grid[zIndex][xIndex].get(), grid[zIndex - 2][xIndex].get(), flexionEdgeLength, SpringType::Flexion);
					springs.push_back(spring);
				}
				if (zIndex + 2 < dimension) { // down+1
					auto spring = std::make_shared<ClothSpring>(grid[zIndex][xIndex].get(), grid[zIndex + 2][xIndex].get(), flexionEdgeLength, SpringType::Flexion);
					springs.push_back(spring);
				}
				if (xIndex - 2 >= 0) { // left+1
					auto spring = std::make_shared<ClothSpring>(grid[zIndex][xIndex].get(), grid[zIndex][xIndex - 2].get(), flexionEdgeLength, SpringType::Flexion);
					springs.push_back(spring);
				}
				if (xIndex + 2 < dimension) { // right+1
					auto spring = std::make_shared<ClothSpring>(grid[zIndex][xIndex].get(), grid[zIndex][xIndex + 2].get(), flexionEdgeLength, SpringType::Flexion);
					springs.push_back(spring);
				}

				Vec3F position = vertices[zIndex * dimension + xIndex];
				grid[zIndex][xIndex]->position = position;
				grid[zIndex][xIndex]->previousPosition = position;
			}
		}

		// get corners
		corners.resize(4);
		corners[0] = grid[0][0].get();
		corners[1] = grid[0][dimension - 1].get();
		corners[2] = grid[dimension - 1][0].get();
		corners[3] = grid[dimension - 1][dimension - 1].get();

		corners[0]->isCorner = true;
		corners[0]->isFixed = true;
		corners[1]->isCorner = true;
		corners[1]->isFixed = true;
		corners[2]->isCorner = true;
		corners[2]->isFixed = true;
		corners[3]->isCorner = true;
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
		ENGINE_ASSERT(cornerIndex < 4 && cornerIndex >- 0, "Invalid corner index used for unpinning corner.")

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

	void PlanarCloth::ClearForces()
	{
		for (auto& zIndex : grid)
		{
			for (int xIndex = 0; xIndex < grid.size(); ++xIndex)
			{
				zIndex[xIndex]->totalForce = 0.0f;
			}
		}
	}

	void PlanarCloth::ApplyForces()
	{
		// Apply gravity
		for (const auto& clothParticle : particles)
		{
			clothParticle->totalForce += Vec3F(0.0f, gravity, 0.0f) * mass;
		}

		// Apply spring forces
		for (const auto& clothSpring : springs)
		{
			Vec3F particlePositionDifference = clothSpring->otherParticle->position - clothSpring->particle->position;

			float springLength = particlePositionDifference.Magnitude();

			Vec3F springDirection = particlePositionDifference.GetNormalize();

			float springConstant = structuralSpringConstant;
			float dampConstant = structuralDampConstant;
			if (clothSpring->type == SpringType::Shear)
			{
				springConstant = shearSpringConstant;
				dampConstant = shearDampConstant;
			}
			else if (clothSpring->type == SpringType::Flexion)
			{
				springConstant = flexionSpringConstant;
				dampConstant = flexionDampConstant;
			}

			const float springStiffnessTerm = springConstant * (springLength - clothSpring->restLength);
			const float springDampTerm = dampConstant * Vec3F::Dot(clothSpring->otherParticle->position - clothSpring->particle->position, springDirection);
			const Vec3F springForce = springDirection * (springStiffnessTerm - springDampTerm);

			clothSpring->particle->totalForce += springForce;
			clothSpring->otherParticle->totalForce -= springForce;
		}

		// Remove force from pinned corners
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
		const float fixedTimeStep = Time::GetFixedDeltaTime();
		for (const auto & clothParticle : particles)
		{
			if (clothParticle->isFixed)
			{
				continue;
			}

			// Verlet
			const Vec3F velocity = (clothParticle->position - clothParticle->previousPosition);
			clothParticle->previousPosition = clothParticle->position;
			clothParticle->position = clothParticle->position + velocity + (clothParticle->totalForce / mass) * fixedTimeStep * fixedTimeStep;
		}
	}

	void PlanarCloth::UpdateVertexData()
	{
		std::vector<Vec3F> vertices = planeMesh->GetVertices();
		for (const auto& clothParticle : particles)
		{
			vertices[clothParticle->row * grid.size() + clothParticle->column] = clothParticle->position;
		}

		std::vector<Vec3F> normals(vertices.size());
		for (int zIndex = 0; zIndex < grid.size() - 1; zIndex++)
		{
			for (int xIndex = 0; xIndex < grid.size() - 1; xIndex++)
			{
				// Indices of the vertices in the quad
				const int index1 = zIndex * grid.size() + xIndex;
				const int index2 = index1 + 1;
				const int index3 = (zIndex + 1) * grid.size() + xIndex;
				const int index4 = index3 + 1;

				const Vec3F normal1 = Vec3F::Cross(grid[zIndex][xIndex + 1]->position - grid[zIndex][xIndex]->position, grid[zIndex + 1][xIndex]->position - grid[zIndex][xIndex]->position);
				const Vec3F normal2 = Vec3F::Cross(grid[zIndex + 1][xIndex + 1]->position - grid[zIndex + 1][xIndex]->position, grid[zIndex + 1][xIndex]->position - grid[zIndex][xIndex + 1]->position);

				// Add face normals to the vertex normals
				normals[index1] += normal1;
				normals[index2] += normal1 + normal2;
				normals[index3] += normal1 + normal2;
				normals[index4] += normal2;
			}
		}

		for (Vec3F& normal : normals)
		{
			normal.Normalize().Negate();
		}

		planeMesh->SetVertices(vertices);
		planeMesh->SetNormals(normals);
	}

	void PlanarCloth::ApplyConstraints()
	{
		for (int i = 0; i < 10; ++i)
		{
			for (const auto& clothSpring : springs)
			{
				Vec3F directionBetweenParticles = clothSpring->otherParticle->position - clothSpring->particle->position;
				const float distanceApart = directionBetweenParticles.Magnitude();

				float maxSpringLength = springStretchTolerance * clothSpring->restLength;

				if (clothSpring->type == SpringType::Flexion ||
					distanceApart < maxSpringLength)
				{
					continue;
				}

				const float difference = distanceApart - maxSpringLength;
				const Vec3F correction = directionBetweenParticles.Normalize() * 0.5f * difference;

				if (clothSpring->particle->isCorner && 
					clothSpring->particle->isFixed)
				{
					clothSpring->otherParticle->position -= correction * 2.0f;
					continue;
				}

				if (clothSpring->otherParticle->isCorner && 
					clothSpring->otherParticle->isFixed)
				{
					clothSpring->particle->position += correction * 2.0f;
					continue;
				}

				clothSpring->particle->position += correction;
				clothSpring->otherParticle->position -= correction;
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
			Vec3F sphereCenterToParticle = clothParticle->position - sphereCollider.first->GetPosition();
			if (sphereCenterToParticle.SquareMagnitude() < squareRadius)
			{
				clothParticle->position += sphereCenterToParticle.GetNormalize() * (sphereCollider.second - sphereCenterToParticle.Magnitude());
			}
		}
	}

	void PlanarCloth::InitializeVertices(std::vector<Vec3F>& outVertices)
	{
		const float dimension = std::sqrtf(outVertices.size());
		const float spaceBetweenVertices = 2.0f / (std::sqrtf(outVertices.size()) - 1);
		constexpr float startingXPosition = -1.0f;
		constexpr float startingZPosition = -1.0f;
		for (int zIndex = 0; zIndex < dimension; ++zIndex)
		{
			float zPos = startingZPosition + spaceBetweenVertices * zIndex;
			for (int xIndex = 0; xIndex < dimension; ++xIndex)
			{
				float xPos = startingXPosition + spaceBetweenVertices * xIndex;
				outVertices[xIndex + zIndex * dimension] = { xPos, 3.0f, zPos };
			}
		}
	}

	void PlanarCloth::Remove()
	{
	}
}
