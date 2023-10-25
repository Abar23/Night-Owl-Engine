#include "NightOwlPch.h"

#include "KeyFrames.h"
#include "NightOwl/Core/Utitlity/Utils.h"

namespace NightOwl
{
	KeyFrames::KeyFrames(const aiNodeAnim* assimpAnimationNode)
	{
		FillKeyPositionsData(assimpAnimationNode);
		FillKeyScalesData(assimpAnimationNode);
		FillKeyRotationsData(assimpAnimationNode);
	}

	void KeyFrames::Update(float animationTime)
	{
		this->animationTime = animationTime;

		InterpolateKeyPosition();
		InterpolateKeyScale();
		InterpolateKeyRotation();
	}

	const Vec3F& KeyFrames::GetFinalPosition()
	{
		return finalPosition;
	}

	const Vec3F& KeyFrames::GetFinalScale()
	{
		return finalScale;
	}

	const QuatF& KeyFrames::GetFinalRotation()
	{
		return finalRotation;
	}

	void KeyFrames::FillKeyPositionsData(const aiNodeAnim* assimpAnimationNode)
	{
		for (unsigned int keyPositionIndex = 0; keyPositionIndex < assimpAnimationNode->mNumPositionKeys; ++keyPositionIndex)
		{
			const aiVectorKey positionVectorKey = assimpAnimationNode->mPositionKeys[keyPositionIndex];

			Vec3F position = Utility::AssimpVec3ToNightOwlVec3F(positionVectorKey.mValue);
			float timeStamp = positionVectorKey.mTime;

			keyPositions.emplace_back(position, timeStamp);
		}
	}

	void KeyFrames::FillKeyScalesData(const aiNodeAnim* assimpAnimationNode)
	{
		for (unsigned int keyScaleIndex = 0; keyScaleIndex < assimpAnimationNode->mNumScalingKeys; ++keyScaleIndex)
		{
			const aiVectorKey scaleVectorKey = assimpAnimationNode->mScalingKeys[keyScaleIndex];

			Vec3F scale = Utility::AssimpVec3ToNightOwlVec3F(scaleVectorKey.mValue);
			float timeStamp = scaleVectorKey.mTime;

			keyScales.emplace_back(scale, timeStamp);
		}
	}

	void KeyFrames::FillKeyRotationsData(const aiNodeAnim* assimpAnimationNode)
	{
		for (unsigned int keyRotationIndex = 0; keyRotationIndex < assimpAnimationNode->mNumRotationKeys; ++keyRotationIndex)
		{
			const aiQuatKey quatVectorKey = assimpAnimationNode->mRotationKeys[keyRotationIndex];

			QuatF orientation = Utility::AssimpQuaternionToNightOwlQuatF(quatVectorKey.mValue);
			float timeStamp = quatVectorKey.mTime;

			keyRotations.emplace_back(orientation, timeStamp);
		}
	}

	float KeyFrames::GetScaleFactor(float previousTimeStamp, float nextTimeStamp, float animationTime)
	{
		const float midWayLength = animationTime - previousTimeStamp;
		const float timeDifferenceBetweenFrames = nextTimeStamp - previousTimeStamp;

		return midWayLength / timeDifferenceBetweenFrames;
	}

	int KeyFrames::GetKeyPositionIndex()
	{
		for (unsigned int keyPositionIndex = 0; keyPositionIndex < keyPositions.size() - 1; ++keyPositionIndex)
		{
			if (animationTime < keyPositions[keyPositionIndex + 1].timeStamp)
			{
				return keyPositionIndex;
			}
		}

		return 0;
	}

	int KeyFrames::GetKeyScaleIndex()
	{
		for (unsigned int keyScaleIndex = 0; keyScaleIndex < keyScales.size() - 1; ++keyScaleIndex)
		{
			if (animationTime < keyScales[keyScaleIndex + 1].timeStamp)
			{
				return keyScaleIndex;
			}
		}

		return 0;
	}

	int KeyFrames::GetKeyRotationIndex()
	{
		for (unsigned int keyRotationIndex = 0; keyRotationIndex < keyRotations.size() - 1; ++keyRotationIndex)
		{
			if (animationTime < keyRotations[keyRotationIndex + 1].timeStamp)
			{
				 return keyRotationIndex;
			}
		}

		return 0;
	}

	void KeyFrames::InterpolateKeyPosition()
	{
		if (keyPositions.size() == 1)
		{
			finalPosition = keyPositions[0].position;
			return;
		}

		const int index = GetKeyPositionIndex();

		const KeyPosition& previousKey = keyPositions.at(index);
		const KeyPosition& nextKey = keyPositions.at(index + 1);
		
		const float scaleFactor = GetScaleFactor(previousKey.timeStamp, nextKey.timeStamp, animationTime);

		finalPosition = Vec3F::Lerp(previousKey.position, nextKey.position, scaleFactor);
	}

	void KeyFrames::InterpolateKeyScale()
	{
		if (keyScales.size() == 1)
		{
			finalScale = keyScales[0].scale;
			return;
		}

		const int index = GetKeyScaleIndex();

		const KeyScale& previousKey = keyScales.at(index);
		const KeyScale& nextKey = keyScales.at(index + 1);
		
		const float scaleFactor = GetScaleFactor(previousKey.timeStamp, nextKey.timeStamp, animationTime);

		finalScale = Vec3F::Elerp(previousKey.scale, nextKey.scale, scaleFactor);
	}

	void KeyFrames::InterpolateKeyRotation()
	{
		if (keyRotations.size() == 1)
		{
			finalRotation = keyRotations[0].orientation;
			return;
		}

		const int index = GetKeyRotationIndex();

		const KeyRotation& previousKey = keyRotations.at(index);
		const KeyRotation& nextKey = keyRotations.at(index + 1);

		const float scaleFactor = GetScaleFactor(previousKey.timeStamp, nextKey.timeStamp, animationTime);

		finalRotation = QuatF::Slerp(previousKey.orientation, nextKey.orientation, scaleFactor);
	}
}
