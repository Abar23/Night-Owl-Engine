#include "NightOwlPch.h"

#include "DebugSystem.h"
#include "NightOwl/Component/Concrete/Camera.h"
#include "NightOwl/Core/Asset/AssetManager.h"
#include "NightOwl/Core/Locator/AssetManagerLocator.h"
#include "NightOwl/Graphics/Graphics.h"
#include "NightOwl/Graphics/Structures/VertexBufferLayout.h"

namespace NightOwl
{
	void DebugSystem::Init()
	{
		AssetManager* assetManager = AssetManagerLocator::GetAssetManager();

		// Set up debug shader for debug material
		debugMaterial = std::make_shared<Material>();
		IShader* debugShader = assetManager->GetShaderRepository().GetAsset("DebugShader");
		debugMaterial->SetShader(debugShader);

		// Define data to line segments vertex buffer
		vertexBuffer = Graphics::CreateVertexBuffer();

		VertexBufferData lineVertexData("Line Vertices", VertexDataType::VectorFloat3, 0);

		VertexBufferLayout vertexDataLayout;
		vertexDataLayout.AddVertexBufferDataDefinition(lineVertexData);

		vertexBuffer->SetVertexBufferLayout(vertexDataLayout);

		// Setup inputs to shader, in this case it is line vertices only
		vertexArrayObject = Graphics::CreateVertexArrayObject();
		vertexArrayObject->SetVertexBuffer(vertexBuffer);
		vertexArrayObject->SetupVertexBufferAttributes();
	}

	void DebugSystem::Update()
	{
		if (lineSegments.empty())
		{
			return;
		}

		IContext* renderContext = Graphics::GetContext().get();

		// Populate vertex buffer with lines
		const VertexBufferLayout vertexBufferLayout = vertexBuffer->GetVertexBufferLayout();
		vertexBuffer->SetSize(vertexBufferLayout.GetDataPerVertex() * lineSegments.size() * 2);
		vertexBuffer->SetData(lineSegments.data());

		// Set camera info
		Camera* mainCamera = Camera::GetMainCamera();
		const Mat4F viewProjectionMatrix = mainCamera->GetViewProjectionMatrix();
		debugMaterial->SetMat4F(viewProjectionMatrix, "viewProjectionMatrix");

		// Draw Lines
		vertexArrayObject->Bind();
		debugMaterial->GetShader()->Bind();
		debugMaterial->Bind();
		for (int lineSegmentIndex = 0; lineSegmentIndex < lineSegments.size(); ++lineSegmentIndex)
		{
			renderContext->DrawArrays(DrawType::Lines, 2, lineSegmentIndex * 2);
		}
		debugMaterial->Unbind();
		debugMaterial->GetShader()->Unbind();
		vertexArrayObject->Unbind();

		lineSegments.clear();
	}

	void DebugSystem::DrawLine(const Vec3F& start, const Vec3F& end)
	{
		lineSegments.emplace_back(start, end);
	}
}
