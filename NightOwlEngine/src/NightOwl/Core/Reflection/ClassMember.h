#pragma once

#include "TypeDescriptor.h"

namespace NightOwl
{
	class ClassMember
	{
	public:
		const char* name;

		std::size_t offsetIntoClass;

		TypeDescriptor* typeDescriptor;
	};
}
