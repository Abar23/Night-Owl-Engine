#pragma once

namespace NightOwl
{
	template <typename T> class Vec3;

	template <typename T> class Mat3;

	template <typename T> class Mat4;

	template <typename T> class Quaternion;

	template <typename T>
	class VecQuatMat
	{
	public:
		Mat3<T> scaleMatrix;

		Vec3<T> vector;

		Quaternion<T> quaternion;

		Vec3<T> GetScale();

		void SetScale(const Vec3<T>& scale);

		Vec3<T> GetTranslation();

		void SetTranslation(const Vec3<T>& translation);

		Quaternion<T> GetRotation();

		void SetRotation(const Quaternion<T>& rotation);

		Mat4<T> GetMatrix();

		VecQuatMat<T> GetInverse();

		VecQuatMat<T>& operator*=(const VecQuatMat<T>& vecQuatMat);

		static VecQuatMat<T> Identity();
	};

	template <typename T>
	VecQuatMat<T> operator*(const VecQuatMat<T>& leftVecQuatMat, const VecQuatMat<T>& rightVecQuatMat);

	using VecQuatMatF = VecQuatMat<float>;
}

#include "VecQuatMat.inl"