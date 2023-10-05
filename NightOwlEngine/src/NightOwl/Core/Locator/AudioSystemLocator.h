#pragma once

namespace NightOwl
{
	class AudioSystem;

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
