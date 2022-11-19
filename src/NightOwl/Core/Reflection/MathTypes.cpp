#include "MathTypes.h"
#include "NightOwl/Math/Math.h"

namespace NightOwl::Core
{
	template <typename T>
	Vec4Descriptor<T>::Vec4Descriptor()
		:   TypeDescriptor{ "Vec4<>",sizeof(Math::Vec4<T>) },
		    itemType{ TypeResolver<T>::Get() }
	{
	}

	template <typename T>
	void Vec4Descriptor<T>::Serialize(const void* object, rapidjson::Document& document, rapidjson::Value& value)
	{
		rapidjson::Value key(name, document.GetAllocator());
		rapidjson::Value array(rapidjson::kArrayType);

		const Math::Vec4<T> vector = *static_cast<const Math::Vec4<T>*>(object);
		for (const auto& element : vector.data)
		{
			array.PushBack(element, document.GetAllocator());
		}

		value.AddMember(key, array, document.GetAllocator());
	}

	template class Vec4Descriptor<float>;


	template <typename T>
	Vec3Descriptor<T>::Vec3Descriptor()
		: TypeDescriptor{ "Vec3<>",sizeof(Math::Vec3<T>) },
		itemType{ TypeResolver<T>::Get() }
	{
	}

	template <typename T>
	void Vec3Descriptor<T>::Serialize(const void* object, rapidjson::Document& document, rapidjson::Value& value)
	{
		rapidjson::Value key(name, document.GetAllocator());
		rapidjson::Value array(rapidjson::kArrayType);

		const Math::Vec3<T> vector = *static_cast<const Math::Vec3<T>*>(object);
		for (const auto& element : vector.data)
		{
			array.PushBack(element, document.GetAllocator());
		}

		value.AddMember(key, array, document.GetAllocator());
	}

	template class Vec3Descriptor<float>;
	template class Vec3Descriptor<unsigned int>;


	template <typename T>
	Vec2Descriptor<T>::Vec2Descriptor()
		: TypeDescriptor{ "Vec2<>",sizeof(Math::Vec2<T>) },
		itemType{ TypeResolver<T>::Get() }
	{
	}

	template <typename T>
	void Vec2Descriptor<T>::Serialize(const void* object, rapidjson::Document& document, rapidjson::Value& value)
	{
		rapidjson::Value key(name, document.GetAllocator());
		rapidjson::Value array(rapidjson::kArrayType);

		const Math::Vec2<T> vector = *static_cast<const Math::Vec2<T>*>(object);
		for (const auto& element : vector.data)
		{
			array.PushBack(element, document.GetAllocator());
		}

		value.AddMember(key, array, document.GetAllocator());
	}

	template class Vec2Descriptor<float>;


	template <typename T>
	Mat4Descriptor<T>::Mat4Descriptor()
		: TypeDescriptor{ "Mat4<>",sizeof(Math::Mat4<T>) },
		itemType{ TypeResolver<T>::Get() }
	{
	}

	template <typename T>
	void Mat4Descriptor<T>::Serialize(const void* object, rapidjson::Document& document, rapidjson::Value& value)
	{
		rapidjson::Value key(name, document.GetAllocator());
		rapidjson::Value array(rapidjson::kArrayType);

		const Math::Mat4<T> matrix = *static_cast<const Math::Mat4<T>*>(object);
		for (const auto& column : matrix.data)
		{
			for (const auto& element : column)
			{
				array.PushBack(element, document.GetAllocator());
			}
		}

		value.AddMember(key, array, document.GetAllocator());
	}

	template class Mat4Descriptor<float>;


	template <typename T>
	QuaternionDescriptor<T>::QuaternionDescriptor()
		: TypeDescriptor{ "Quaternion<>",sizeof(Math::Mat4<T>) },
		itemType{ TypeResolver<T>::Get() }
	{
	}

	template <typename T>
	void QuaternionDescriptor<T>::Serialize(const void* object, rapidjson::Document& document, rapidjson::Value& value)
	{
		rapidjson::Value key(name, document.GetAllocator());
		rapidjson::Value array(rapidjson::kArrayType);

		const Math::Quaternion<T> quaternion = *static_cast<const Math::Quaternion<T>*>(object);
		Math::Vec3<T> angles = quaternion.GetEulerAngles();
		for (const auto& element : angles.data)
		{
			array.PushBack(element, document.GetAllocator());
		}

		value.AddMember(key, array, document.GetAllocator());
	}

	template class QuaternionDescriptor<float>;
}
