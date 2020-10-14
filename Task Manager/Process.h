#pragma once
#include<iostream>
#include<string>

using std::string;
//
//typedef enum int{
//	IDLE = 64,
//	BELOW_NORMAL = 16384,
//	NORMAL = 32,
//	ABOVE_NORMAL = 32768,
//	HIGH_PRIORITY = 128,
//	REAL_TIME = 256,
//};

class Process
{
private:
	string name;
	unsigned long priority;
	unsigned long PID;
public:
	Process(string name, unsigned long  priority, unsigned long  PID);
	Process(string name, unsigned long  priority);
	string getName();
	unsigned long getPriority();
	string getFormattedPriority();
	unsigned long getPID();
};


