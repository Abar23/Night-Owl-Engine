#pragma once

#include "NightOwl/Component/Abstrtact/Renderer.h"
#include "NightOwl/Graphics/Interfaces/IShader.h"
#include "NightOwl/Graphics/Interfaces/ITexture2D.h"
#include <map>

namespace NightOwl
{
	class Material
	{
	public:
		Material();

		Material(const std::string& shaderName);

		std::shared_ptr<Material> Clone();

		void Bind();

		void Unbind();

		void SetMat4F(const Mat4F& matrix, const std::string& uniformName);

		Mat4F GetMat4F(const std::string& uniformName);

		void SetVec4F(const Vec4F& vector, const std::string& uniformName);

		Vec4F GetVec4F(const std::string& uniformName);

		void SetInteger(const int value, const std::string& uniformName);

		int GetInteger(const std::string& uniformName);

		void SetFloat(const float value, const std::string& uniformName);

		float GetFloat(const std::string& uniformName);

		void SetTexture(const ITexture2D* texture, const std::string& uniformName);

		const ITexture2D* GetTexture(const std::string& uniformName);

		const IShader* GetShader() const;

		void SetShader(IShader* shader);

	private:
		IShader* shader;

		std::map<std::string, std::pair<int, const ITexture2D*>> textureUniformMap;

		std::map<std::string, std::pair<int, float>> floatUniformMap;

		std::map<std::string, std::pair<int, int>> integerUniformMap;

		std::map<std::string, std::pair<int, Vec4F>> vectorUniformMap;

		std::map<std::string, std::pair<int, Mat4F>> matrixUniformMap;

		void ProcessShaderUniforms();

		void ClearUniformMaps();
	};
}
