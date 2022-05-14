/*
Web Class Helper Commands Module Header File 2.0.0
This source code file is under MIT License.
Copyright (c) 2022 Class Tools Develop Team
Contributors: jsh-jsh ren-yc
*/
#ifndef COMMANDS_H
#define COMMANDS_H
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
#include "file-process.h"
#include "init.h"
#include "functions.h"
#include "apis.h"
#include "variables.h"
using namespace std;

#ifdef URLDownloadToFile
#undef URLDownloadToFile
#endif

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

typedef int(__stdcall *UDF)(LPVOID, LPCSTR, LPCSTR, DWORD, LPVOID);
UDF URLDownloadToFile = (UDF)(int*)GetProcAddress(LoadLibrary(L"urlmon.dll"), "URLDownloadToFileA");

void WCH_quit() {
	// Quit.
	if ((int)WCH_command_list.size() != 1) {
		WCH_Error(WCH_ERRNO_UNCORRECT);
		return;
	}
	WCH_program_end = true;
	exit(0);
}

void WCH_hide() {
	// Hide the window.
	if ((int)WCH_command_list.size() != 1) {
		WCH_Error(WCH_ERRNO_UNCORRECT);
		return;
	}
	WCH_SetWindowStatus(false);
}

void WCH_wiki() {
	// Visit the wiki page.
	if ((int)WCH_command_list.size() != 1) {
		WCH_Error(WCH_ERRNO_UNCORRECT);
		return;
	}
	cout << "Jumping to wiki page..." << endl;
	system("start resources/website/wiki.url");
}

void WCH_update() {
	// Visit the website to update the program.
	if ((int)WCH_command_list.size() != 1) {
		WCH_Error(WCH_ERRNO_UNCORRECT);
		return;
	}
	try {
		cout << "Checking update..." << endl;
		WCH_ProgressBarTot = 5;
		thread T(WCH_ProgressBar);
		T.detach();
		string url = "https://class-tools.gq/update/WCH?";
		srand((unsigned)time(NULL));
		url += to_string(rand());
		URLDownloadToFile(0, url.c_str(), "WCH_UPD.tmp", 0, 0);
		string res;
		fin.open("WCH_UPD.tmp");
		getline(fin, res);
		fin.close();
		WCH_Sleep(5000);
		if (res == "") {
			throw runtime_error(WCH_ERRNO_NETWORK_FAILURE);
		}
		if (WCH_CheckVersion(WCH_GetVersion(WCH_VER), WCH_GetVersion(res))) {
			cout << "Program version is less than latest released version, jumping to releases page..." << endl;
			system("start resources/website/releases.url");
			WCH_printlog(WCH_LOG_MODE_UPD, {"Updating to version", res});
		} else {
			cout << "Program version equals or is greater than latest released version." << endl;
			WCH_printlog(WCH_LOG_MODE_UPD, {"Program version equals or is greater than", res});
		}
		DeleteFile(L"WCH_UPD.tmp");
	} catch (...) {
		WCH_Error(WCH_ERRNO_NETWORK_FAILURE);
		return;
	}
}

void WCH_license() {
	// Print the license.
	if ((int)WCH_command_list.size() != 1) {
		WCH_Error(WCH_ERRNO_UNCORRECT);
		return;
	}
	fin.open("LICENSE");
	if (!fin.is_open()) {
		WCH_Error(WCH_ERRNO_FILE_NOT_FOUND);
		return;
	}
	string _res;
	while (getline(fin, _res)) {
		cout << _res << endl;
	}
	fin.close();
}

