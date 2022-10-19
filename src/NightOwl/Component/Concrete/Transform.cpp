#include "Transform.h"
#include "NightOwl/Core/Utitlity/Assert.h"

// NEED TO CLEAN UP LOGIC, KINDA NASTY
namespace NightOwl::Component
{
	Transform::Transform()
		: Component(ComponentType::Transform),
		  localModelMatrix(1.0f),
	      worldMatrix(1.0f),
		  parentLocalMatrix(1.0f),
		  localScale(1.0f),
		  worldScale(1.0f),
		  root(this),
		  parent(nullptr),
		  isLocalDirty(false),
		  isWorldDirty(false)
	{
	}

	void Transform::Scale(float scaleX, float scaleY, float scaleZ, Space space)
	{
		const Math::Vec3F scale(scaleX, scaleY, scaleZ);

		Scale(scale, space);
	}

	void Transform::Scale(const Math::Vec3F& scale, Space space)
	{
		if (parent == nullptr || space == Space::Local)
		{
			localScale += scale;

			SetLocalDirtyFlag();
		}
		else if (space == Space::World)
		{
			worldScale += scale;

			SetWorldDirtyFlag();
		}
	}

	void Transform::Rotate(float angleX, float angleY, float angleZ, Space space)
	{
		const Math::Vec3F eulers(angleX, angleY, angleZ);

		Rotate(eulers, space);
	}

	void Transform::Rotate(const Math::Vec3F& eulers, Space space)
	{
		if (parent == nullptr || space == Space::Local)
		{
			localEulerAngles += eulers;

			localRotation = Math::QuatF::MakeRotationFromEulers(eulers) * localRotation;

			localRotation.Renormalize();

			SetLocalDirtyFlag();

		}
		else if (space == Space::World)
		{
			worldEulerAngles += eulers;

			worldRotation = Math::QuatF::MakeRotationFromEulers(eulers) * worldRotation;

			SetWorldDirtyFlag();
		}
	}

	void Transform::Translate(float positionX, float positionY, float positionZ, Space space)
	{
		const Math::Vec3F translation(positionX, positionY, positionZ);

		Translate(translation, space);
	}

	void Transform::Translate(const Math::Vec3F& translation, Space space)
	{
		if(parent == nullptr || space == Space::Local)
		{
			localPosition += translation;

			SetLocalDirtyFlag();

		}
		else if (space == Space::World)
		{
			worldPosition += translation;

			SetWorldDirtyFlag();
		}
	}

	const Math::Vec3F& Transform::GetLocalScale()
	{
		return localScale;
	}

	void Transform::SetLocalScale(float scaleX, float scaleY, float scaleZ)
	{
		localScale = Math::Vec3F(scaleX, scaleY, scaleZ);

		SetLocalDirtyFlag();
	}

	void Transform::SetLocalScale(const Math::Vec3F& scale)
	{
		localScale = scale;

		SetLocalDirtyFlag();
	}

	const Math::Vec3F& Transform::GetLocalEulerAngles()
	{
		return localEulerAngles;
	}

	void Transform::SetLocalEulerAngles(float angleX, float angleY, float angleZ)
	{
		localEulerAngles = Math::Vec3F(angleX, angleY, angleZ);

		SetLocalDirtyFlag();
	}

	void Transform::SetLocalEulerAngles(const Math::Vec3F& eulers)
	{
		localEulerAngles = eulers;

		SetLocalDirtyFlag();
	}

	const Math::Vec3F& Transform::GetLocalPosition()
	{
		return localPosition;
	}

	void Transform::SetLocalPosition(float positionX, float positionY, float positionZ)
	{
		localPosition = Math::Vec3F(positionX, positionY, positionZ);

		SetLocalDirtyFlag();
	}

	void Transform::SetLocalPosition(const Math::Vec3F& position)
	{
		localPosition = position;

		SetLocalDirtyFlag();
	}

	int Transform::GetNumberOfChildren()
	{
		return children.size();
	}

	const Transform& Transform::GetParent()
	{
		return *parent;
	}

	void Transform::SetParent(Transform* parentTransform)
	{
		parent = parentTransform;
		parent->SetChild(*this);
		
		for (const auto& childTransform : parentTransform->children)
		{
			childTransform->PropagateParentLocalTransform(parent->GetLocalModelMatrix());
		}
	}

	Transform* Transform::RemoveParent()
	{
		Transform* parentBeingRemoved = parent;

		int indexOfChildToRemove;
		for (indexOfChildToRemove = 0; indexOfChildToRemove < parent->children.size(); indexOfChildToRemove++)
		{
			if (this == parent->children[indexOfChildToRemove]) break;
		}
		parent->children.erase(parent->children.begin() + indexOfChildToRemove);

		parent = nullptr;

		parentLocalMatrix = Math::Mat4F::Identity();

		return parentBeingRemoved;
	}

