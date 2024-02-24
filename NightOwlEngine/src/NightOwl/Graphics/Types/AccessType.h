#pragma once

namespace NightOwl
{
	enum class AccessType
	{
		Read,
		Write,
		ReadWrite
	};

	int AccessTypeToOpenGlType(AccessType accessType);
}
