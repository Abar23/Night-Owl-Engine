#include "NightOwlPch.h"

#include "AccessType.h"
#include <glad/glad.h>

namespace NightOwl
{
	int AccessTypeToOpenGlType(AccessType accessType)
	{
		switch (accessType)
		{
			case AccessType::Read: 
				return GL_READ_ONLY;

			case AccessType::Write: 
				return GL_WRITE_ONLY;

			case AccessType::ReadWrite: 
				return GL_READ_WRITE;
		}

		return GL_READ_ONLY;
	}
}
