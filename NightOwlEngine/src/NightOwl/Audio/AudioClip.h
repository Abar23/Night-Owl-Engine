#pragma once

#include "AudioClipLoadType.h"
#include "AudioSampleType.h"
#include <string>
#include <libsndfile/sndfile.hh>
#include <libsndfile/sndfile.h>

namespace NightOwl
{
	class AudioClip
	{
	public:
		AudioClip();

		AudioClip(const std::string& pathToAudioFile, AudioClipLoadType loadType = AudioClipLoadType::DecompressOnLoad);

		~AudioClip();

		bool IsAmbisonic();

		int GetChannels() const;

		int GetFrequency() const;

		long long GetSamples() const;

		float GetLengthInMilliseconds() const;

		AudioSampleType GetSampleType() const;

		AudioClipLoadType GetLoadType() const;

		const std::string& GetName() const;

	private:
		long long samples;

		float lengthInMilliseconds;

		// Same as sample rate
		int frequency;

		int	channels;

		int	format;

		int byteBlockAlignment;

		bool isAmbisonic;

		unsigned int buffer;

		SNDFILE* audioFileHandle;

		std::string name;

		AudioClipLoadType loadType;

		AudioSampleType sampleType;

		// Should move to separate audio file loader class
		void LoadAudioFile(const std::string& pathToAudioFile);
	};
}
