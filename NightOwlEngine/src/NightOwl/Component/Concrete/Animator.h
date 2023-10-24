#pragma once

#include "NightOwl/Animation/3D/Structures/AnimationCollection.h"
#include "NightOwl/Animation/Types/AnimatorUpdateMode.h"
#include "NightOwl/Component/Component.h"

namespace NightOwl
{
	class Transform;

	class Animator : public Component
	{
	public:
		Animator();

		std::shared_ptr<Component> Clone() override;

		void Update();

		void Play();

		void Pause();

		void Stop();

		void Reset();

		bool IsPlaying();

		void AddMotion(Motion* motion);

		void SetCurrentMotion(const std::string& motionName);

		AnimatorUpdateMode GetUpdateMode();

		void SetUpdateMode(AnimatorUpdateMode updateMode);

		void SetSkeleton(Transform* skeleton);

		Transform* GetSkeleton() const;

		Motion* GetCurrentMotion() const;

		float GetFloat(const std::string& parameterName);

		void SetFloat(const std::string& parameterName, float value);

	private:
		bool isPlaying;

		float deltaTime;

		AnimatorUpdateMode updateMode;

		Motion* currentMotion;

		Transform* skeleton;

		AnimationCollection animationCollection;

		std::map<std::string, float> floatParameterMap;

		void Remove() override;

		friend class Animation;

		friend class BlendTree;
	};
}
