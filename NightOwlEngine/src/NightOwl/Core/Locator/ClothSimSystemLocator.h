#pragma once

namespace NightOwl
{
	class ClothSimSystem;

	class ClothSimSystemLocator
	{
	public:
		static ClothSimSystem* GetClothSimSystem()
		{
			return clothSimSystem;
		}

		static void Provide(ClothSimSystem* clothSimSystemInstance)
		{
			clothSimSystem = clothSimSystemInstance;
		}

	private:
		inline static ClothSimSystem* clothSimSystem{ nullptr };
	};
}
