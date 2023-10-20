#pragma once

#include <cassert>
#include <cmath>
#include <string>

namespace NightOwl
{
	template <typename T>
	Vec2<T>::Vec2()
		:	x(0), y(0)
	{ }

	template<typename T>
	Vec2<T>::Vec2(std::initializer_list<T> args)
	{
		int index = 0;
		for (auto begin = args.begin(); begin != args.end(); ++begin)
		{
			this->data.at(index++) = *begin;
		}
	}

	template <typename T>
	Vec2<T>::Vec2(const T x, const T y)
		:	x(x), y(y)
	{ }

	template <typename T>
	Vec2<T>::Vec2(const T scalar)
		:	x(scalar), y(scalar)
	{ }

	template <typename T>
	Vec2<T>::Vec2(const Vec2& vector)
		:	x(vector.x), y(vector.y)
	{ }

	template <typename T>
	Vec2<T> Vec2<T>::Zero()
	{
		return Vec2<T>();
	}

	template <typename T>
	Vec2<T> Vec2<T>::XAxis()
	{
		return Vec2<T>(1, 0);
	}

	template <typename T>
	Vec2<T> Vec2<T>::YAxis()
	{
		return Vec2<T>(0, 1);
	}

	template <typename T>
	Vec2<T> Vec2<T>::Up()
	{
		return Vec2<T>(0, 1);
	}

	template <typename T>
	Vec2<T> Vec2<T>::Down()
	{
		return Vec2<T>(0, -1);
	}

	template <typename T>
	Vec2<T> Vec2<T>::Left()
	{
		return Vec2<T>(-1, 0);
	}

	template <typename T>
	Vec2<T> Vec2<T>::Right()
	{
		return Vec2<T>(1, 0);
	}

	template <typename T>
	T Vec2<T>::Magnitude() const
	{
		return std::sqrt(SquareMagnitude());
	}

	template <typename T>
	T Vec2<T>::SquareMagnitude() const
	{
		return x * x + y * y;
	}

	template <typename T>
	Vec2<T> Vec2<T>::GetNormalize() const
	{
		assert(!NearEquals(Magnitude(), static_cast<T>(0)));

		T inverseMagnitude = static_cast<T>(1) / Magnitude();

		return Vec2<T>(x * inverseMagnitude, y * inverseMagnitude);
	}

	template <typename T>
	Vec2<T>& Vec2<T>::Normalize()
	{
		assert(!NearEquals(Magnitude(), static_cast<T>(0)));

		*this /= Magnitude();

		return *this;
	}

	template <typename T>
	Vec2<T> Vec2<T>::GetRenormalize() const
	{
		T inverseMagnitude = FastInverseSquareRootAroundOne(SquareMagnitude());
		return Vec2<T>(x * inverseMagnitude, y * inverseMagnitude);
	}

	template <typename T>
	Vec2<T>& Vec2<T>::Renormalize()
	{
		T inverseMagnitude = FastInverseSquareRootAroundOne(SquareMagnitude());
		return *this *= inverseMagnitude;
	}

	template <typename T>
	Vec2<T> Vec2<T>::GetNegate() const
	{
		return *this * static_cast<T>(-1);
	}

	template <typename T>
	Vec2<T>& Vec2<T>::Negate()
	{
		*this *= static_cast<T>(-1);
		return *this;
	}

	template<typename T>
	const T* Vec2<T>::GetValuePointer() const
	{
		return data.data();
	}

	template <typename T>
	const std::string Vec2<T>::ToString() const
	{
		std::string vectorString = "(" + std::to_string(x) + ", " + std::to_string(y) + ")";
		return vectorString;
	}

	template <typename T>
	Vec2<T> Vec2<T>::Max(const Vec2<T>& leftVector, const Vec2<T>& rightVector)
	{
		return Vec2<T>(std::max(leftVector.x, rightVector.x), std::max(leftVector.y, rightVector.y));
	}

	template <typename T>
	Vec2<T> Vec2<T>::Min(const Vec2<T>& leftVector, const Vec2<T>& rightVector)
	{
		return Vec2<T>(std::min(leftVector.x, rightVector.x), std::min(leftVector.y, rightVector.y));
	}

	template <typename T>
	T Vec2<T>::Dot(const Vec2<T>& leftVector, const Vec2<T>& rightVector)
	{
		return leftVector.x * rightVector.x + leftVector.y * rightVector.y;
	}

	template <typename T>
	Vec2<T> Vec2<T>::Project(const Vec2<T>& leftVector, const Vec2<T>& rightVector)
	{
		return rightVector * (Dot(leftVector, rightVector) / Dot(rightVector, rightVector));
	}

	template <typename T>
	Vec2<T> Vec2<T>::Reject(const Vec2<T>& leftVector, const Vec2<T>& rightVector)
	{
		return leftVector - Project(leftVector, rightVector);
	}

	template <typename T>
	Vec2<T> Vec2<T>::Lerp(const Vec2<T>& leftVector, const Vec2<T>& rightVector, const T t)
	{
		T clampedT = std::max(0.0f, std::min(1.0f, t));

		return (leftVector * (static_cast<T>(1) - clampedT)) + (rightVector * clampedT);
	}

	template <typename T>
	Vec2<T> Vec2<T>::Elerp(const Vec2<T>& leftVector, const Vec2<T>& rightVector, const T t)
	{
		Vec2<T> interpolatedVector;

		T clampedT = std::max(0.0f, std::min(1.0f, t));

		T leftVectorX = std::max(SMALL_EPSILON, std::abs(leftVector.x)) * std::copysign(static_cast<T>(1), leftVector.x);
		T leftVectorY = std::max(SMALL_EPSILON, std::abs(leftVector.y)) * std::copysign(static_cast<T>(1), leftVector.y);

		interpolatedVector.x = leftVectorX * std::pow(rightVector.x / leftVectorX, clampedT);
		interpolatedVector.y = leftVectorX * std::pow(rightVector.y / leftVectorY, clampedT);

		return interpolatedVector;
	}

	template <typename T>
	Vec2<T>& Vec2<T>::operator=(const Vec2<T>& vector)
	{
		x = vector.x;
		y = vector.y;
		return *this;
	}

	template <typename T>
	Vec2<T>& Vec2<T>::operator+=(const Vec2<T>& vector)
	{
		x += vector.x;
		y += vector.y;
		return *this;
	}

	template <typename T>
	Vec2<T>& Vec2<T>::operator-=(const Vec2<T>& vector)
	{
		x -= vector.x;
		y -= vector.y;
		return *this;
	}

	template <typename T>
	Vec2<T>& Vec2<T>::operator*=(const T scalar)
	{
		x *= scalar;
		y *= scalar;
		return *this;
	}

	template <typename T>
	Vec2<T>& Vec2<T>::operator/=(const T scalar)
	{
		x /= scalar;
		y /= scalar;
		return *this;
	}
}
