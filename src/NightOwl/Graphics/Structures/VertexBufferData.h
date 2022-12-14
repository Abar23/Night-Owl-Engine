#pragma once

#include "NightOwl/Graphics/Types/VertexDataTypes.h"
#include <string>

namespace NightOwl::Graphics
{
	class VertexBufferData
	{
	public:
		VertexBufferData();

		VertexBufferData(const std::string& shaderAttributeName, VertexDataType type, unsigned int attributeLocation, bool normalize = false);

		const std::string& GetShaderAttributeName() const;

		void SetShaderAttributeName(const std::string& shaderAttributeName);

		VertexDataType GetVertexDataType() const;

		void SetVertexDataType(VertexDataType type);

		bool GetNormalize() const;

		void SetNormalize(bool normalize);

		unsigned int GetNumberOfComponents() const;

		unsigned int GetSizeofData() const;

		void SetAttributeLocation(unsigned int attributeLocation);

		unsigned int GetAttributeLocation() const;

	private:
		std::string shaderAttributeName;

		VertexDataType type;

		bool normalize;

		unsigned int numberOfComponents;

		unsigned int sizeOfData;

		unsigned int attributeLocation;

		void SetDataSizeAndNumberOfComponents();
	};
}
