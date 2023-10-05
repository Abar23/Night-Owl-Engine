#include "NightOwlPch.h"

#include "AudioClip.h"
#include "NightOwl/Core/Utitlity/Assert.h"
#include "NightOwl/Core/Utitlity/AlErrorCheck.h"
#include <AL/al.h>
#include <AL/alext.h>

namespace NightOwl
{
	AudioClip::AudioClip()
		:	samples(0),
			lengthInMilliseconds(0),
			frequency(0.0f),
			channels(0),
			format(0),
			byteBlockAlignment(0),
			isAmbisonic(false),
			audioFileHandle(nullptr),
			loadType(AudioClipLoadType::DecompressOnLoad),
			sampleType(AudioSampleType::None)
	{
		AL_CALL(alGenBuffers, 1, &buffer);
	}

	AudioClip::AudioClip(const std::string& pathToAudioFile, AudioClipLoadType loadType)
		:	samples(0),
			lengthInMilliseconds(0),
			frequency(0.0f),
			channels(0),
			format(0),
			byteBlockAlignment(0),
			isAmbisonic(false),
			buffer(0),
			audioFileHandle(nullptr),
			loadType(loadType),
			sampleType(AudioSampleType::None)
	{
		AL_CALL(alGenBuffers, 1, &buffer);

		LoadAudioFile(pathToAudioFile);
	}

	AudioClip::~AudioClip()
	{
		AL_CALL(alDeleteBuffers, 1, &buffer);
	}

	bool AudioClip::IsAmbisonic()
	{
		return isAmbisonic;
	}

	const std::string& AudioClip::GetName() const
	{
		return name;
	}

	float AudioClip::GetLengthInMilliseconds() const
	{
		return lengthInMilliseconds;
	}

	AudioSampleType AudioClip::GetSampleType() const
	{
		return sampleType;
	}

	int AudioClip::GetChannels() const
	{
		return channels;
	}

	long long AudioClip::GetSamples() const
	{
		return samples;
	}

	int AudioClip::GetFrequency() const
	{
		return frequency;
	}

	AudioClipLoadType AudioClip::GetLoadType() const
	{
		return loadType;
	}

	void AudioClip::LoadAudioFile(const std::string& pathToAudioFile)
	{
		SF_INFO audioFileInfo;
		audioFileHandle = sf_open(pathToAudioFile.c_str(), SFM_READ, &audioFileInfo);

		ENGINE_ASSERT(audioFileHandle != nullptr, "Failed to open audio file {0}! Reported error is {1}", pathToAudioFile, sf_strerror(NULL));

		sampleType = AudioSampleType::None;
		samples = audioFileInfo.frames;
		frequency = audioFileInfo.samplerate;
		channels = audioFileInfo.channels;
		format = audioFileInfo.format;
		lengthInMilliseconds = static_cast<float>(samples) / static_cast<float>(frequency) * 1000.0f;

		switch (format & SF_FORMAT_SUBMASK)
		{
			case SF_FORMAT_ALAC_16:
			case SF_FORMAT_PCM_16:
				sampleType = AudioSampleType::Short;
				break;

			case SF_FORMAT_PCM_24:
			case SF_FORMAT_PCM_32:
			case SF_FORMAT_FLOAT:
			case SF_FORMAT_DOUBLE:
			case SF_FORMAT_VORBIS:
			case SF_FORMAT_OPUS:
			case SF_FORMAT_ALAC_20:
			case SF_FORMAT_ALAC_24:
			case SF_FORMAT_ALAC_32:
					sampleType = AudioSampleType::Float;
				break;

			default:
				ENGINE_LOG_ERROR("Audio format for file {0} not supported!", pathToAudioFile);
				std::terminate();
		}

		if (sampleType == AudioSampleType::Short)
		{
			this->byteBlockAlignment = channels * 2;
		}
		else if (sampleType == AudioSampleType::Float)
		{
			this->byteBlockAlignment = channels * 4;
		}

		format = AL_NONE;
		if (channels == 1)
		{
			if (sampleType == AudioSampleType::Short)
			{
				format = AL_FORMAT_MONO16;
			}
			if (sampleType == AudioSampleType::Float)
			{
				format = AL_FORMAT_MONO_FLOAT32;
			}
		}
		else if (channels == 2)
		{
			if (sampleType == AudioSampleType::Short)
			{
				format = AL_FORMAT_STEREO16;
			}
			else if (sampleType == AudioSampleType::Float)
			{
				format = AL_FORMAT_STEREO_FLOAT32;
			}
		}
		else if (channels == 3)
		{
			if (sf_command(audioFileHandle, SFC_WAVEX_GET_AMBISONIC, nullptr, 0) == SF_AMBISONIC_B_FORMAT)
			{
				if (sampleType == AudioSampleType::Short)
				{
					format = AL_FORMAT_BFORMAT2D_16;
				}
				else if (sampleType == AudioSampleType::Float)
				{
					format = AL_FORMAT_BFORMAT2D_FLOAT32;
				}
			}
		}
		else if (channels == 4)
		{
			if (sf_command(audioFileHandle, SFC_WAVEX_GET_AMBISONIC, nullptr, 0) == SF_AMBISONIC_B_FORMAT)
			{
				if (sampleType == AudioSampleType::Short)
				{
					format = AL_FORMAT_BFORMAT3D_16;
				}
				else if (sampleType == AudioSampleType::Float)
				{
					format = AL_FORMAT_BFORMAT3D_FLOAT32;
				}
			}
		}

		if (format == AL_NONE)
		{
			sf_close(audioFileHandle);
			ENGINE_LOG_ERROR("Audio file has unsupported number of channels {0}!", channels);
			std::terminate();
		}

		if (loadType == AudioClipLoadType::Streaming)
		{
			return;
		}

		// load all audio data into OpenAL buffer
		std::vector<unsigned char> audioDataBuffer(samples * this->byteBlockAlignment);
		unsigned long long numberOfFramesRead = 0;

		if (sampleType == AudioSampleType::Short)
		{
			numberOfFramesRead = sf_readf_short(audioFileHandle, reinterpret_cast<short*>(audioDataBuffer.data()), samples);
		}
		else if (sampleType == AudioSampleType::Float)
		{
			numberOfFramesRead = sf_readf_float(audioFileHandle, reinterpret_cast<float*>(audioDataBuffer.data()), samples);
		}

		if (numberOfFramesRead < 1 || numberOfFramesRead != samples)
		{
			audioDataBuffer.clear();

			ENGINE_LOG_ERROR("Failed to read samples for audio file {0}!", pathToAudioFile);

			const int successfullyCloseAudioFile = sf_close(audioFileHandle);
			if (successfullyCloseAudioFile > 0)
			{
				ENGINE_LOG_ERROR("Failed to close audio file {0}!", pathToAudioFile);
			}
			std::terminate();
		}
		
		AL_CALL(alBufferData, buffer, format, audioDataBuffer.data(), audioDataBuffer.size(), frequency);

		audioDataBuffer.clear();
		const int successfullyCloseAudioFile = sf_close(audioFileHandle);
		if (successfullyCloseAudioFile > 0)
		{
			ENGINE_LOG_ERROR("Failed to close audio file {0}!", pathToAudioFile);
			std::terminate();
		}
	}
}
