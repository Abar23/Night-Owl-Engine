#pragma once

#include "TypeDescriptor.h"
#include "ClassDescriptor.h"
#include "VectorDescriptor.h"
#include "SharedPointerDescriptor.h"
#include "UserPointerTypes.h"
#include "MathTypes.h"
#include "Resolver.h"

namespace NightOwl::Core
{
#define REFLECT() \
	friend class NightOwl::Core::Resolver; \
	static NightOwl::Core::ClassDescriptor classDescription; \
	static void reflectClass(NightOwl::Core::ClassDescriptor*);

#define START_REFLECTION(clazz) \
    void clazz::reflectClass(NightOwl::Core::ClassDescriptor* classDescriptor) { \
        using T = clazz; \
        classDescriptor->name = #clazz; \
        classDescriptor->size = sizeof(T); \
        classDescriptor->classMembers = {

#define CLASS_MEMBER_REFLECTION(member) \
            {#member, offsetof(T, member), NightOwl::Core::TypeResolver<decltype(member)>::Get()},

#define END_REFLECTION(clazz) \
        }; \
    } \
    NightOwl::Core::ClassDescriptor clazz::classDescription{clazz::reflectClass}; 
}
