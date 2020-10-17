#include "TaskManager.h"
#include <windows.h>
#include <tlhelp32.h>
#include <tchar.h>
#include <vector>
#include <string>
#include <iostream>
#include <psapi.h>

#pragma warning(disable:4996)

TaskManager::~TaskManager() {
	if (processList != nullptr) {
		delete processList;
	}
}
TaskManager::TaskManager() {
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
		if (!TerminateProcess(explorer, 1)) {
			std::string message = std::system_category().message(GetLastError());
			std::cout << message;
		}
		else {
			std::cout << "Process " << _PID << " terminated" << std::endl;
		}
		CloseHandle(explorer);
	}
	else {
		std::cout << "Process " << _PID << " not found" << std::endl;
	}
}

//https://docs.microsoft.com/en-us/windows/win32/procthread/creating-processes
void TaskManager::runNewProcess(string path) {
	STARTUPINFO si;
	PROCESS_INFORMATION pi;

	//memset
	ZeroMemory(&si, sizeof(si));
	si.cb = sizeof(si);
	ZeroMemory(&pi, sizeof(pi));


	wchar_t* wide_string = new wchar_t[path.length() + 1];
	std::copy(path.begin(), path.end(), wide_string);
	wide_string[path.length()] = 0;

	//need to run this programme to run apps that needs admin priviliges like (c:\Windows\System32\magnify.exe)

	if (!CreateProcess(NULL,   // No module name (use command line)
		wide_string,			// Command line
		NULL,           // Process handle not inheritable
		NULL,           // Thread handle not inheritable
		FALSE,          // Set handle inheritance to FALSE
		0,              // No creation flags
		NULL,           // Use parent's environment block
		NULL,           // Use parent's starting directory 
		&si,            // Pointer to STARTUPINFO structure
		&pi)			// Pointer to PROCESS_INFORMATION structure
		)
	{
		//https://stackoverflow.com/questions/455434/how-should-i-use-formatmessage-properly-in-c
		std::string message = std::system_category().message(GetLastError());
		std::cout << message;
		delete wide_string;
		return;
	}

	// Wait until child process exits.
	// WaitForSingleObject(pi.hProcess, INFINITE);
	std::cout << "Process started with PID: " << pi.dwProcessId << std::endl;

	// Close process and thread handles. 
	CloseHandle(pi.hProcess);
	CloseHandle(pi.hThread);
	delete wide_string;
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
		//PROCESS_MEMORY_COUNTERS mem;
		//GetProcessMemoryInfo(process, &mem, sizeof(mem));
		//printf("\tWorkingSetSize: 0x%08X - %u\n", mem.WorkingSetSize,
		//	mem.WorkingSetSize / 1024);
		CloseHandle(process);

		//processList->push_back(Process(processName, dwPriorityClass, PID));
		processList->insert( std::pair<unsigned long, Process>(PID, Process(processName, dwPriorityClass)) );

	} while (Process32Next(processSnap, &pe32));

	CloseHandle(processSnap);
}