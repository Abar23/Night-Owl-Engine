#pragma once

#include <memory>

int main();

namespace NightOwl::Core
{
	class IApplication
	{
	public:
		virtual ~IApplication() = default;

		virtual void Init() = 0;

		virtual void Update() = 0;

		virtual void Shutdown() = 0;

	private:
		friend int ::main();
	};

	// Define in client game projects
	std::shared_ptr<IApplication> CreateApplication();
}
