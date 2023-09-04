#pragma once

namespace NightOwl
{
	class Channel
	{
	public:
		Channel();

		Channel(unsigned int channelId);

		unsigned int GetChannelId() const;

		void SetChannelId(unsigned int channelId);

		bool IsBeingUsed() const;

		void SetIsBeingUsed(bool isBeingUsed);

	private:
		unsigned int channelId;

		bool isBeingUsed;
	};
}
