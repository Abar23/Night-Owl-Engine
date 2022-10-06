#pragma once

#include "Vec2.h"

namespace NightOwl::Math
{
	template <typename T>
	class Vec4
	{
	public:
		union
		{
			std::array<T, 4> data;

			struct
			{
				T x, y, z, w;
			};
		};

		Vec4();
		Vec4(const Vec4<T>& vector);
		Vec4(std::initializer_list<T> args);
		Vec4(const T x, const T y, const T z, const T w);
		Vec4(const T x, const T y, const T z);
		Vec4(const T scalar);

		~Vec4() = default;

		T Magnitude() const;
		T SqrMagnitude() const;
		Vec4<T> GetNormalize() const;
		Vec4<T>& Normalize();
		Vec4<T> GetRenormalize() const;
		Vec4<T>& Renormalize();
		Vec4<T> GetNegate() const;
		Vec4<T>& Negate();
		const T* GetValuePointer() const;

		static T Dot(const Vec4<T>& leftVector, const Vec4<T>& rightVector);
		static Vec4<T> Lerp(const Vec4<T>& leftVector, const Vec4<T>& rightVector, const T t);
		static Vec4<T> Zero();

		Vec4<T>& operator=(const Vec4<T>& vector);
		Vec4<T>& operator+=(const Vec4<T>& vector);
		Vec4<T>& operator-=(const Vec4<T>& vector);

		Vec4& operator*=(const T scalar);
		Vec4& operator/=(const T scalar);

		friend bool operator==(const Vec4<T>& leftVector, const Vec4<T>& rightVector)
		{
			return NearEquals(leftVector.x, rightVector.x) &&
				NearEquals(leftVector.y, rightVector.y) &&
				NearEquals(leftVector.z, rightVector.z) &&
				NearEquals(leftVector.w, rightVector.w);
		}

		friend bool operator!=(const Vec4<T>& leftVector, const Vec4<T>& rightVector)
		{
			return !(leftVector == rightVector);
		}

		friend Vec4<T> operator+(const Vec4<T>& leftVector, const Vec4<T>& rightVector)
		{
			Vec4<T> leftVectorCopy = leftVector;
			return leftVectorCopy += rightVector;
		}

		friend Vec4<T> operator-(const Vec4<T>& leftVector, const Vec4<T>& rightVector)
		{
			Vec4<T> leftVectorCopy = leftVector;
			return leftVectorCopy -= rightVector;
		}

		friend Vec4<T> operator*(const Vec4<T>& vector, const T scalar)
		{
			Vec4<T> vectorCopy = vector;
			return vectorCopy *= scalar;
		}

		friend Vec4<T> operator*(const T scalar, const Vec4<T>& vector)
		{
			Vec4<T> vectorCopy = vector;
			return vectorCopy *= scalar;
		}

		friend Vec4<T> operator/(const Vec4<T>& vector, const T scalar)
		{
			Vec4<T> vectorCopy = vector;
			return vectorCopy /= scalar;
		}

		friend Vec4<T> operator/(const T scalar, const Vec4<T>& vector)
		{
			Vec4<T> vectorCopy = vector;
			return vectorCopy /= scalar;
		}

		friend std::ostream& operator<<(std::ostream& stream, const Vec4<T>& vector)
		{
			stream << "(" << vector.x << ", " << vector.y << ", " << vector.z << ", " << vector.w << ")";
			return stream;
		}
	};

	using Vec4F = Vec4<float>;
}

#include "Vec4.inl"
