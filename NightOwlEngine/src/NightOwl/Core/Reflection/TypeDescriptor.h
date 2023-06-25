#pragma once

#include "rapidjson/document.h"
#include <cstddef>

namespace NightOwl
{
	class TypeDescriptor
	{
	public:
		TypeDescriptor(const char* name, std::size_t size)
			:	name(name),
				size(size)
		{ }

		virtual ~TypeDescriptor() = default;

		virtual void Serialize(const void* object, rapidjson::Document& document, rapidjson::Value& value) = 0;

		const char* name;

		std::size_t size;
	};

	template<typename T>
	TypeDescriptor* GetPrimitiveDescriptor();
}