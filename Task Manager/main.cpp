#include <windows.h>
#include <tlhelp32.h>
#include <tchar.h>
#include <iostream>
#include "Process.h"
#include "TaskManager.h"
#include <vector>
#include <string>
#include <iostream>
#include <iomanip>
//using std::vector;

int main(void)
{
	TaskManager tm;

	std::map<unsigned long, Process> *m = tm.getProcessList();


	std::cout << std::left
		<< std::setw(30) << "NAME"
		<< std::setw(30) << "PID"
		<< std::setw(30) << "PRIORITY" << std::endl << std::endl;
	for (auto it = m->begin(); it != m->end(); ++it) {
		std::cout
			<< std::setw(30) << it->second.getName()
			<< std::setw(30) << it->first
			<< std::setw(30) << std::left
			<< it->second.getFormattedPriority() << std::endl;
	}
	unsigned long pid;
	std::cin >> pid;

	tm.deleteProcess(pid);


	//for (Process p : (*tm.getProcessList())) {
	//	std::cout 
	//		<< std::setw(30) << p.getName() 
	//		<< std::setw(30) << p.getPID() 
	//		<< std::setw(30) << std::left 
	//		<< p.getFormattedPriority() << std::endl;
	//}
	return 0;
}
