#pragma once

#include "NightOwl/Math/Math.h"
#include "NightOwl/Component/Component.h"
#include "NightOwl/Component/Types/Space.h"
#include <vector>

namespace NightOwl::Component
{
	class Transform : public Component
	{
	public:
		Transform(GameObject::GameObject& gameObject);

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

		void SetParent(Transform& parentTransform);

		void SetChild(Transform& childTransform);

		const Math::Mat4F& GetLocalModelMatrix();

		const Math::Mat4F& GetWorldMatrix();

		Transform* GetChildAtIndex(int index) const;

		Math::Vec3F GetWorldScale() const;

		void SetWorldScale(const Math::Vec3F& worldScale);

		Math::Vec3F GetWorldEulerAngles() const;

		void SetWorldEulerAngles(const Math::Vec3F& worldEulerAngles);

		Math::Vec3F GetWorldPosition() const;

		void SetWorldPosition(const Math::Vec3F& worldPosition);

	private:
		Math::Mat4F localModelMatrix;

		Math::Mat4F worldMatrix;

		Math::Mat4F parentWorldMatrix;

		std::vector<Transform*> children;

		Math::Vec3F localScale;

		Math::Vec3F localEulerAngles;

		Math::Vec3F localPosition;

		Math::Vec3F worldScale;

		Math::Vec3F worldEulerAngles;

		Math::Vec3F worldPosition;

		Transform* root;

		Transform* parent;

		bool isLocalDirty;

		bool isWorldDirty;

		void SetLocalDirtyFlag();

		void SetWorldDirtyFlag();

		void SetFlagBasedOnSpace(Space space);

		void PropagateParentWorldTransform(const Math::Mat4F& parentWorldTransform);

		void RestrictEulerAngles(Math::Vec3F& eulerAngles);
	};
}