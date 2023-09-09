#pragma once

#include "KeyPosition.h"
#include "KeyScale.h"
#include "KeyRotation.h"
#include "NightOwl/Math/Mat4.h"
#include <vector>
#include <assimp/anim.h>


namespace NightOwl
{
	class BoneKeyFrames
	{
	public:
		BoneKeyFrames(const aiNodeAnim* assimpAnimationNode);

		void Update(float animationTime);

		const Vec3F& GetFinalPosition();

		const Vec3F& GetFinalScale();

		const QuatF& GetFinalRotation();

	private:
		float animationTime;

		unsigned int currentKeyPositionIndex;

		unsigned int currentKeyScaleIndex;

		unsigned int currentKeyRotationIndex;

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

		void UpdateKeyPositionIndex();

		void UpdateKeyScaleIndex();

		void UpdateKeyRotationIndex();

		Vec3F InterpolateKeyPosition();

		Vec3F InterpolateKeyScale();

		QuatF InterpolateKeyRotation();
	};
}
