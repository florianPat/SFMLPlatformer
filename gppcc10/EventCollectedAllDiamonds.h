#pragma once

#include "EventData.h"

struct EventCollectedAllDiamonds : public EventData
{
	static constexpr int EVENT_COLLECTEDALLDIAMONDS_ID = 0x17db004f;
	EventCollectedAllDiamonds() : EventData(EVENT_COLLECTEDALLDIAMONDS_ID) {};
};