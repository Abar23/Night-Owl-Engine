#pragma once

#include "TypeDescriptor.h"
#include <string>

namespace NightOwl::Core
{
	class BoolTypeDescriptor : public TypeDescriptor
	{
	public:
		BoolTypeDescriptor()
			:	TypeDescriptor("bool", sizeof(bool))
		{ }

		void Serialize(const void* object, rapidjson::Document& document, rapidjson::Value& value) override
		{
			rapidjson::Value key(name, document.GetAllocator());
			rapidjson::Value keyValue(*static_cast<const bool*>(object));
			value.AddMember(key, keyValue, document.GetAllocator());
		}
	};

	template<>
	inline TypeDescriptor* GetPrimitiveDescriptor<bool>()
	{
		static BoolTypeDescriptor typeDescriptor;
		return &typeDescriptor;
	}

	class CharTypeDescriptor : public TypeDescriptor
	{
	public:
		CharTypeDescriptor()
			:	TypeDescriptor("char", sizeof(char))
		{ }

		void Serialize(const void* object, rapidjson::Document& document, rapidjson::Value& value) override
		{
			rapidjson::Value key(name, document.GetAllocator());
			rapidjson::Value keyValue(*static_cast<const char*>(object));
			value.AddMember(key, keyValue, document.GetAllocator());
		}
	};

	template<>
	inline TypeDescriptor* GetPrimitiveDescriptor<char>()
	{
		static CharTypeDescriptor typeDescriptor;
		return &typeDescriptor;
	}

	class IntegerTypeDescriptor : public TypeDescriptor 
	{
	public:
		IntegerTypeDescriptor()
			:	TypeDescriptor("int", sizeof(int))
		{ }

		void Serialize(const void* object, rapidjson::Document& document, rapidjson::Value& value) override
		{
			rapidjson::Value key(name, document.GetAllocator());
			rapidjson::Value keyValue(*static_cast<const int*>(object));
			value.AddMember(key, keyValue, document.GetAllocator());
		}
	};

	template<>
	inline TypeDescriptor* GetPrimitiveDescriptor<int>()
	{
		static IntegerTypeDescriptor typeDescriptor;
		return &typeDescriptor;
	}

	class UnsignedIntegerTypeDescriptor : public TypeDescriptor
	{
	public:
		UnsignedIntegerTypeDescriptor()
			: TypeDescriptor("unsigned int", sizeof(unsigned int))
		{ }

		void Serialize(const void* object, rapidjson::Document& document, rapidjson::Value& value) override
		{
			rapidjson::Value key(name, document.GetAllocator());
			rapidjson::Value keyValue(*static_cast<const unsigned int*>(object));
			value.AddMember(key, keyValue, document.GetAllocator());
		}
	};

	template<>
	inline TypeDescriptor* GetPrimitiveDescriptor<unsigned int>()
	{
		static UnsignedIntegerTypeDescriptor typeDescriptor;
		return &typeDescriptor;
	}

	class FloatTypeDescriptor : public TypeDescriptor
	{
	public:
		FloatTypeDescriptor()
			:	TypeDescriptor("float", sizeof(float))
		{ }

		void Serialize(const void* object, rapidjson::Document& document, rapidjson::Value& value) override
		{
			rapidjson::Value key(name, document.GetAllocator());
			rapidjson::Value keyValue(*static_cast<const float*>(object));
			value.AddMember(key, keyValue, document.GetAllocator());
		}
	};

	template<>
	inline TypeDescriptor* GetPrimitiveDescriptor<float>()
	{
		static FloatTypeDescriptor typeDescriptor;
		return &typeDescriptor;
	}

	class DoubleTypeDescriptor : public TypeDescriptor
	{
	public:
		DoubleTypeDescriptor()
			:	TypeDescriptor("double", sizeof(double))
		{ }

		void Serialize(const void* object, rapidjson::Document& document, rapidjson::Value& value) override
		{
			rapidjson::Value key(name, document.GetAllocator());
			rapidjson::Value keyValue(*static_cast<const double*>(object));
			value.AddMember(key, keyValue, document.GetAllocator());
		}
	};

	template<>
	inline TypeDescriptor* GetPrimitiveDescriptor<double>()
	{
		static DoubleTypeDescriptor typeDescriptor;
		return &typeDescriptor;
	}

	class StringTypeDescriptor : public TypeDescriptor
	{
	public:
		StringTypeDescriptor()
			:	TypeDescriptor("std::string", sizeof(std::string))
		{ }

		void Serialize(const void* object, rapidjson::Document& document, rapidjson::Value& value) override
		{
			rapidjson::Value key(name, document.GetAllocator());
			rapidjson::Value keyValue(static_cast<const std::string*>(object)->c_str(), document.GetAllocator());
			value.AddMember(key, keyValue, document.GetAllocator());
		}
	};

	template<>
	inline TypeDescriptor* GetPrimitiveDescriptor<std::string>()
	{
		static StringTypeDescriptor typeDescriptor;
		return &typeDescriptor;
	}
}
