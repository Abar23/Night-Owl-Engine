#pragma once

#include <vector>

namespace NightOwl
{
	class PlanarCloth;

	class ClothSimSystem
	{
	public:
		void FixedUpdate();

		void Update();

		void AddClothSimComponent(PlanarCloth* planarCloth);

		void RemoveClothSimComponent(const PlanarCloth* planarCloth);

	private:
		std::vector<PlanarCloth*> clothSimComponents;
	};
}


