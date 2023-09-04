#include "NightOwlPch.h"

#include "AudioSystem.h"
#include "NightOwl/Core/Utitlity/AlErrorCheck.h"
#include <sstream>
#include <AL/al.h>
#include <AL/alc.h>
#include <AL/efx.h>

#include "AudioConstants.h"
#include "NightOwl/Core/Utitlity/Assert.h"

namespace NightOwl
{
	AudioSystem::AudioSystem()
		:	audioListener{ nullptr }
	{
		GetAllDevices();

		InitializeContextWithDefaultDevice();

		CheckForRequiredExtensions();

		InitializeAllChannels();
	}

	void AudioSystem::Shutdown()
	{
		AL_CONTEXT_CALL(alcMakeContextCurrent, audioDevice, nullptr);
		AL_CONTEXT_CALL(alcDestroyContext, audioDevice, context);
		AL_CONTEXT_CALL(alcCloseDevice, audioDevice, audioDevice);
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

	void AudioSystem::CheckForRequiredExtensions()
	{
		ENGINE_ASSERT(alIsExtensionPresent("AL_EXT_FLOAT32"), "OpenAL is missing required extension AL_EXT_FLOAT32!");
		ENGINE_ASSERT(alIsExtensionPresent("AL_EXT_IMA4"), "OpenAL is missing required extension AL_EXT_IMA4!");
		ENGINE_ASSERT(alIsExtensionPresent("AL_SOFT_MSADPCM"), "OpenAL is missing required extension AL_SOFT_MSADPCM!");
		ENGINE_ASSERT(alIsExtensionPresent("AL_SOFT_block_alignment"), "OpenAL is missing required extension AL_SOFT_block_alignment!");
		ENGINE_ASSERT(alIsExtensionPresent("AL_SOFT_source_spatialize"), "OpenAL is missing required extension AL_SOFT_source_spatialize!");
		ENGINE_ASSERT(alcIsExtensionPresent(audioDevice, "ALC_SOFT_HRTF"), "OpenAL is missing required extension AL_SOFT_source_spatialize!");
	}

	void AudioSystem::GetAllDevices()
	{
		const char* allDevices = alcGetString(nullptr, ALC_ALL_DEVICES_SPECIFIER);

		ENGINE_ASSERT(allDevices != nullptr, "OpenAL could not enumerate audio devices!");

		const char* device = allDevices;
		const char* nextDevice = allDevices + 1;

		while (device && *device != '\0' && nextDevice && *nextDevice != '\0')
		{
			const size_t length = strlen(device);
			validDeviceNames.emplace_back(device);
			device += length + 1;
			nextDevice += length + 2;
		}
	}

	void AudioSystem::InitializeContextWithDefaultDevice()
	{
		audioDevice = AL_CONTEXT_CALL(alcOpenDevice, audioDevice, validDeviceNames[0].c_str());

		ENGINE_ASSERT( audioDevice != nullptr, "OpenAL could not open default device {0}!", validDeviceNames[0]);

		context = AL_CONTEXT_CALL(alcCreateContext, audioDevice, audioDevice, nullptr);

		ENGINE_ASSERT(audioDevice != nullptr, "OpenAL failed to create context with device {0}!", validDeviceNames[0]);

		AL_CONTEXT_CALL(alcMakeContextCurrent, audioDevice, context);
	}

	void AudioSystem::InitializeAllChannels()
	{
		// channels.resize(MAX_CHANNELS);
		//
		// constexpr unsigned int invalidChannelId = std::numeric_limits<unsigned int>::max();
		//
		// for (int channelIndex = 0; channelIndex < MAX_CHANNELS; ++channelIndex)
		// {
		// 	unsigned int channelId = invalidChannelId;
		// 	AL_CALL(alGenSources, 1, &channelId);
		//
		// 	if(channelId == invalidChannelId)
		// 	{
		// 		ENGINE_LOG_ERROR("OpenAL failed to load all 256 channels!");
		//
		// 		break;
		// 	}
		//
		// 	channels.emplace_back(channelId);
		// }
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
