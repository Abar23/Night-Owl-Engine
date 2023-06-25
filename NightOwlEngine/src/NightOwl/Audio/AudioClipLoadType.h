#pragma once

#include "Fmod/fmod.hpp"

namespace NightOwl
{
	enum class AudioClipLoadType
	{
		DecompressOnLoad = FMOD_CREATESAMPLE,
		CompressedInMemory = FMOD_CREATECOMPRESSEDSAMPLE,
		Streaming = FMOD_CREATESTREAM
	};
}
