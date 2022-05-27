/*
Web Class Helper Functions Module Header File 2.0.1
This source code file is under MIT License.
Copyright (c) 2022 Class Tools Develop Team
Contributors: jsh-jsh ren-yc
*/
#ifndef FUNCTIONS_H
#define FUNCTIONS_H
#include "file-process.h"
#include "init.h"
#include "commands.h"
#include "apis.h"
#include "variables.h"

extern const string WCH_WDName[7];
extern map <string, function <void ()>> WCH_command_support;
extern vector <string> WCH_command_list;
extern multimap <int, pair <int, string>> WCH_clock_list;
extern set <string> WCH_task_list;
extern set <string> WCH_work_list;
extern HWND WCH_hWnd;
extern int WCH_clock_num;
extern int WCH_task_num;
extern int WCH_work_num;
extern int WCH_ProgressBarCount;
extern int WCH_ProgressBarTot;
extern int WCH_InputTimes;
extern bool WCH_cmd_line;
extern bool WCH_anti_idle;
extern bool WCH_program_end;
extern bool WCH_wait_cmd;
extern string WCH_command;
extern string WCH_ProgressBarStr;
extern ifstream fin;
extern ofstream fout;
WCH_Time WCH_GetTime();
void WCH_Sleep(int _ms);
void WCH_Error(int _in);
void WCH_printlog(string _mode, string _info);
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
			str[i] = (char)tolower(str[i]);
		}
	}
	return str;
}

void WCH_Error(int _in) {
	// Error message checker.
	string _info;
	string _mode;
	if (_in == WCH_ERRNO_UNCORRECT) {
		_info = "Your input code is uncorrect, please check and try again";
		_mode = WCH_LOG_STATUS_WARN;
	} else if (_in == WCH_ERRNO_NETWORK_FAILURE) {
		_info = "An network error occurred, please check your network connection and try to update this program";
		_mode = WCH_LOG_STATUS_WARN;
	} else if (_in == WCH_ERRNO_FILE_NOT_FOUND) {
		_info = "File processing failed. Please try reinstalling this program";
		_mode = WCH_LOG_STATUS_ERROR;
	} else if (_in == WCH_ERRNO_CLOCK_OPERATION) {
		_info = "Cannot operate the clock list, please try to restart this program";
		_mode = WCH_LOG_STATUS_ERROR;
	} else if (_in == WCH_ERRNO_TASK_OPERATION) {
		_info = "Cannot operate the task list, please try to restart this program";
		_mode = WCH_LOG_STATUS_ERROR;
	} else if (_in == WCH_ERRNO_WORK_OPERATION) {
		_info = "Cannot operate the work list, please try to restart this program";
		_mode = WCH_LOG_STATUS_ERROR;
	} else if (_in == WCH_ERROR_DPI_GET_FAILED) {
		_info = "Cannot get the DPI, please try to restart this program";
		_mode = WCH_LOG_STATUS_ERROR;
	}
	cout << _info << "." << endl;
	WCH_printlog(_mode, _info);
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

void WCH_safety_input_loop() {
	// Check if the program input causes error.
	while (!WCH_program_end) {
		if (WCH_InputTimes >= 2) {
			raise(SIGINT);
		}
		WCH_InputTimes = 0;
		WCH_Sleep(500);
	}
}

void WCH_CL_Init() {
	// Initialize the command line.
	WCH_wait_cmd = true;
	BEGIN: cout << ">>> ";
	getline(cin, WCH_command);
	if (cin.eof()) {
		raise(SIGINT);
	}
	WCH_command_list = WCH_split(WCH_command);
	if ((int)WCH_command_list.size() == 0) {
		cout << endl;
		goto BEGIN;
	}
	transform(WCH_command_list[0].begin(), WCH_command_list[0].end(), WCH_command_list[0].begin(), ::tolower);
	WCH_InputTimes++;
	WCH_wait_cmd = false;
}

#endif