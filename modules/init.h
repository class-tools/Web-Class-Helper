/*
Web Class Helper Initialization Module Header File 2.0.0
This source code file is under MIT License.
Copyright (c) 2022 Class Tools Develop Team
Contributors: jsh-jsh ren-yc
*/
#ifndef INIT_H
#define INIT_H
#include <cassert>
#include <cctype>
#include <cerrno>
#include <cfloat>
#include <ciso646>
#include <climits>
#include <clocale>
#include <cmath>
#include <csetjmp>
#include <csignal>
#include <cstdarg>
#include <cstddef>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <ctime>
#include <cwchar>
#include <cwctype>
#include <ccomplex>
#include <cfenv>
#include <cinttypes>
#include <cstdalign>
#include <cstdbool>
#include <cstdint>
#include <ctgmath>
#include <cuchar>
#include <algorithm>
#include <bitset>
#include <complex>
#include <deque>
#include <exception>
#include <fstream>
#include <functional>
#include <iomanip>
#include <ios>
#include <iosfwd>
#include <iostream>
#include <istream>
#include <iterator>
#include <limits>
#include <list>
#include <locale>
#include <map>
#include <memory>
#include <new>
#include <numeric>
#include <ostream>
#include <queue>
#include <set>
#include <sstream>
#include <stack>
#include <stdexcept>
#include <streambuf>
#include <string>
#include <typeinfo>
#include <utility>
#include <valarray>
#include <vector>
#include <array>
#include <atomic>
#include <chrono>
#include <codecvt>
#include <condition_variable>
#include <forward_list>
#include <future>
#include <initializer_list>
#include <mutex>
#include <random>
#include <ratio>
#include <regex>
#include <scoped_allocator>
#include <system_error>
#include <thread>
#include <tuple>
#include <typeindex>
#include <type_traits>
#include <unordered_map>
#include <unordered_set>
#include <shared_mutex>
#include <any>
#include <charconv>
#include <filesystem>
#include <optional>
#include <memory_resource>
#include <string_view>
#include <variant>
#include <bit>
#include <compare>
#include <concepts>
#include <coroutine>
#include <numbers>
#include <ranges>
#include <span>
#include <stop_token>
#include <version>
#include <io.h>
#include <windows.h>
#include <wininet.h>
#include <tlhelp32.h>
#include <conio.h>
#include <direct.h>
#include <VersionHelpers.h>
#include "functions.h"
#include "file-process.h"
#include "commands.h"
#include "apis.h"
#include "variables.h"
using namespace std;

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
extern wifstream wfin;
extern wofstream wfout;
WCH_Time WCH_GetTime();
void WCH_Sleep(int _ms);
void WCH_Error(string INFO);
void WCH_printlog(int w, initializer_list <string> other);
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
	WCH_Time now = WCH_GetTime();
	if (_access("logs/latest.log", 0) != -1) {
		return rename("logs/latest.log", format("logs/{:04}{:02}{:02}{:02}{:02}{:02}.log", now.Year, now.Month, now.Day, now.Hour, now.Minute, now.Second).c_str());
	} else {
		return 0;
	}
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

void WCH_Init_Cmd() {
	// Initialization for command support list.
	WCH_command_support.insert(pair <string, function <void ()>> ("clock", WCH_check_clock));
	WCH_command_support.insert(pair <string, function <void ()>> ("task", WCH_check_task));
	WCH_command_support.insert(pair <string, function <void ()>> ("work", WCH_check_work));
	WCH_command_support.insert(pair <string, function <void ()>> ("help", WCH_help));
	WCH_command_support.insert(pair <string, function <void ()>> ("ow", WCH_ow));
	WCH_command_support.insert(pair <string, function <void ()>> ("hide", WCH_hide));
	WCH_command_support.insert(pair <string, function <void ()>> ("game", WCH_game));
	WCH_command_support.insert(pair <string, function <void ()>> ("time", WCH_time));
	WCH_command_support.insert(pair <string, function <void ()>> ("pi", WCH_pi));
	WCH_command_support.insert(pair <string, function <void ()>> ("speedtest", WCH_speedtest));
	WCH_command_support.insert(pair <string, function <void ()>> ("trans", WCH_trans));
	WCH_command_support.insert(pair <string, function <void ()>> ("anti-idle", WCH_anti_idle_func));
	WCH_command_support.insert(pair <string, function <void ()>> ("update", WCH_update));
	WCH_command_support.insert(pair <string, function <void ()>> ("wiki", WCH_wiki));
	WCH_command_support.insert(pair <string, function <void ()>> ("license", WCH_license));
	WCH_command_support.insert(pair <string, function <void ()>> ("quit", WCH_quit));
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