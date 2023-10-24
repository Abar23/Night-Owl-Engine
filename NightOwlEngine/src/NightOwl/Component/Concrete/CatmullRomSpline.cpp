#include "NightOwlPch.h"

#include "CatmullRomSpline.h"
#include "NightOwl/Core/Utitlity/Assert.h"
#include <cmath>

namespace NightOwl
{
	CatmullRomSpline::CatmullRomSpline()
		: Component(ComponentType::CatmullRomSpline),
		  tension(DEFAULT_TENSION),
		  shouldLoop(false)
	{ }

	Vec3F CatmullRomSpline::EvaluateUsingParameter(float u)
	{
		if (controlPoints.empty())
		{
			return Vec3F();
		}

		if (controlPoints.size() == 1 || u < 0.0f)
		{
			return controlPoints.front();
		}

		const int numberOfCurveSegments = GetNumberOfCurveSegments();
		if (u > static_cast<float>(numberOfCurveSegments) ||
			NearEquals(u, static_cast<float>(numberOfCurveSegments)) == true)
		{
			return controlPoints.back();
		}

		float integralPart;
		const float normalizedU = std::modf(u, &integralPart);

		// Adding one to make sure that pi is the third point
		const int curveIndex = static_cast<int>(integralPart);

		if (curveIndex >= controlPoints.size() - 1)
		{
			return controlPoints.back();
		}

		return GetPointOnCatmullRomSpline(normalizedU, curveIndex);
	}

	Vec3F CatmullRomSpline::EvaluateTangentUsingParameter(float u)
	{
		if (controlPoints.empty() ||
			controlPoints.size() == 1)
		{
			return Vec3F::Forward();
		}

		const int numberOfCurveSegments = GetNumberOfCurveSegments();
		u = std::clamp(u, 0.0f, static_cast<float>(numberOfCurveSegments));

		float integralPart;
		float normalizedU = std::modf(u, &integralPart);

		int curveIndex = static_cast<int>(integralPart);
		if (curveIndex >= numberOfCurveSegments)
		{
			normalizedU = 0.99f;
			curveIndex = numberOfCurveSegments - 1;
		}

		return GetTangentOfCatmullRomSpline(normalizedU, curveIndex);
	}

	Vec3F CatmullRomSpline::EvaluateUsingArcLength(float s)
	{
		if (controlPoints.empty())
		{
			return Vec3F();
		}

		if (controlPoints.size() == 1 || s < 0.0f)
		{
			return controlPoints[0];
		}

		if (s > 1.0f || NearEquals(s, 1.0f))
		{
			return controlPoints.back();
		}

		const float nonNormalizedS = s * GetArcLength();

		float arcLengthOfPreviousSegments;
		int curveIndex;
		GetCurveIndexForArcLength(nonNormalizedS, arcLengthOfPreviousSegments, curveIndex);

		const float u = GetUWithBisection(nonNormalizedS, arcLengthOfPreviousSegments, curveIndex);

		return EvaluateUsingParameter(u + static_cast<float>(curveIndex));
	}

	Vec3F CatmullRomSpline::EvaluateTangentUsingArcLength(float s)
	{
		if (controlPoints.empty() ||
			controlPoints.size() == 1)
		{
			return Vec3F::Forward();
		}

		const float clampedS = std::clamp(s, 0.0f, 1.0f);

		const float totalArcLength = GetArcLength();
		const float nonNormalizedS = clampedS * GetArcLength();
		if (nonNormalizedS > totalArcLength ||
			NearEquals(totalArcLength, nonNormalizedS))
		{
			return EvaluateTangentUsingParameter(GetNumberOfCurveSegments()).Normalize();
		}

		float arcLengthOfPreviousSegments;
		int curveIndex;
		GetCurveIndexForArcLength(nonNormalizedS, arcLengthOfPreviousSegments, curveIndex);

		const float u = GetUWithBisection(nonNormalizedS, arcLengthOfPreviousSegments, curveIndex);

		return EvaluateTangentUsingParameter(u + static_cast<float>(curveIndex)).Normalize();
	}

	void CatmullRomSpline::AddControlPoint(const Vec3F& controlPoint)
	{
		controlPoints.push_back(controlPoint);

		UpdateStartAndEndPoint();
	}

	void CatmullRomSpline::RemoveControlPointAtIndex(unsigned int index)
	{
		ENGINE_ASSERT(index < controlPoints.size(), "Trying to retrieve control point with invalid index!");

		controlPoints.erase(controlPoints.begin() + index);
	}

