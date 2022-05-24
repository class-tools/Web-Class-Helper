/*
Web Class Helper Initialization Module Header File 2.0.1
This source code file is under MIT License.
Copyright (c) 2022 Class Tools Develop Team
Contributors: jsh-jsh ren-yc hjl2011
*/
#ifndef INIT_H
#define INIT_H
#include "functions.h"
#include "file-process.h"
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

int WCH_Init_Log() {
	// Initialization for log.
	int returnVal = 0;
	WCH_Time now = WCH_GetTime();
	if (_waccess(L"logs/latest.log", 0) != -1) {
		returnVal = _wrename(L"logs/latest.log", format(L"logs/{:04}{:02}{:02}{:02}{:02}{:02}.log", now.Year, now.Month, now.Day, now.Hour, now.Minute, now.Second).c_str());
	}
	WCH_printlog(WCH_LOG_STATUS_INFO, "Starting \"Web Class Helper (x" + to_string(WCH_DisplayFramework) + ")\"");
	return returnVal;
}

void WCH_Init_Win() {
	// Initialization for window.
	SetConsoleTitleW((L"Web Class Helper (x" + to_wstring(WCH_DisplayFramework) + L")").c_str());
}

void WCH_Init_Bind() {
	// Initialization for bind.
	atexit(WCH_save);
	WCH_signalHandler();
	WCH_SetWindowStatus(true);
}

void WCH_Init_Cmd() {
	// Initialization for command support list.
	WCH_command_support.insert(make_pair("clock", WCH_check_clock));
	WCH_command_support.insert(make_pair("task", WCH_check_task));
	WCH_command_support.insert(make_pair("work", WCH_check_work));
	WCH_command_support.insert(make_pair("help", WCH_help));
	WCH_command_support.insert(make_pair("ow", WCH_ow));
	WCH_command_support.insert(make_pair("hide", WCH_hide));
	WCH_command_support.insert(make_pair("game", WCH_game));
	WCH_command_support.insert(make_pair("time", WCH_time));
	WCH_command_support.insert(make_pair("pi", WCH_pi));
	WCH_command_support.insert(make_pair("speedtest", WCH_speedtest));
	WCH_command_support.insert(make_pair("trans", WCH_trans));
	WCH_command_support.insert(make_pair("anti-idle", WCH_anti_idle_func));
	WCH_command_support.insert(make_pair("update", WCH_update));
	WCH_command_support.insert(make_pair("wiki", WCH_wiki));
	WCH_command_support.insert(make_pair("license", WCH_license));
	WCH_command_support.insert(make_pair("quit", WCH_quit));
	WCH_command_support.insert(make_pair("clear", WCH_clear));
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
	if (WCH_Init_Log() == -1) {
		raise(SIGABRT);
	}
	WCH_Init_Win();
	WCH_Init_Bind();
	WCH_Init_Cmd();
	WCH_read();
	thread T1(WCH_check_clock_loop);
	T1.detach();
	thread T2(WCH_safety_input_loop);
	T2.detach();
	WCH_Init_Out();
}

#endif