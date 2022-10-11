#pragma once

#include "Component.h"
#include "NightOwl/Math/Math.h"
#include "NightOwl/Components/Types/Space.h"
#include <vector>

namespace NightOwl::Components
{
	class Transform : public Component
	{
	public:
		Transform(GameObjects::GameObject* gameObject);

		void Scale(float scaleX, float scaleY, float scaleZ, Space space);

		void Scale(const Math::Vec3F& scale, Space space);

		const Math::Vec3F& GetLocalScale();

		void SetLocalScale(float scaleX, float scaleY, float scaleZ);

		void SetLocalScale(const Math::Vec3F& scale);

		void Rotate(float angleX, float angleY, float angleZ, Space space);

		void Rotate(const Math::Vec3F& eulers, Space space);

		const Math::Vec3F& GetLocalEulerAngles();

		void SetLocalEulerAngles(float angleX, float angleY, float angleZ);

		void SetLocalEulerAngles(const Math::Vec3F& eulers);

		void Translate(float positionX, float positionY, float positionZ, Space space);

		void Translate(const Math::Vec3F& translation, Space space);

		const Math::Vec3F& GetLocalPosition();

		void SetLocalPosition(float positionX, float positionY, float positionZ);

		void SetLocalPosition(const Math::Vec3F& position);

		int GetNumberOfChildren();

		const Transform& GetParent();

		void SetParent(Transform* transform);

		void SetChild(Transform* transform);

		const Math::Mat4F& GetLocalModelMatrix();

		const Math::Mat4F& GetWorldMatrix();

		Transform* GetChildAtIndex(int index) const;

	private:
		Math::Mat4F localModelMatrix;

		std::vector<Transform*> children;

		Math::Vec3F localScale;

		Math::Vec3F localEulerAngles;

		Math::Vec3F localPosition;

		Math::Mat4F worldMatrix;

		Math::Vec3F worldScale;

		Math::Vec3F worldEulerAngles;

		Math::Vec3F worldPosition;

		Transform* root;

		Transform* parent;

		bool isLocalDirty;

		bool isWorldDirty;

		void SetLocalDirtyFlag();

		void SetWorldDirtyFlag();
	};
}
