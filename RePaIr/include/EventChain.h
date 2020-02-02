#pragma once
#include <vector>
#include <string>

class Event {
public:
	//include some condition in the callback to make the event "completed"
	Event(const std::string& name);


	std::string _name = "";
	bool _completed = false;
private:

};
class EventChain {
public:
	EventChain(const std::vector<Event>& events);


	std::vector<Event> _events;
private:
};