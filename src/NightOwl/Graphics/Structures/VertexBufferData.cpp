#include "VertexBufferData.h"
#include "NightOwl/Graphics/Types/VertexDataTypes.h"

namespace NightOwl::Graphics
{
	VertexBufferData::VertexBufferData()
		: type(VertexDataType::None), normalize(false), numberOfComponents(0), sizeOfData(0)
	{
	}

	VertexBufferData::VertexBufferData(const std::string& shaderAttributeName, VertexDataType type, bool normalize)
		: shaderAttributeName{ shaderAttributeName }, type(type), normalize(normalize)
	{
		SetDataSizeAndNumberOfComponents();
	}

	const std::string& VertexBufferData::GetShaderAttributeName() const
	{
		return shaderAttributeName;
	}

	void VertexBufferData::SetShaderAttributeName(const std::string& shaderAttributeName)
	{
		this->shaderAttributeName = shaderAttributeName;
	}

	VertexDataType VertexBufferData::GetVertexDataType() const
	{
		return type;
	}

	void VertexBufferData::SetVertexDataType(VertexDataType type)
	{
		this->type = type;
		SetDataSizeAndNumberOfComponents();
	}

	bool VertexBufferData::GetNormalize() const
	{
		return normalize;
	}

	void VertexBufferData::SetNormalize(bool normalize)
	{
		this->normalize = normalize;
	}

	unsigned VertexBufferData::GetNumberOfComponents() const
	{
		return numberOfComponents;
	}

	unsigned VertexBufferData::GetSizeofData() const
	{
		return sizeOfData;
	}

	void VertexBufferData::SetDataSizeAndNumberOfComponents()
	{
		numberOfComponents = VertexDataTypeToNumberOfComponents(type);
		sizeOfData = VertexDataTypeToDataTypeSize(type);
	}
}
