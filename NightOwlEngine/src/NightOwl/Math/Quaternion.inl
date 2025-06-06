#pragma once

#include "MathFunctions.h"
#include "Mat3.h"
#include "Vec4.h"
#include <algorithm>
#include <cmath>

namespace NightOwl
{
	template <typename T>
	Quaternion<T>::Quaternion()
		:	x(0), y(0), z(0), w(1)
	{ }

	template <typename T>
	Quaternion<T>::Quaternion(const T x, const T y, const T z, const T w)
		:	x(x), y(y), z(z), w(w)
	{ }

	template <typename T>
	Quaternion<T>::Quaternion(const Vec3<T>& unitVector, const T angleInDegrees)
	{
		T halfAngle = DegreesToRad(angleInDegrees) / static_cast<T>(2);
		T cosHalfAngle = std::cos(halfAngle);
		T sinHalfAngle = std::sin(halfAngle);

		x = unitVector.x * sinHalfAngle;
		y = unitVector.y * sinHalfAngle;
		z = unitVector.z * sinHalfAngle;
		w = cosHalfAngle;
	}

	template <typename T>
	Quaternion<T>::Quaternion(const Quaternion<T>& quaternion)
		: x(quaternion.x), y(quaternion.y), z(quaternion.z), w(quaternion.w)
	{ }

	template <typename T>
	T Quaternion<T>::Norm() const
	{
		return std::sqrt(SquareNorm());
	}

	template <typename T>
	T Quaternion<T>::SquareNorm() const
	{
		return x * x + y * y + z * z + w * w;
	}

	template <typename T>
	Quaternion<T> Quaternion<T>::GetNormalize() const
	{
		T norm = Norm();
		return Quaternion<T>(x / norm, y / norm, z / norm, w / norm);
	}

	template<typename T>
	Quaternion<T>& Quaternion<T>::Normalize()
	{
		T inverseNorm = static_cast<T>(1) / Norm();
		this->x *= inverseNorm;
		this->y *= inverseNorm;
		this->z *= inverseNorm;
		this->w *= inverseNorm;
		return *this;
	}

	template <typename T>
	Quaternion<T> Quaternion<T>::GetRenormalize() const
	{
		T inverseNorm = FastInverseSquareRootAroundOne(SquareNorm());
		return Quaternion<T>(x * inverseNorm, y * inverseNorm, z * inverseNorm, w * inverseNorm);
	}

	template <typename T>
	Quaternion<T>& Quaternion<T>::Renormalize()
	{
		T inverseNorm = FastInverseSquareRootAroundOne(SquareNorm());
		*this *= inverseNorm;
		return *this;
	}

	template <typename T>
	Quaternion<T> Quaternion<T>::GetInverse() const
	{
		return GetConjugate();
	}

	template <typename T>
	Quaternion<T>& Quaternion<T>::Inverse()
	{
		x = -x;
		y = -y;
		z = -z;
		return *this;
	}

	template <typename T>
	Quaternion<T> Quaternion<T>::GetConjugate() const
	{
		return Quaternion<T>(-x, -y, -z, w);
	}

	template <typename T>
	Mat3<T> Quaternion<T>::GetRotationMatrix() const
	{
		Mat3<T> matrix;

		T xSquared = x * x;
		T ySquared = y * y;
		T zSquared = z * z;
		T wSquared = w * w;
		T xy = x * y;
		T xz = x * z;
		T yz = y * z;
		T wx = w * x;
		T wy = w * y;
		T wz = w * z;
		
		matrix(0, 0) = xSquared - ySquared - zSquared + wSquared;
		matrix(1, 1) = -xSquared + ySquared - zSquared + wSquared;
		matrix(2, 2) = -xSquared - ySquared + zSquared + wSquared;

		matrix(1, 0) = static_cast<T>(2) * (xy + wz);
		matrix(0, 1) = static_cast<T>(2) * (xy - wz);

		matrix(2, 0) = static_cast<T>(2) * (xz - wy);
		matrix(0, 2) = static_cast<T>(2) * (xz + wy);

		matrix(2, 1) = static_cast<T>(2) * (yz + wx);
		matrix(1, 2) = static_cast<T>(2) * (yz - wx);

		return matrix;
	}

