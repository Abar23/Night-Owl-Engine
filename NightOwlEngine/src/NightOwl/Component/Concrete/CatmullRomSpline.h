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

		Vec3F EvaluateTangentUsingParameter(float u);

		Vec3F EvaluateUsingArcLength(float s);

		Vec3F EvaluateTangentUsingArcLength(float s);

		void AddControlPoint(const Vec3F& controlPoint);

		void RemoveControlPointAtIndex(unsigned int index);

		Vec3F GetControlPointAtIndex(unsigned int index);

		void SetControlPointAtIndex(unsigned int index, const Vec3F& controlPoint);

		float GetArcLength() const;

		int GetNumberOfCurveSegments() const;

		int GetNumberOfControlPoints() const;

		bool IsLooped() const;

		void ShouldLoop(bool enable);

		float GetTension() const;

		void SetTension(float tension);

		void ResetTensionToDefault();

	private:
		Vec3F startPoint;

		Vec3F endPoint;

		float tension;

		static inline float constexpr DEFAULT_TENSION = 0.5f;

		bool shouldLoop;

		std::vector<Vec3F> controlPoints;

		void GetCurveIndexForArcLength(float nonNormalizedS, float& arcLengthOfPreviousSegments, int& curveIndex) const;

		float GetUWithBisection(float nonNormalizedS, float arcLengthOfPreviousSegments, int curveIndex) const;

		Vec3F GetPointOnCatmullRomSpline(float u, int curveIndex);

		Vec3F GetTangentOfCatmullRomSpline(float u, int curveIndex);

		void UpdateStartAndEndPoint();

		float GaussianQuadrature(float u, int curveIndex) const;

		float ArcLengthDistanceFunction(float u, int curveIndex) const;

		void GetControlPointForCurveIndex(int curveIndex, Vec3F& pIMinus2, Vec3F& pIMinus1, Vec3F& pI, Vec3F& pIPlus1) const;

		void Remove() override;
	};
}
