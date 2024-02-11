#pragma once

namespace NightOwl
{
	enum class FaceType
	{
		Front,
		Back,
		FrontAndBack
	};

	int CullFaceTypeToOpenGlType(FaceType type);
}