	void Transform::SetChild(Transform& childTransform)
	{
		children.push_back(&childTransform);
	}

	const Math::Mat4F& Transform::GetLocalModelMatrix(bool overrideDirtyFlag)
	{
		if (isLocalDirty || overrideDirtyFlag)
		{
			const Math::Mat4F translationMatrix = Math::Mat4F::MakeTranslation(localPosition);

			const Math::Mat4F scaleMatrix = Math::Mat4F::MakeScale(localScale);

			localModelMatrix = translationMatrix * localRotation.GetRotationMatrix() * scaleMatrix;

			if(parent == nullptr)
			{
				worldMatrix = localModelMatrix;
			}

			isLocalDirty = false;
		}

		return localModelMatrix;
	}

	const Math::Mat4F& Transform::GetWorldMatrix()
	{
		if (isWorldDirty || isLocalDirty)
		{
			const Math::Mat4F translationMatrix = Math::Mat4F::MakeTranslation(worldPosition);

			const Math::Mat4F scaleMatrix = Math::Mat4F::MakeScale(worldScale);

			if(parent != nullptr)
			{
				const Math::Mat4F parentChildCombined = parentLocalMatrix * GetLocalModelMatrix();

				const Math::Mat4F parentChildCombinedTranslation = Math::Mat4F::MakeTranslation(parentChildCombined.GetTranslation());

				worldMatrix = parentChildCombinedTranslation * translationMatrix * worldRotation.GetRotationMatrix() * scaleMatrix * parentChildCombinedTranslation.GetInverse()* parentChildCombined;
			}
			else
			{
				worldMatrix =  GetLocalModelMatrix() * translationMatrix * worldRotation.GetRotationMatrix() * scaleMatrix;
			}

			isWorldDirty = false;
			isLocalDirty = false;
		}

		return worldMatrix;
	}

	Transform* Transform::GetChildAtIndex(int index) const
	{
		ENGINE_ASSERT(index < children.size(), std::format("Transform does not have child at index {0}", index));

		return children[index];
	}

	void Transform::SetLocalDirtyFlag()
	{
		isLocalDirty = true;
	}

	void Transform::SetWorldDirtyFlag()
	{
		isWorldDirty = true;
	}

	void Transform::SetFlagBasedOnSpace(Space space)
	{
		(space == Space::Local) ? SetLocalDirtyFlag() : SetWorldDirtyFlag();
	}

	void Transform::PropagateParentLocalTransform(const Math::Mat4F& parentLocalTransform)
	{
		this->parentLocalMatrix = parentLocalTransform;

		for (const auto& childTransform : this->children)
		{
			childTransform->PropagateParentLocalTransform(parentLocalTransform * this->GetLocalModelMatrix(true));
		}

		SetWorldDirtyFlag();
	}

	void Transform::RestrictEulerAngles(Math::Vec3F& eulerAngles)
	{
		for (float& angle : eulerAngles.data)
		{
			
		}
	}

	Math::Vec3F Transform::GetWorldScale()
	{
		GetWorldMatrix();

		const Math::Vec3F worldXScale = worldMatrix.GetColumn(0).xyz;
		const Math::Vec3F worldYScale = worldMatrix.GetColumn(1).xyz;
		const Math::Vec3F worldZScale = worldMatrix.GetColumn(2).xyz;

		return {worldXScale.Magnitude(), worldZScale.Magnitude(), worldYScale.Magnitude() };
	}

	void Transform::SetWorldScale(const Math::Vec3F& worldScale)
	{
		this->worldScale = worldScale;
	}

	Math::Vec3F Transform::GetWorldEulerAngles() const
	{
		return worldEulerAngles + localEulerAngles;
	}

	void Transform::SetWorldEulerAngles(const Math::Vec3F& worldEulerAngles)
	{
		this->worldEulerAngles = worldEulerAngles;
		worldRotation = Math::QuatF::MakeRotationFromEulers(worldEulerAngles);
	}

	Math::Vec3F Transform::GetWorldPosition() const
	{
		return worldMatrix.GetTranslation();
	}

	void Transform::SetWorldPosition(const Math::Vec3F& worldPosition)
	{
		this->worldPosition = worldPosition;
	}

	bool Transform::HasParent() const
	{
		return parent != nullptr;
	}

	bool Transform::HasChildren() const
	{
		return !children.empty();
	}
}
