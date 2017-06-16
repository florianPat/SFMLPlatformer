#pragma once

#include "EventData.h"

struct EventAddKey : public EventData
{
	static constexpr int EVENT_KEYADD_ID = 0x49f33ebc;
	EventAddKey() : EventData(EVENT_KEYADD_ID) {};
};