#include <NightOwlPch.h>

#include "NightOwl/Graphics/Materials/Material.h"
#include "NightOwl/Component/Concrete/Camera.h"
#include "NightOwl/GameObject/GameObject.h"
#include "NightOwl/Graphics/RenderAPI.h"
#include "NightOwl/Core/Asset/AssetManager.h"
#include "NightOwl/Core/Locator/AssetManagerLocator.h"
#include "NightOwl/Core/Utitlity/GlErrorCheck.h"

namespace NightOwl
{
	Material::Material()
	{
		AssetManager* assetManager = AssetManagerLocator::GetAssetManager();
		shader = assetManager->LoadShader("Standard Shader", "./assets/Shaders/Standard.vert", "./assets/Shaders/Standard.frag");
		ProcessShaderUniforms();
	}

	std::shared_ptr<Material> Material::Clone()
	{
		std::shared_ptr<Material> clone = std::make_shared<Material>();

		clone->textureUniformMap = textureUniformMap;
		clone->floatUniformMap = floatUniformMap;
		clone->integerUniformMap = integerUniformMap;
		clone->vectorUniformMap = vectorUniformMap;
		clone->matrixUniformMap = matrixUniformMap;
		clone->shader = shader;

		return clone;
	}

	void Material::Bind()
	{
		for (const auto& uniformIdValuePair : vectorUniformMap | std::views::values)
		{
			shader->SetUniformVec4F(uniformIdValuePair.second, uniformIdValuePair.first);
		}

		for (const auto& uniformIdValuePair : matrixUniformMap | std::views::values)
		{
			shader->SetUniformMat4F(uniformIdValuePair.second, uniformIdValuePair.first);
		}

		for (const auto& uniformIdValuePair : integerUniformMap | std::views::values)
		{
			shader->SetUniformInt(uniformIdValuePair.second, uniformIdValuePair.first);
		}

		for (const auto& uniformIdValuePair : floatUniformMap | std::views::values)
		{
			shader->SetUniformFloat(uniformIdValuePair.second, uniformIdValuePair.first);
		}

		int currentTextureUnit = 0;
		for (const auto& [uniformId, texture] : textureUniformMap | std::views::values)
		{
			shader->SetUniformInt(currentTextureUnit, uniformId);
			if (texture != nullptr)
			{
				texture->Bind(currentTextureUnit);
			}
			currentTextureUnit++;
		}
	}

	void Material::UnBind()
	{
		for (const auto& uniformIdValuePair : textureUniformMap | std::views::values)
		{
			if (uniformIdValuePair.second == nullptr)
			{
				continue;
			}

			uniformIdValuePair.second->Unbind();
		}
	}

	void Material::SetMat4F(const Mat4F& matrix, const std::string& uniformName)
	{
		if (matrixUniformMap.contains(uniformName) == false)
		{
			return;
		}

		matrixUniformMap[uniformName].second = matrix;
	}

	Mat4F Material::GetMat4F(const std::string& uniformName)
	{
		if (matrixUniformMap.contains(uniformName) == false)
		{
			return Mat4F::Identity();
		}

		return matrixUniformMap[uniformName].second;
	}

	void Material::SetVec4F(const Vec4F& vector, const std::string& uniformName)
	{
		if (vectorUniformMap.contains(uniformName) == false)
		{
			return;
		}

		vectorUniformMap[uniformName].second = vector;
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
		if (integerUniformMap.contains(uniformName) == false)
		{
			return;
		}

		integerUniformMap[uniformName].second = value;
	}

	int Material::GetInteger(const std::string& uniformName)
	{
		if (integerUniformMap.contains(uniformName) == false)
		{
			return -1;
		}

		return integerUniformMap[uniformName].second;
	}

	void Material::SetFloat(const float value, const std::string& uniformName)
	{
		if (floatUniformMap.contains(uniformName) == false)
		{
			return;
		}

		floatUniformMap[uniformName].second = value;
	}

	float Material::GetFloat(const std::string& uniformName)
	{
		if (floatUniformMap.contains(uniformName) == false)
		{
			return -1.0f;
		}

		return floatUniformMap[uniformName] .second;
	}

	void Material::SetTexture(const ITexture2D* texture, const std::string& uniformName)
	{
		if (textureUniformMap.contains(uniformName) == false)
		{
			return;
		}

		textureUniformMap[uniformName].second = texture;
	}

	const ITexture2D* Material::GetTexture(const std::string& uniformName)
	{
		if (textureUniformMap.contains(uniformName) == false)
		{
			return nullptr;
		}

		return textureUniformMap[uniformName].second;
	}

	const IShader* Material::GetShader() const
	{
		return shader;
	}

	void Material::ProcessShaderUniforms()
	{
		const auto& uniformMap = shader->GetUniformDataMap();

		for (int shaderUniformMapIndex = 0; shaderUniformMapIndex < static_cast<int>(UniformDataTypes::NumberOfTypes); ++shaderUniformMapIndex)
		{
			const auto& uniformNameToLocationVector = uniformMap.at(shaderUniformMapIndex);
			for (const auto& [uniformName, uniformLocation] : uniformNameToLocationVector)
			{
				switch (static_cast<UniformDataTypes>(shaderUniformMapIndex))
				{
				case UniformDataTypes::Float: 
					floatUniformMap[uniformName] = std::make_pair(uniformLocation, -1.0f);
					break;

				case UniformDataTypes::Vec4F:
					vectorUniformMap[uniformName] = std::make_pair(uniformLocation, Vec4F(1.0f));
					break;

				case UniformDataTypes::Mat4F:
					matrixUniformMap[uniformName] = std::make_pair(uniformLocation, Mat4F::Identity());
					break;

				case UniformDataTypes::Int:
					integerUniformMap[uniformName] = std::make_pair(uniformLocation, -1);
					break;

				case UniformDataTypes::Texture:
					textureUniformMap[uniformName] = std::make_pair(uniformLocation, nullptr);
					break;

				case UniformDataTypes::Buffer:
					// create buffer property map
					break;
				default: 
					break;
				}
			}
		}
	}
}
