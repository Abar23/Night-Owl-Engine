#include "NightOwlPch.h"

#include "ContextCapabilityType.h"
#include <glad/glad.h>

namespace NightOwl
{
	int ContextCapabilityTypeToOpenGlType(ContextCapabilityType type)
	{
		switch (type) {
			case ContextCapabilityType::DepthTest: 
				return GL_DEPTH_TEST;

			case ContextCapabilityType::CullFace: 
				return GL_CULL_FACE;

			case ContextCapabilityType::ColorBlend: 
				return GL_BLEND;

			case ContextCapabilityType::LineSmooth: 
				return GL_LINE_SMOOTH;

			case ContextCapabilityType::VertexProgramPointSize: 
				return GL_VERTEX_PROGRAM_POINT_SIZE;

			case ContextCapabilityType::DebugOutput:
				return GL_DEBUG_OUTPUT;

			default: 
				return -1;
		}
	}
}
