#include <NightOwlPch.h>

#include "NightOwl/Graphics/Materials/Material.h"
#include "NightOwl/Component/Concrete/Camera.h"
#include "NightOwl/GameObject/GameObject.h"
#include "NightOwl/Graphics/RenderAPI.h"
#include "NightOwl/Core/Asset/AssetManager.h"
#include "NightOwl/Core/Locator/AssetManagerLocator.h"

namespace NightOwl
{
	Material::Material()
		:	diffuseTexture(nullptr),
			alpha(1.0f)
	{
		AssetManager* assetManager = AssetManagerLocator::GetAssetManager();
		shader = assetManager->LoadShader("Standard Shader", "./assets/Shaders/Standard.vert", "./assets/Shaders/Standard.frag");
	}

	std::shared_ptr<Material> Material::Clone()
	{
		std::shared_ptr<Material> clone = std::make_shared<Material>();

		clone->shader = shader;
		clone->diffuseTexture = diffuseTexture;

		return clone;
	}

	void Material::Draw(Renderer& renderer)
	{
		const std::shared_ptr<Mesh> mesh = renderer.GetMesh();
		Transform* transform = renderer.GetGameObject().GetTransform();

		shader->Bind();
		shader->SetUniformMat4F(transform->GetWorldMatrix(), "modelMatrix");
		shader->SetUniformMat4F(Camera::GetMainCamera()->GetViewProjectionMatrix(), "viewProjectionMatrix");

		shader->SetUniformFloat(alpha, "doesNotExist");

		if(diffuseTexture != nullptr)
		{
			shader->SetUniformInt(0, "inputTexture");
			shader->SetUniformInt(1, "isInputTextureSet");
			diffuseTexture->Bind(0);
		}
		else
		{
			shader->SetUniformInt(0, "isInputTextureSet");
		}

		if (finalBoneMatrices.empty() == false)
		{
			for (int i = 0; i < finalBoneMatrices.size(); ++i)
				shader->SetUniformMat4F(finalBoneMatrices[i], "finalBonesMatrices[" + std::to_string(i) + "]");
			shader->SetUniformInt(1, "hasBones");
		}
		else
		{
			shader->SetUniformInt(0, "hasBones");
		}

		mesh->Bind();
		for (const auto& subMesh : mesh->GetSubMeshes())
		{
			RenderApi::GetContext()->DrawIndexedBaseVertex(DrawType::Triangles, subMesh.indexCount, subMesh.indexStart, subMesh.baseVertex);
		}
		mesh->Unbind();


		if(diffuseTexture != nullptr)
		{
			shader->SetUniformInt(0, "isInputTextureSet");
			diffuseTexture->Unbind();
		}
		shader->Unbind();
	}

	void Material::Bind()
	{
		shader->Bind();

		for (const auto& uniformIdVectorPair : vectorUniformMap | std::views::values)
		{
			shader->SetUniformVec4F(uniformIdVectorPair.second, uniformIdVectorPair.first);
		}

		for (const auto& uniformIdVectorPair : matrixUniformMap | std::views::values)
		{
			shader->SetUniformMat4F(uniformIdVectorPair.second, uniformIdVectorPair.first);
		}

		for (const auto& uniformIdVectorPair : integerUniformMap | std::views::values)
		{
			shader->SetUniformInt(uniformIdVectorPair.second, uniformIdVectorPair.first);
		}

		for (const auto& uniformIdVectorPair : floatUniformMap | std::views::values)
		{
			shader->SetUniformFloat(uniformIdVectorPair.second, uniformIdVectorPair.first);
		}

		int currentTextureUnit;
		for (const auto& uniformIdVectorPair : textureUniformMap | std::views::values)
		{
			shader->SetUniformInt(0, "inputTexture");
		}
	}

	void Material::SetMat4F(const Mat4F& matrix, const std::string& uniformName)
	{
		int uniformLocationId = shader->GetUniformLocation(uniformName);
		if (uniformLocationId == -1)
		{
			return;
		}

		matrixUniformMap[uniformName] = std::make_pair(uniformLocationId, matrix);
	}

	Mat4F Material::GetMat4F(const std::string& uniformName)
	{
		if (matrixUniformMap.contains(uniformName) == false)
		{
			return Mat4F();
		}

		return matrixUniformMap[uniformName].second;
	}

	void Material::SetVec4F(const Vec4F& vector, const std::string& uniformName)
	{
		int uniformLocationId = shader->GetUniformLocation(uniformName);
		if (uniformLocationId == -1)
		{
			return;
		}

		vectorUniformMap[uniformName] = std::make_pair(uniformLocationId, vector);
	}

	Vec4F Material::GetVec4F(const std::string& uniformName)
	{
		if (vectorUniformMap.contains(uniformName) == false)
		{
			return Vec4F::Zero();
		}

		return vectorUniformMap[uniformName].second;
	}
	
	void Material::SetInteger(const int value, const std::string& uniformName)
	{
		int uniformLocationId = shader->GetUniformLocation(uniformName);
		if (uniformLocationId == -1)
		{
			return;
		}

		integerUniformMap[uniformName] = std::make_pair(uniformLocationId, value);
	}

	int Material::GetInteger(const std::string& uniformName)
	{
		if (integerUniformMap.contains(uniformName) == false)
		{
			return 0;
		}

		return integerUniformMap[uniformName].second;
	}

	void Material::SetFloat(const float value, const std::string& uniformName)
	{
		int uniformLocationId = shader->GetUniformLocation(uniformName);
		if (uniformLocationId == -1)
		{
			return;
		}

		floatUniformMap[uniformName] = std::make_pair(uniformLocationId, value);
	}

	float Material::GetFloat(const std::string& uniformName)
	{
		if (floatUniformMap.contains(uniformName) == false)
		{
			return 0.0f;
		}

		return floatUniformMap[uniformName] .second;
	}

	void Material::SetTexture(const std::shared_ptr<ITexture2D>& texture, const std::string& uniformName)
	{
		int uniformLocationId = shader->GetUniformLocation(uniformName);
		if (uniformLocationId == -1)
		{
			return;
		}

		textureUniformMap[uniformName] = std::make_pair(uniformLocationId, texture.get());
	}

	ITexture2D* Material::GetTexture(const std::string& uniformName)
	{
		if (textureUniformMap.contains(uniformName) == false)
		{
			return nullptr;
		}

		return textureUniformMap[uniformName].second;
	}

	void Material::SetTexture(ITexture2D* texture2D)
	{
		this->diffuseTexture = texture2D;
	}

	ITexture2D* Material::GetTexture()
	{
		return diffuseTexture;
	}
}
