#pragma once

#include "TypeDescriptor.h"
#include "Resolver.h"
#include <vector>

namespace NightOwl
{
    template <typename T>
    class VectorDescriptor : public TypeDescriptor
    {
    public:
        VectorDescriptor()
            :   TypeDescriptor{ "std::vector<>",sizeof(std::vector<T>) },
				itemType{ TypeResolver<T>::Get() }
        { }

        std::size_t GetSize(const void* vector)
        {
            const std::vector<T>& concreteVector = *static_cast<const std::vector<T>*>(vector);
            return concreteVector.size();
        }

        std::size_t GetItem(const void* vector, size_t index) const
        {
            const std::vector<T>& concreteVector = *static_cast<const std::vector<T>*>(vector);
            return &concreteVector[index];
        }

        void Serialize(const void* object, rapidjson::Document& document, rapidjson::Value& value) override
        {
            rapidjson::Value key(name, document.GetAllocator());
            rapidjson::Value jsonObject(rapidjson::kObjectType);

            const std::vector<T> vector = *static_cast<const std::vector<T>*>(object);
            for (const T& element : vector)
            {
                itemType->Serialize(&element, document, jsonObject);
            }

            value.AddMember(key, jsonObject, document.GetAllocator());
        }

        TypeDescriptor* itemType;
    };

    template <typename T>
    class TypeResolver<std::vector<T>>
    {
    public:
        static TypeDescriptor* Get()
    	{
            static VectorDescriptor<T> typeDescriptor;
            return &typeDescriptor;
        }
    };
}
