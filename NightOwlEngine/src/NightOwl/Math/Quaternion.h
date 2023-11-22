#pragma once

#include <utility>

namespace NightOwl
{
	template <typename T> class Vec3;

	template <typename T> class Vec4;

	template <typename T> class Mat3;

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

			struct
			{
				Vec3<T> vector;
				T imaginary;
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

		std::pair<T, Vec3<T>> GetAngleAxis() const;

		void ConstrainTwist(T minAngleInDegrees, T maxAngleInDegrees);

		void SetOrthogonalRotationMatrix(const Mat3<T>& matrix);

		void SetNonOrthogonalRotationMatrix(const Mat3<T>& matrix);

		Vec3<T> GetEulerAngles() const;

		static Vec3<T> TransformVector(const Quaternion<T>& quaternion, const Vec3<T>& vector);

		static T Dot(const Quaternion<T>& leftQuaternion, const Quaternion<T>& rightQuaternion);

		static Quaternion<T> RotateFromTo(const Vec3<T>& fromDirection, const Vec3<T>& toDirection);

		static Quaternion<T> LookAt(const Vec3<T>& direction);

		static Quaternion<T> MakeRotationX(const T angleInDegrees);

		static Quaternion<T> MakeRotationY(const T angleInDegrees);

		static Quaternion<T> MakeRotationZ(const T angleInDegrees);

		static Quaternion<T> MakeRotationAxisAngle(const Vec3<T>& unitVector, const T angleInDegrees);

		static Quaternion<T> MakeRotationFromEulers(const T xAngleInDegrees, const T yAngleInDegrees, const T zAngleInDegrees);

		static Quaternion<T> MakeRotationFromEulers(Vec3<T> anglesInDegrees);

		static Quaternion<T> Lerp(const Quaternion<T>& leftQuaternion, const Quaternion<T>& rightQuaternion, const T t);

		static Quaternion<T> Nlerp(const Quaternion<T>& leftQuaternion, const Quaternion<T>& rightQuaternion, const T t);

		static Quaternion<T> Slerp(const Quaternion<T>& leftQuaternion, const Quaternion<T>& rightQuaternion, const T t);

		static void Decompose(const Quaternion<T>& quaternion, const Vec3<T>& twistAxis, Quaternion<T>& outSwing, Quaternion<T>& outTwist);

		Quaternion<T>& operator=(const Quaternion<T>& quaternion);

		Quaternion<T>& operator*=(const Quaternion<T>& quaternion);

		Quaternion<T>& operator*=(const T scalar);

		Quaternion<T>& operator/=(const T scalar);

		Quaternion<T>& operator+=(const Quaternion<T>& quaternion);

		Quaternion<T>& operator-=(const Quaternion<T>& quaternion);

		friend Vec3<T> operator*(const Quaternion<T>& quaternion, const Vec3<T>& vector)
		{
			return Quaternion<T>::TransformVector(quaternion, vector);
		}

		friend Vec3<T> operator*(const Vec3<T>& vector, const Quaternion<T>& quaternion)
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

	using QuatF = Quaternion<float>;
}

#include "Quaternion.inl"