	template <typename T>
	std::pair<T, Vec3<T>> Quaternion<T>::GetAngleAxis() const
	{
		// Extract the angle
		T angle = RadToDegrees(static_cast<T>(2) * std::acos(w));

		// Calculate the sine of the angle
		T sinAngle = std::sqrt(static_cast<T>(1) - w * w);

		// Handle the case when the angle is close to zero
		if (std::abs(sinAngle) < EPSILON) {
			// When close to zero, use a small angle approximation to avoid division by zero
			return { static_cast<T>(0), Vec3<T>::Zero() };
		}

		// Calculate the axis components
		T inverseSinAngle = static_cast<T>(1) / sinAngle;
		Vec3<T> rotationAxis(x, y, z);
		rotationAxis *= inverseSinAngle;

		return { angle, rotationAxis };
	}

	template <typename T>
	void Quaternion<T>::ConstrainTwist(T minAngleInDegrees, T maxAngleInDegrees)
	{
		// // Normalize the quaternion while constraining the twist in the y-axis direction
		// //T magnitude = Norm();
		//
		// T minAngleInRad = DegreesToRad(minAngleInDegrees);
		// T maxAngleInRad = DegreesToRad(maxAngleInDegrees);
		//
		// // Adjust the y-component while maintaining the direction
		// T inverseMagnitude = static_cast<T>(1) / magnitude;
		// w *= inverseMagnitude;
		// x *= inverseMagnitude;
		// y = std::max(maxAngleInRad, std::min(minAngleInRad, y * inverseMagnitude)); // Constrain y-component
		// z *= inverseMagnitude;

		T magnitude = std::sin(DegreesToRad(maxAngleInDegrees) / static_cast<T>(2));
		T squareMagnitude = magnitude * magnitude;
		
		Vec3F vector(x, y, z);
		
		if (vector.SquareMagnitude() > squareMagnitude)
		{
			vector = vector.GetNormalize() * magnitude;
		
			x = vector.x;
			y = vector.y;
			z = vector.z;
		
			T signOfW = std::signbit(w) == 0 ? static_cast<T>(1) : static_cast<T>(-1);
		
			w = std::sqrt(static_cast<T>(1) - squareMagnitude) * signOfW;
		}
	}


	template <typename T>
	void Quaternion<T>::SetOrthogonalRotationMatrix(const Mat3<T>& matrix)
	{
		T m00 = matrix(0, 0);
		T m11 = matrix(1, 1);
		T m22 = matrix(2, 2);
		T diagonalSum = m00 + m11 + m22;

		if (diagonalSum > static_cast<T>(0))
		{
			T fraction = static_cast<T>(0.5) / std::sqrt(diagonalSum + static_cast<T>(1));

			x = (matrix(2, 1) - matrix(1, 2)) * fraction;
			y = (matrix(0, 2) - matrix(2, 0)) * fraction;
			z = (matrix(1, 0) - matrix(0, 1)) * fraction;
			w = static_cast<T>(0.25) / fraction;
		}
		else if (m00 > m11 && m00 > m22)
		{
			T fraction = std::sqrt(m00 - m11 - m22 + static_cast<T>(1)) * static_cast<T>(2);

			x = static_cast<T>(0.25) * fraction;
			y = (matrix(0, 1) + matrix(1, 0)) / fraction;
			z = (matrix(0, 2) + matrix(2, 0)) / fraction;
			w = (matrix(2, 1) - matrix(1, 2)) / fraction;
		}
		else if (m11 > m22)
		{
			T fraction = std::sqrt(m11 - m00 - m22 + static_cast<T>(1)) * static_cast<T>(2);

			x = (matrix(0, 1) + matrix(1, 0)) / fraction;
			y = static_cast<T>(0.25) * fraction;
			z = (matrix(1, 2) + matrix(2, 1)) / fraction;
			w = (matrix(0, 2) - matrix(2, 0)) / fraction;
		}
		else
		{
			T fraction = std::sqrt(m22 - m00 - m11 + static_cast<T>(1)) * static_cast<T>(2);

			x = (matrix(0, 2) + matrix(2, 0)) / fraction;
			y = (matrix(1, 2) + matrix(2, 1)) / fraction;
			z = static_cast<T>(0.25) * fraction;
			w = (matrix(1, 0) - matrix(0, 1)) / fraction;
		}
	}

