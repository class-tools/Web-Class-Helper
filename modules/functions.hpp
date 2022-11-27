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

extern const vector<wstring> WCH_list_weekday;
extern const map<wstring, set<wstring>> WCH_choice_settings;
extern const map<wstring, wstring> WCH_MIME_list;
extern const map<wstring, function<void()>> WCH_support_command;
extern const set<tuple<wstring, wstring, wstring, bool>> WCH_support_settings;
extern const set<wstring> WCH_support_language;
extern const wstring WCH_progress_bar_str;
extern const wstring WCH_path_data;
extern const wstring WCH_path_temp;
extern vector<wstring> WCH_list_command;
extern set<tuple<int32_t, int32_t, wstring>> WCH_list_clock;
extern set<wstring> WCH_list_task;
extern set<pair<wstring, wstring>> WCH_list_work;
extern wstring WCH_title_window;
extern HWND WCH_handle_window;
extern HWND WCH_handle_tray;
extern HMENU WCH_handle_menu;
extern NOTIFYICONDATAW WCH_NID;
extern ATL::CComPtr<ITaskbarList3> WCH_TBL;
extern Json::Value WCH_Settings;
extern Json::Value WCH_Language;
extern int32_t WCH_num_clock;
extern int32_t WCH_num_task;
extern int32_t WCH_num_work;
extern int32_t WCH_progress_bar_duration;
extern bool WCH_cmd_line;
extern bool WCH_is_focus;
extern bool WCH_is_countdown;
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
		for (auto it = WCH_list_clock.begin(); it != WCH_list_clock.end(); it++) {
			if (get<0>(*it) == NOW.Hour && get<1>(*it) == NOW.Minute && get<2>(*it).size() > 0) {
				wcout << L"\a";
				bool _tmp = WCH_cmd_line;
				if (_tmp) {
					WCH_SetWindowStatus(false);
				}
				MessageBoxW(NULL, (get<2>(*it)).c_str(), L"WCH CLOCK", MB_OK | MB_TOPMOST);
				if (!WCH_is_focus || _tmp) {
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
	wndclass.lpszClassName = WCH_title_window.c_str();
	RegisterClassW(&wndclass);
	WCH_handle_tray = CreateWindowExW(WS_EX_TOOLWINDOW, WCH_title_window.c_str(), WCH_title_window.c_str(), WS_POPUP, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, NULL, NULL, NULL, NULL);
	ShowWindow(WCH_handle_tray, SW_HIDE);
	UpdateWindow(WCH_handle_tray);
	RegisterHotKey(WCH_handle_tray, WCH_HOTKEY_SHOW, MOD_CONTROL, VK_DOWN);
	while (GetMessageW(&msg, NULL, 0, 0)) {
		TranslateMessage(&msg);
		DispatchMessageW(&msg);
	}
}

void WCH_check_task_loop() {
	// Check if the running task is in the task list. (Another thread)
	while (WCH_is_focus && !WCH_program_end) {
		for (auto it = WCH_list_task.begin(); it != WCH_list_task.end(); it++) {
			if (WCH_TaskKill(*it)) {
				WCH_printlog(WCH_LOG_STATUS_INFO, L"Successfully killed \"" + *it + L"\"");
			} else {
				WCH_printlog(WCH_LOG_STATUS_INFO, L"Failed to kill \"" + *it + L"\"");
			}
		}
		WCH_Sleep(stoi(StrToWstr(WCH_Settings["FocusKillInterval"].asString())));
	}
}

void WCH_CL_Init() {
	// Initialize the command line.
BEGIN:
	wstring _in;
	wcout << StrToWstr(WCH_Settings["CommandPrompt"].asString()) << L" ";
	getline(wcin, _in);
	if (wcin.eof()) {
		raise(SIGINT);
	}
	WCH_list_command = WCH_split(_in);
	if (WCH_list_command.size() == 0) {
		wcout << endl;
		goto BEGIN;
	}
	transform(WCH_list_command[0].begin(), WCH_list_command[0].end(), WCH_list_command[0].begin(), ::tolower);
}

#endif