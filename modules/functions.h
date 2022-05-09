/*
Web Class Helper Function Module Header File 2.0.0
This source code file is under MIT License.
Copyright (c) 2022 Class Tools Develop Team
Contributors: jsh-jsh ren-yc
*/
#ifndef FUNCTIONS_H
#define FUNCTIONS_H
#include <bits/stdc++.h>
#include <io.h>
#include <thread>
#include <windows.h>
#include <wininet.h>
#include <tlhelp32.h>
#include <conio.h>
#include <direct.h>
#include <VersionHelpers.h>
#include "commands.h"
#include "file-process.h"
#include "apis.h"
#include "variables.h"
using namespace std;

extern const string WCH_WDName[7];
extern multimap <int, pair <int, string>> WCH_clock_list;
extern set <string> WCH_task_list;
extern set <string> WCH_work_list;
extern HWND WCH_hWnd;
extern int WCH_clock_num;
extern int WCH_task_num;
extern int WCH_work_num;
extern int WCH_ProgressBarCount;
extern int WCH_ProgressBarTot;
extern bool WCH_cmd_line;
extern bool WCH_anti_idle;
extern bool WCH_program_end;
extern bool WCH_wait_cmd;
extern string WCH_command;
extern string WCH_ProgressBarStr;
extern ifstream fin;
extern ofstream fout;
extern wifstream wfin;
extern wofstream wfout;
WCH_Time WCH_GetTime();
void WCH_Sleep(int _ms);
void WCH_Error(string INFO);
void WCH_printlog(int w, initializer_list <string> other);
void WCH_read();
void WCH_save();
int WCH_GetNumDigits(int n);

WCH_Time WCH_GetTime() {
	// Get current time and return a WCH_Time object.
	WCH_Time NowTime;
	time_t rawtime;
	struct tm *ptminfo;
	time(&rawtime);
	ptminfo = localtime(&rawtime);
	NowTime.Year = ptminfo -> tm_year + 1900;
	NowTime.Month = ptminfo -> tm_mon + 1;
	NowTime.Day = ptminfo -> tm_mday;
	NowTime.Hour = ptminfo -> tm_hour;
	NowTime.Minute = ptminfo -> tm_min;
	NowTime.Second = ptminfo -> tm_sec;
	NowTime.Name = "NULL";
	return NowTime;
}

string WCH_TransStrChar(string str) {
	// Translate the upper cases in string to lower cases.
	for (int i = 0; i < (int)str.size(); i++) {
		if (str[i] >= 'A' && str[i] <= 'Z') {
			str[i] = tolower(str[i]);
		}
	}
	return str;
}

void WCH_Error(string INFO) {
	// Error message checker.
	string tmp;
	if (INFO == WCH_ERRNO_UNCORRECT) {
		tmp = "Your input code is uncorrect, please check and try again";
	} else if (INFO == WCH_ERRNO_NETWORK_FAILURE) {
		tmp = "An network error occurred, please check your network connection and try to update this program";
	} else if (INFO == WCH_ERRNO_CLOCK_OPERATION) {
		tmp = "Cannot operate the clock list, please try to restart this program";
	} else if (INFO == WCH_ERRNO_TASK_OPERATION) {
		tmp = "Cannot operate the task list, please try to restart this program";
	} else if (INFO == WCH_ERRNO_WORK_OPERATION) {
		tmp = "Cannot operate the work list, please try to restart this program";
	}
	cout << tmp << "." << endl;
	WCH_printlog(WCH_LOG_MODE_ERROR, {tmp});
}

bool WCH_ShortCutKeyCheck() {
	// Check if the shortcut key is pressed.
	int a = GetKeyState(VK_CONTROL);
	int b = GetKeyState(VK_DOWN);
	if (a < 0 && b < 0) {
		return true;
	} else {
		return false;
	}
}

