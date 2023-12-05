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
		SetupLineGraphics();

		SetupPointGraphics();
	}

	void DebugSystem::Update()
	{
		IContext* renderContext = Graphics::GetContext().get();

		DrawLines(renderContext);

		DrawPoints(renderContext);
	}

	void DebugSystem::DrawLine(const Vec3F& start, const Vec3F& end, const Vec3F& color /* = { 0.0f, 1.0f, 0.0 }) */)
	{
		lineSegments.emplace_back(start, color);
		lineSegments.emplace_back(end, color);
	}

	void DebugSystem::DrawPoint(const Vec3F& point, const Vec3F& color /* = { 0.0f, 1.0f, 0.0 } */)
	{
		points.push_back({ point, color });
	}

	void DebugSystem::SetupLineGraphics()
	{
		AssetManager* assetManager = AssetManagerLocator::GetAssetManager();

		// Set up debug shader for debug material
		debugLineMaterial = std::make_shared<Material>();
		IShader* debugShader = assetManager->GetShaderRepository().GetAsset("DebugLineShader");
		debugLineMaterial->SetShader(debugShader);

		// Define data to line segments vertex buffer
		lineVertexBuffer = Graphics::CreateVertexBuffer();

		VertexBufferData lineVertexData("Position", VertexDataType::VectorFloat3, 0);
		VertexBufferData colorData("Color", VertexDataType::VectorFloat3, 1);

		VertexBufferLayout vertexDataLayout;
		vertexDataLayout.AddVertexBufferDataDefinition(lineVertexData);
		vertexDataLayout.AddVertexBufferDataDefinition(colorData);

		lineVertexBuffer->SetVertexBufferLayout(vertexDataLayout);

		// Setup inputs to shader, in this case it is line vertices only
		lineVertexArrayObject = Graphics::CreateVertexArrayObject();
		lineVertexArrayObject->SetVertexBuffer(lineVertexBuffer);
		lineVertexArrayObject->SetupVertexBufferAttributes();
	}

	void DebugSystem::SetupPointGraphics()
	{
		AssetManager* assetManager = AssetManagerLocator::GetAssetManager();

		// Set up debug shader for debug material
		debugPointMaterial = std::make_shared<Material>();
		IShader* debugShader = assetManager->GetShaderRepository().GetAsset("DebugPointShader");
		debugPointMaterial->SetShader(debugShader);

		// Define data to line segments vertex buffer
		pointVertexBuffer = Graphics::CreateVertexBuffer();

		VertexBufferData positionData("Position", VertexDataType::VectorFloat3, 0);
		VertexBufferData colorData("Color", VertexDataType::VectorFloat3, 1);

		VertexBufferLayout vertexDataLayout;
		vertexDataLayout.AddVertexBufferDataDefinition(positionData);
		vertexDataLayout.AddVertexBufferDataDefinition(colorData);

		pointVertexBuffer->SetVertexBufferLayout(vertexDataLayout);

		// Setup inputs to shader, in this case it is line vertices only
		pointVertexArrayObject = Graphics::CreateVertexArrayObject();
		pointVertexArrayObject->SetVertexBuffer(pointVertexBuffer);
		pointVertexArrayObject->SetupVertexBufferAttributes();
	}

	void DebugSystem::DrawLines(IContext* renderContext)
	{
		if (lineSegments.empty())
		{
			return;
		}

		// Set camera info
		Camera* mainCamera = Camera::GetMainCamera();
		const Mat4F viewProjectionMatrix = mainCamera->GetViewProjectionMatrix();
		debugLineMaterial->SetMat4F(viewProjectionMatrix, "viewProjectionMatrix");


		// Populate vertex buffer with lines
		const VertexBufferLayout vertexBufferLayout = lineVertexBuffer->GetVertexBufferLayout();
		lineVertexBuffer->SetSize(vertexBufferLayout.GetDataPerVertex() * lineSegments.size());
		lineVertexBuffer->SetData(lineSegments.data());

		// Draw Lines
		lineVertexArrayObject->Bind();
		debugLineMaterial->GetShader()->Bind();
		debugLineMaterial->Bind();

		renderContext->DrawArrays(DrawType::Lines, lineSegments.size(), 0);

		debugLineMaterial->Unbind();
		debugLineMaterial->GetShader()->Unbind();
		lineVertexArrayObject->Unbind();

		lineSegments.clear();
	}

	void DebugSystem::DrawPoints(IContext* renderContext)
	{
		if (points.empty())
		{
			return;
		}

		// Set camera info
		Camera* mainCamera = Camera::GetMainCamera();
		const Mat4F viewProjectionMatrix = mainCamera->GetViewProjectionMatrix();
		debugPointMaterial->SetMat4F(viewProjectionMatrix, "viewProjectionMatrix");

		// Populate vertex buffer with lines
		const VertexBufferLayout vertexBufferLayout = pointVertexBuffer->GetVertexBufferLayout();
		pointVertexBuffer->SetSize(vertexBufferLayout.GetDataPerVertex() * points.size());
		pointVertexBuffer->SetData(points.data());

		// Draw Lines
		pointVertexArrayObject->Bind();
		debugPointMaterial->GetShader()->Bind();
		debugPointMaterial->Bind();

		renderContext->DrawArrays(DrawType::Points, points.size(), 0);

		debugPointMaterial->Unbind();
		debugPointMaterial->GetShader()->Unbind();
		pointVertexArrayObject->Unbind();

		points.clear();
	}
}
