#pragma once

#include "NightOwl/Component/Structures/DebugPointVertex.h"
#include "NightOwl/Graphics/Interfaces/IVertexArrayObject.h"
#include "NightOwl/Graphics/Interfaces/IVertexBuffer.h"
#include "NightOwl/Graphics/Materials/Material.h"
#include "NightOwl/Math/Vec3.h"
#include <vector>


namespace NightOwl
{
	class IContext;

	class DebugSystem
	{
	public:
		void Init();

		void Update();

		void DrawLine(const Vec3F& start, const Vec3F& end);

		void DrawPoint(const Vec3F& point, const Vec3F& color = { 0.0f, 1.0f, 0.0 }); // default color is green, TODO: make color constants

	private:
		std::shared_ptr<Material> debugLineMaterial;

		std::shared_ptr<Material> debugPointMaterial;

		// change pair to line debug struct with start, end, and color
		std::vector<std::pair<Vec3F, Vec3F>> lineSegments;

		std::shared_ptr<IVertexArrayObject> lineVertexArrayObject;

		std::shared_ptr<IVertexBuffer> lineVertexBuffer;

		std::vector<DebugPointVertex> points;

		std::shared_ptr<IVertexArrayObject> pointVertexArrayObject;

		std::shared_ptr<IVertexBuffer> pointVertexBuffer;

		void SetupLineGraphics();

		void SetupPointGraphics();

		void DrawLines(IContext* renderContext);

		void DrawPoints(IContext* renderContext);
	};
}
