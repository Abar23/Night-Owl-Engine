#pragma once

#include "TypeDescriptor.h"
#include "ClassDescriptor.h"
#include "VectorDescriptor.h"
#include "SharedPointerDescriptor.h"
#include "UserPointerTypes.h"
#include "MathTypes.h"
#include "Resolver.h"

namespace NightOwl
{
#define REFLECT() \
	friend class NightOwl::Resolver; \
	static NightOwl::ClassDescriptor classDescription; \
	static void reflectClass(NightOwl::ClassDescriptor*);

#define START_REFLECTION(clazz) \
    void clazz::reflectClass(NightOwl::ClassDescriptor* classDescriptor) { \
        using T = clazz; \
        classDescriptor->name = #clazz; \
        classDescriptor->size = sizeof(T); \
        classDescriptor->classMembers = {

#define CLASS_MEMBER_REFLECTION(member) \
            {#member, offsetof(T, member), NightOwl::TypeResolver<decltype(member)>::Get()},

#define END_REFLECTION(clazz) \
        }; \
    } \
    NightOwl::ClassDescriptor clazz::classDescription{clazz::reflectClass}; 
}
