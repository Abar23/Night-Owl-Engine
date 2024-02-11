#include "NightOwlPch.h"
#include "BlendFunctionType.h"

#include "glad/glad.h"

namespace NightOwl
{
	int BlendFunctionTypeToOpenGlType(BlendFunctionType type)
	{
		switch (type) {
			case BlendFunctionType::Zero: 
				return GL_ZERO;

			case BlendFunctionType::One: 
				return GL_ONE;

			case BlendFunctionType::SourceColor: 
				return GL_SRC_COLOR;

			case BlendFunctionType::OneMinusSourceColor: 
				return GL_ONE_MINUS_SRC_ALPHA;

			case BlendFunctionType::DestinationColor: 
				return GL_DST_COLOR;

			case BlendFunctionType::OneMinusDestinationColor: 
				return GL_ONE_MINUS_DST_COLOR;

			case BlendFunctionType::SourceAlpha: 
				return GL_SRC_ALPHA;

			case BlendFunctionType::OneMinusSourceAlpha: 
				return GL_ONE_MINUS_SRC_ALPHA;
				
			case BlendFunctionType::DestinationAlpha: 
				return GL_DST_ALPHA;

			case BlendFunctionType::OneMinusDestinationAlpha: 
				return GL_ONE_MINUS_DST_ALPHA;

			case BlendFunctionType::ConstantColor: 
				return GL_CONSTANT_COLOR;

			case BlendFunctionType::OneMinusConstantColor: 
				return GL_ONE_MINUS_CONSTANT_COLOR;

			case BlendFunctionType::ConstantAlpha: 
				return GL_CONSTANT_ALPHA;

			case BlendFunctionType::OneMinusConstantAlpha: 
				return GL_ONE_MINUS_CONSTANT_ALPHA;

			default: 
				return -1;
		}
	}
}
