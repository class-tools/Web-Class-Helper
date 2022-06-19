/*
Web Class Helper Functions Module Header File 2.0.1
This source code file is under MIT License.
Copyright (c) 2022 Class Tools Develop Team
Contributors: jsh-jsh ren-yc
*/
#ifndef FUNCTIONS_H
#define FUNCTIONS_H
#include "file-process.hpp"
#include "init.hpp"
#include "commands.hpp"
#include "apis.hpp"
#include "basic.hpp"

extern const wstring WCH_WDName[7];
extern map <wstring, function <void ()>> WCH_command_support;
extern vector <wstring> WCH_command_list;
extern set <tuple <int, int, wstring>> WCH_clock_list;
extern set <wstring> WCH_task_list;
extern set <pair <wstring, wstring>> WCH_work_list;
extern wstring WCH_window_title;
extern HWND WCH_Win_hWnd;
extern HWND WCH_Tray_hWnd;
extern HMENU WCH_hMenu;
extern NOTIFYICONDATA WCH_NID;
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
extern bool WCH_pre_start;
extern wstring WCH_command;
extern wstring WCH_ProgressBarStr;
extern ifstream fin;
extern wifstream wfin;
extern ofstream fout;
extern wofstream wfout;
extern Json::StreamWriterBuilder Json_SWB;
WCH_Time WCH_GetTime();
void WCH_Sleep(int _ms);
void WCH_printlog(wstring _mode, wstring _info);
void WCH_read();
bool WCH_save_func();
int WCH_GetNumDigits(int _n);

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
		for (auto it = WCH_clock_list.begin(); it != WCH_clock_list.end(); it++) {
			if (get <0> (*it) == NOW.Hour && get <1> (*it) == NOW.Minute && get <2> (*it).size() > 0) {
				wcout << L"\a";
				MessageBoxW(NULL, (get <2> (*it)).c_str(), L"WCH CLOCK", MB_OK);
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
	WCH_Tray_hWnd = CreateWindowExW(WS_EX_TOOLWINDOW, WCH_window_title.c_str(), WCH_window_title.c_str(), WS_POPUP, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, NULL, NULL, NULL, NULL);
	ShowWindow(WCH_Tray_hWnd, SW_HIDE);
	UpdateWindow(WCH_Tray_hWnd);
	RegisterHotKey(WCH_Tray_hWnd, WCH_HOTKEY_SHOW, MOD_CONTROL, VK_DOWN);
	while (GetMessageW(&msg, NULL, 0, 0)) {
		TranslateMessage(&msg);
		DispatchMessageW(&msg);
	}
}

void WCH_CL_Init() {
	// Initialize the command line.
	BEGIN: wcout << L">>> ";
	getline(wcin, WCH_command);
	if (wcin.eof()) {
		raise(SIGINT);
	}
	WCH_command_list = WCH_split(WCH_command);
	if ((int)WCH_command_list.size() == 0) {
		wcout << endl;
		goto BEGIN;
	}
	transform(WCH_command_list[0].begin(), WCH_command_list[0].end(), WCH_command_list[0].begin(), ::tolower);
	WCH_InputTimes++;
}

#endif