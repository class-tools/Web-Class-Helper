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
extern int WCH_task_num;
extern int WCH_ProcessBarCount;
extern int WCH_ProcessBarTot;
extern bool cmd_line;
extern bool anti_idle;
extern bool isend;
extern string op;
extern ifstream fin;
extern ofstream fout;
WCH_Time WCH_GetTime();
void WCH_Error(string INFO);
void WCH_printlog(int w, initializer_list <string> other);
int WCH_GetNumDigits(int n);

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
	ShowWindow(hwnd, flag);
	cmd_line = flag;
	WCH_printlog(WCH_LOG_MODE_WD, {"CONSOLE", "STATUS", (flag == true ? "SHOW" : "HIDE")});
}

void WCH_SetTrayStatus(bool flag) {
	// Set the tray status by Windows API.
	ShowWindow(FindWindow("Shell_trayWnd", NULL), (flag == true ? SW_SHOW : SW_HIDE));
	WCH_printlog(WCH_LOG_MODE_WD, {"TRAY", "STATUS", (flag == true ? "SHOW" : "HIDE")});
}

void WCH_SetWindowSize(int mode, HWND hWnd) {
	// Set the window size by Windows API.
	ShowWindow(hWnd, mode);
	WCH_printlog(WCH_LOG_MODE_WD, {"CURWND", "SIZE", (mode == SW_MAXIMIZE ? "MAXIMIZE" : "NORMAL")});
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

int WCH_GetNumDigits(int n) {
	int cnt = 1;
	while ((n /= 10) != 0) {
		cnt++;
	}
	return cnt;
}

void WCH_PrintProcessBar(int n, bool flag) {
	// Print a process bar.
	if (flag) {
		for (int i = 0; i < WCH_ProcessBarCount; i++) {
			cout << "\b";
		}
	}
	cout << "[";
	for (int i = 0; i < n; i++) {
		cout << "=";
	}
	for (int i = n; i < 100; i++) {
		cout << " ";
	}
	cout << "] " << n << "%";
	WCH_ProcessBarCount = 104 + WCH_GetNumDigits(n);
}

void WCH_ProcessBar() {
	// Process bar.
	WCH_PrintProcessBar(0, false);
	for (int i = WCH_ProcessBarTot; i > 1 && !isend; i--) {
		Sleep(1000);
		WCH_PrintProcessBar(100 / i, true);
	}
	WCH_PrintProcessBar(100, true);
	cout << endl;
}

#endif
