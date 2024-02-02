#include "NightOwlPch.h"

#include "AudioListener.h"
#include "NightOwl/Audio/AudioSystem.h"
#include "NightOwl/Core/Locator/Locator.h"
#include <algorithm>

namespace NightOwl
{
	AudioListener::AudioListener()
		:	Component(ComponentType::AudioListener),
			volume(1.0),
			isPaused(false),
			isVolumeDirty(false),
			isPauseDirty(false)
	{
		AudioSystemLocator::Get()->SetAudioListener(this);
	}

	std::shared_ptr<Component> AudioListener::Clone()
	{
		std::shared_ptr<AudioListener> clone = std::make_shared<AudioListener>();

		clone->volume = volume;
		clone->isPaused = isPaused;
		clone->isVolumeDirty = isVolumeDirty;
		clone->isPauseDirty = isPauseDirty;

		return clone;
	}

	void AudioListener::SetVolume(float volume)
	{
		this->volume = std::clamp(volume, 0.0001f, 1.0f);
		isVolumeDirty = true;
	}

	float AudioListener::GetVolume()
	{
		return volume;
	}

	void AudioListener::Pause()
	{
		isPaused = true;
		isPauseDirty = true;
	}

	void AudioListener::UnPause()
	{
		isPaused = false;
		isPauseDirty = true;
	}

	void AudioListener::Remove()
	{
		AudioSystemLocator::Get()->RemoveAudioListener();
	}
}
