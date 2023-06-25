#include "NightOwlPch.h"

#include "AudioSource.h"
#include "NightOwl/Core/Utitlity/FmodErrorCheck.h"
#include "NightOwl/Core/Locator/AudioSystemLocator.h"
#include <algorithm>

namespace NightOwl
{
	AudioSource::AudioSource()
		:	Component(ComponentType::AudioSource),
			clip{ nullptr },
			channel{ nullptr },
			pitch(1.0f),
			volume(1.0),
			frequencyRate(0.0f),
			isLooping(false),
			isPaused(false),
			isMuted(false),
			enableVolumeRamp(false)
	{
		AudioSystemLocator::GetAudioSystem()->AddAudioSource(this);
	}

	std::shared_ptr<Component> AudioSource::Clone()
	{
		std::shared_ptr<AudioSource> clone = std::make_shared<AudioSource>();

		clone->clip = clip;
		clone->channel = channel;
		clone->pitch = pitch;
		clone->volume = volume;
		clone->frequencyRate = frequencyRate;
		clone->isLooping = isLooping;
		clone->isPaused = isPaused;
		clone->isMuted = isMuted;
		clone->enableVolumeRamp = enableVolumeRamp;

		return clone;
	}

	void AudioSource::Play()
	{
		if(clip != nullptr)
		{
			Stop();

			AudioSystem* audioSystem = AudioSystemLocator::GetAudioSystem();

			const float audioListenerVolume = audioSystem->GetAudioListenerVolume();

			channel = audioSystem->PlaySound(*clip);
			FMOD_CALL(channel->setPitch, pitch);
			FMOD_CALL(channel->setVolume, volume * audioListenerVolume);
			FMOD_CALL(channel->setVolumeRamp, enableVolumeRamp);
			FMOD_CALL(channel->setFrequency, frequencyRate);
			FMOD_CALL(channel->setLoopCount, isLooping ? -1 : 0);
		}
	}

	bool AudioSource::IsPlaying() const
	{
		bool isPlaying = false;

		if(channel == nullptr)
		{
			return isPlaying;
		}

		FMOD_CALL(channel->isPlaying, &isPlaying);

		return isPlaying;
	}

	bool AudioSource::IsPaused() const
	{
		bool isPaused = false;
		
		if(IsPlaying())
		{
			FMOD_CALL(channel->getPaused, &isPaused);
		}

		return isPaused;
	}

	void AudioSource::Stop()
	{
		if(IsPlaying())
		{
			FMOD_CALL(channel->stop);
		}
	}

	void AudioSource::Mute()
	{
		isMuted = true;

		if(IsPlaying())
		{
			FMOD_CALL(channel->setMute, isMuted);
		}
	}

	void AudioSource::UnMute()
	{
		isMuted = false;

		if(IsPlaying())
		{
			FMOD_CALL(channel->setMute, isMuted);
		}
	}

	void AudioSource::Pause()
	{
		isPaused = true;

		if(IsPlaying())
		{
			FMOD_CALL(channel->setPaused, isPaused);
		}
	}

	void AudioSource::UnPause()
	{
		isPaused = false;

		if(IsPlaying())
		{
			FMOD_CALL(channel->setPaused, isPaused);
		}
	}

	void AudioSource::SetPitch(float pitch)
	{
		this->pitch = pitch;

		if(IsPlaying())
		{
			FMOD_CALL(channel->setPitch, this->pitch);
		}
	}

	float AudioSource::GetPitch() const
	{
		return this->pitch;
	}

	void AudioSource::SetVolume(float volume)
	{
		this->volume = std::clamp(volume, 0.0f, 1.0f);

		if(IsPlaying())
		{
			const float audioListenerVolume = AudioSystemLocator::GetAudioSystem()->GetAudioListenerVolume();

			FMOD_CALL(channel->setVolume, this->volume * audioListenerVolume);
		}
	}

	float AudioSource::GetVolume() const
	{
		return volume;
	}

	bool AudioSource::IsVolumeRampEnabled() const
	{
		return enableVolumeRamp;
	}

	void AudioSource::SetVolumeRamp(bool enable)
	{
		enableVolumeRamp = enable;

		if(IsPlaying())
		{
			FMOD_CALL(channel->setVolumeRamp, enableVolumeRamp);
		}
	}

	void AudioSource::SetFrequencyRate(float frequencyRate)
	{
		this->frequencyRate = frequencyRate;

		if(IsPlaying())
		{
			FMOD_CALL(channel->setFrequency, this->frequencyRate);
		}
	}

	float AudioSource::GetFrequencyRate() const
	{
		return frequencyRate;
	}

	void AudioSource::ShouldLoop(bool enable)
	{
		if(enable == isLooping)
		{
			return;
		}

		isLooping = enable;

		if(IsPlaying())
		{
			FMOD_CALL(channel->setLoopCount, isLooping ? -1 : 0);
		}
	}

	bool AudioSource::IsLooping() const
	{
		return isLooping;
	}

	const AudioClip* AudioSource::Clip() const
	{
		return clip;
	}

	void AudioSource::SetClip(AudioClip* clip)
	{
		this->clip = clip;

		frequencyRate = clip->GetFrequency();
	}

	FMOD::Channel* AudioSource::GetChannel()
	{
		return channel;
	}

	void AudioSource::Remove()
	{
		AudioSystemLocator::GetAudioSystem()->RemoveAudioSource(this);
	}
}
