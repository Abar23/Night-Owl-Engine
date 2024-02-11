#include <NightOwlPch.h>

#include "VertexBufferData.h"
#include "..\Types\VertexDataType.h"

namespace NightOwl
{
	VertexBufferData::VertexBufferData()
		:	type(VertexDataType::None),
			numberOfComponents(0),
			sizeOfData(0),
			attributeLocation(0)
	{
	}

	VertexBufferData::VertexBufferData(VertexDataType type, unsigned int attributeLocation)
		:	type(type),
			attributeLocation(attributeLocation)
	{
		SetDataSizeAndNumberOfComponents();
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

	unsigned VertexBufferData::GetNumberOfComponents() const
	{
		return numberOfComponents;
	}

	unsigned VertexBufferData::GetSizeofData() const
	{
		return sizeOfData;
	}

	void VertexBufferData::SetAttributeLocation(unsigned int attributeLocation)
	{
		this->attributeLocation = attributeLocation;
	}

	unsigned int VertexBufferData::GetAttributeLocation() const
	{
		return attributeLocation;
	}

	unsigned int VertexBufferData::GetOffset() const
	{
		return offset;
	}

	void VertexBufferData::SetOffset(unsigned int offset)
	{
		this->offset = offset;
	}

	void VertexBufferData::SetDataSizeAndNumberOfComponents()
	{
		numberOfComponents = VertexDataTypeToNumberOfComponents(type);
		sizeOfData = VertexDataTypeToDataTypeSize(type);
	}
}