void WCH_add_clock() {
	// Add a new clock.
	try {
		int h = stoi(WCH_command_list[2]);
		int m = stoi(WCH_command_list[3]);
		string Tname = WCH_command.substr(12 + (int)WCH_command_list[2].size() + (int)WCH_command_list[3].size(), WCH_command.size() - 1);
		if ((int)WCH_command_list.size() < 5 || h > 24 || m >= 60 || h < 1 || m < 0) {
			WCH_Error(WCH_ERRNO_UNCORRECT);
			return;
		} else {
			bool flag = false;
			for (auto it = WCH_clock_list.equal_range(h).first; it != WCH_clock_list.equal_range(h).second; it++) {
				if ((it -> second).first == m) {
					flag = true;
					break;
				}
			}
			if (!flag) {
				WCH_clock_num++;
				WCH_clock_list.emplace(make_pair(h, make_pair(m, Tname)));
			} else {
				WCH_Error(WCH_ERRNO_UNCORRECT);
			}
		}
	} catch (...) {
		WCH_Error(WCH_ERRNO_UNCORRECT);
	}
}

void WCH_delete_clock() {
	// Delete a clock.
	if ((int)WCH_command_list.size() < 5) {
		WCH_Error(WCH_ERRNO_UNCORRECT);
		return;
	}
	try {
		int h = stoi(WCH_command_list[2]);
		int m = stoi(WCH_command_list[3]);
		string Tname = WCH_command.substr(15 + (int)WCH_command_list[2].size() + (int)WCH_command_list[3].size(), WCH_command.size() - 1);
		bool flag = false;
		for (auto it = WCH_clock_list.equal_range(h).first; it != WCH_clock_list.equal_range(h).second; it++) {
			if ((it -> second).first == m && (it -> second).second == Tname) {
				WCH_clock_list.erase(it);
				flag = true;
				WCH_clock_num--;
				break;
			}
		}
		if (!flag) {
			WCH_Error(WCH_ERRNO_CLOCK_OPERATION);
		}
	} catch (...) {
		WCH_Error(WCH_ERRNO_CLOCK_OPERATION);
	}
}

void WCH_change_clock() {
	// Change a clock.
	if ((int)WCH_command_list.size() < 5) {
		WCH_Error(WCH_ERRNO_UNCORRECT);
		return;
	}
	try {
		int h = stoi(WCH_command_list[2]);
		int m = stoi(WCH_command_list[3]);
		string Tname = WCH_command.substr(15 + (int)WCH_command_list[2].size() + (int)WCH_command_list[3].size(), WCH_command.size() - 1);
		bool flag = false;
		for (auto it = WCH_clock_list.equal_range(h).first; it != WCH_clock_list.equal_range(h).second; it++) {
			if ((it -> second).first == m) {
				(it -> second).second = Tname;
				flag = true;
			}
		}
		if (!flag) {
			WCH_Error(WCH_ERRNO_CLOCK_OPERATION);
		}
	} catch (...) {
		WCH_Error(WCH_ERRNO_CLOCK_OPERATION);
	}
}

void WCH_clear_clock() {
	// Clear clock list.
	if ((int)WCH_command_list.size() != 2) {
		WCH_Error(WCH_ERRNO_UNCORRECT);
		return;
	}
	WCH_clock_list.clear();
	WCH_clock_num = 0;
}

