#pragma once

#include "TypeDescriptor.h"
#include <iostream>

namespace NightOwl::Core
{
	class IntegerTypeDescriptor : public TypeDescriptor 
	{
	public:
		IntegerTypeDescriptor()
			: TypeDescriptor("int", sizeof(int))
		{
		}

		void Dump(const void* object, int indentLevel) const override
		{
			std::cout << "int{" << *static_cast<const int*>(object) << "}";
		}
	};

	template<>
	inline TypeDescriptor* GetPrimitiveDescriptor<int>()
	{
		static IntegerTypeDescriptor typeDescriptor;
		return &typeDescriptor;
	}
}
