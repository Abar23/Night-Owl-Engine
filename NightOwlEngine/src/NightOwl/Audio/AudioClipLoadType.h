#pragma once


namespace NightOwl
{
	enum class AudioClipLoadType
	{
		DecompressOnLoad,
		//CompressedInMemory = FMOD_CREATECOMPRESSEDSAMPLE,
		Streaming
	};
}
