/*
Web Class Helper Initialization Module Header File 2.1.0
This source code file is under MIT License.
Copyright (c) 2022 Class Tools Develop Team
Contributors: jsh-jsh ren-yc hjl2011
*/
#ifndef INIT_H
#define INIT_H
#include "functions.hpp"
#include "file-process.hpp"
#include "commands.hpp"
#include "apis.hpp"
#include "basic.hpp"

extern const wstring WCH_WDName[7];
extern map<wstring, function<void()>> WCH_command_support;
extern set<tuple<wstring, wstring, wstring>> WCH_settings_support;
extern vector<wstring> WCH_command_list;
extern set<tuple<int, int, wstring>> WCH_clock_list;
extern set<wstring> WCH_task_list;
extern set<pair<wstring, wstring>> WCH_work_list;
extern wstring WCH_window_title;
extern wstring WCH_command;
extern wstring WCH_ProgressBarStr;
extern HWND WCH_Win_hWnd;
extern HWND WCH_Tray_hWnd;
extern HMENU WCH_hMenu;
extern NOTIFYICONDATA WCH_NID;
extern ATL::CComPtr<ITaskbarList3> WCH_TBL;
extern Json::Value WCH_Settings;
extern int WCH_clock_num;
extern int WCH_task_num;
extern int WCH_work_num;
extern int WCH_clock_change;
extern int WCH_task_change;
extern int WCH_work_change;
extern int WCH_settings_change;
extern int WCH_ProgressBarTot;
extern int WCH_InputTimes;
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
void WCH_Sleep(int _ms);
void WCH_printlog(wstring _mode, wstring _info);
void WCH_read_settings();
void WCH_read();
void WCH_save_settings();
bool WCH_save_func(bool output);
size_t WCH_GetNumDigits(size_t _n);

void WCH_Init_Bind() {
	// Initialization for bind.
	atexit(WCH_quit);
	WCH_signalHandler();
	CoInitializeEx(nullptr, COINIT_APARTMENTTHREADED | COINIT_DISABLE_OLE1DDE);
	WCH_TBL.CoCreateInstance(CLSID_TaskbarList);
}

void WCH_Init_Dir() {
	// Initialization for directory.
	if (_waccess(L"data", 0) != 0) {
		CreateDirectoryW(L"data", NULL);
	}
	if (_waccess(L"logs", 0) != 0) {
		CreateDirectoryW(L"logs", NULL);
	}
}

void WCH_Init_Var() {
	// Initialization for varible.
	WCH_window_title = L"Web Class Helper ";
	WCH_window_title.append(WCH_VER_MAIN);
#if WCH_VER_TYPE == 1
	WCH_window_title.append(L" Internal Preview");
#endif
#if WCH_VER_TYPE == 2
	WCH_window_title.append(L" Public Preview");
#endif
	WCH_window_title.append(L" (");
	WCH_window_title.append(WCH_Framework);
	WCH_window_title.append(L")");
	WCH_Win_hWnd = GetConsoleWindow();
	WCH_ProgressBarStr = IsWindows10OrGreater() ? L"━" : L"-";
	JSON_SWB.settings_ = []() {
		Json::Value def;
		Json::StreamWriterBuilder::setDefaults(&def);
		def["emitUTF8"] = true;
		return def;
	}();
	_setmode(_fileno(stdin), _O_WTEXT);
	_setmode(_fileno(stdout), _O_WTEXT);
	wfin.imbue(locale(".UTF-8", LC_CTYPE));
	wfout.imbue(locale(".UTF-8", LC_CTYPE));
}

void WCH_Init_Ver() {
	// Initialization for version.
	wstring vertype = L"";
#if WCH_VER_TYPE == 1
	vertype = L"Internal Preview";
#endif
#if WCH_VER_TYPE == 2
	vertype = L"Public Preview";
#endif
#if WCH_VER_TYPE == 1 || WCH_VER_TYPE == 2
	WCH_SetWindowStatus(false);
	if (MessageBoxW(NULL, (L"This " + vertype + L" of the program is only used for testing.\nAre you sure you want to start the program?\nCompile time: " + WCH_GetCompileTime()).c_str(), L"WCH WARN", MB_ICONWARNING | MB_YESNO | MB_TOPMOST) == IDNO) {
		_exit(0);
	}
	WCH_SetWindowStatus(true);
#endif
}

