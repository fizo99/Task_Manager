#pragma once
#include "Process.h"
//#include <vector>
#include <map>

class TaskManager
{
private:
	//std::vector<Process> *processList = nullptr;
	std::map<unsigned long,Process> *processList = nullptr;
public:
	TaskManager();
	~TaskManager();
	void loadProcessList();
	void deleteProcess(unsigned long _PID);
	//std::vector<Process> *getProcessList();
	void runNewProcess(string path);
	std::map<unsigned long, Process> *getProcessList();
};

