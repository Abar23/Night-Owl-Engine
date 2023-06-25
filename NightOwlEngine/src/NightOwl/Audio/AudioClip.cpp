#include "NightOwlPch.h"

#include "AudioClip.h"
#include "NightOwl/Core/Locator/AudioSystemLocator.h"
#include "NightOwl/Core/Utitlity/FmodErrorCheck.h"

namespace NightOwl
{
	AudioClip::AudioClip()
		:	sound{ nullptr },
			frequency(0.0f),
			type(FMOD_SOUND_TYPE_UNKNOWN),
			format(FMOD_SOUND_FORMAT_NONE),
			loopType(AudioClipLoopType::Normal),
			loadType(AudioClipLoadType::DecompressOnLoad),
			channels(0),
			bitsPerSample(0),
			priority(0),
			lengthInMilliseconds(0)
	{ }

	AudioClip::~AudioClip()
	{
		FMOD_CALL(sound->release);
	}

	const std::string& AudioClip::GetName() const
	{
		return name;
	}

	const unsigned int AudioClip::GetLengthInMilliseconds() const
	{
		return lengthInMilliseconds;
	}

	const FMOD_SOUND_TYPE AudioClip::GetSoundType() const
	{
		return type;
	}

	const FMOD_SOUND_FORMAT AudioClip::GetSoundFormat() const
	{
		return format;
	}

	const int AudioClip::GetChannels() const
	{
		return channels;
	}

	const int AudioClip::GetBitsPerSample() const
	{
		return bitsPerSample;
	}

	WeakPointer<FMOD::Sound> AudioClip::GetClip() const
	{
		return sound;
	}

	float AudioClip::GetFrequency() const
	{
		return frequency;
	}

	AudioClipLoopType AudioClip::GetLoopType() const
	{
		return loopType;
	}

	void AudioClip::SetLoopType(const AudioClipLoopType loopType)
	{
		FMOD_MODE currentMode;
		FMOD_CALL(sound->getMode, &currentMode);

		currentMode &= ~static_cast<FMOD_MODE>(this->loopType);

		this->loopType = loopType; 

		currentMode |= static_cast<FMOD_MODE>(this->loopType);

		FMOD_CALL(sound->setMode, currentMode);
	}

	AudioClipLoadType AudioClip::GetLoadType() const
	{
		return loadType;
	}

	void AudioClip::SetLoadType(const AudioClipLoadType loadType)
	{
		FMOD_MODE currentMode;
		FMOD_CALL(sound->getMode, &currentMode);

		currentMode &= ~static_cast<FMOD_MODE>(this->loadType);

		this->loadType = loadType; 

		currentMode |= static_cast<FMOD_MODE>(this->loadType);

		FMOD_CALL(sound->setMode, currentMode);
	}

	void AudioClip::SetSound(FMOD::Sound* sound)
	{
		this->sound = sound;

		FMOD_CALL(sound->setMode, FMOD_DEFAULT | static_cast<FMOD_MODE>(loopType) | static_cast<FMOD_MODE>(loadType));

		GetClipInfo();
	}

	void AudioClip::GetClipInfo()
	{
		FMOD_CALL(sound->getFormat, &type, &format, &channels, &bitsPerSample);

		name.resize(MAX_NAME_SIZE);

		FMOD_CALL(sound->getName, name.data(), MAX_NAME_SIZE);

		FMOD_CALL(sound->getLength, &lengthInMilliseconds, FMOD_TIMEUNIT_MS);

		FMOD_CALL(sound->getDefaults, &frequency, &priority);
	}
}
