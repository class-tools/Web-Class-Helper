/*
Web Class Helper Initialization Module Header File 2.0.1
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
extern map <wstring, function <void ()>> WCH_command_support;
extern vector <wstring> WCH_command_list;
extern multimap <int, pair <int, wstring>> WCH_clock_list;
extern set <wstring> WCH_task_list;
extern set <pair <wstring, wstring>> WCH_work_list;
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
extern wstring WCH_command;
extern wstring WCH_ProgressBarStr;
extern ifstream fin;
extern wifstream wfin;
extern ofstream fout;
extern wofstream wfout;
extern Json::StreamWriterBuilder bui;
WCH_Time WCH_GetTime();
void WCH_Sleep(int _ms);
void WCH_printlog(wstring _mode, wstring _info);
void WCH_read();
bool WCH_save_func();
int WCH_GetNumDigits(int _n);

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
	WCH_window_title = L"Web Class Helper";
	#if ALPHA == TRUE
	WCH_window_title += L" Internal Preview";
	#endif
	WCH_window_title += L" (x" + to_wstring(WCH_DisplayFramework) + L")";
	WCH_hWnd = GetForegroundWindow();
	WCH_ProgressBarStr = IsWindows10OrGreater() ? L"━" : L"-";
	bui.settings_ = []() {
		Json::Value def;
		Json::StreamWriterBuilder::setDefaults(&def);
		def["emitUTF8"] = true;
		return def;
	} ();
	wcin.imbue(locale("chs"));
	wcout.imbue(locale("chs"));
	wfin.imbue(locale("chs"));
	wfout.imbue(locale("chs"));
}

int WCH_Init_Log() {
	// Initialization for log.
	int returnVal = 0;
	WCH_Time now = WCH_GetTime();
	Json::Value val;
	Json::Reader rea;
	unique_ptr <Json::StreamWriter> sw(bui.newStreamWriter());
	fin.open(L"settings.json");
	if (_waccess(L"logs/latest.log", 0) != -1) {
		if (rea.parse(fin, val)) {
			returnVal = _wrename(L"logs/latest.log", format(L"logs/{}.log", StrToWstr(val["Start"].asString())).c_str());
		} else {
			returnVal = _wrename(L"logs/latest.log", L"logs/00000000000000.log");
		}
	}
	fin.close();
	val["Start"] = WstrToStr(format(L"{:04}{:02}{:02}{:02}{:02}{:02}", now.Year, now.Month, now.Day, now.Hour, now.Minute, now.Second));
	fout.open(L"settings.json");
	sw -> write(val, &fout);
	fout.close();
	WCH_printlog(WCH_LOG_STATUS_INFO, L"Starting \"" + WCH_window_title + L"\"");
	return returnVal == -1;
}

void WCH_Init_Win() {
	// Initialization for window.
	if (FindWindowW(NULL, WCH_window_title.c_str()) != NULL) {
		MessageBoxW(NULL, L"Application is already running.\nQuiting...", WCH_window_title.c_str(), MB_ICONERROR);
		WCH_printlog(WCH_LOG_STATUS_WARN, L"Application is already running");
		exit(0);
	}
	SetConsoleTitleW(WCH_window_title.c_str());
}

void WCH_Init_Bind() {
	// Initialization for bind.
	atexit(WCH_quit);
	WCH_signalHandler();
}

void WCH_Init_Cmd() {
	// Initialization for command support list.
	WCH_command_support.insert(make_pair(L"clock", WCH_check_clock));
	WCH_command_support.insert(make_pair(L"task", WCH_check_task));
	WCH_command_support.insert(make_pair(L"work", WCH_check_work));
	WCH_command_support.insert(make_pair(L"help", WCH_help));
	WCH_command_support.insert(make_pair(L"ow", WCH_ow));
	WCH_command_support.insert(make_pair(L"hide", WCH_hide));
	WCH_command_support.insert(make_pair(L"game", WCH_game));
	WCH_command_support.insert(make_pair(L"time", WCH_time));
	WCH_command_support.insert(make_pair(L"pi", WCH_pi));
	WCH_command_support.insert(make_pair(L"speedtest", WCH_speedtest));
	WCH_command_support.insert(make_pair(L"trans", WCH_trans));
	WCH_command_support.insert(make_pair(L"anti-idle", WCH_anti_idle_func));
	WCH_command_support.insert(make_pair(L"update", WCH_update));
	WCH_command_support.insert(make_pair(L"wiki", WCH_wiki));
	WCH_command_support.insert(make_pair(L"license", WCH_license));
	WCH_command_support.insert(make_pair(L"clear", WCH_clear));
	WCH_command_support.insert(make_pair(L"save", WCH_save_cmd));
	WCH_command_support.insert(make_pair(L"quit", WCH_quit));
}

void WCH_Init_Out() {
	// Initialization for output.
	wcout << WCH_window_title << endl;
	wcout << L"Copyright (c) 2022 Class Tools Develop Team." << endl;
	wcout << L"Type \"help\", \"update\" or \"license\" for more information." << endl;
	wcout << endl;
}

void WCH_Init() {
	// Initialize the whole program.
	#if ALPHA == TRUE
	WCH_SetWindowStatus(false);
	if (MessageBoxW(NULL, (L"This version of the program is only used for internal testing and is strictly prohibited to be transmitted externally.\nAre you sure you want to start the program?\nCompile time: " + WCH_GetCompileTime()).c_str(), L"WCH WARN", MB_ICONWARNING | MB_YESNO) == IDNO) {
		WCH_command_list.clear();
		WCH_command_list.push_back(L"quit");
		WCH_quit();
	} else {
		WCH_SetWindowStatus(true);
	}
	#endif
	WCH_Init_Dir();
	WCH_Init_Var();
	if (WCH_Init_Log()) {
		raise(SIGABRT);
	}
	WCH_Init_Win();
	WCH_Init_Bind();
	WCH_Init_Cmd();
	WCH_read();
	WCH_SetWindowStatus(true);
	thread T1(WCH_check_clock_loop);
	T1.detach();
	thread T2(WCH_safety_input_loop);
	T2.detach();
	thread T3(WCH_message_loop);
	T3.detach();
	WCH_Init_Out();
}

#endif