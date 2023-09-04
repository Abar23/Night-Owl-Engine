#include "NightOwlPch.h"

#include "Channel.h"
#include <limits>

namespace NightOwl
{
	Channel::Channel()
		:	channelId(std::numeric_limits<unsigned int>::max()),
			isBeingUsed(false)
	{ }

	Channel::Channel(unsigned int channelId)
		:	channelId(channelId),
			isBeingUsed(false)
	{ }

	unsigned Channel::GetChannelId() const
	{
		return channelId;
	}

	void Channel::SetChannelId(unsigned int channelId)
	{
		this->channelId = channelId;
	}

	bool Channel::IsBeingUsed() const
	{
		return isBeingUsed;
	}

	void Channel::SetIsBeingUsed(bool isBeingUsed)
	{
		this->isBeingUsed = isBeingUsed;
	}
}
