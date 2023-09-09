#pragma once

#include "NightOwl/Math/Math.h"
#include "NightOwl/Component/Component.h"
#include "NightOwl/Component/Types/Space.h"
#include "NightOwl/Core/Reflection/Reflection.h"
#include <vector>

namespace NightOwl
{
	class Scene;

	class Transform : public Component
	{
	public:
		Transform();

		void Scale(float scaleX, float scaleY, float scaleZ, Space space);

		void Scale(const Vec3F& scale, Space space);

		const Vec3F& GetLocalScale();

		void SetLocalScale(float scaleX, float scaleY, float scaleZ);

		void SetLocalScale(const Vec3F& scale);

		void Rotate(float angleX, float angleY, float angleZ, Space space);

		void Rotate(const Vec3F& eulers, Space space);

		const Vec3F GetLocalEulerAngles();

		void SetLocalRotation(const QuatF& rotation);

		void SetLocalEulerAngles(float angleX, float angleY, float angleZ);

		void SetLocalEulerAngles(const Vec3F& eulers);

		void Translate(float positionX, float positionY, float positionZ, Space space);

		void Translate(const Vec3F& translation, Space space);

		const Vec3F& GetLocalPosition();

		void SetLocalPosition(float positionX, float positionY, float positionZ);

		void SetLocalPosition(const Vec3F& position);

		int GetNumberOfChildren();

		Transform& GetParent();

		void SetParent(Transform* parentTransform);

		Transform* RemoveParent();

		void SetChild(Transform* childTransform);

		bool IsChildOf(Transform* parent);

		const Mat4F& GetLocalModelMatrix(bool overrideDirtyFlag = false);

		const Mat4F& GetWorldMatrix();

		Transform* GetChildAtIndex(int index) const;

		QuatF GetRotation();

		void SetRotation(const QuatF& newRotation);

		Vec3F GetPosition();

		void SetPosition(const Vec3F& worldPosition);

		bool HasParent() const;

		bool HasChildren() const;

		Vec3F GetForward();

		Vec3F GetRight();

		Vec3F GetUp();

		void Clone(const Transform& transformToClone, Scene* currentScene = nullptr);	

		REFLECT()

	private:
		Mat4F localModelMatrix;

		Mat4F worldMatrix;

		Mat4F parentLocalMatrix;

		Mat4F inverseOfOriginalParentLocalModelMatrix;

		std::vector<Transform*> children;

		Vec3F localScale;

		QuatF localRotation;

		Vec3F localPosition;

		Vec3F worldScaleOffset;

		QuatF worldRotationOffset;

		Vec3F worldPosition;

		Transform* root;

		Transform* parent;

		friend class Scene;

		bool isLocalDirty;

		bool isWorldDirty;

		void SetLocalDirtyFlag();

		void SetWorldDirtyFlag();

		void SetFlagBasedOnSpace(Space space);

		void PropagateParentLocalTransform(const Mat4F& parentLocalTransform);

		void Remove() override;
	};
}
