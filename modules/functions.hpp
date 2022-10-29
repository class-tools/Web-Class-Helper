/*
Web Class Helper Functions Module Header File 2.1.1
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

extern const array<wstring, 7> WCH_weekday_list;
extern const array<wstring, 2> WCH_language_list;
extern const map<wstring, function<void()>> WCH_command_support;
extern const set<tuple<wstring, wstring, wstring>> WCH_settings_support;
extern const set<wstring> WCH_language_support;
extern vector<wstring> WCH_command_list;
extern set<tuple<int32_t, int32_t, wstring>> WCH_clock_list;
extern set<wstring> WCH_task_list;
extern set<pair<wstring, wstring>> WCH_work_list;
extern wstring WCH_window_title;
extern wstring WCH_command;
extern wstring WCH_ProgressBarStr;
extern HWND WCH_window_handle;
extern HWND WCH_tray_handle;
extern HMENU WCH_menu_handle;
extern NOTIFYICONDATA WCH_NID;
extern ATL::CComPtr<ITaskbarList3> WCH_TBL;
extern Json::Value WCH_Settings;
extern Json::Value WCH_Language;
extern int32_t WCH_clock_num;
extern int32_t WCH_task_num;
extern int32_t WCH_work_num;
extern int32_t WCH_clock_change;
extern int32_t WCH_task_change;
extern int32_t WCH_work_change;
extern int32_t WCH_settings_change;
extern int32_t WCH_ProgressBarTot;
extern int32_t WCH_InputTimes;
extern bool WCH_cmd_line;
extern bool WCH_anti_idle;
extern bool WCH_count_down;
extern bool WCH_program_end;
extern bool WCH_pre_start;
extern ifstream fin;
extern wifstream wfin;
extern ofstream fout;
extern wofstream wfout;
extern Json::Reader JSON_Reader;
extern Json::StreamWriterBuilder JSON_SWB;
extern unique_ptr<Json::StreamWriter> JSON_SW;
WCH_Time WCH_GetTime();
void WCH_Sleep(int32_t _ms);
void WCH_printlog(wstring _mode, wstring _info);
void WCH_read_settings();
void WCH_read();
void WCH_save_settings();
bool WCH_save_func(bool output);
size_t WCH_GetNumDigits(size_t _n);

void WCH_check_clock_loop() {
	// Check if the time equals to the clock. (Another thread)
	WCH_Sleep((60 - WCH_GetTime().Second) * 1000);
	while (!WCH_program_end) {
		WCH_Time NOW = WCH_GetTime();
		for (auto it = WCH_clock_list.begin(); it != WCH_clock_list.end(); it++) {
			if (get<0>(*it) == NOW.Hour && get<1>(*it) == NOW.Minute && get<2>(*it).size() > 0) {
				wcout << L"\a";
				bool _tmp = WCH_cmd_line;
				if (_tmp) {
					WCH_SetWindowStatus(false);
				}
				MessageBoxW(NULL, (get<2>(*it)).c_str(), L"WCH CLOCK", MB_OK | MB_TOPMOST);
				if (!WCH_anti_idle || _tmp) {
					WCH_SetWindowStatus(true);
				}
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
	RegisterClassW(&wndclass);
	WCH_tray_handle = CreateWindowExW(WS_EX_TOOLWINDOW, WCH_window_title.c_str(), WCH_window_title.c_str(), WS_POPUP, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, NULL, NULL, NULL, NULL);
	ShowWindow(WCH_tray_handle, SW_HIDE);
	UpdateWindow(WCH_tray_handle);
	RegisterHotKey(WCH_tray_handle, WCH_HOTKEY_SHOW, MOD_CONTROL, VK_DOWN);
	while (GetMessageW(&msg, NULL, 0, 0)) {
		TranslateMessage(&msg);
		DispatchMessageW(&msg);
	}
}

void WCH_AutoSave_loop() {
	// Check if the data files are changed and save them.
	while (!WCH_program_end) {
		WCH_Sleep(stoi(StrToWstr(WCH_Settings["AutoSaveTime"].asString())));
		WCH_save_func(false);
	}
}

void WCH_CL_Init() {
	// Initialize the command line.
BEGIN:
	wcout << StrToWstr(WCH_Settings["CommandPrompt"].asString()) + L" ";
	getline(wcin, WCH_command);
	if (wcin.eof()) {
		raise(SIGINT);
	}
	WCH_command_list = WCH_split(WCH_command);
	if (WCH_command_list.size() == 0) {
		wcout << endl;
		goto BEGIN;
	}
	transform(WCH_command_list[0].begin(), WCH_command_list[0].end(), WCH_command_list[0].begin(), ::tolower);
	WCH_InputTimes++;
}

#endif