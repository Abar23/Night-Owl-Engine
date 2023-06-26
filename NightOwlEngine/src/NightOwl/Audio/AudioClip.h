#pragma once

#include <string>

#include "AudioClipLoadType.h"
#include "AudioClipLoopType.h"

namespace NightOwl
{
	class AudioClip
	{
	public:
		AudioClip();

		~AudioClip();

		const std::string& GetName() const;

		const unsigned int GetLengthInMilliseconds() const;

		//const FMOD_SOUND_TYPE GetSoundType() const;

		//const FMOD_SOUND_FORMAT GetSoundFormat() const;

		const int GetChannels() const;

		const int GetBitsPerSample() const;

		//FMOD::Sound* GetClip() const;

		float GetFrequency() const;

		AudioClipLoopType GetLoopType() const;

		void SetLoopType(AudioClipLoopType loopType);

		AudioClipLoadType GetLoadType() const;

		void SetLoadType(AudioClipLoadType loadType);

		//void SetSound(FMOD::Sound* sound);

	private:
		//FMOD::Sound* sound;

		std::string name;

		float frequency;

		//FMOD_SOUND_TYPE type;

		//FMOD_SOUND_FORMAT format;

		AudioClipLoopType loopType;

		AudioClipLoadType loadType;

		int channels;

		int bitsPerSample;

		int priority;

		unsigned int lengthInMilliseconds;

		static constexpr int MAX_NAME_SIZE = 256;

		void GetClipInfo();
	};
}