void WCH_check_clock_loop() {
	// Check if the time equals to the clock. (Another thread)
	WCH_Sleep((60 - WCH_GetTime().Second) * 1000);
	while (!WCH_program_end) {
		WCH_Time NOW = WCH_GetTime();
		for (auto it = WCH_clock_list.equal_range(NOW.Hour).first; it != WCH_clock_list.equal_range(NOW.Hour).second; it++) {
			if ((it -> second).first == NOW.Minute && ((it -> second).second).size() > 0) {
				cout << "\a";
				MessageBoxA(NULL, ((it -> second).second).c_str(), "WCH Clock", MB_OK);
			}
		}
		WCH_Sleep(60000);
	}
}

void WCH_check_task_loop() {
	// Check if the running task is in the task list. (Another thread)
	while (WCH_anti_idle && !WCH_program_end) {
		for (auto it = WCH_task_list.begin(); it != WCH_task_list.end(); it++) {
			if (WCH_TaskKill(*it)) {
				WCH_printlog(WCH_LOG_MODE_KT, {*it, "Successfully killed"});
			} else {
				WCH_printlog(WCH_LOG_MODE_KT, {*it, "Failed to kill"});
			}
		}
		WCH_Sleep(15000);
	}
}

void WCH_CL_Init() {
	// Initialize the command line.
	WCH_wait_cmd = true;
	cout << ">>> ";
	cin >> WCH_command;
	transform(WCH_command.begin(), WCH_command.end(), WCH_command.begin(), ::tolower);
	WCH_wait_cmd = false;
}

void WCH_PutPicture() {
	// Press PrintScreen. (Keyboard)
	keybd_event(VK_SNAPSHOT, 0, 0, 0);
	keybd_event(VK_SNAPSHOT, 0, KEYEVENTF_KEYUP, 0);
	cout << "The picture is in the clipboard and be saved in your Pictures folder." << endl;
}

void WCH_SaveImg() {
	// Run image saver Python program.
	string tmp = "IMG";
	tmp += to_string(WCH_Framework);
	tmp += ".EXE";
	system(tmp.c_str());
	WCH_Sleep(500);
}

void WCH_Init_Dir() {
	// Initialization for directory.
	if (_access("data", 0) != 0) {
		CreateDirectory(L"data", NULL);
	}
	if (_access("logs", 0) != 0) {
		CreateDirectory(L"logs", NULL);
	}
}

void WCH_Init_Var() {
	// Initialization for varible.
	WCH_hWnd = GetForegroundWindow();
	WCH_ProgressBarStr = IsWindows10OrGreater() ? UTF8ToANSI("━") : "-";
}

void WCH_Init_Log() {
	// Initialization for log.
	WCH_Time now = WCH_GetTime();
	rename("logs/latest.log", format("logs/{:04}{:02}{:02}{:02}{:02}{:02}.log", now.Year, now.Month, now.Day, now.Hour, now.Minute, now.Second).c_str());
}

void WCH_Init_Win() {
	// Initialization for window.
	SetConsoleTitleA(format("Web Class Helper (x{})", to_string(WCH_DisplayFramework)).c_str());
}

void WCH_Init_Bind() {
	// Initialization for bind.
	WCH_printlog(WCH_LOG_MODE_ST, {"s", to_string(WCH_DisplayFramework)});
	atexit(WCH_save);
	WCH_signalHandler();
	WCH_SetWindowStatus(true);
}

void WCH_Init_Out() {
	// Initialization for output.
	cout << "Web Class Helper " << WCH_VER << " (x" << to_string(WCH_DisplayFramework) << ")" << endl;
	cout << "Copyright (c) 2022 Class Tools Develop Team." << endl;
	cout << "Type \"help\", \"update\" or \"license\" for more information." << endl;
	cout << endl;
}

void WCH_Init() {
	// Initialize the whole program.
	WCH_Init_Dir();
	WCH_Init_Var();
	WCH_Init_Log();
	WCH_Init_Win();
	WCH_Init_Bind();
	WCH_read();
	thread T(WCH_check_clock_loop);
	T.detach();
	WCH_Init_Out();
}

#endif