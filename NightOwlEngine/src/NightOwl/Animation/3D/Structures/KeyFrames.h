#pragma once

#include "KeyPosition.h"
#include "KeyRotation.h"
#include "KeyScale.h"
#include <assimp/anim.h>
#include <vector>

namespace NightOwl
{
	class KeyFrames
	{
	public:
		KeyFrames(const aiNodeAnim* assimpAnimationNode);

		void Update(float animationTime);

		const Vec3F& GetFinalPosition();

		const Vec3F& GetFinalScale();

		const QuatF& GetFinalRotation();

	private:
		float animationTime;

		Vec3F finalPosition;

		Vec3F finalScale;

		QuatF finalRotation;

		std::vector<KeyPosition> keyPositions;

		std::vector<KeyScale> keyScales;

		std::vector<KeyRotation> keyRotations;

		void FillKeyPositionsData(const aiNodeAnim* assimpAnimationNode);

		void FillKeyScalesData(const aiNodeAnim* assimpAnimationNode);

		void FillKeyRotationsData(const aiNodeAnim* assimpAnimationNode);

		float GetScaleFactor(float previousTimeStamp, float nextTimeStamp, float animationTime);

		int GetKeyPositionIndex();

		int GetKeyScaleIndex();

		int GetKeyRotationIndex();

		void InterpolateKeyPosition();

		void InterpolateKeyScale();

		void InterpolateKeyRotation();
	};
}
