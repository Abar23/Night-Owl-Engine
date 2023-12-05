#include "NightOwlPch.h"

#include "ClothSimSystem.h"
#include "NightOwl/Component/Concrete/PlanarCloth.h"

namespace NightOwl
{
	void ClothSimSystem::Update()
	{
		for (const auto & clothSimComponent : clothSimComponents)
		{
			clothSimComponent->Update();
		}
	}

	void ClothSimSystem::AddClothSimComponent(PlanarCloth* planarCloth)
	{
		clothSimComponents.push_back(planarCloth);
	}

	void ClothSimSystem::RemoveClothSimComponent(const PlanarCloth* planarCloth)
	{
		int clothSimComponentIndex = 0;
		for (const auto* planarClothComponent : clothSimComponents)
		{
			if (planarClothComponent == planarCloth)
			{
				break;
			}
			clothSimComponentIndex++;
		}

		if (clothSimComponentIndex != clothSimComponents.size())
		{
			clothSimComponents.erase(clothSimComponents.begin() + clothSimComponentIndex);
		}
	}
}
