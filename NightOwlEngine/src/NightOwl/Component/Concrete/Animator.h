#pragma once

#include "Transform.h"
#include "NightOwl/Animation/3D/Structures/AnimationCollection.h"
#include "NightOwl/Animation/Types/AnimatorUpdateMode.h"
#include "NightOwl/Component/Component.h"

namespace NightOwl
{
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

		void AddAnimation(Animation* animation);

		void SetCurrentAnimation(const std::string& animationName);

		AnimatorUpdateMode GetUpdateMode();

		void SetUpdateMode(AnimatorUpdateMode updateMode);

		void SetSkeleton(Transform* skeleton);

		Transform* GetSkeleton() const;

		Animation* GetCurrentAnimation() const;

	private:
		AnimatorUpdateMode updateMode;

		float elapsedTime;

		bool isPlaying;

		Animation* currentAnimation;

		Transform* skeleton;

		AnimationCollection animationCollection;

		void Remove() override;
	};
}
