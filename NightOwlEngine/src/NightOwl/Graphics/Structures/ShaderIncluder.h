#pragma once

#include <unordered_set>
#include <shaderc/shaderc.hpp>

namespace NightOwl
{
	class ShaderIncluder : public shaderc::CompileOptions::IncluderInterface
	{
	public:
		shaderc_include_result* GetInclude(const char* requested_source, shaderc_include_type type, const char* requesting_source, size_t include_depth) override;

		void ReleaseInclude(shaderc_include_result* data) override;

	private:
		std::unordered_set<std::string> shadersAlreadyIncluded;

		bool HasShaderBeenIncluded(const std::string& shaderName);
	};
}
