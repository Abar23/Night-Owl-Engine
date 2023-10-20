#include "NightOwlPch.h"

#include "CatmullRomSpline.h"
#include "NightOwl/Core/Utitlity/Assert.h"
#include <cmath>

namespace NightOwl
{
	CatmullRomSpline::CatmullRomSpline()
		: Component(ComponentType::CatmullRomSpline)
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

		if (s > 1.0f ||
			NearEquals(s, 1.0f) == true)
		{
			return controlPoints.back();
		}

		const float nonNormalizedS = s * GetArcLength();

		float previousArcLength = 0.0f;
		float arcLength = 0.0f;
		int curveIndex = 0;
		for (; curveIndex < controlPoints.size() - 1; ++curveIndex)
		{
			previousArcLength = arcLength;
			arcLength += GaussianQuadrature(1.0f, curveIndex);

			if (nonNormalizedS - arcLength < 0.0f)
			{
				break;
			}
		}

		// bisect algorithm
		float ua = 0.0f;
		float ub = 1.0f;
		float um = 0.5f;

		float sm = GaussianQuadrature(um, curveIndex);

		while (std::abs(previousArcLength + sm - nonNormalizedS) > BIG_EPSILON)
		{
			if (previousArcLength + sm < nonNormalizedS)
			{
				ua = um;
			}
			else
			{
				ub = um;
			}

			um = (ua + ub) / 2.0f;

			sm = GaussianQuadrature(um, curveIndex);
		}

		// use um from the bisect algorithm to get the point on the curve that matches the arc length
		return EvaluateUsingParameter(um + static_cast<float>(curveIndex));
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

	float CatmullRomSpline::GetArcLength()
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

	int CatmullRomSpline::GetNumberOfCurveSegments()
	{
		return controlPoints.size() - 1;
	}

	int CatmullRomSpline::GetNumberOfControlPoints()
	{
		return controlPoints.size();
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

		Vec3F point = 0.5f * (muCoefficientOne * pIMinus2 + muCoefficientTwo * pIMinus1 + muCoefficientThree * pI + muCoefficientFour * pIPlus1);

		return point;
	}

	void CatmullRomSpline::UpdateStartAndEndPoint()
	{
		if (controlPoints.size() <= 1)
		{
			return;
		}

		const Vec3F& firstPoint = controlPoints.front();
		const Vec3F& secondPoint = controlPoints[1];
		const Vec3F& secondToLastPoint = controlPoints[controlPoints.size() - 2];
		const Vec3F& lastPoint = controlPoints.back();

		startPoint = firstPoint + (firstPoint - secondPoint);
		endPoint = lastPoint + (lastPoint - secondToLastPoint);
	}

	float CatmullRomSpline::GaussianQuadrature(float u, int curveIndex)
	{
		std::vector<double> nodes, weights;

		// 2-point quadrature weights and nodes:
		nodes.push_back(-0.5773502691896257);
		nodes.push_back(0.5773502691896257);
		weights.push_back(1.0);
		weights.push_back(1.0);

		double integral = 0.0;

		for (int nodeIndex = 0; nodeIndex < nodes.size(); ++nodeIndex) 
		{
			const double node = 0.5f * (u * nodes[nodeIndex] + u);
			integral += weights[nodeIndex] * ArcLengthDistanceFunction(node, curveIndex);
		}
		
		integral *= 0.5f * u;

		return integral;
	}

	float CatmullRomSpline::ArcLengthDistanceFunction(float u, int curveIndex)
	{
		// get the four points of the curve
		Vec3F p0, p1, p2, p3;
		GetControlPointForCurveIndex(curveIndex, p0, p1, p2, p3);

		const Vec3F a = 1.5f * (-1.0f * p0 + 3.0f * p1 - 3.0f * p2 + p3);
		const Vec3F b = 2.0f * p0 - 5.0f * p1 + 4.0f * p2 - p3;
		const Vec3F c = 0.5f * (-1.0f * p0 + p2);

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

	void CatmullRomSpline::GetControlPointForCurveIndex(int curveIndex, Vec3F& pIMinus2, Vec3F& pIMinus1, Vec3F& pI, Vec3F& pIPlus1)
	{
		pIMinus2 = (curveIndex - 1 < 0) ? startPoint : controlPoints[curveIndex - 1];
		pIMinus1 = controlPoints[curveIndex];
		pI = controlPoints[curveIndex + 1];
		pIPlus1 = (curveIndex + 2 >= controlPoints.size()) ? endPoint : controlPoints[curveIndex + 2];
	}

	void CatmullRomSpline::Remove()
	{ }
}
