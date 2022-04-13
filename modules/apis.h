/*
Web-Class-Helper APIs Module Header File 1.1.2
This source code file is under MIT License.
Copyright (c) 2022 Class Tools Develop Team
Contributors: jsh-jsh ren-yc
*/
#ifndef APIS_H
#define APIS_H
#include <bits/stdc++.h>
#include <windows.h>
#include <wininet.h>
#include <tlhelp32.h>
#include <conio.h>
#include <direct.h>
#include "commands.h"
#include "functions.h"
#include "file-process.h"
#include "variables.h"
using namespace std;

extern const string Weekdayname[7];
extern multimap <int, pair <int, string>> WCH_clock;
extern set <string> WCH_task_list;
extern HWND hwnd;
extern int WCH_clock_num;
extern bool cmd_line;
extern bool anti_idle;
extern bool mode;
extern string op;
extern string UserName;
extern ifstream fin;
extern ofstream fout;
WCH_Time WCH_GetTime();
void WCH_Error(string INFO);

string WCH_GetUserName() {
	// Get current user name and return a string by Windows API.
	char tmpc[256] = {0};
	DWORD Size = 256;
	GetUserName(tmpc, &Size);
	string tmps = tmpc;
	return tmps;
}

DWORD WCH_GetPID(string name) {
	// Get PID by process name.
	DWORD pid;
	PROCESSENTRY32 entry;
	entry.dwSize = sizeof(PROCESSENTRY32);
	HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (Process32First(snapshot, &entry) == TRUE) {
		while (Process32Next(snapshot, &entry) == TRUE) {
			if (string(entry.szExeFile) == name) {
				HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, entry.th32ProcessID);
				pid = GetProcessId(hProcess);
				CloseHandle(hProcess);
			}
		}
	}
	CloseHandle(snapshot);
	return pid;
}

void WCH_SetWindowStatus(bool flag) {
	// Set the window status by Windows API.
	if (hwnd) {
		ShowWindow(hwnd, flag);
	}
	cmd_line = flag;
}

void WCH_SetWindowPos(bool flag) {
	// Set the window position by Windows API.
	ShowWindow(hwnd, (flag == true ? SW_SHOW : SW_HIDE));
}

void WCH_SetTrayStatus(bool flag) {
	// Set the tray status by Windows API.
	ShowWindow(FindWindow("Shell_trayWnd", NULL), (flag == true ? SW_SHOW : SW_HIDE));
}

void WCH_GetPath(bool mode, string UserName) {
	// Get current path by Windows API.
	char ExeFile[256];
	_getcwd(ExeFile, 256);
	if (mode) {
		cout << ExeFile << "> ";
	} else {
		string tmp;
		tmp = ExeFile;
		tmp.replace(1, 2, "\\");
		for (long long unsigned int i = 0; i < tmp.size(); i++) {
			if (isupper(tmp[i])) {
				tmp[i] = tolower(tmp[i]);
			}
		}
		cout << UserName << "@\\" << tmp << "$ ";
	}
}

bool WCH_TaskKill(string name) {
	// Kill a task by Windows API.
	HANDLE hProcess = OpenProcess(PROCESS_TERMINATE, FALSE, WCH_GetPID(name));
	if (hProcess) {
		TerminateProcess(hProcess, 0);
		CloseHandle(hProcess);
		return true;
	} else {
		return false;
	}
}

#endif