void WCH_list_clock() {
	// List all tasks.
	if ((int)WCH_command_list.size() != 2) {
		WCH_Error(WCH_ERRNO_UNCORRECT);
		return;
	}
	int MAXH = -1;
	int MAXM = -1;
	int MAXT = -1;
	for (auto it = WCH_clock_list.begin(); it != WCH_clock_list.end(); it++) {
		MAXH = max(MAXH, WCH_GetNumDigits(it -> first));
		MAXM = max(MAXM, WCH_GetNumDigits((it -> second).first));
		MAXT = max(MAXT, (int)((it -> second).second).size());
	}
	if (MAXH == -1 && MAXM == -1 && MAXT == -1) {
		return;
	}
	MAXH = max(MAXH, 4);
	MAXM = max(MAXM, 6);
	MAXT = max(MAXT, 4);
	cout << "Hour";
	WCH_PrintChar(MAXH - 4, ' ');
	cout << " | Minute";
	WCH_PrintChar(MAXM - 6, ' ');
	cout << " | Name";
	WCH_PrintChar(MAXT - 4, ' ');
	cout << endl;
	WCH_PrintChar(MAXH, '-');
	cout << " | ";
	WCH_PrintChar(MAXM, '-');
	cout << " | ";
	WCH_PrintChar(MAXT, '-');
	cout << endl;
	for (int i = 0; i <= 24; i++) {
		for (auto it = WCH_clock_list.equal_range(i).first; it != WCH_clock_list.equal_range(i).second; it++) {
			cout << i;
			WCH_PrintChar(MAXH - WCH_GetNumDigits(i), ' ');
			cout << " | " << (it -> second).first;
			WCH_PrintChar(MAXM - WCH_GetNumDigits((it -> second).first), ' ');
			cout << " | " << (it -> second).second << endl;
		}
	}
}

void WCH_check_clock() {
	// Clock series command input.
	if (WCH_command_list.size() == 1) {
		WCH_Error(WCH_ERRNO_UNCORRECT);
		return;
	}
	transform(WCH_command_list[1].begin(), WCH_command_list[1].end(), WCH_command_list[1].begin(), ::tolower);
	if (WCH_command_list[1] == "add") {
		WCH_add_clock();
	} else if (WCH_command_list[1] == "delete") {
		WCH_delete_clock();
	} else if (WCH_command_list[1] == "clear") {
		WCH_clear_clock();
	} else if (WCH_command_list[1] == "change") {
		WCH_change_clock();
	} else if (WCH_command_list[1] == "list") {
		WCH_list_clock();
	} else {
		WCH_Error(WCH_ERRNO_UNCORRECT);
	}
}

void WCH_add_task() {
	// Add a new task.
	if ((int)WCH_command_list.size() < 3) {
		WCH_Error(WCH_ERRNO_UNCORRECT);
		return;
	}
	string task = WCH_command.substr(9, WCH_command.size() - 1);
	transform(task.begin(), task.end(), task.begin(), ::tolower);
	if (WCH_task_list.find(task) != WCH_task_list.end()) {
		WCH_Error(WCH_ERRNO_WORK_OPERATION);
	} else {
		WCH_task_list.insert(task);
		WCH_task_num++;
	}
}

void WCH_delete_task() {
	// Delete a task.
	if ((int)WCH_command_list.size() < 3) {
		WCH_Error(WCH_ERRNO_UNCORRECT);
		return;
	}
	string task = WCH_command.substr(12, WCH_command.size() - 1);
	transform(task.begin(), task.end(), task.begin(), ::tolower);
	if (WCH_task_list.find(task) == WCH_task_list.end()) {
		WCH_Error(WCH_ERRNO_TASK_OPERATION);
	} else {
		WCH_task_list.erase(task);
		WCH_task_num--;
	}
}

void WCH_clear_task() {
	// Clear task list.
	if ((int)WCH_command_list.size() != 2) {
		WCH_Error(WCH_ERRNO_UNCORRECT);
		return;
	}
	WCH_task_list.clear();
	WCH_task_num = 0;
}

void WCH_list_task() {
	// List all tasks.
	if ((int)WCH_command_list.size() != 2) {
		WCH_Error(WCH_ERRNO_UNCORRECT);
		return;
	}
	int MAX = -1;
	for (auto it = WCH_task_list.begin(); it != WCH_task_list.end(); it++) {
		MAX = max(MAX, (int)(*it).size());
	}
	if (MAX == -1) {
		return;
	}
	MAX = max(MAX, 12);
	cout << "Process Name" << endl;
	WCH_PrintChar(MAX, '-');
	cout << endl;
	for (auto it = WCH_task_list.begin(); it != WCH_task_list.end(); it++) {
		cout << *it << endl;
	}
}

