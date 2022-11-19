#pragma once

#include "TypeDescriptor.h"
#include "Resolver.h"
#include <memory>

namespace NightOwl::Core
{
    template <typename T>
    class SharedPointerDescriptor : public TypeDescriptor
    {
    public:
        SharedPointerDescriptor()
            :   TypeDescriptor{ "std::shared_ptr<>", sizeof(std::shared_ptr<T>) },
				targetType{ TypeResolver<T>::Get() }
    	{ }

        T GetTarget(const void* sharedPointer)
        {
            const std::shared_ptr<T>& concreteSharedPointer = *static_cast<const std::shared_ptr<T>*>(sharedPointer);
            return concreteSharedPointer.get();
        }

        void Serialize(const void* object, rapidjson::Document& document, rapidjson::Value& value) override
        {
            
        }

        TypeDescriptor* targetType;
    };

    template <typename T>
    class TypeResolver<std::shared_ptr<T>>
    {
    public:
        static TypeDescriptor* Get()
    	{
            static SharedPointerDescriptor<T> typeDescriptor;
            return &typeDescriptor;
        }
    };
}
