#pragma once

#include <cassert>
#include <cmath>
#include <string>
#include "MathFunctions.h"
#include "Vec4.h"

namespace NightOwl
{
	template <typename T>
	Vec4<T>::Vec4()
		:	x(0), y(0), z(0), w(0)
	{ }

	template <typename T>
	Vec4<T>::Vec4(const Vec4<T>& vector)
		:	x(vector.x), y(vector.y), z(vector.z), w(vector.w)
	{ }

	template<typename T>
	Vec4<T>::Vec4(std::initializer_list<T> args)
	{
		int index = 0;
		for (auto begin = args.begin(); begin != args.end(); ++begin)
		{
			this->data.at(index++) = *begin;
		}
	}

	template <typename T>
	Vec4<T>::Vec4(const T x, const T y, const T z, const T w)
		:	x(x), y(y), z(z), w(w)
	{ }

	template <typename T>
	Vec4<T>::Vec4(const T x, const T y, const T z)
		:	x(x), y(y), z(z), w(0)
	{ }

	template <typename T>
	Vec4<T>::Vec4(const T scalar)
		:	x(scalar), y(scalar), z(scalar), w(scalar)
	{ }

	template <typename T>
	T Vec4<T>::Magnitude() const
	{
		return std::sqrt(SqrMagnitude());
	}

	template <typename T>
	T Vec4<T>::SqrMagnitude() const
	{
		return x * x + y * y + z * z + w * w;
	}

	template <typename T>
	Vec4<T> Vec4<T>::GetNormalize() const
	{
		assert(!NearEquals(Magnitude(), static_cast<T>(0)));

		T magnitude = Magnitude();

		return Vec4<T>(x / magnitude, y / magnitude, z / magnitude, w / magnitude);
	}

	template <typename T>
	Vec4<T>& Vec4<T>::Normalize()
	{
		assert(!NearEquals(Magnitude(), static_cast<T>(0)));

		*this /= Magnitude();

		return *this;
	}

	template <typename T>
	Vec4<T> Vec4<T>::GetRenormalize() const
	{
		T inverseMagnitude = FastInverseSquareRootAroundOne(SqrMagnitude());
		return Vec4<T>(x * inverseMagnitude, y * inverseMagnitude, z * inverseMagnitude, w * inverseMagnitude);
	}

	template <typename T>
	Vec4<T>& Vec4<T>::Renormalize()
	{
		T inverseMagnitude = FastInverseSquareRootAroundOne(SqrMagnitude());
		return *this *= inverseMagnitude;
	}

	template <typename T>
	Vec4<T> Vec4<T>::GetNegate() const
	{
		return *this * static_cast<T>(-1);
	}

	template <typename T>
	Vec4<T>& Vec4<T>::Negate()
	{
		*this *= static_cast<T>(-1);
		return *this;
	}

	template<typename T>
	const T* Vec4<T>::GetValuePointer() const
	{
		return data.data();
	}

	template <typename T>
	std::string Vec4<T>::ToString() const
	{
		std::string vectorString = "(" + std::to_string(x) + ", " + std::to_string(y) + ", " + std::to_string(z) + ", " + std::to_string(w) + ")";
		return vectorString;
	}

	template <typename T>
	T Vec4<T>::Dot(const Vec4<T>& leftVector, const Vec4<T>& rightVector)
	{
		return leftVector.x * rightVector.x +
			   leftVector.y * rightVector.y +
			   leftVector.z * rightVector.z +
			   leftVector.w * rightVector.w;
	}

	template <typename T>
	Vec4<T> Vec4<T>::Max(const Vec4<T>& leftVector, const Vec4<T>& rightVector)
	{
		return Vec2<T>(std::max(leftVector.x, rightVector.x), std::max(leftVector.y, rightVector.y), std::max(leftVector.z, rightVector.z), std::max(leftVector.w, rightVector.w));
	}

	template <typename T>
	Vec4<T> Vec4<T>::Min(const Vec4<T>& leftVector, const Vec4<T>& rightVector)
	{
		return Vec2<T>(std::min(leftVector.x, rightVector.x), std::min(leftVector.y, rightVector.y), std::min(leftVector.z, rightVector.z), std::min(leftVector.w, rightVector.w));
	}

	template <typename T>
	Vec4<T> Vec4<T>::Lerp(const Vec4<T>& leftVector, const Vec4<T>& rightVector, const T t)
	{
		return leftVector * (static_cast<T>(1) - t) + rightVector * t;
	}

	template <typename T>
	Vec4<T> Vec4<T>::Zero()
	{
		return Vec4<T>();
	}

	template <typename T>
	Vec4<T>& Vec4<T>::operator=(const Vec4<T>& vector)
	{
		x = vector.x;
		y = vector.y;
		z = vector.z;
		w = vector.w;
		return *this;
	}

	template <typename T>
	Vec4<T>& Vec4<T>::operator+=(const Vec4<T>& vector)
	{
		x += vector.x;
		y += vector.y;
		z += vector.z;
		w += vector.w;
		return *this;
	}

	template <typename T>
	Vec4<T>& Vec4<T>::operator-=(const Vec4<T>& vector)
	{
		x -= vector.x;
		y -= vector.y;
		z -= vector.z;
		w -= vector.w;
		return *this;
	}

	template <typename T>
	Vec4<T>& Vec4<T>::operator*=(const T scalar)
	{
		x *= scalar;
		y *= scalar;
		z *= scalar;
		w *= scalar;
		return *this;
	}

	template <typename T>
	Vec4<T>& Vec4<T>::operator/=(const T scalar)
	{
		x /= scalar;
		y /= scalar;
		z /= scalar;
		w /= scalar;
		return *this;
	}
}
