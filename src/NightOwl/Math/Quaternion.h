#pragma once

#include "Vec4.h"
#include "Mat3.h"

namespace NightOwl::Math
{
	template <typename T>
	class Quaternion
	{
	public:
		union
		{
			struct
			{
				T x, y, z, w;
			};

			Vec4<T> components;
		};

		Quaternion();

		Quaternion(const T x, const T y, const T z, const T w);

		Quaternion(const Vec3<T>& unitVector, const T angleInDegrees);

		Quaternion(const Quaternion<T>& quaternion);

		~Quaternion() = default;

		T Norm() const;

		T SquareNorm() const;

		Quaternion<T> GetNormalize() const;

		Quaternion<T>& Normalize();

		Quaternion<T> GetRenormalize() const;

		Quaternion<T>& Renormalize();

		Quaternion<T> GetInverse() const;

		Quaternion<T>& Inverse();

		Quaternion<T> GetConjugate() const;

		Mat3<T> GetRotationMatrix() const;

		void SetOrthogonalRotationMatrix(const Mat3<T>& matrix);

		void SetNonOrthogonalRotationMatrix(const Mat3<T>& matrix);

		Vec3<T> GetEulerAngles() const;

		static Vec3<T> TransformVector(const Quaternion<T>& quaternion, const Vec3<T>& vector);

		static T Dot(const Quaternion<T>& leftQuaternion, const Quaternion<T>& rightQuaternion);

		static Quaternion<T> MakeRotationX(const T angleInDegrees);

		static Quaternion<T> MakeRotationY(const T angleInDegrees);

		static Quaternion<T> MakeRotationZ(const T angleInDegrees);

		static Quaternion<T> MakeRotationAxisAngle(const Vec3<T>& unitVector, const T angleInDegrees);

		static Quaternion<T> MakeRotationFromEulers(float xAngleInDegrees, float yAngleInDegrees, float zAngleInDegrees);

		static Quaternion<T> MakeRotationFromEulers(Vec3<T> anglesInDegrees);

		static Quaternion<T> Lerp(const Quaternion<T>& leftQuaternion, const Quaternion<T>& rightQuaternion, const T t);

		static Quaternion<T> Nlerp(const Quaternion<T>& leftQuaternion, const Quaternion<T>& rightQuaternion, const T t);

		static Quaternion<T> Slerp(const Quaternion<T>& leftQuaternion, const Quaternion<T>& rightQuaternion, const T t);

		Quaternion<T>& operator=(const Quaternion<T>& quaternion);

		Quaternion<T>& operator*=(const Quaternion<T>& quaterion);

		Quaternion<T>& operator*=(const T scalar);

		Quaternion<T>& operator/=(const T scalar);

		Quaternion<T>& operator+=(const Quaternion<T>& quaternion);

		Quaternion<T>& operator-=(const Quaternion<T>& quaternion);

		friend Vec3<T> operator*(const Quaternion<T>& quaternion, const Vec3<T>& vector)
		{
			return Quaternion<T>::TransformVector(quaternion, vector);
		}
	};

	template <typename T>
	Quaternion<T> operator*(const Quaternion<T>& leftQuaterion, const Quaternion<T>& rightQuaterion);

	template <typename T>
	Quaternion<T> operator*(const Quaternion<T>& quaterion, const T scalar);

	template <typename T>
	Quaternion<T> operator*(const T scalar, const Quaternion<T>& quaterion);

	template <typename T>
	Quaternion<T> operator/(const Quaternion<T>& quaterion, const T scalar);

	template <typename T>
	Quaternion<T> operator/(const T scalar, const Quaternion<T>& quaterion);

	template <typename T>
	Quaternion<T> operator+(const Quaternion<T>& leftQuaterion, const Quaternion<T>& rightQuaterion);

	template <typename T>
	Quaternion<T> operator-(const Quaternion<T>& leftQuaterion, const Quaternion<T>& rightQuaterion);

	template <typename T>
	std::ostream& operator<<(std::ostream& stream, const Quaternion<T>& quaternion);

	using QuatF = Math::Quaternion<float>;
}

#include "Quaternion.inl"