void WCH_check_task() {
	// Task series command input.
	if ((int)WCH_command_list.size() == 1) {
		WCH_Error(WCH_ERRNO_UNCORRECT);
		return;
	}
	transform(WCH_command_list[1].begin(), WCH_command_list[1].end(), WCH_command_list[1].begin(), ::tolower);
	if (WCH_command_list[1] == "add") {
		WCH_add_task();
	} else if (WCH_command_list[1] == "delete") {
		WCH_delete_task();
	} else if (WCH_command_list[1] == "clear") {
		WCH_clear_task();
	} else if (WCH_command_list[1] == "list") {
		WCH_list_task();
	} else {
		WCH_Error(WCH_ERRNO_UNCORRECT);
	}
}

void WCH_add_work() {
	// Add a new work.
	if ((int)WCH_command_list.size() < 3) {
		WCH_Error(WCH_ERRNO_UNCORRECT);
		return;
	}
	string work = WCH_command.substr(9, WCH_command.size() - 1);
	if (WCH_work_list.find(work) != WCH_work_list.end()) {
		WCH_Error(WCH_ERRNO_WORK_OPERATION);
	} else {
		WCH_work_list.insert(work);
		WCH_work_num++;
	}
}

void WCH_delete_work() {
	// Delete a work.
	if ((int)WCH_command_list.size() < 3) {
		WCH_Error(WCH_ERRNO_UNCORRECT);
		return;
	}
	string work = WCH_command.substr(10, WCH_command.size() - 1);
	if (WCH_work_list.find(work) == WCH_work_list.end()) {
		WCH_Error(WCH_ERRNO_WORK_OPERATION);
	} else {
		WCH_work_list.erase(work);
		WCH_work_num--;
	}
}

void WCH_clear_work() {
	// Clear work list.
	if ((int)WCH_command_list.size() != 2) {
		WCH_Error(WCH_ERRNO_UNCORRECT);
		return;
	}
	WCH_work_list.clear();
	WCH_work_num = 0;
}

void WCH_list_work() {
	// List all works.
	if ((int)WCH_command_list.size() != 2) {
		WCH_Error(WCH_ERRNO_UNCORRECT);
		return;
	}
	int MAX = -1;
	for (auto it = WCH_work_list.begin(); it != WCH_work_list.end(); it++) {
		MAX = max(MAX, (int)(*it).size());
	}
	if (MAX == -1) {
		return;
	}
	MAX = max(MAX, 4);
	cout << "Name" << endl;
	WCH_PrintChar(MAX, '-');
	cout << endl;
	for (auto it = WCH_work_list.begin(); it != WCH_work_list.end(); it++) {
		cout << *it << endl;
	}
}

void WCH_check_work() {
	// Work series command input.
	if (WCH_command_list.size() == 1) {
		WCH_Error(WCH_ERRNO_UNCORRECT);
		return;
	}
	transform(WCH_command_list[1].begin(), WCH_command_list[1].end(), WCH_command_list[1].begin(), ::tolower);
	if (WCH_command_list[1] == "add") {
		WCH_add_work();
	} else if (WCH_command_list[1] == "delete") {
		WCH_delete_work();
	} else if (WCH_command_list[1] == "clear") {
		WCH_clear_work();
	} else if (WCH_command_list[1] == "list") {
		WCH_list_work();
	} else {
		WCH_Error(WCH_ERRNO_UNCORRECT);
	}
}