	Vec3F CatmullRomSpline::GetControlPointAtIndex(unsigned int index)
	{
		ENGINE_ASSERT(index < controlPoints.size(), "Trying to retrieve control point with invalid index!");

		return controlPoints[index];
	}

	void CatmullRomSpline::SetControlPointAtIndex(unsigned int index, const Vec3F& controlPoint)
	{
		ENGINE_ASSERT(index < controlPoints.size(), "Trying to retrieve control point with invalid index!");

		controlPoints[index] = controlPoint;

		UpdateStartAndEndPoint();
	}

	float CatmullRomSpline::GetArcLength() const
	{
		if (controlPoints.empty() ||
			controlPoints.size() == 1)
		{
			return 0.0f;
		}

		float arcLengthApproximation = 0.0f;
		for (int curveIndex = 0; curveIndex < controlPoints.size() - 1; ++curveIndex)
		{
			arcLengthApproximation += GaussianQuadrature(1.0f, curveIndex);
		}

		return arcLengthApproximation;
	}

	int CatmullRomSpline::GetNumberOfCurveSegments() const
	{
		return controlPoints.size() - 1;
	}

	int CatmullRomSpline::GetNumberOfControlPoints() const
	{
		return controlPoints.size();
	}

	bool CatmullRomSpline::IsLooped() const
	{
		return shouldLoop;
	}

	void CatmullRomSpline::ShouldLoop(bool enable)
	{
		if (enable == shouldLoop)
		{
			return;
		}

		shouldLoop = enable;

		UpdateStartAndEndPoint();
	}

	float CatmullRomSpline::GetTension() const
	{
		return tension;
	}

	void CatmullRomSpline::SetTension(float tension)
	{
		this->tension = tension;
	}

	void CatmullRomSpline::ResetTensionToDefault()
	{
		tension = DEFAULT_TENSION;
	}

	void CatmullRomSpline::GetCurveIndexForArcLength(float nonNormalizedS, float& arcLengthOfPreviousSegments, int& curveIndex) const
	{
		arcLengthOfPreviousSegments = 0.0f;
		curveIndex = 0;

		float arcLength = 0.0f;
		while (curveIndex < GetNumberOfCurveSegments())
		{
			arcLengthOfPreviousSegments = arcLength;
			arcLength += GaussianQuadrature(1.0f, curveIndex);

			if (nonNormalizedS - arcLength < 0.0f)
			{
				break;
			}

			++curveIndex;
		}
	}

	float CatmullRomSpline::GetUWithBisection(float nonNormalizedS, float arcLengthOfPreviousSegments, int curveIndex) const
	{
		// bisect algorithm
		float uLow = 0.0f;
		float uHigh = 1.0f;
		float uMiddle = 0.5f;

		float sOfUMiddle = GaussianQuadrature(uMiddle, curveIndex);

		while (std::abs(arcLengthOfPreviousSegments + sOfUMiddle - nonNormalizedS) > EPSILON)
		{
			if (arcLengthOfPreviousSegments + sOfUMiddle < nonNormalizedS)
			{
				uLow = uMiddle;
			}
			else
			{
				uHigh = uMiddle;
			}

			uMiddle = (uLow + uHigh) / 2.0f;

			sOfUMiddle = GaussianQuadrature(uMiddle, curveIndex);
		}

		return uMiddle;
	}

	Vec3F CatmullRomSpline::GetPointOnCatmullRomSpline(float u, int curveIndex)
	{
		if (controlPoints.empty())
		{
			return Vec3F();
		}

		if (controlPoints.size() == 1)
		{
			return controlPoints[0];
		}

		// For reference: https://www.cs.cmu.edu/~fp/courses/graphics/asst5/catmullRom.pdf - tensor is assumed to be 0.5 for all calculations below
		Vec3F pIMinus2, pIMinus1, pI, pIPlus1;
		GetControlPointForCurveIndex(curveIndex, pIMinus2, pIMinus1, pI, pIPlus1);

		const float uSquared = u * u;
		const float uCubed = uSquared * u;

		// Calculating each piece of Mu from the document, again tensor is 0.5
		const float muCoefficientOne = -u + 2.0f * uSquared - uCubed;
		const float muCoefficientTwo = 2.0f - 5.0f * uSquared + 3.0f * uCubed;
		const float muCoefficientThree = u + 4.0f * uSquared - 3.0f * uCubed;
		const float muCoefficientFour = -uSquared + uCubed;

		Vec3F point = tension * (muCoefficientOne * pIMinus2 + muCoefficientTwo * pIMinus1 + muCoefficientThree * pI + muCoefficientFour * pIPlus1);

		return point;
	}

