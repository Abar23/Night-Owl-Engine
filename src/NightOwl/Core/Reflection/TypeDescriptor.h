#pragma once

#include <cstddef>
#include <string>

namespace NightOwl::Core
{
	class TypeDescriptor
	{
	public:
		TypeDescriptor(const char* name, std::size_t size)
			:	name(name),
				size(size)
		{ }

		virtual ~TypeDescriptor();

		virtual std::string getName() const
		{
			return name;
		}

		virtual void Dump(const void* object, int indentLevel = 0) const = 0;

	private:
		const char* name;

		std::size_t size;
	};

	template<typename T>
	TypeDescriptor* GetPrimitiveDescriptor();
}