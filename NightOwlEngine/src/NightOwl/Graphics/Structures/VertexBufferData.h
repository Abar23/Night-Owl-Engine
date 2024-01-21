#pragma once

namespace NightOwl
{
	enum class VertexDataType;

	class VertexBufferData
	{
	public:
		VertexBufferData();

		VertexBufferData(VertexDataType type, unsigned int attributeLocation);

		VertexDataType GetVertexDataType() const;

		void SetVertexDataType(VertexDataType type);

		unsigned int GetNumberOfComponents() const;

		unsigned int GetSizeofData() const;

		void SetAttributeLocation(unsigned int attributeLocation);

		unsigned int GetAttributeLocation() const;

		unsigned int GetOffset() const;

		void SetOffset(unsigned int offset);

	private:
		VertexDataType type;

		unsigned int numberOfComponents;

		unsigned int sizeOfData;

		unsigned int attributeLocation;

		unsigned int offset;

		void SetDataSizeAndNumberOfComponents();
	};
}