	Vec3F CatmullRomSpline::GetTangentOfCatmullRomSpline(float u, int curveIndex)
	{
		if (controlPoints.empty() ||
			controlPoints.size() == 1)
		{
			return Vec3F::Forward();
		}

		Vec3F pIMinus2, pIMinus1, pI, pIPlus1;
		GetControlPointForCurveIndex(curveIndex, pIMinus2, pIMinus1, pI, pIPlus1);

		const float uSquared = u * u;

		// Calculating each piece of Mu from the document, again tensor is 0.5
		const float muCoefficientOne = -1.0f + 4.0f * u - 3.0f * uSquared;
		const float muCoefficientTwo = -10.0f * u + 9.0f * uSquared;
		const float muCoefficientThree = 1.0f + 8.0f * u - 9.0f * uSquared;
		const float muCoefficientFour = -2.0f * u + 3.0f * uSquared;

		Vec3F point = tension * (muCoefficientOne * pIMinus2 + muCoefficientTwo * pIMinus1 + muCoefficientThree * pI + muCoefficientFour * pIPlus1);

		return point;
	}

	void CatmullRomSpline::UpdateStartAndEndPoint()
	{
		if (controlPoints.size() <= 1)
		{
			return;
		}

		if (shouldLoop)
		{
			startPoint = controlPoints[controlPoints.size() - 2];
			endPoint = controlPoints[1];

			return;
		}

		const Vec3F& firstPoint = controlPoints.front();
		const Vec3F& secondPoint = controlPoints[1];
		const Vec3F& secondToLastPoint = controlPoints[controlPoints.size() - 2];
		const Vec3F& lastPoint = controlPoints.back();

		startPoint = firstPoint + (firstPoint - secondPoint);
		endPoint = lastPoint + (lastPoint - secondToLastPoint);
	}

	float CatmullRomSpline::GaussianQuadrature(float u, int curveIndex) const
	{
		// 3-point quadrature weights and nodes:
		static float nodes[3] = { -0.7745967f, 0.0f, 0.7745967f }; // sqrt(3.0 / 5.0)
		static float weights[3] = { 0.5555556f, 0.8888889f,0.5555556f }; // 5.0 / 9.0, 8.0 / 9.0

		float integral = 0.0;

		for (int nodeIndex = 0; nodeIndex < 3; ++nodeIndex) 
		{
			const double node = 0.5f * (u * nodes[nodeIndex] + u);
			integral += weights[nodeIndex] * ArcLengthDistanceFunction(node, curveIndex);
		}
		
		integral *= 0.5f * u;

		return integral;
	}

	float CatmullRomSpline::ArcLengthDistanceFunction(float u, int curveIndex) const
	{
		// get the four points of the curve
		Vec3F p0, p1, p2, p3;
		GetControlPointForCurveIndex(curveIndex, p0, p1, p2, p3);

		const Vec3F a = tension * (-3.0f * p0 + 9.0f * p1 - 9.0f * p2 + 3.0f * p3);
		const Vec3F b = tension * (4.0f * p0 - 10.0f * p1 + 8.0f * p2 - 2.0f * p3);
		const Vec3F c = tension * (-1.0f * p0 + p2);

		const float A = Vec3F::Dot(a, a);
		const float B = 2.0f * Vec3F::Dot(a, b);
		const float C = 2.0f * Vec3F::Dot(a, c) + Vec3F::Dot(b, b);
		const float D = 2.0f * Vec3F::Dot(b, c);
		const float E = Vec3F::Dot(c, c);

		const float uSquared = u * u;
		const float uCubed = uSquared * u;
		const float uFourthed = uCubed * u;

		return std::sqrtf(A * uFourthed + B * uCubed + C * uSquared + D * u + E);
	}

	void CatmullRomSpline::GetControlPointForCurveIndex(int curveIndex, Vec3F& pIMinus2, Vec3F& pIMinus1, Vec3F& pI, Vec3F& pIPlus1) const
	{
		pIMinus2 = (curveIndex - 1 < 0) ? startPoint : controlPoints[curveIndex - 1];
		pIMinus1 = controlPoints[curveIndex];
		pI = controlPoints[curveIndex + 1];
		pIPlus1 = (curveIndex + 2 >= controlPoints.size()) ? endPoint : controlPoints[curveIndex + 2];
	}

	void CatmullRomSpline::Remove()
	{ }
}
