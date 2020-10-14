#include "TaskManager.h"
#include <windows.h>
#include <tlhelp32.h>
#include <tchar.h>
#include <vector>
#include <string>
#include <iostream>

TaskManager::~TaskManager() {
	if (processList != nullptr) {
		delete processList;
	}
}
TaskManager::TaskManager() {
	loadProcessList();
}

//std::vector<Process>* TaskManager::getProcessList() {
//	return processList;
//}

std::map<unsigned long, Process>* TaskManager::getProcessList() {
	return processList;
}

void TaskManager::deleteProcess(unsigned long _PID) {
	if (processList->find(_PID) != processList->end()){
		HANDLE explorer;
		explorer = OpenProcess(PROCESS_ALL_ACCESS, false, _PID);
		TerminateProcess(explorer, 1);
		CloseHandle(explorer);
	}
}

// src:
// https://docs.microsoft.com/en-us/windows/win32/toolhelp/taking-a-snapshot-and-viewing-processes
void TaskManager::loadProcessList() {
	HANDLE processSnap;
	HANDLE process;

	processSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

	if (processSnap == INVALID_HANDLE_VALUE) {
		std::cout << "CreateToolhelp32Snapshot error";
		return;
	}

	// Set the size of the structure before using it.
	PROCESSENTRY32 pe32;
	pe32.dwSize = sizeof(PROCESSENTRY32);

	// Retrieve information about the first process,
	// and exit if unsuccessful
	if (!Process32First(processSnap, &pe32)) {
		//printError(TEXT("Process32First")); // show cause of failure
		CloseHandle(processSnap);          // clean the snapshot object
		return;
	}

	if (processList != nullptr) {
		delete processList;
	}
	//processList = new std::vector<Process>();
	processList = new std::map<unsigned long, Process>();
	do {
		std::wstring temp(pe32.szExeFile);
		string processName(temp.begin(), temp.end());

		process = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pe32.th32ProcessID);

		unsigned long PID = pe32.th32ProcessID;
		unsigned long dwPriorityClass = GetPriorityClass(process);

		CloseHandle(process);

		//processList->push_back(Process(processName, dwPriorityClass, PID));
		processList->insert( std::pair<unsigned long, Process>(PID, Process(processName, dwPriorityClass)) );

	} while (Process32Next(processSnap, &pe32));

	CloseHandle(processSnap);
}