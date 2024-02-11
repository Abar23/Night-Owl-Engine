#pragma once

namespace NightOwl
{
	enum class BlendFunctionType
	{
		Zero,
		One,
		SourceColor,
		OneMinusSourceColor,
		DestinationColor,
		OneMinusDestinationColor,
		SourceAlpha,
		OneMinusSourceAlpha,
		DestinationAlpha,
		OneMinusDestinationAlpha,
		ConstantColor,
		OneMinusConstantColor,
		ConstantAlpha,
		OneMinusConstantAlpha
	};

	int BlendFunctionTypeToOpenGlType(BlendFunctionType type);
}
