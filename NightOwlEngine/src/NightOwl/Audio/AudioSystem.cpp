#include "NightOwlPch.h"

#include "AudioSystem.h"
#include "AL/alc.h"
#include "NightOwl/Core/Utitlity/AlErrorCheck.h"
#include <sstream>

namespace NightOwl
{
	AudioSystem::AudioSystem()
		:	audioListener{ nullptr }
	{
		std::stringstream rawDeviceStringNames;
		rawDeviceStringNames << AL_CALL(alcGetString, nullptr, ALC_ALL_DEVICES_SPECIFIER);

		std::string deviceName;
		while(std::getline(rawDeviceStringNames, deviceName, '\0'))
		{
			if(deviceName.length() > 0)
			{
				deviceNames.push_back(deviceName);
			}
		}
	}

	void AudioSystem::Shutdown()
	{

	}

	void AudioSystem::Update()
	{


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
					//FMOD::Channel* audioSourceChannel = audioSource->GetChannel().GetPointer();
					//FMOD_CALL(audioSourceChannel->setVolume, audioSource->GetVolume() * audioListener->GetVolume());	
				}
			}
			audioListener->isVolumeDirty = false;
		}
	}

	void AudioSystem::AddAudioSource(AudioSource* audioSource)
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

	void AudioSystem::SetAudioListener(AudioListener* audioListener)
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

	//FMOD::Sound* AudioSystem::CreateSound(const std::string& filePath, FMOD_MODE mode)
	//{
	//	FMOD::Sound* sound;

	//	FMOD_CALL(audioSystem->createSound, filePath.c_str(), mode, nullptr, &sound);

	//	return sound;
	//}

	//FMOD::Channel* AudioSystem::PlaySound(const AudioClip& audioClip, bool startInPausedState)
	//{
	//	FMOD::Channel* channel;

	//	FMOD_CALL(audioSystem->playSound, audioClip.GetClip(), nullptr, startInPausedState, &channel);

	//	return channel;
	//}
}
