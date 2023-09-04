#include "NightOwlPch.h"

#include "AlErrorCheck.h"
#include "Utils.h"
#include "AL/al.h"
#include "Logging/LoggerManager.h"

bool checkOpenAlError(const std::string& filename, const std::uint32_t line)
{
	ALenum error = alGetError();
	if (error != AL_NO_ERROR)
	{
		std::string errorMessage;

		const std::string fileNameStripped = NightOwl::Utility::StripFilePathToName(filename);

		switch (error)
		{
			case AL_INVALID_NAME:
				errorMessage += "AL_INVALID_NAME: a bad name (ID) was passed to an OpenAL function";
				break;

			case AL_INVALID_ENUM:
				errorMessage += "AL_INVALID_ENUM: an invalid enum value was passed to an OpenAL function";
				break;

			case AL_INVALID_VALUE:
				errorMessage += "AL_INVALID_VALUE: an invalid value was passed to an OpenAL function";
				break;

			case AL_INVALID_OPERATION:
				errorMessage += "AL_INVALID_OPERATION: the requested operation is not valid";
				break;

			case AL_OUT_OF_MEMORY:
				errorMessage += "AL_OUT_OF_MEMORY: the requested operation resulted in OpenAL running out of memory";
				break;

			default:
				errorMessage += "UNKNOWN AL ERROR: " + error;
		}

		ENGINE_LOG_ERROR("OpenAL error from file {0}({1}): {2}", fileNameStripped, std::to_string(line), errorMessage);

		return false;
	}
	return true;
}

bool checkOpenAlContextError(const std::string& filename, const std::uint32_t line, ALCdevice* device)
{
	ALCenum error = alcGetError(device);
	if (error != ALC_NO_ERROR)
	{
		std::string errorMessage;

		const std::string fileNameStripped = NightOwl::Utility::StripFilePathToName(filename);

		switch (error)
		{
			case ALC_INVALID_VALUE:
				errorMessage += "ALC_INVALID_VALUE: an invalid value was passed to an OpenAL function";
				break;

			case ALC_INVALID_DEVICE:
				errorMessage += "ALC_INVALID_DEVICE: a bad device was passed to an OpenAL function";
				break;

			case ALC_INVALID_CONTEXT:
				errorMessage += "ALC_INVALID_CONTEXT: a bad context was passed to an OpenAL function";
				break;

			case ALC_INVALID_ENUM:
				errorMessage += "ALC_INVALID_ENUM: an unknown enum value was passed to an OpenAL function";
				break;

			case ALC_OUT_OF_MEMORY:
				errorMessage += "ALC_OUT_OF_MEMORY: an unknown enum value was passed to an OpenAL function";
				break;

			default:
				errorMessage += "UNKNOWN AL ERROR: " + error;
		}

		ENGINE_LOG_ERROR("OpenAL error from file {0}({1}): {2}", fileNameStripped, std::to_string(line), errorMessage);

		return false;
	}
	return true;
}