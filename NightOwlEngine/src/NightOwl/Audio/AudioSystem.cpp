#include "NightOwlPch.h"

#include "AudioSystem.h"
#include "Fmod/fmod_errors.h"

namespace NightOwl
{
	AudioSystem::AudioSystem()
		:	audioListener{ nullptr }
	{
		FMOD_CALL(FMOD::System_Create, &audioSystem);

		FMOD_CALL(audioSystem->init, 512, FMOD_INIT_NORMAL, nullptr);
	}

	void AudioSystem::Shutdown()
	{
		FMOD_CALL(audioSystem->release);
	}

	void AudioSystem::Update()
	{
		FMOD_CALL(audioSystem->update);

		if(audioListener == nullptr)
		{
			return;
		}

		if(audioListener->isPauseDirty)
		{
			for (const auto& audioSource : audioSources)
			{
				audioListener->isPaused ? audioSource->Pause() : audioSource->UnPause();
			}
			audioListener->isPauseDirty = true;
		}

		if(audioListener->isVolumeDirty)
		{
			for (const auto& audioSource : audioSources)
			{
				if(audioSource->IsPlaying())
				{
					FMOD::Channel* audioSourceChannel = audioSource->GetChannel().GetPointer();
					FMOD_CALL(audioSourceChannel->setVolume, audioSource->GetVolume() * audioListener->GetVolume());	
				}
			}
			audioListener->isVolumeDirty = false;
		}
	}

	void AudioSystem::AddAudioSource(const AudioSource* audioSource)
	{
		audioSources.push_back(audioSource);
	}

	void AudioSystem::RemoveAudioSource(const AudioSource* audioSource)
	{
		int audioSourceIndex = 0;
		for (const auto renderer : audioSources)
		{
			if (renderer == audioSource)
			{
				break;
			}
			audioSourceIndex++;
		}

		if (audioSourceIndex != audioSources.size())
		{
			audioSources.erase(audioSources.begin() + audioSourceIndex);
		}
	}

	void AudioSystem::SetAudioListenerVolume(float volume)
	{
		audioListener->SetVolume(volume);
	}

	float AudioSystem::GetAudioListenerVolume() const
	{
		return audioListener ? audioListener->GetVolume() : 0.0f;
	}

	void AudioSystem::SetAudioListener(const AudioListener* audioListener)
	{
		this->audioListener = audioListener;
	}

	std::vector<AudioSource*> AudioSystem::GetAudioSources()
	{
		return audioSources;
	}

	void AudioSystem::RemoveAudioListener()
	{
		audioListener = nullptr;
	}

	FMOD::Sound* AudioSystem::CreateSound(const std::string& filePath, FMOD_MODE mode)
	{
		FMOD::Sound* sound;

		FMOD_CALL(audioSystem->createSound, filePath.c_str(), mode, nullptr, &sound);

		return sound;
	}

	FMOD::Channel* AudioSystem::PlaySound(const AudioClip& audioClip, bool startInPausedState)
	{
		FMOD::Channel* channel;

		FMOD_CALL(audioSystem->playSound, audioClip.GetClip().GetPointer(), nullptr, startInPausedState, &channel);

		return channel;
	}
}
