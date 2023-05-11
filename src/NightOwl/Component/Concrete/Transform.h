#pragma once

#include "NightOwl/Math/Math.h"
#include "NightOwl/Component/Component.h"
#include "NightOwl/Component/Types/Space.h"
#include "NightOwl/Core/Reflection/Reflection.h"
#include <vector>

namespace NightOwl::Core
{
	class Scene;
}

namespace NightOwl::Component
{
	class Transform : public Component
	{
	public:
		Transform();

		void Scale(float scaleX, float scaleY, float scaleZ, Space space);

		void Scale(const Math::Vec3F& scale, Space space);

		const Math::Vec3F& GetLocalScale();

		void SetLocalScale(float scaleX, float scaleY, float scaleZ);

		void SetLocalScale(const Math::Vec3F& scale);

		void Rotate(float angleX, float angleY, float angleZ, Space space);

		void Rotate(const Math::Vec3F& eulers, Space space);

		const Math::Vec3F GetLocalEulerAngles();

		void SetLocalEulerAngles(float angleX, float angleY, float angleZ);

		void SetLocalEulerAngles(const Math::Vec3F& eulers);

		void Translate(float positionX, float positionY, float positionZ, Space space);

		void Translate(const Math::Vec3F& translation, Space space);

		const Math::Vec3F& GetLocalPosition();

		void SetLocalPosition(float positionX, float positionY, float positionZ);

		void SetLocalPosition(const Math::Vec3F& position);

		int GetNumberOfChildren();

		Transform& GetParent();

		void SetParent(Core::WeakPointer<Transform> parentTransform);

		Core::WeakPointer <Transform> RemoveParent();

		void SetChild(Transform* childTransform);

		bool IsChildOf(Transform* parent);

		const Math::Mat4F& GetLocalModelMatrix(bool overrideDirtyFlag = false);

		const Math::Mat4F& GetWorldMatrix();

		Core::WeakPointer<Transform> GetChildAtIndex(int index) const;

		Math::QuatF GetRotation();

		void SetRotation(const Math::QuatF& newRotation);

		Math::Vec3F GetPosition();

		void SetPosition(const Math::Vec3F& worldPosition);

		bool HasParent() const;

		bool HasChildren() const;

		Math::Vec3F GetForward();

		Math::Vec3F GetRight();

		Math::Vec3F GetUp();

		void Clone(const Transform& transformToClone);

		REFLECT()

	private:
		Math::Mat4F localModelMatrix;

		Math::Mat4F worldMatrix;

		Math::Mat4F parentLocalMatrix;

		Math::Mat4F inverseOfOriginalParentLocalModelMatrix;

		std::vector<Core::WeakPointer<Transform>> children;

		Math::Vec3F localScale;

		Math::QuatF localRotation;

		Math::Vec3F localPosition;

		Math::Vec3F worldScaleOffset;

		Math::QuatF worldRotationOffset;

		Math::Vec3F worldPosition;

		Core::WeakPointer<Transform> root;

		Core::WeakPointer<Transform> parent;

		friend class Core::Scene;

		bool isLocalDirty;

		bool isWorldDirty;

		void SetLocalDirtyFlag();

		void SetWorldDirtyFlag();

		void SetFlagBasedOnSpace(Space space);

		void PropagateParentLocalTransform(const Math::Mat4F& parentLocalTransform);

		void Remove() override;
	};
}