	template <typename T>
	void Quaternion<T>::SetNonOrthogonalRotationMatrix(const Mat3<T>& matrix)
	{
		T m00 = matrix(0, 0);
		T m11 = matrix(1, 1);
		T m22 = matrix(2, 2);

		T absQ2 = std::pow(matrix.Determinant(), static_cast<T>(1) / static_cast<T>(3));

		w = std::sqrt(std::max(static_cast<T>(0), absQ2 + m00 + m11 + m22)) / static_cast<T>(2);
		x = std::sqrt(std::max(static_cast<T>(0), absQ2 + m00 - m11 - m22)) / static_cast<T>(2);
		y = std::sqrt(std::max(static_cast<T>(0), absQ2 - m00 + m11 - m22)) / static_cast<T>(2);
		z = std::sqrt(std::max(static_cast<T>(0), absQ2 - m00 - m11 + m22)) / static_cast<T>(2);

		x = std::copysign(x, matrix(2, 1) - matrix(1, 2));
		y = std::copysign(y, matrix(0, 2) - matrix(2, 0));
		z = std::copysign(z, matrix(1, 0) - matrix(0, 1));
	}

	template <typename T>
	Vec3<T> Quaternion<T>::GetEulerAngles() const
	{
		Vec3<T> angles;

		// T singularityTest = x * y + z * w;
		//
		// T one = static_cast<T>(1);
		// T two = static_cast<T>(2);
		//
		// if(singularityTest > NORTH_POLE_SINGULARITY_VALUE)
		// {
		// 	angles.y = two * std::atan2(x, w);
		// 	angles.z = static_cast<T>(FLOAT_PI) / two;
		// }
		// else if(singularityTest < SOUTH_POLE_SINGULARITY_VALUE)
		// {
		// 	angles.y = -two * std::atan2(x, w);
		// 	angles.z = static_cast<T>(-FLOAT_PI) / two;
		// }
		// else
		// {
		// 	T xSquared = x * x;
		// 	T ySquared = y * y;
		// 	T zSquared = z * z;
		//
		// 	angles.y = std::atan2(two * y * w - two * x * z, one - two * ySquared - two * zSquared);
		// 	angles.z = std::asin(two * singularityTest);
		// 	angles.x = std::atan2(two * x * w - two * y * z, one - two * xSquared - two * zSquared);
		// }
		//
		// angles.x = RadToDegrees(angles.x);
		// angles.y = RadToDegrees(angles.y);
		// angles.z = RadToDegrees(angles.z);

		// Roll (x-axis rotation)
		T sinRoll = 2.0f * (w * x + y * z);
		T cosRoll = 1.0f - 2.0f * (x * x + y * y);
		angles.x = RadToDegrees(std::atan2(sinRoll, cosRoll));
		
		// Pitch (y-axis rotation)
		T sinPitch = 2.0f * (w * y - z * x);
		// Handle special cases to avoid undefined behavior with atan2
		if (std::abs(sinPitch) >= static_cast<T>(1))
			angles.y = RadToDegrees(std::copysign(static_cast<T>(FLOAT_PI) / static_cast<T>(2), sinPitch));
		else
			angles.y = RadToDegrees(std::asin(sinPitch));
		
		// Yaw (z-axis rotation)
		T sinYaw = static_cast<T>(2) * (w * z + x * y);
		T cosYaw = static_cast<T>(1) - static_cast<T>(2) * (y * y + z * z);
		angles.z = RadToDegrees(std::atan2(sinYaw, cosYaw));
		
		return angles;
	}

	template <typename T>
	Vec3<T> Quaternion<T>::TransformVector(const Quaternion<T>& quaternion, const Vec3<T>& vector)
	{
		const Vec3<T> quatCrossVector = static_cast<T>(2) * Vec3<T>::Cross(quaternion.components.xyz, vector);

		Vec3<T> transformedVector = vector + quaternion.w * quatCrossVector + Vec3<T>::Cross(quaternion.components.xyz, quatCrossVector);

		T scalar = quaternion.w;

		return transformedVector;
	}

