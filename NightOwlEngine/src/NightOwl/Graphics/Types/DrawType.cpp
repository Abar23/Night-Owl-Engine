#include "NightOwlPch.h"

#include "DrawType.h"
#include <glad/glad.h>

namespace NightOwl
{
	int DrawTypeToOpenGlDrawType(DrawType drawType)
	{
		switch (drawType)
		{
		case DrawType::Lines:
			return GL_LINES;

		case DrawType::Points:
			return GL_POINTS;

		case DrawType::Triangles:
			return GL_TRIANGLES;

		default:
			return GL_TRIANGLES;
		}
	}
}
