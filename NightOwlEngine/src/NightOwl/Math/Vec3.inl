#pragma once

#include "MathFunctions.h"
#include <cassert>
#include <cmath>
#include <string>

namespace NightOwl
{
	template <typename T>
	Vec3<T>::Vec3()
		:	x(0), y(0), z(0)
	{ }

	template<typename T>
	Vec3<T>::Vec3(std::initializer_list<T> args)
	{
		int index = 0;
		for (auto begin = args.begin(); begin != args.end(); ++begin)
		{
			this->data.at(index++) = *begin;
		}
	}

	template <typename T>
	Vec3<T>::Vec3(const T x, const T y, const T z)
		:	x(x), y(y), z(z)
	{ }

	template <typename T>
	Vec3<T>::Vec3(const T value)
		:	x(value), y(value), z(value)
	{ }

	template <typename T>
	T Vec3<T>::Magnitude() const
	{
		return std::sqrt(x * x + y * y + z * z);
	}

	template <typename T>
	T Vec3<T>::SquareMagnitude() const
	{
		return x * x + y * y + z * z;
	}

	template <typename T>
	Vec3<T> Vec3<T>::GetNormalize() const
	{
		if (NearEquals(Magnitude(), static_cast<T>(0)))
		{
			return Vec3F();
		}

		T inverseMagnitude = static_cast<T>(1) / Magnitude();

		return Vec3<T>(x * inverseMagnitude, y * inverseMagnitude, z * inverseMagnitude);
	}

	template <typename T>
	Vec3<T>& Vec3<T>::Normalize()
	{
		if (NearEquals(Magnitude(), static_cast<T>(0)))
		{
			this->x = static_cast<T>(0);
			this->y = static_cast<T>(0);
			this->z = static_cast<T>(0);

			return *this;
		}

		*this /= Magnitude();

		return *this;
	}

	template <typename T>
	Vec3<T> Vec3<T>::GetRenormalize() const
	{
		T inverseMagnitude = FastInverseSquareRootAroundOne(SquareMagnitude());
		return Vec3(x * inverseMagnitude, y * inverseMagnitude, z * inverseMagnitude);
	}

	template <typename T>
	Vec3<T>& Vec3<T>::Renormalize()
	{
		T inverseMagnitude = FastInverseSquareRootAroundOne(SquareMagnitude());
		return *this *= inverseMagnitude;
	}

	template <typename T>
	Vec3<T> Vec3<T>::GetNegate() const
	{
		return *this * static_cast<T>(-1);
	}

	template <typename T>
	Vec3<T>& Vec3<T>::Negate()
	{
		*this *= static_cast<T>(-1);
		return *this;
	}

	template<typename T>
	const T* Vec3<T>::GetValuePointer() const
	{
		return data.data();
	}

	template <typename T>
	const std::string Vec3<T>::ToString() const
	{
		std::string vectorString = "(" + std::to_string(x) + ", " + std::to_string(y) + ", " + std::to_string(z) + ")";
		return vectorString;
	}

	template <typename T>
	T Vec3<T>::Dot(const Vec3& leftVector, const Vec3& rightVector)
	{
		return leftVector.x * rightVector.x +
			   leftVector.y * rightVector.y +
			   leftVector.z * rightVector.z;
	}

	template <typename T>
	Vec3<T> Vec3<T>::Max(const Vec3<T>& leftVector, const Vec3<T>& rightVector)
	{
		return Vec3<T>(std::max(leftVector.x, rightVector.x), std::max(leftVector.y, rightVector.y), std::max(leftVector.z, rightVector.z));
	}

	template <typename T>
	Vec3<T> Vec3<T>::Min(const Vec3<T>& leftVector, const Vec3<T>& rightVector)
	{
		return Vec3<T>(std::min(leftVector.x, rightVector.x), std::min(leftVector.y, rightVector.y), std::min(leftVector.z, rightVector.z));
	}

	template <typename T>
	Vec3<T> Vec3<T>::Cross(const Vec3& leftVector, const Vec3& rightVector)
	{
		return Vec3(leftVector.y * rightVector.z - leftVector.z * rightVector.y,
					leftVector.z * rightVector.x - leftVector.x * rightVector.z,
					leftVector.x * rightVector.y - leftVector.y * rightVector.x);
	}