void WCH_Init_Invar() {
	// Initialization for invariable lists.
	WCH_command_support.insert(make_pair(L"clock", WCH_check_clock));
	WCH_command_support.insert(make_pair(L"task", WCH_check_task));
	WCH_command_support.insert(make_pair(L"work", WCH_check_work));
	WCH_command_support.insert(make_pair(L"help", WCH_help));
	WCH_command_support.insert(make_pair(L"ow", WCH_ow));
	WCH_command_support.insert(make_pair(L"hide", WCH_hide));
	WCH_command_support.insert(make_pair(L"game", WCH_game));
	WCH_command_support.insert(make_pair(L"time", WCH_time));
	WCH_command_support.insert(make_pair(L"pi", WCH_pi));
	WCH_command_support.insert(make_pair(L"trans", WCH_trans));
	WCH_command_support.insert(make_pair(L"fate", WCH_fate));
	WCH_command_support.insert(make_pair(L"anti-idle", WCH_anti_idle_func));
	WCH_command_support.insert(make_pair(L"count-down", WCH_count_down_func));
	WCH_command_support.insert(make_pair(L"update", WCH_update));
	WCH_command_support.insert(make_pair(L"wiki", WCH_wiki));
	WCH_command_support.insert(make_pair(L"license", WCH_license));
	WCH_command_support.insert(make_pair(L"clear", WCH_clear));
	WCH_command_support.insert(make_pair(L"set", WCH_set));
	WCH_command_support.insert(make_pair(L"save", WCH_save_cmd));
	WCH_command_support.insert(make_pair(L"quit", WCH_quit));
	WCH_settings_support.insert(make_tuple(L"AutoSave", L"Boolean", L"True"));
	WCH_settings_support.insert(make_tuple(L"AutoSaveTime", L"Number", L"60000"));
	WCH_settings_support.insert(make_tuple(L"CountDownSoundPrompt", L"Boolean", L"False"));
	WCH_settings_support.insert(make_tuple(L"ScreenshotSavePath", L"String", L"C:\\Users\\" + WCH_GetUserName() + L"\\Pictures\\"));
}

void WCH_Init_Log() {
	// Initialization for log.
	WCH_Time now = WCH_GetTime();
	WCH_read_settings();
	if (_waccess(L"logs/latest.log", 0) != -1) {
		_wrename(L"logs/latest.log", format(L"logs/{}.log", StrToWstr(WCH_Settings["StartTime"].asString())).c_str());
	}
	WCH_Settings["StartTime"] = WstrToStr(format(L"{:04}{:02}{:02}{:02}{:02}{:02}", now.Year, now.Month, now.Day, now.Hour, now.Minute, now.Second));
	WCH_save_settings();
	WCH_pre_start = false;
	WCH_printlog(WCH_LOG_STATUS_INFO, L"Starting \"" + WCH_window_title + L"\"");
}

void WCH_Init_Win() {
	// Initialization for window.
	if (FindWindowW(NULL, WCH_window_title.c_str()) != NULL) {
		WCH_TBL->SetProgressState(WCH_Win_hWnd, TBPF_INDETERMINATE);
		MessageBoxW(NULL, L"Application is already running.\nQuiting...", WCH_window_title.c_str(), MB_ICONERROR | MB_TOPMOST);
		WCH_TBL->SetProgressState(WCH_Win_hWnd, TBPF_NOPROGRESS);
		WCH_printlog(WCH_LOG_STATUS_WARN, L"Application is already running");
		exit(0);
	}
	SetConsoleTitleW(WCH_window_title.c_str());
	WCH_TBL->SetProgressState(WCH_Win_hWnd, TBPF_NOPROGRESS);
}

void WCH_Init_Loop() {
	// Initialization for multiple thread loop.
	thread T1(WCH_check_clock_loop);
	T1.detach();
	thread T2(WCH_safety_input_loop);
	T2.detach();
	thread T3(WCH_message_loop);
	T3.detach();
	if (StrToWstr(WCH_Settings["AutoSave"].asString()) == L"True") {
		thread T4(WCH_AutoSave_loop);
		T4.detach();
	}
}

void WCH_Init() {
	// Initialize the whole program.
	WCH_Init_Bind();
	WCH_Init_Dir();
	WCH_Init_Var();
	WCH_Init_Ver();
	WCH_Init_Invar();
	WCH_Init_Log();
	WCH_Init_Win();
	_wsystem(L"CHCP 65001 > NUL");
	WCH_read();
	WCH_SetWindowStatus(true);
	WCH_Init_Loop();
	wcout << WCH_window_title << endl;
	wcout << L"Copyright (c) 2022 Class Tools Develop Team." << endl;
	wcout << L"Type \"help\", \"update\" or \"license\" for more information." << endl;
	wcout << endl;
}

#endif