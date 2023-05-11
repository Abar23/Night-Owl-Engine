#include "NightOwlPch.h"

#include "Time.h"

namespace NightOwl::Core
{
	std::unique_ptr<Time> Time::timeInstance{ nullptr };

	std::mutex Time::mutexLock;

	Time::Time()
		:	startingTime(std::chrono::high_resolution_clock::now()),
			minFrameTime(0.0f),
			frameTimeCounter(0),
			deltaTime(0.0f),
			unscaledDeltaTime(0.0f),
			calculatedFps(0.0f),
			unscaledFixedDeltaTime(0.0f),
			totalTime(0.0f),
			totalFixedTime(0),
			timeScale(1.0f),
			frameCount(0),
			shouldRender(true)
	{
		previousTime = startingTime;
	}

	void Time::Init()
	{
		GetInstance();
	}

	void Time::Reset()
	{
		timeInstance->startingTime = std::chrono::high_resolution_clock::now();
		timeInstance->frameCount = 0;
		timeInstance->frameTime = 0;
		timeInstance->totalFixedTime = 0;
		timeInstance->totalTime = 0;
		timeInstance->shouldRender = true;
		timeInstance->timeScale = 1.0f;
	}

	void Time::Update()
	{
		const auto currentTime = std::chrono::high_resolution_clock::now();
		const float unscaledDeltaTime = static_cast<float>(std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::high_resolution_clock::now() - timeInstance->previousTime).count()) * 0.000000001f;
		timeInstance->previousTime = currentTime;

		// Accumulate total time
		timeInstance->totalTime += unscaledDeltaTime;

		// Frame lock time counter
		timeInstance->frameTimeCounter += unscaledDeltaTime;

		// Don't update time when frame time is not greater than the minimum frame time
		if (timeInstance->frameTimeCounter <= timeInstance->minFrameTime)
		{
			return;
		}

		timeInstance->unscaledDeltaTime = timeInstance->frameTimeCounter;
		timeInstance->deltaTime = timeInstance->timeScale * timeInstance->frameTimeCounter;
		timeInstance->calculatedFps = 1.0f / timeInstance->frameTimeCounter;
		timeInstance->shouldRender = true;
		timeInstance->frameCount++;

		timeInstance->frameTimeCounter = (timeInstance->minFrameTime == 0.0f) ? 0.0f : timeInstance->frameTimeCounter - timeInstance->minFrameTime;
	}

	float Time::GetElapsedTime()
	{
		return timeInstance->totalTime;
	}

	float Time::GetDeltaTime()
	{
		return timeInstance->deltaTime;
	}

	float Time::GetUnscaledDeltaTime()
	{
		return timeInstance->unscaledDeltaTime;
	}

	int Time::GetFrameCount()
	{
		return timeInstance->frameCount;
	}

	int Time::GetFrameRate()
	{
		return static_cast<int>(timeInstance->calculatedFps);
	}

	void Time::UnlockFrameRate()
	{
		timeInstance->minFrameTime = 0.0f;
	}

	void Time::LockFrameRate(int maxFrameRate)
	{
		timeInstance->minFrameTime = 1.0f / static_cast<float>(maxFrameRate);
		timeInstance->frameTimeCounter = 0.0f;
	}

	bool Time::ShouldRenderFrame()
	{
		if (timeInstance->shouldRender)
		{
			timeInstance->shouldRender = false;
			return true;
		}
		return false;
	}

	void Time::UpdateFixedTime()
	{
		timeInstance->totalFixedTime += timeInstance->unscaledFixedDeltaTime;
	}

	float Time::GetFixedDeltaTime()
	{
		return timeInstance->unscaledFixedDeltaTime * timeInstance->timeScale;
	}

	float Time::GetUnscaledFixedDeltaTime()
	{
		return timeInstance->unscaledFixedDeltaTime;
	}

	float Time::GetMinFrameTime()
	{
		return timeInstance->minFrameTime;
	}

	void Time::SetFixedTime(int maxUpdatePerFrame)
	{
		timeInstance->unscaledFixedDeltaTime = 1.0f / static_cast<float>(maxUpdatePerFrame);
	}

	void Time::SetTimeScale(float timeScale)
	{
		timeInstance->timeScale = timeScale;
	}

	float Time::GetTimeScale()
	{
		return timeInstance->timeScale;
	}

	bool Time::ShouldExecuteFixedUpdate()
	{
		// Should call fixed update when total fixed time is less than 1 fixed time step from total time
		return timeInstance->totalTime - timeInstance->totalFixedTime > timeInstance->unscaledFixedDeltaTime;
	}

	Time* Time::GetInstance()
	{
		std::lock_guard lock(mutexLock);
		if (timeInstance == nullptr)
		{
			timeInstance = std::unique_ptr<Time>(new Time());
		}
		return timeInstance.get();
	}
}