void WCH_game() {
	// Guessing game.
	if ((int)WCH_command_list.size() != 1) {
		WCH_Error(WCH_ERRNO_UNCORRECT);
		return;
	}
	srand((unsigned)time(NULL));
	int WCH_clock_num = rand() % 10000 + 1;
	bool flag = true;
	string z = "0";
	vector <string> zv;
	zv.push_back("0");
	try {
		while (stoi(zv[0]) != WCH_clock_num) {
			BEGIN: cout << "Please input your number (1 ~ 10000): ";
			getline(cin, z);
			zv = WCH_split(z);
			if ((int)zv.size() != 1) {
				if ((int)zv.size() > 1) {
					cout << "Number out of range." << endl;
					zv.clear();
					zv.push_back("0");
				} else {
					flag = false;
					goto END;
				}
			} else if (zv[0][0] == '-' || zv[0][0] == '0' || (zv[0].size() >= 5 && zv[0] != "10000")) {
				cout << "Number out of range." << endl;
			} else if (stoi(zv[0]) > WCH_clock_num) {
				cout << "The answer is smaller." << endl;
			} else if (stoi(zv[0]) < WCH_clock_num) {
				cout << "The answer is bigger." << endl;
			}
		}
	} catch (...) {
		cout << "Number out of range." << endl;
		goto BEGIN;
	}
	END: cout << "The number is " << WCH_clock_num << ".";
	if (flag) {
		cout << " You WIN!";
	}
	cout << endl;
}

void WCH_speedtest() {
	// Start a speed test with Python program.
	if ((int)WCH_command_list.size() != 1) {
		WCH_Error(WCH_ERRNO_UNCORRECT);
		return;
	}
	string tmp = "SPEEDTEST";
	tmp += to_string(WCH_Framework);
	tmp += ".EXE";
	system(tmp.c_str());
}

void WCH_pi() {
	// A sequence of function to make a screenshot.
	if ((int)WCH_command_list.size() != 1) {
		WCH_Error(WCH_ERRNO_UNCORRECT);
		return;
	}
	WCH_SetWindowStatus(false);
	WCH_PutPicture();
	WCH_SetWindowStatus(true);
	WCH_SaveImg();
}

void WCH_check_task_loop() {
	// Check if the running task is in the task list. (Another thread)
	while (WCH_anti_idle && !WCH_program_end) {
		for (auto it = WCH_task_list.begin(); it != WCH_task_list.end(); it++) {
			if (WCH_TaskKill(*it)) {
				WCH_printlog(WCH_LOG_MODE_KT, {"Successfully killed", *it});
			} else {
				WCH_printlog(WCH_LOG_MODE_KT, {"Failed to kill", *it});
			}
		}
		WCH_Sleep(15000);
	}
}

void WCH_anti_idle_func() {
	// Enable anti-idle function.
	if ((int)WCH_command_list.size() != 1) {
		WCH_Error(WCH_ERRNO_UNCORRECT);
		return;
	}
	string ch;
	cout << "Are you sure to enable anti-idle function? If you want to disable it, press Ctrl + Down. (Y/N): ";
	getline(cin, ch);
	if (ch == "Y" || ch == "y") {
		WCH_SetWindowStatus(false);
		WCH_anti_idle = true;
		WCH_Sleep(500);
		WCH_SetWindowSize(SW_MAXIMIZE, GetActiveWindow());
		WCH_Sleep(500);
		WCH_SetTrayStatus(false);
		WCH_Sleep(500);
		thread T(WCH_check_task_loop);
		T.detach();
	}
}

void WCH_trans() {
	// Translate string between Chinese / English.
	if ((int)WCH_command_list.size() == 1) {
		WCH_Error(WCH_ERRNO_UNCORRECT);
		return;
	}
	try {
		string info;
		string res;
		info = "TRANS";
		info += to_string(WCH_Framework);
		info += " -i \"";
		info += WCH_command.substr(5, WCH_command.size() - 1);
		info += "\" > WCH_TRANS.tmp";
		system(info.c_str());
		WCH_Sleep(1000);
		WCH_cmd_line = false;
		fin.open("WCH_TRANS.tmp");
		getline(fin, res);
		cout << res << endl;
		fin.close();
		DeleteFile(L"WCH_TRANS.tmp");
		WCH_cmd_line = true;
	} catch (...) {
		WCH_Error(WCH_ERRNO_NETWORK_FAILURE);
		return;
	}
}

