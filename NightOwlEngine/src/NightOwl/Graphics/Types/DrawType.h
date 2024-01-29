#pragma once

namespace NightOwl
{
	enum class DrawType
	{
		None,
		Lines,
		Points,
		Triangles
	};

	int DrawTypeToOpenGlDrawType(DrawType drawType);
}
