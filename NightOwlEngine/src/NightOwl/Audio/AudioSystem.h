#pragma once

#include "AudioClip.h"
#include "NightOwl/Component/Concrete/AudioSource.h"
#include "NightOwl/Component/Concrete/AudioListener.h"
#include <AL/alc.h>
#include <vector>

#include "Structures/Channel.h"

namespace NightOwl
{
	class AudioSystem
	{
	public:
		AudioSystem();

		void Shutdown();

		void Update();

		void AddAudioSource(AudioSource* audioSource);

		void RemoveAudioSource(const AudioSource* audioSource);

		void SetAudioListenerVolume(float volume);

		float GetAudioListenerVolume() const;

		void SetAudioListener(AudioListener* audioListener);

		std::vector<AudioSource*> GetAudioSources();

		void RemoveAudioListener();

	private:
		ALCcontext* context{};

		ALCdevice* audioDevice{};

		std::vector<Channel> channels;

		std::vector<AudioSource*> audioSources;

		// Need to write logic for this!! Need to make audio sources are also removed from this vector
		std::vector<AudioSource*> audioSourcesRequestingToPlay;

		std::vector<std::string> validDeviceNames;

		AudioListener* audioListener;

		void CheckForRequiredExtensions();

		void GetAllDevices();

		void InitializeContextWithDefaultDevice();

		void InitializeAllChannels();
	};
}
