#include "Process.h"

Process::Process(string name, unsigned long  priority, unsigned long  PID) {
	this->name = name;
	this->priority = priority;
	this->PID = PID;
}
Process::Process(string name, unsigned long  priority) {
	this->name = name;
	this->priority = priority;
}

string Process::getName() {
	return name;
}
unsigned long  Process::getPriority() {
	return priority;
}
string Process::getFormattedPriority() {
	string output;
	switch (priority) {
	case 32:
		output = "(32) normal";
		break;
	case 64:
		output = "(64) idle";
		break;
	case 128:
		output = "(128) high priority";
		break;
	case 256:
		output = "(256) real time";
		break;
	case 16384:
		output = "(16384) below normal";
		break;
	case 32768:
		output = "(32768) above normal";
		break;
	default:
		output = "(" + std::to_string(priority) + ")";
		break;
	}
	return output;
}
unsigned long  Process::getPID() {
	return PID;
}