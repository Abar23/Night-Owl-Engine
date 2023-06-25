#pragma once

#include "ClassMember.h"
#include <vector>

namespace NightOwl
{
	class ClassDescriptor : public TypeDescriptor
	{
	public:
		ClassDescriptor(const char* name, std::size_t size, const std::initializer_list<ClassMember>& members)
			:	TypeDescriptor(name, size), classMembers(members)
		{ }

		ClassDescriptor(void (*SetupClassDescriptor)(ClassDescriptor*))
			:	TypeDescriptor{ nullptr, 0 }
		{
			SetupClassDescriptor(this);
		}

		void Serialize(const void* object, rapidjson::Document& document, rapidjson::Value& value) override
		{
			rapidjson::Value classKey(name, document.GetAllocator());
			rapidjson::Value classValueObj(rapidjson::kObjectType);
			for (auto& classMember : classMembers)
			{
				rapidjson::Value classMemberKey(classMember.name, document.GetAllocator());
				rapidjson::Value classMemberObj(rapidjson::kObjectType);
				classMember.typeDescriptor->Serialize(static_cast<const char*>(object) + classMember.offsetIntoClass, document, classMemberObj);
				classValueObj.AddMember(classMemberKey, classMemberObj, document.GetAllocator());
			}

			value.AddMember(classKey, classValueObj, document.GetAllocator());
		}

		std::vector<ClassMember> classMembers;
	};
}