	template <typename T>
	T Quaternion<T>::Dot(const Quaternion<T>& leftQuaternion, const Quaternion<T>& rightQuaternion)
	{
		return leftQuaternion.x * rightQuaternion.x +
			   leftQuaternion.y * rightQuaternion.y +
			   leftQuaternion.z * rightQuaternion.z +
			   leftQuaternion.w * rightQuaternion.w;
	}

	template <typename T>
	Quaternion<T> Quaternion<T>::RotateFromTo(const Vec3<T>& fromDirection, const Vec3<T>& toDirection)
	{
		Vec3F fromDirectionNormalized = fromDirection.GetNormalize();
		Vec3F toDirectionNormalized = toDirection.GetNormalize();
		
		float dot = Vec3F::Dot(fromDirectionNormalized, toDirectionNormalized);

		if (dot > static_cast<T>(1) - EPSILON)
		{
			return Quaternion<T>();
		}

		if (dot < static_cast<T>(-1) + EPSILON)
		{
			Vec3F rotationAxis = Vec3F::Cross(Vec3F::Right(), fromDirectionNormalized);
			if (rotationAxis.SquareMagnitude() < EPSILON)
			{
				rotationAxis = Vec3F::Cross(Vec3F::Up(), fromDirectionNormalized);
			}

			rotationAxis.Normalize();

			return Quaternion<T>(rotationAxis.x, rotationAxis.y, rotationAxis.z, static_cast<T>(0));
		}

		Vec3F rotationAxis = Vec3F::Cross(fromDirectionNormalized, toDirectionNormalized).Normalize();
		float angle = std::acos(dot);

		return Quaternion<T>(rotationAxis, RadToDegrees(angle));
	}

	template <typename T>
	Quaternion<T> Quaternion<T>::LookAt(const Vec3<T>& direction)
	{
		const float dot = Vec3<T>::Dot(Vec3<T>::Forward(), direction);

		if (std::abs(dot + static_cast<T>(1)) < EPSILON)
		{
			// vector a and b point exactly in the opposite direction, 
			// so it is a 180 degrees turn around the up-axis
			return Quaternion<T>(Vec3<T>::Up(), static_cast<T>(180));
		}

		if (std::abs(dot - static_cast<T>(1)) < EPSILON)
		{
			// vector a and b point exactly in the same direction
			// so we return the identity quaternion
			return Quaternion<T>();
		}

		T rotationAngle = RadToDegrees(std::acos(dot));

		Vec3<T> rotationAxis = Vec3<T>::Cross(Vec3<T>::Forward(), direction);

		return Quaternion<T>(rotationAxis.Normalize(), rotationAngle);
	}

	template <typename T>
	Quaternion<T> Quaternion<T>::MakeRotationX(const T angleInDegrees)
	{
		T halfAngle = DegreesToRad(angleInDegrees) / static_cast<T>(2);
		return Quaternion<T>(std::sin(halfAngle), static_cast<T>(0), static_cast<T>(0), std::cos(halfAngle));
	}

	template <typename T>
	Quaternion<T> Quaternion<T>::MakeRotationY(const T angleInDegrees)
	{
		T halfAngle = DegreesToRad(angleInDegrees) / static_cast<T>(2);
		return Quaternion<T>(static_cast<T>(0), std::sin(halfAngle), static_cast<T>(0), std::cos(halfAngle));
	}

	template <typename T>
	Quaternion<T> Quaternion<T>::MakeRotationZ(const T angleInDegrees)
	{
		T halfAngle = DegreesToRad(angleInDegrees) / static_cast<T>(2);
		return Quaternion<T>(static_cast<T>(0), static_cast<T>(0), std::sin(halfAngle), std::cos(halfAngle));
	}

	template <typename T>
	Quaternion<T> Quaternion<T>::MakeRotationAxisAngle(const Vec3<T>& unitVector, const T angleInDegrees)
	{
		return Quaternion<T>(unitVector, angleInDegrees);
	}

