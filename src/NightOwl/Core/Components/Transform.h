#pragma once

#include "NightOwl/Math/Math.h"
#include <vector>

namespace NightOwl::Components
{
	class Transform
	{
	public:
		Transform();

		void Rotate(const Math::Vec3F& eulers);

		void Translate(const Math::Vec3F& translation);

		int GetNumberOfChildren();

		Transform GetParent();

		void SetParent(const Transform& transform);

		bool hasChanged();

		void SetChild(const Transform& transform);

		Transform& GetChildAtIndex();

		Math::Vec3F localScale;

		Math::Vec3F localRotation;

		Math::Vec3F localPosition;

	private:
		Math::Mat4F localModelMatrix;

		std::vector<Transform*> children;

		static Transform* root;

		Transform* parent;

		bool isDirty;
	};
}
