#include "EventChain.h"

EventChain::EventChain(const std::vector<Event>& events)
{
	_events = events;
}


Event::Event(const std::string& name)
{
	_name = name;
}
