#pragma once

#include "Fmod/fmod.hpp"
#include "AudioClip.h"
#include "NightOwl/Component/Concrete/AudioSource.h"
#include "NightOwl/Component/Concrete/AudioListener.h"
#include <vector>

namespace NightOwl
{
	class AudioSystem
	{
	public:
		AudioSystem();

		void Shutdown();

		void Update();

		void AddAudioSource(const AudioSource* audioSource);

		void RemoveAudioSource(const AudioSource*( audioSource);

		void SetAudioListenerVolume(float volume);

		float GetAudioListenerVolume() const;

		void SetAudioListener(const AudioListener* audioListener);

		std::vector<AudioSource*> GetAudioSources();

		void RemoveAudioListener();

		FMOD::Sound* CreateSound(const std::string& filePath, FMOD_MODE mode = FMOD_DEFAULT);

		FMOD::Channel* PlaySound(const AudioClip& audioClip, bool startInPausedState = false);

	private:
		FMOD::System* audioSystem;

		std::vector<AudioSource*> audioSources;

		AudioListener* audioListener;
	};
}
