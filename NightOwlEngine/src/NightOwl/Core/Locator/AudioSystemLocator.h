#pragma once

#include "NightOwl/Audio/AudioSystem.h"

namespace NightOwl
{
	class AudioSystemLocator
	{
	public:
		static AudioSystem* GetAudioSystem()
		{
			return audioSystem;
		}

		static void Provide(AudioSystem* audioSystemInstance)
		{
			audioSystem = audioSystemInstance;
		}

	private:
		inline static AudioSystem* audioSystem{ nullptr };
	};
}
