#include "Time.h"

namespace NightOwl::Core
{
	std::unique_ptr<Time> Time::timeInstance{ nullptr };
	std::mutex Time::mutexLock;

	void Time::Init()
	{
		GetInstance();
	}

	void Time::Reset()
	{
		timeInstance->startingTime = std::chrono::high_resolution_clock::now();
		timeInstance->frameCount = 0;
		timeInstance->deltaTime = 0;
	}

	void Time::Update()
	{
		timeInstance->deltaTime = static_cast<float>(std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::high_resolution_clock::now() - timeInstance->previousTime).count()) * 0.000000001f;
		timeInstance->previousTime = std::chrono::high_resolution_clock::now();
		timeInstance->frameCount++;
	}

	float Time::GetElapsedTime()
	{
		return static_cast<float>(std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::high_resolution_clock::now() - timeInstance->startingTime).count()) * 0.000000001f;
	}

	float Time::GetDeltaTime()
	{
		return timeInstance->deltaTime;
	}

	int Time::GetFrameCount()
	{
		return timeInstance->frameCount;
	}

	Time::Time()
		:	deltaTime(0.0f),
			frameCount(0)
	{
		startingTime = std::chrono::high_resolution_clock::now();
		previousTime = startingTime;
		frameCount = 0;
		deltaTime = 0;
	}

	Time* Time::GetInstance()
	{
		std::lock_guard lock(mutexLock);
		if(timeInstance == nullptr)
		{
			timeInstance = std::unique_ptr<Time>(new Time());
		}
		return timeInstance.get();
	}
}