void WCH_ow() {
	// Get a random sentence.
	if ((int)WCH_command_list.size() != 1) {
		WCH_Error(WCH_ERRNO_UNCORRECT);
		return;
	}
	try {
		URLDownloadToFile(0, "https://v1.hitokoto.cn/?encode=text", "WCH_STDL.tmp", 0, 0);
		WCH_Sleep(1000);
		WCH_cmd_line = false;
		fin.open("WCH_STDL.tmp");
		string res;
		getline(fin, res);
		cout << UTF8ToANSI(res) << endl;
		fin.close();
		DeleteFile(L"WCH_STDL.tmp");
		WCH_cmd_line = true;
	} catch (...) {
		WCH_Error(WCH_ERRNO_NETWORK_FAILURE);
		return;
	}
}

void WCH_time() {
	// Print current time.
	if ((int)WCH_command_list.size() != 1) {
		WCH_Error(WCH_ERRNO_UNCORRECT);
		return;
	}
	WCH_Time q = WCH_GetTime();
	cout << format("{:04}/{:02}/{:02} {:02}:{:02}:{:02}", q.Year, q.Month, q.Day, q.Hour, q.Minute, q.Second) << endl;
}

void WCH_help() {
	// Print help information.
	if ((int)WCH_command_list.size() == 1) {
		cout << "If you want to get the full information of a command, please input \"HELP {command name}\"." << endl;
		cout << "clock add {hour} {minute} {name} (Add clock at {hour}:{minute})" << endl;
		cout << "clock delete {hour} {minute} {name} (Delete clock at {hour}:{minute})" << endl;
		cout << "clock change {hour} {minute} {name} (Change clock at {hour}:{minute})" << endl;
		cout << "clock list (List all clocks)" << endl;
		cout << "task add {process name} (Add task {process name} to kill when enable \"anti-idle\")" << endl;
		cout << "task delete {process name} (Delete task {process name} to kill when enable \"anti-idle\")" << endl;
		cout << "task list (List all tasks)" << endl;
		cout << "work add {name} (Add {name} to work plan)" << endl;
		cout << "work done {name} (Done work plan item {name})" << endl;
		cout << "work list (List all items in work plan)" << endl;
		cout << "help (Get help output)" << endl;
		cout << "ow (Get a sentence) **From web**" << endl;
		cout << "hide (Hide the command line window)" << endl;
		cout << "game (Guessing game)" << endl;
		cout << "time (Get time at once)" << endl;
		cout << "pi (Make a screenshot and save in \"Pictures\" folder)" << endl;
		cout << "speedtest (Start a speed test with a GUI window)" << endl;
		cout << "trans {info} (Translate a sentence between English / Chinese) **From web**" << endl;
		cout << "anti-idle (Enable anti-idle mode)" << endl;
		cout << "update (Visit the releases page in default browser)" << endl;
		cout << "license (Print license information)" << endl;
		cout << "wiki (Jump to program wiki page in default browser)" << endl;
		cout << "quit (Quit this program)" << endl;
	} else {
		transform(WCH_command_list[1].begin(), WCH_command_list[1].end(), WCH_command_list[1].begin(), ::tolower);
		if ((int)WCH_command_list.size() == 2 && _access(("resources/help/" + WCH_command_list[1] + ".dat").c_str(), 0) != -1) {
			string _res;
			fin.open(("resources/help/" + WCH_command_list[1] + ".dat").c_str());
			while (getline(fin, _res)) {
				cout << _res << endl;
			}
			fin.close();
		} else {
			if (WCH_command_support.find(WCH_command_list[1]) != WCH_command_support.end()) {
				WCH_Error(WCH_ERRNO_FILE_NOT_FOUND);
			} else {
				WCH_Error(WCH_ERRNO_UNCORRECT);
			}
			return;
		}
	}
}

#endif