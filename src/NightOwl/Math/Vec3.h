#pragma once

#include "Vec2.h"

namespace NightOwl::Math
{
	template <typename T> class Vec4;

	template <typename T>
	class Vec3
	{
	public:
		union
		{
			std::array<T, 3> data;

			struct
			{
				T x, y, z;
			};
		};

		Vec3();
		Vec3(const Vec3<T>& v) = default;
		Vec3(std::initializer_list<T> args);
		Vec3(const T x, const T y, const T z);
		Vec3(const T value);

		~Vec3() = default;

		T Magnitude() const;
		T SqrMagnitude() const;
		Vec3<T> GetNormalize() const;
		Vec3<T>& Normalize();
		Vec3<T> GetRenormalize() const;
		Vec3<T>& Renormalize();
		Vec3<T> GetNegate() const;
		Vec3<T>& Negate();
		const T* GetValuePointer() const;

		static T Dot(const Vec3<T>& leftVector, const Vec3<T>& rightVector);
		static Vec3<T> Cross(const Vec3<T>& leftVector, const Vec3<T>& rightVector);
		static Vec3<T> Project(const Vec3<T>& leftVector, const Vec3<T>& rightVector);
		static Vec3<T> Reject(const Vec3<T>& leftVector, const Vec3<T>& rightVector);
		static Vec3<T> Lerp(const Vec3<T>& leftVector, const Vec3<T>& rightVector, const float t);
		static Vec3<T> Zero();
		static Vec3<T> XAxis();
		static Vec3<T> YAxis();
		static Vec3<T> ZAxis();
		static Vec3<T> Up();
		static Vec3<T> Down();
		static Vec3<T> Left();
		static Vec3<T> Right();
		static Vec3<T> Back();
		static Vec3<T> Forward();

		Vec3<T>& operator=(const Vec3<T>& vector);
		Vec3<T>& operator+=(const Vec3<T>& vector);
		Vec3<T>& operator-=(const Vec3<T>& vector);

		Vec3<T>& operator*=(const T scalar);
		Vec3<T>& operator/=(const T scalar);

		friend bool operator==(const Vec3<T>& leftVector, const Vec3<T>& rightVector)
		{
			return NearEquals(leftVector.x, rightVector.x) &&
				NearEquals(leftVector.y, rightVector.y) &&
				NearEquals(leftVector.z, rightVector.z);
		}

		friend bool operator!=(const Vec3<T>& leftVector, const Vec3<T>& rightVector)
		{
			return !(leftVector == rightVector);
		}

		friend Vec3<T> operator+(const Vec3<T>& leftVector, const Vec3<T>& rightVector)
		{
			Vec3<T> leftVectorCopy = leftVector;
			return leftVectorCopy += rightVector;
		}

		friend Vec3<T> operator-(const Vec3<T>& leftVector, const Vec3<T>& rightVector)
		{
			Vec3<T> leftVectorCopy = leftVector;
			return leftVectorCopy -= rightVector;
		}

		friend Vec3<T> operator*(const Vec3<T>& vector, const T scalar)
		{
			Vec3<T> vectorCopy = vector;
			return vectorCopy *= scalar;
		}

		friend Vec3<T> operator*(const T scalar, const Vec3<T>& vector)
		{
			Vec3<T> vectorCopy = vector;
			return vectorCopy *= scalar;
		}

		friend Vec3<T> operator/(const Vec3<T>& vector, const T scalar)
		{
			Vec3<T> vectorCopy = vector;
			return vectorCopy /= scalar;
		}

		friend Vec3<T> operator/(const T scalar, const Vec3<T>& vector)
		{
			Vec3<T> vectorCopy = vector;
			return vectorCopy /= scalar;
		}

		friend std::ostream& operator<<(std::ostream& stream, const Vec3<T>& vector)
		{
			stream << "(" << vector.x << ", " << vector.y << ", " << vector.z << ")";
			return stream;
		}
	};

	using Vec3UF = Vec3<unsigned int>;
	using Vec3F = Vec3<float>;
}

#include "Vec3.inl"
