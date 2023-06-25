#pragma once

#include "NightOwl/Component/Component.h"

namespace NightOwl
{
	class AudioSystem;

	class AudioListener : public Component
	{
	public:
		AudioListener();

		std::shared_ptr<Component> Clone() override;

		void SetVolume(float volume);

		float GetVolume();

		void Pause();

		void UnPause();

	private:
		float volume;

		bool isPaused;

		bool isVolumeDirty;

		bool isPauseDirty;

		friend class AudioSystem;

		void Remove() override;
	};

}