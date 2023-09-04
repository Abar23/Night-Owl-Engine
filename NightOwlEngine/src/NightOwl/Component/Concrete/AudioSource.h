#pragma once

#include "NightOwl/Audio/AudioClip.h"
#include "NightOwl/Component/Component.h"

namespace NightOwl
{
	class AudioSource : public Component
	{
	public:
		AudioSource();

		std::shared_ptr<Component> Clone() override;

		void Play();

		bool IsPlaying() const;

		bool IsPaused() const;

		void Stop();

		void Mute();

		void UnMute();

		void Pause();

		void UnPause();

		void SetPitch(float pitch);

		float GetPitch() const;

		void SetVolume(float volume);

		float GetVolume() const;

		bool IsVolumeRampEnabled() const;

		void SetVolumeRamp(bool enable);

		void SetFrequencyRate(float frequencyRate);

		float GetFrequencyRate() const;

		void ShouldLoop(bool enable);

		bool IsLooping() const;

		const AudioClip* Clip() const;

		void SetClip(AudioClip* clip);

		//FMOD::Channel* GetChannel();
		
	private:
		AudioClip* clip;

		//FMOD::Channel* channel;

		float pitch;

		float volume;

		float minDistance;

		float maxDistance;

		float dopplerFactor; 

		float frequencyRate;

		bool isLooping;

		bool isPaused;

		bool isMuted;

		bool enableVolumeRamp;

		void Remove() override;
	};

}