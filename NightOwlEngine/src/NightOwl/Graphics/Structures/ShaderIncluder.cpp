#include "NightOwlPch.h"

#include "ShaderIncluder.h"
#include "NightOwl/Core/Asset/AssetManager.h"
#include "NightOwl/Core/Locator/Locator.h"
#include "NightOwl/Core/Utitlity/Assert.h"
#include <cstring>

namespace NightOwl
{
	shaderc_include_result* ShaderIncluder::GetInclude(const char* requested_source, shaderc_include_type type, const char* requesting_source, size_t include_depth)
	{
		AssetManager* assetManager = AssetManagerLocator::Get();

		auto& shaderIncludeRepo = assetManager->GetShaderIncludeRepository();

		ENGINE_ASSERT(shaderIncludeRepo.HasAsset(requested_source) == true, "Invalid include {0} requested in {1}", requested_source, requesting_source);

		const std::string* shaderIncludeContent = shaderIncludeRepo.GetAsset(requested_source);

		shaderc_include_result* shadercIncludeResult = new shaderc_include_result();

		shadercIncludeResult->content = shaderIncludeContent->c_str();
		shadercIncludeResult->content_length = shaderIncludeContent->size();

		shadercIncludeResult->source_name = requested_source;
		shadercIncludeResult->source_name_length = std::strlen(requested_source);

		return shadercIncludeResult;
	}

	void ShaderIncluder::ReleaseInclude(shaderc_include_result* data)
	{
		delete data;
	}

	bool ShaderIncluder::HasShaderBeenIncluded(const std::string& shaderName)
	{
		return false;
	}
}
