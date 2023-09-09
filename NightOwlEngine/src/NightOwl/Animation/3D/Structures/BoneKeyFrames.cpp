#include "NightOwlPch.h"

#include "BoneKeyFrames.h"

#include "NightOwl/Core/Utitlity/Utils.h"

namespace NightOwl
{
	BoneKeyFrames::BoneKeyFrames(const aiNodeAnim* assimpAnimationNode)
		: currentKeyPositionIndex(0),
		  currentKeyScaleIndex(0),
		  currentKeyRotationIndex(0)
	{
		FillKeyPositionsData(assimpAnimationNode);
		FillKeyScalesData(assimpAnimationNode);
		FillKeyRotationsData(assimpAnimationNode);
	}

	void BoneKeyFrames::Update(float animationTime)
	{
		this->animationTime = animationTime;

		finalPosition = InterpolateKeyPosition();
		finalScale = InterpolateKeyScale();
		finalRotation = InterpolateKeyRotation();
	}

	const Vec3F& BoneKeyFrames::GetFinalPosition()
	{
		return finalPosition;
	}

	const Vec3F& BoneKeyFrames::GetFinalScale()
	{
		return finalScale;
	}

	const QuatF& BoneKeyFrames::GetFinalRotation()
	{
		return finalRotation;
	}

	void BoneKeyFrames::FillKeyPositionsData(const aiNodeAnim* assimpAnimationNode)
	{
		for (unsigned int keyPositionIndex = 0; keyPositionIndex < assimpAnimationNode->mNumPositionKeys; ++keyPositionIndex)
		{
			const aiVectorKey positionVectorKey = assimpAnimationNode->mPositionKeys[keyPositionIndex];

			Vec3F position = Utility::AssimpVec3ToNightOwlVec3F(positionVectorKey.mValue);
			float timeStamp = positionVectorKey.mTime;

			keyPositions.emplace_back(position, timeStamp);
		}
	}

	void BoneKeyFrames::FillKeyScalesData(const aiNodeAnim* assimpAnimationNode)
	{
		for (unsigned int keyScaleIndex = 0; keyScaleIndex < assimpAnimationNode->mNumScalingKeys; ++keyScaleIndex)
		{
			const aiVectorKey scaleVectorKey = assimpAnimationNode->mScalingKeys[keyScaleIndex];

			Vec3F scale = Utility::AssimpVec3ToNightOwlVec3F(scaleVectorKey.mValue);
			float timeStamp = scaleVectorKey.mTime;

			keyScales.emplace_back(scale, timeStamp);
		}
	}

	void BoneKeyFrames::FillKeyRotationsData(const aiNodeAnim* assimpAnimationNode)
	{
		for (unsigned int keyRotationIndex = 0; keyRotationIndex < assimpAnimationNode->mNumRotationKeys; ++keyRotationIndex)
		{
			const aiQuatKey quatVectorKey = assimpAnimationNode->mRotationKeys[keyRotationIndex];

			QuatF orientation = Utility::AssimpQuaternionToNightOwlQuatF(quatVectorKey.mValue);
			float timeStamp = quatVectorKey.mTime;

			keyRotations.emplace_back(orientation, timeStamp);
		}
	}

	float BoneKeyFrames::GetScaleFactor(float previousTimeStamp, float nextTimeStamp, float animationTime)
	{
		const float midWayLength = nextTimeStamp - animationTime;
		const float timeDifferenceBetweenFrames = nextTimeStamp - previousTimeStamp;

		return midWayLength / timeDifferenceBetweenFrames;
	}

	void BoneKeyFrames::UpdateKeyPositionIndex()
	{
		if (currentKeyPositionIndex > keyPositions.size() - 1)
		{
			currentKeyPositionIndex = 0;
		}

		if (animationTime > keyPositions[currentKeyPositionIndex + 1].timeStamp)
		{
			currentKeyPositionIndex++;
		}
	}

	void BoneKeyFrames::UpdateKeyScaleIndex()
	{
		if (currentKeyScaleIndex > keyScales.size() - 1)
		{
			currentKeyScaleIndex = 1;
		}

		if (animationTime > keyScales[currentKeyScaleIndex + 1].timeStamp)
		{
			currentKeyScaleIndex++;
		}
	}

	void BoneKeyFrames::UpdateKeyRotationIndex()
	{
		if (currentKeyRotationIndex > keyRotations.size() - 1)
		{
			currentKeyRotationIndex = 1;
		}

		if (animationTime > keyRotations[currentKeyRotationIndex + 1].timeStamp)
		{
			currentKeyRotationIndex++;
		}
	}

	Vec3F BoneKeyFrames::InterpolateKeyPosition()
	{
		if (keyPositions.size() == 1)
		{
			return keyPositions[0].position;
		}

		UpdateKeyPositionIndex();

		const KeyPosition& previousKey = keyPositions.at(currentKeyPositionIndex);
		const KeyPosition& nextKey = keyPositions.at(currentKeyPositionIndex + 1);

		const float scaleFactor = GetScaleFactor(previousKey.timeStamp, nextKey.timeStamp, animationTime);

		Vec3F finalPosition = Vec3F::Lerp(previousKey.position, nextKey.position, scaleFactor);

		return finalPosition;
	}

	Vec3F BoneKeyFrames::InterpolateKeyScale()
	{
		if (keyScales.size() == 1)
		{
			return keyScales[0].scale;
		}

		UpdateKeyScaleIndex();

		const KeyScale& previousKey = keyScales.at(currentKeyPositionIndex);
		const KeyScale& nextKey = keyScales.at(currentKeyPositionIndex + 1);

		const float scaleFactor = GetScaleFactor(previousKey.timeStamp, nextKey.timeStamp, animationTime);

		Vec3F finalScale = Vec3F::Lerp(previousKey.scale, nextKey.scale, scaleFactor);

		return finalScale;
	}

	QuatF BoneKeyFrames::InterpolateKeyRotation()
	{
		if (keyRotations.size() == 1)
		{
			return keyRotations[0].orientation;
		}

		UpdateKeyRotationIndex();

		const KeyRotation& previousKey = keyRotations.at(currentKeyPositionIndex);
		const KeyRotation& nextKey = keyRotations.at(currentKeyPositionIndex + 1);

		const float scaleFactor = GetScaleFactor(previousKey.timeStamp, nextKey.timeStamp, animationTime);

		QuatF finalRotation = QuatF::Slerp(previousKey.orientation, nextKey.orientation, scaleFactor);

		return finalRotation;
	}
}
