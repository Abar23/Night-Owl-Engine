#pragma once

#include "NightOwl/Component/Component.h"
#include "NightOwl/Math/Vec3.h"
#include <vector>

namespace NightOwl
{
	class CatmullRomSpline : public Component
	{
	public:
		CatmullRomSpline();

		Vec3F EvaluateUsingParameter(float u);

		Vec3F EvaluateUsingArcLength(float s);

		void AddControlPoint(const Vec3F& controlPoint);

		void RemoveControlPointAtIndex(unsigned int index);

		Vec3F GetControlPointAtIndex(unsigned int index);

		void SetControlPointAtIndex(unsigned int index, const Vec3F& controlPoint);

		float GetArcLength();

		int GetNumberOfCurveSegments();

		int GetNumberOfControlPoints();

	private:
		Vec3F startPoint;

		Vec3F endPoint;

		std::vector<Vec3F> controlPoints;

		Vec3F GetPointOnCatmullRomSpline(float u, int curveIndex);

		void UpdateStartAndEndPoint();

		float GaussianQuadrature(float u, int curveIndex);

		float ArcLengthDistanceFunction(float u, int curveIndex);

		void GetControlPointForCurveIndex(int curveIndex, Vec3F& pIMinus2, Vec3F& pIMinus1, Vec3F& pI, Vec3F& pIPlus1);

		void Remove() override;
	};
}
