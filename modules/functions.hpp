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
extern const wstring WCH_progress_bar;
extern const wstring WCH_path_data;
extern const wstring WCH_path_temp;
extern vector<wstring> WCH_command_list;
extern set<tuple<int32_t, int32_t, wstring>> WCH_clock_list;
extern set<wstring> WCH_task_list;
extern set<pair<wstring, wstring>> WCH_work_list;
extern wstring WCH_window_title;
extern HWND WCH_window_handle;
extern HWND WCH_tray_handle;
extern HMENU WCH_menu_handle;
extern NOTIFYICONDATAW WCH_NID;
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

void WCH_message_loop() {
	// Message loop.
	MSG msg = {};
	WNDCLASS wndclass = {};
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

void WCH_auto_save_loop() {
	// Check if the data files are changed and save them.
	while (!WCH_program_end) {
		WCH_Sleep(stoi(StrToWstr(WCH_Settings["AutoSaveInterval"].asString())));
		WCH_save_func(false);
	}
}

void WCH_check_task_loop() {
	// Check if the running task is in the task list. (Another thread)
	while (WCH_anti_idle && !WCH_program_end) {
		for (auto it = WCH_task_list.begin(); it != WCH_task_list.end(); it++) {
			if (WCH_TaskKill(*it)) {
				WCH_printlog(WCH_LOG_STATUS_INFO, L"Successfully killed \"" + *it + L"\"");
			} else {
				WCH_printlog(WCH_LOG_STATUS_INFO, L"Failed to kill \"" + *it + L"\"");
			}
		}
		WCH_Sleep(stoi(StrToWstr(WCH_Settings["AntiIdleKillInterval"].asString())));
	}
}

void WCH_CL_Init() {
	// Initialize the command line.
BEGIN:
	wstring _in;
	wcout << StrToWstr(WCH_Settings["CommandPrompt"].asString()) + L" ";
	getline(wcin, _in);
	if (wcin.eof()) {
		raise(SIGINT);
	}
	WCH_command_list = WCH_split(_in);
	if (WCH_command_list.size() == 0) {
		wcout << endl;
		goto BEGIN;
	}
	transform(WCH_command_list[0].begin(), WCH_command_list[0].end(), WCH_command_list[0].begin(), ::tolower);
}

#endif