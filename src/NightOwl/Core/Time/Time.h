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

		static float GetUnscaledDeltaTime();

		static int GetFrameCount();

		static int GetFrameRate();

		static void UnlockFrameRate();

		static void LockFrameRate(int maxFrameRate);

		static bool ShouldRenderFrame();

		static void UpdateFixedTime();

		static float GetFixedDeltaTime();

		static float GetUnscaledFixedDeltaTime();

		static float GetMinFrameTime();

		static void SetFixedTime(int maxUpdatePerFrame);

		static void SetTimeScale(float timeScale);

		static float GetTimeScale();

		static bool ShouldExecuteFixedUpdate();


	private:
		Time();

		static Time* GetInstance();

		static std::unique_ptr<Time> timeInstance;

		static std::mutex mutexLock;

		std::chrono::time_point<std::chrono::high_resolution_clock> startingTime;

		std::chrono::time_point<std::chrono::high_resolution_clock> previousTime;

		float minFrameTime;

		float frameTimeCounter;

		float deltaTime;

		float unscaledDeltaTime;

		float calculatedFps;

		float frameTime;

		float unscaledFixedDeltaTime;

		float totalTime;

		float totalFixedTime;

		float timeScale;

		int frameCount;

		bool shouldRender;
	};
}
