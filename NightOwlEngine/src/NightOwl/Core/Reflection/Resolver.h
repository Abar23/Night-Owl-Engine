#pragma once

#include "TypeDescriptor.h"
#include "PrimitiveTypes.h"

namespace NightOwl
{
	template<typename T>
	class IsReflected
	{
		template<typename U> static char TestIfClassIsReflected(decltype(&U::classDescription));
		template<typename U> static int TestIfClassIsReflected(...);

	public:
		enum { Value = sizeof(TestIfClassIsReflected<T>(nullptr)) == sizeof(char) };
	};

	class Resolver
	{
	public:
		template<typename T, std::enable_if_t<IsReflected<T>::Value, int> = 0>
		static TypeDescriptor* Get()
		{
			return &T::classDescription;
		}

		template<typename T, std::enable_if_t<!IsReflected<T>::Value, int> = 0>
		static TypeDescriptor* Get()
		{
			return GetPrimitiveDescriptor<T>();
		}
	};

	template<typename T>
	class TypeResolver
	{
	public:
		static TypeDescriptor* Get()
		{
			return Resolver::Get<T>();
		}
	};
}
