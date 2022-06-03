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
#include "basic.h"

extern const string WCH_WDName[7];
extern map <string, function <void ()>> WCH_command_support;
extern vector <string> WCH_command_list;
extern multimap <int, pair <int, string>> WCH_clock_list;
extern set <string> WCH_task_list;
extern set <string> WCH_work_list;
extern wstring WCH_window_title;
extern HWND WCH_hWnd;
extern HMENU WCH_hMenu;
extern int WCH_clock_num;
extern int WCH_task_num;
extern int WCH_work_num;
extern int WCH_clock_change;
extern int WCH_task_change;
extern int WCH_work_change;
extern int WCH_ProgressBarCount;
extern int WCH_ProgressBarTot;
extern int WCH_InputTimes;
extern bool WCH_cmd_line;
extern bool WCH_anti_idle;
extern bool WCH_program_end;
extern string WCH_command;
extern string WCH_ProgressBarStr;
extern ifstream fin;
extern ofstream fout;
WCH_Time WCH_GetTime();
void WCH_Sleep(int _ms);
void WCH_Error(int _in);
void WCH_printlog(string _mode, string _info);
void WCH_read();
bool WCH_save_func();
int WCH_GetNumDigits(int _n);

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

void WCH_message_loop() {
	// Message loop.
	HWND hwnd {};
	MSG msg {};
	WNDCLASS wndclass {};
	wndclass.style = CS_HREDRAW | CS_VREDRAW;
	wndclass.lpfnWndProc = WndProc;
	wndclass.cbClsExtra = 0;
	wndclass.cbWndExtra = 0;
	wndclass.hInstance = NULL;
	wndclass.hIcon = LoadIconW(NULL, IDI_APPLICATION);
	wndclass.hCursor = LoadCursorW(NULL, IDC_ARROW);
	wndclass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wndclass.lpszMenuName = NULL;
	wndclass.lpszClassName = WCH_window_title.c_str();
	if (!RegisterClassW(&wndclass)) {
		MessageBoxW(NULL, L"This program requires Windows NT!", WCH_window_title.c_str(), MB_ICONERROR);
		exit(0);
	}
	hwnd = CreateWindowExW(WS_EX_TOOLWINDOW, WCH_window_title.c_str(), WCH_window_title.c_str(), WS_POPUP, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, NULL, NULL, NULL, NULL);
	ShowWindow(hwnd, SW_HIDE);
	UpdateWindow(hwnd);
	while (GetMessageW(&msg, NULL, 0, 0)) {
		TranslateMessage(&msg);
		DispatchMessageW(&msg);
	}
}

void WCH_CL_Init() {
	// Initialize the command line.
	BEGIN: cout << ">>> ";
	getline(cin, WCH_command);
	if (cin.eof()) {
		raise(SIGINT);
	}
	WCH_command_list = WCH_split(WCH_command);
	if ((int)WCH_command_list.size() == 0) {
		cout << endl << endl;
		goto BEGIN;
	}
	transform(WCH_command_list[0].begin(), WCH_command_list[0].end(), WCH_command_list[0].begin(), ::tolower);
	WCH_InputTimes++;
}

#endif