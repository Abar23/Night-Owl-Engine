#pragma once

#include <chrono>
#include <mutex>
#include <memory>

namespace NightOwl::Core
{
	class Time
	{
	public:
		static void Init();

		Time(const Time& other) = delete;

		void operator=(const Time& other) = delete;

		static void Reset();

		static void Update();

		static float GetElapsedTime();

		static float GetDeltaTime();

		static int GetFrameCount();

	private:
		Time();

		static Time* GetInstance();

		static std::unique_ptr<Time> timeInstance;

		static std::mutex mutexLock;

		std::chrono::time_point<std::chrono::high_resolution_clock> startingTime;

		std::chrono::time_point<std::chrono::high_resolution_clock> previousTime;

		float deltaTime;

		int frameCount;
	};
}
