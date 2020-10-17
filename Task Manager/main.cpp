
#include <Windows.h>
#include <tlhelp32.h>
#include <tchar.h>
#include <iostream>
#include "Process.h"
#include "TaskManager.h"
#include <vector>
#include <string>
#include <iostream>
#include <iomanip>
#include <sstream> 
#include <stdlib.h>
//using std::vector;

void showUI(std::map<unsigned long, Process> *m) {
	std::cout << std::left
		<< std::setw(60) << " NAME"
		<< std::setw(30) << " PID"
		<< std::setw(30) << " PRIORITY" << std::endl;

	for (auto it = m->begin(); it != m->end(); ++it) {
		std::cout
			<< std::left
			<< std::setfill('-')
			<< std::setw(60) << "+"
			<< std::setw(30) << "+"
			<< std::setw(29) << "+" << "+"
			<< std::endl << std::setfill(' ');

		std::cout
			<< std::left
			<< std::setw(1)  << "|" << std::setw(59) << it->second.getName()
			<< std::setw(1)  << "|" << std::setw(29) << it->first
			<< std::setw(1)  << "|" << std::setw(28) << it->second.getFormattedPriority() << "|" << std::endl;
	}
	std::cout
		<< std::left
		<< std::setfill('-')
		<< std::setw(60) << "+"
		<< std::setw(30) << "+"
		<< std::setw(29) << "+" << "+"
		<< std::endl << std::setfill(' ');
	std::cout << std::endl << std::left
		<< std::setw(30) << "0 - EXIT"
		<< std::setw(30) << "1 - KILL"
		<< std::setw(30) << "2 - RUN NEW PROCESS" 
		<< std::setw(30) << "3 - REFRESH LIST" << std::endl << std::endl;
}

int main(void)
{
	//polish letters
	setlocale(LC_CTYPE, "Polish");
	
	TaskManager tm;
	tm.loadProcessList();

	string option = "5";
	showUI(tm.getProcessList());
	while (option != "0") {
		//showProcesses(tm.getProcessList());
		std::cout << "Option: ";
		std::cin >> option;
		if (option == "0") return 0;
		else if (option == "1") {
			unsigned long PID;
			std::cout << "Enter process PID: ";
			std::cin >> PID;
			tm.deleteProcess(PID);
		}
		else if (option == "2") {
			string path;
			std::cout << "Enter process name(or it`s full path): ";
			std::cin >> path;
			tm.runNewProcess(path);
			tm.loadProcessList();
		}
		else if (option == "3") {
			tm.loadProcessList();
			system("CLS");
			showUI(tm.getProcessList());
		}
		else {
			std::cout << "Wrong option" << std::endl;
		}
		option = "5";
	}
	return 0;
}
