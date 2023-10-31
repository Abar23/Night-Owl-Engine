#pragma once

#include <vector>

#include "NightOwl/Graphics/Interfaces/IShader.h"
#include "NightOwl/Graphics/Interfaces/IVertexArrayObject.h"
#include "NightOwl/Graphics/Interfaces/IVertexBuffer.h"
#include "NightOwl/Graphics/Materials/Material.h"
#include "NightOwl/Math/Vec3.h"

namespace NightOwl
{
	class DebugSystem
	{
	public:
		void Init();

		void Update();

		void DrawLine(const Vec3F& start, const Vec3F& end);

	private:
		std::shared_ptr<Material> debugMaterial;

		// change pair to line debug struct with start, end, and color
		std::vector<std::pair<Vec3F, Vec3F>> lineSegments;

		std::shared_ptr<IVertexArrayObject> vertexArrayObject;

		std::shared_ptr<IVertexBuffer> vertexBuffer;
	};
}