	template <typename T>
	Quaternion<T> Quaternion<T>::MakeRotationFromEulers(T xAngleInDegrees, T yAngleInDegrees, T zAngleInDegrees)
	{
		T two = static_cast<T>(2);

		T xHalfAngle = DegreesToRad(xAngleInDegrees / two);
		T yHalfAngle = DegreesToRad(yAngleInDegrees / two);
		T zHalfAngle = DegreesToRad(zAngleInDegrees / two);

		T xCos = std::cos(xHalfAngle);
		T yCos = std::cos(yHalfAngle);
		T zCos = std::cos(zHalfAngle);

		T xSin = std::sin(xHalfAngle);
		T ySin = std::sin(yHalfAngle);
		T zSin = std::sin(zHalfAngle);

		T zCosXCos = zCos * xCos;
		T zSinXSin = zSin * xSin;
		T zSinXCos = zSin * xCos;
		T zCosXSin = zCos * xSin;

		T w = zCosXCos * yCos + zSinXSin * ySin;
		T x = zCosXSin * yCos - zSinXCos * ySin;
		T y = zCosXCos * ySin + zSinXSin * yCos;
		T z = zSinXCos * yCos - zCosXSin * ySin;

		return Quaternion(x, y, z, w);
	}

	template <typename T>
	Quaternion<T> Quaternion<T>::MakeRotationFromEulers(Vec3<T> anglesInDegrees)
	{
		return MakeRotationFromEulers(anglesInDegrees.x, anglesInDegrees.y, anglesInDegrees.z);
	}


	template <typename T>
	Quaternion<T> Quaternion<T>::Lerp(const Quaternion<T>& leftQuaternion, const Quaternion<T>& rightQuaternion, const T t)
	{
		T clampedT = std::max(0.0f, std::min(1.0f, t));

		return leftQuaternion * (static_cast<T>(1) - clampedT) + rightQuaternion * clampedT;
	}

	template <typename T>
	Quaternion<T> Quaternion<T>::Nlerp(const Quaternion<T>& leftQuaternion, const Quaternion<T>& rightQuaternion, const T t)
	{
		return Lerp(leftQuaternion, rightQuaternion, t).GetNormalize();
	}

	template <typename T>
	Quaternion<T> Quaternion<T>::Slerp(const Quaternion<T>& leftQuaternion, const Quaternion<T>& rightQuaternion, const T t)
	{
		T clampedT = std::clamp(t, static_cast<T>(0), static_cast<T>(1));

		T dot = Quaternion<T>::Dot(leftQuaternion, rightQuaternion);

		Quaternion<T> newRightQuaternion = rightQuaternion;
		if (dot < static_cast<T>(0))
		{
			dot = -dot;
			newRightQuaternion = rightQuaternion * static_cast<T>(-1);
		}

		if (dot > SLERP_DOT_THRESHOLD)
		{
			return Nlerp(leftQuaternion, newRightQuaternion, t);
		}
		
		dot = std::clamp(dot, static_cast<T>(-1), static_cast<T>(1));
		T theta = std::acos(dot) * clampedT; // Angle between leftQuaternion and new quaternion at t
		Quaternion<T> newQuaternion = newRightQuaternion - leftQuaternion * dot;
		newQuaternion.Normalize();
		
		return leftQuaternion * std::cos(theta) + newQuaternion * std::sin(theta);
	}

	template <typename T>
	void Quaternion<T>::Decompose(const Quaternion<T>& quaternion, const Vec3<T>& twistAxis, Quaternion<T>& outSwing, Quaternion<T>& outTwist)
	{
		Vec3<T> r = quaternion.vector;

		// singularity: rotation by 180 degree
		if (r.SquareMagnitude() < EPSILON)
		{
			Vec3<T> rotatedTwistAxis = quaternion * twistAxis;
			Vec3<T> swingAxis = Vec3<T>::Cross(twistAxis, rotatedTwistAxis);

			if (swingAxis.SquareMagnitude() > EPSILON)
			{
				float swingAngle = Vec3<T>::Angle(twistAxis, rotatedTwistAxis);
				outSwing = Quaternion<T>(swingAxis, swingAngle);
			}
			else
			{
				// more singularity:
				// rotation axis parallel to twist axis
				outSwing = Quaternion<T>(); // no swing
			}

			// always twist 180 degree on singularity
			outTwist = Quaternion<T>(twistAxis, 180.0f);
			return;
		}

		Vec3<T> p = Vec3<T>::Project(r, twistAxis);
		outTwist = Quaternion<T>(p.x, p.y, p.z, quaternion.w);
		outTwist.Normalize();
		outSwing = quaternion * outTwist.GetInverse();
	}

