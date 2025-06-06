#pragma once

#include "NightOwl/Component/Component.h"
#include "NightOwl/Component/Types/Space.h"
#include "NightOwl/Core/Reflection/Reflection.h"
#include "NightOwl/Math/Math.h"
#include <vector>

namespace NightOwl
{
	class Scene;

	class ChainIK;

	class Transform : public Component
	{
	public:
		Transform();

		void LookAt(const Vec3F& targetPoint);

		void Scale(float scaleX, float scaleY, float scaleZ, Space space);

		void Scale(const Vec3F& scale, Space space);

		const Vec3F& GetLocalScale();

		void SetLocalScale(float scaleX, float scaleY, float scaleZ);

		void SetLocalScale(const Vec3F& scale);

		void Rotate(const QuatF& rotation, Space space);

		void Rotate(float angleX, float angleY, float angleZ, Space space);

		void Rotate(const Vec3F& eulers, Space space);

		const Vec3F GetLocalEulerAngles();

		const QuatF GetLocalRotation();

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

		void SetParent(Transform* parentTransform, bool shouldSetParentInverse = true);

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

		void Clone(const Transform& transformToClone, Scene* currentScene = nullptr, Transform* parent = nullptr);

		REFLECT()

	private:
		Mat4F localModelMatrix;

		Mat4F worldMatrix;

		VecQuatMatF worldVecQuatMat;

		VecQuatMatF parentLocalVecQuatMat;

		VecQuatMatF inverseOfOriginalParentLocalVecQuatMat;

		std::vector<Transform*> children;

		VecQuatMatF localVecQuatMat;

		VecQuatMatF worldOffsetVecQuatMat;

		Transform* root;

		Transform* parent;

		friend class Scene;

		friend class ChainIK;

		bool isLocalDirty;

		bool isWorldDirty;

		void SetLocalDirtyFlag();

		void SetWorldDirtyFlag();

		void SetFlagBasedOnSpace(Space space);

		void PropagateParentTransform(const VecQuatMatF& parentLocalTransform);

		void Remove() override;
	};
}