	template <typename T>
	Vec3<T> Vec3<T>::Project(const Vec3& leftVector, const Vec3& rightVector)
	{
		return rightVector * (Dot(leftVector, rightVector) / Dot(rightVector, rightVector));
	}

	template <typename T>
	Vec3<T> Vec3<T>::Reject(const Vec3& leftVector, const Vec3& rightVector)
	{
		return leftVector - Project(leftVector, rightVector);
	}

	template <typename T>
	Vec3<T> Vec3<T>::Lerp(const Vec3& leftVector, const Vec3& rightVector, const T t)
	{
		T clampedT = std::max(0.0f, std::min(1.0f, t));

		return leftVector * (1.0f - clampedT) + rightVector * clampedT;
	}

	template <typename T>
	Vec3<T> Vec3<T>::Elerp(const Vec3<T>& leftVector, const Vec3<T>& rightVector, const T t)
	{
		Vec3<T> interpolatedVector;

		T clampedT = std::max(0.0f, std::min(1.0f, t));

		T leftVectorX = std::max(EPSILON, std::abs(leftVector.x)) * std::copysign(static_cast<T>(1), leftVector.x);
		T leftVectorY = std::max(EPSILON, std::abs(leftVector.y)) * std::copysign(static_cast<T>(1), leftVector.y);
		T leftVectorZ = std::max(EPSILON, std::abs(leftVector.z)) * std::copysign(static_cast<T>(1), leftVector.z);

		interpolatedVector.x = leftVectorX * std::pow(rightVector.x / leftVectorX, clampedT);
		interpolatedVector.y = leftVectorX * std::pow(rightVector.y / leftVectorY, clampedT);
		interpolatedVector.z = leftVectorX * std::pow(rightVector.z / leftVectorZ, clampedT);

		return interpolatedVector;
	}

	template <typename T>
	Vec3<T> Vec3<T>::Zero()
	{
		return Vec3<T>(0, 0, 0);
	}

	template <typename T>
	Vec3<T> Vec3<T>::XAxis()
	{
		return Vec3<T>(1, 0, 0);
	}

	template <typename T>
	Vec3<T> Vec3<T>::YAxis()
	{
		return Vec3<T>(0, 1, 0);
	}

	template <typename T>
	Vec3<T> Vec3<T>::ZAxis()
	{
		return Vec3<T>(0, 0, 1);
	}

	template <typename T>
	Vec3<T> Vec3<T>::Up()
	{
		return Vec3<T>(0, 1, 0);
	}

	template <typename T>
	Vec3<T> Vec3<T>::Down()
	{
		return Vec3<T>(0, -1, 0);
	}

	template <typename T>
	Vec3<T> Vec3<T>::Left()
	{
		return Vec3<T>(-1, 0, 0);
	}

	template <typename T>
	Vec3<T> Vec3<T>::Right()
	{
		return Vec3<T>(1, 0, 0);
	}

	template <typename T>
	Vec3<T> Vec3<T>::Back()
	{
		return Vec3<T>(0, 0, -1);
	}

	template <typename T>
	Vec3<T> Vec3<T>::Forward()
	{
		return Vec3<T>(0, 0, 1);
	}

	template <typename T>
	Vec3<T>& Vec3<T>::operator=(const Vec3<T>& vector)
	{
		x = vector.x;
		y = vector.y;
		z = vector.z;
		return *this;
	}

	template <typename T>
	Vec3<T>& Vec3<T>::operator+=(const Vec3<T>& vector)
	{
		x += vector.x;
		y += vector.y;
		z += vector.z;
		return *this;
	}

	template <typename T>
	Vec3<T>& Vec3<T>::operator-=(const Vec3<T>& vector)
	{
		x -= vector.x;
		y -= vector.y;
		z -= vector.z;
		return *this;
	}

	template <typename T>
	Vec3<T>& Vec3<T>::operator*=(const T scalar)
	{
		x *= scalar;
		y *= scalar;
		z *= scalar;
		return *this;
	}

	template <typename T>
	Vec3<T>& Vec3<T>::operator/=(const T scalar)
	{
		x /= scalar;
		y /= scalar;
		z /= scalar;
		return *this;
	}
}
