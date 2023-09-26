#pragma once

#include "Vec3.h"
#include "Mat3.h"
#include "Mat4.h"
#include "Quaternion.h"

namespace NightOwl
{
	template <typename T>
	Vec3<T> VecQuatMat<T>::GetScale()
	{
		return Vec3<T>(scaleMatrix(0, 0), scaleMatrix(1, 1), scaleMatrix(2, 2));
	}

	template <typename T>
	void VecQuatMat<T>::SetScale(const Vec3<T>& scale)
	{
		scaleMatrix.data[0][0] = scale.x;
		scaleMatrix.data[1][1] = scale.y;
		scaleMatrix.data[2][2] = scale.z;
	}

	template <typename T>
	Vec3<T> VecQuatMat<T>::GetTranslation()
	{
		return vector;
	}

	template <typename T>
	void VecQuatMat<T>::SetTranslation(const Vec3<T>& translation)
	{
		vector = translation;
	}

	template <typename T>
	Quaternion<T> VecQuatMat<T>::GetRotation()
	{
		return quaternion;
	}

	template <typename T>
	void VecQuatMat<T>::SetRotation(const Quaternion<T>& rotation)
	{
		this->quaternion = rotation;
	}

	template <typename T>
	Mat4<T> VecQuatMat<T>::GetMatrix()
	{
		Mat4F result;

		result.SetTranslation(vector);

		result *= quaternion.GetRotationMatrix() * scaleMatrix;

		return result;
	}

	template <typename T>
	VecQuatMat<T> VecQuatMat<T>::Identity()
	{
		return VecQuatMat();
	}

	template <typename T>
	VecQuatMat<T>& VecQuatMat<T>::operator*=(const VecQuatMat<T>& vecQuatMat)
	{
		VecQuatMat result;

		const Mat3F rotationMatrix = quaternion.GetRotationMatrix();
		const Mat3F inverseRotationMatrix = quaternion.GetInverse().GetRotationMatrix();

		result.vector = quaternion * (scaleMatrix * vecQuatMat.vector) + vecQuatMat.vector;
		result.quaternion = quaternion * vecQuatMat.quaternion;
		result.scaleMatrix = (inverseRotationMatrix * scaleMatrix * rotationMatrix) * (rotationMatrix * vecQuatMat.scaleMatrix * inverseRotationMatrix);

		return result;
	}

	template <typename T>
	VecQuatMat<T> operator*(const VecQuatMat<T>& leftVecQuatMat, const VecQuatMat<T>& rightVecQuatMat)
	{
		VecQuatMat<T> leftVecQuatMatCopy = leftVecQuatMat;
		return leftVecQuatMatCopy *= rightVecQuatMat;
	}
}
