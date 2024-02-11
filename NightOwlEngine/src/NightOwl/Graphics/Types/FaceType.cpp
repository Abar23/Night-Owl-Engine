#include "NightOwlPch.h"

#include "FaceType.h"
#include <glad/glad.h>

namespace NightOwl
{
	int CullFaceTypeToOpenGlType(FaceType type)
	{
		switch (type)
		{
			case FaceType::Front: 
				return GL_FRONT;

			case FaceType::Back: 
				return GL_BACK;

			case FaceType::FrontAndBack: 
				return GL_FRONT_AND_BACK;

			default: 
				return GL_BACK;
		}
	}
}
