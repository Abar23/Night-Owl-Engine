#pragma once

namespace NightOwl
{
	enum class ContextCapabilityType
	{
		DepthTest,
		CullFace,
		ColorBlend,
		LineSmooth,
		VertexProgramPointSize,
		DebugOutput
	};

	int ContextCapabilityTypeToOpenGlType(ContextCapabilityType type);
}