	template <typename T>
	Quaternion<T>& Quaternion<T>::operator=(const Quaternion<T>& quaternion)
	{
		x = quaternion.x;
		y = quaternion.y;
		z = quaternion.z;
		w = quaternion.w;
		return *this;
	}

	template <typename T>
	Quaternion<T>& Quaternion<T>::operator*=(const Quaternion<T>& quaternion)
	{
		Quaternion<T> temp = *this;

		this->x = temp.w * quaternion.x + temp.x * quaternion.w + temp.y * quaternion.z - temp.z * quaternion.y;
		this->y = temp.w * quaternion.y + temp.y * quaternion.w + temp.z * quaternion.x - temp.x * quaternion.z;
		this->z = temp.w * quaternion.z + temp.z * quaternion.w + temp.x * quaternion.y - temp.y * quaternion.x;
		this->w = temp.w * quaternion.w - temp.x * quaternion.x - temp.y * quaternion.y - temp.z * quaternion.z;

		return *this;
	}

	template <typename T>
	Quaternion<T>& Quaternion<T>::operator*=(const T scalar)
	{
		x *= scalar;
		y *= scalar;
		z *= scalar;
		w *= scalar;
		return *this;
	}

	template <typename T>
	Quaternion<T>& Quaternion<T>::operator/=(const T scalar)
	{
		x /= scalar;
		y /= scalar;
		z /= scalar;
		w /= scalar;
		return *this;
	}

	template <typename T>
	Quaternion<T>& Quaternion<T>::operator+=(const Quaternion<T>& quaternion)
	{
		x += quaternion.x;
		y += quaternion.y;
		z += quaternion.z;
		w += quaternion.w;
		return *this;
	}

	template <typename T>
	Quaternion<T>& Quaternion<T>::operator-=(const Quaternion<T>& quaternion)
	{
		x -= quaternion.x;
		y -= quaternion.y;
		z -= quaternion.z;
		w -= quaternion.w;
		return *this;
	}

	template <typename T>
	Quaternion<T> operator*(const Quaternion<T>& leftQuaterion, const Quaternion<T>& rightQuaterion)
	{
		Quaternion<T> leftQuaterionCopy = leftQuaterion;
		return leftQuaterionCopy *= rightQuaterion;
	}

	template <typename T>
	Quaternion<T> operator*(const Quaternion<T>& quaterion, const T scalar)
	{
		Quaternion quaterionCopy = quaterion;
		return quaterionCopy *= scalar;
	}

	template<typename T>
	Quaternion<T> operator*(const T scalar, const Quaternion<T>& quaterion)
	{
		Quaternion quaterionCopy = quaterion;
		return quaterionCopy *= scalar;
	}

	template <typename T>
	Quaternion<T> operator/(const Quaternion<T>& quaterion, const T scalar)
	{
		Quaternion<T> quaterionCopy = quaterion;
		return quaterionCopy /= scalar;
	}

	template<typename T>
	Quaternion<T> operator/(const T scalar, const Quaternion<T>& quaterion)
	{
		Quaternion<T> quaterionCopy = quaterion;
		return quaterionCopy /= scalar;
	}

	template <typename T>
	Quaternion<T> operator+(const Quaternion<T>& leftQuaterion, const Quaternion<T>& rightQuaterion)
	{
		Quaternion<T> leftQuaterionCopy = leftQuaterion;
		return leftQuaterionCopy += rightQuaterion;
	}

	template <typename T>
	Quaternion<T> operator-(const Quaternion<T>& leftQuaterion, const Quaternion<T>& rightQuaterion)
	{
		Quaternion<T> leftQuaterionCopy = leftQuaterion;
		return leftQuaterionCopy -= rightQuaterion;
	}

	template <typename T>
	std::ostream& operator<<(std::ostream& stream, const Quaternion<T>& quaternion)
	{
		stream << "([" << quaternion.x << ", " << quaternion.y << ", " << quaternion.z << "], " << quaternion.w << ")";
		return stream;
	}
}