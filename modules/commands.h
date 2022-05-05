/*
Web-Class-Helper Commands Module Header File 1.1.2
This source code file is under MIT License.
Copyright (c) 2022 Class Tools Develop Team
Contributors: jsh-jsh ren-yc
*/
#ifndef COMMANDS_H
#define COMMANDS_H
#include <bits/stdc++.h>
#include <windows.h>
#include <wininet.h>
#include <tlhelp32.h>
#include <conio.h>
#include <direct.h>
#include <VersionHelpers.h>
#include "functions.h"
#include "file-process.h"
#include "apis.h"
#include "variables.h"
using namespace std;

#ifdef URLDownloadToFile
#undef URLDownloadToFile
#endif

extern const string WCH_WDName[7];
extern multimap <int, pair <int, string>> WCH_clock_list;
extern set <string> WCH_task_list;
extern set <string> WCH_work_list;
extern HWND WCH_hWnd;
extern int WCH_clock_num;
extern int WCH_task_num;
extern int WCH_work_num;
extern int WCH_ProgressBarCount;
extern int WCH_ProgressBarTot;
extern bool WCH_cmd_line;
extern bool WCH_anti_idle;
extern bool WCH_program_end;
extern bool WCH_wait_cmd;
extern string WCH_command;
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
UDF URLDownloadToFile = (UDF)(int*)GetProcAddress(LoadLibrary("urlmon.dll"), "URLDownloadToFileA");

void WCH_hide() {
	// Hide the window.
	WCH_SetWindowStatus(false);
}

void WCH_update() {
	// Visit the website to update the program.
	try {
		cout << "Checking update..." << endl;
		WCH_ProgressBarTot = 5;
		thread T(WCH_ProgressBar);
		T.detach();
		string url = "https://class-tools.gq/update/WCH?";
		srand(time(NULL));
		url += to_string(rand());
		URLDownloadToFile(0, url.c_str(), "WCH_UPD.tmp", 0, 0);
		string res;
		fin.open("WCH_UPD.tmp");
		getline(fin, res);
		fin.close();
		WCH_Sleep(5500);
		if (res == "") {
			throw runtime_error(WCH_ERRNO_NETWORK_FAILURE);
		}
		if (res != WCH_VER) {
			system("start https://github.com/class-tools/Web-Class-Helper/releases/latest/");
			WCH_printlog(WCH_LOG_MODE_UPD, {"Updating to version", res});
		} else {
			cout << "Already up to date." << endl;
			WCH_printlog(WCH_LOG_MODE_UPD, {"Program version is already", res});
		}
		DeleteFile("WCH_UPD.tmp");
	} catch (...) {
		WCH_Error(WCH_ERRNO_NETWORK_FAILURE);
		return;
	}
}

void WCH_license() {
	// Print the license.
	fin.open("LICENSE");
	string tmp;
	for (int i = 1; i <= 21; i++) {
		getline(fin, tmp);
		cout << tmp << endl;
	}
	fin.close();
}

void WCH_add_clock() {
	// Add a new clock.
	int h = 0;
	int m = 0;
	string Tname = "NULL";
	bool flag = false;
	cin >> h >> m >> Tname;
	for (auto it = WCH_clock_list.equal_range(h).first; it != WCH_clock_list.equal_range(h).second; it++) {
		if ((it -> second).first == m) {
			flag = true;
			break;
		}
	}
	if (h > 24 || m >= 60 || h < 1 || m < 0 || flag) {
		WCH_Error(WCH_ERRNO_OUT_OF_RANGE);
	} else {
		WCH_clock_num++;
		WCH_clock_list.emplace(make_pair(h, make_pair(m, Tname)));
	}
}

void WCH_delete_clock() {
	// Delete a clock.
	int h = 0;
	int m = 0;
	bool flag = false;
	string Tname = "NULL";
	try {
		cin >> h >> m >> Tname;
		for (auto it = WCH_clock_list.equal_range(h).first; it != WCH_clock_list.equal_range(h).second; it++) {
			if ((it -> second).first == m && (it -> second).second == Tname) {
				WCH_clock_list.erase(it);
				flag = true;
				WCH_clock_num--;
				break;
			}
		}
	} catch (...) {
		goto Error;
	}
	if (!flag) {
		Error: WCH_Error(WCH_ERRNO_CLOCK_OPERATION);
		return;
	}
}

void WCH_change_clock() {
	// Change a clock.
	int h = 0;
	int m = 0;
	bool flag = false;
	string Tname = "NULL";
	try {
		cin >> h >> m >> Tname;
		for (auto it = WCH_clock_list.equal_range(h).first; it != WCH_clock_list.equal_range(h).second; it++) {
			if ((it -> second).first == m) {
				(it -> second).second = Tname;
				flag = true;
			}
		}
	} catch (...) {
		goto Error;
	}
	if (!flag) {
		Error: WCH_Error(WCH_ERRNO_CLOCK_OPERATION);
		return;
	}
}

void WCH_list_clock() {
	// List all tasks.
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
	string cmd;
	cin >> cmd;
	WCH_printlog(WCH_LOG_MODE_RC, {"sub command", cmd});
	if (cmd == "add") {
		WCH_add_clock();
	} else if (cmd == "delete") {
		WCH_delete_clock();
	} else if (cmd == "change") {
		WCH_change_clock();
	} else if (cmd == "list") {
		WCH_list_clock();
	} else {
		string tmp;
		getline(cin, tmp);
		WCH_Error(WCH_ERRNO_OUT_OF_RANGE);
	}
}

void WCH_add_task() {
	// Add a new task.
	string task;
	cin >> task;
	WCH_task_list.insert(task);
	WCH_task_num++;
}

void WCH_delete_task() {
	// Delete a task.
	string task;
	cin >> task;
	if (WCH_task_list.find(task) == WCH_task_list.end()) {
		WCH_Error(WCH_ERRNO_TASK_OPERATION);
	} else {
		WCH_task_list.erase(task);
		WCH_task_num--;
	}
}

void WCH_list_task() {
	// List all tasks.
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
	string cmd;
	cin >> cmd;
	WCH_printlog(WCH_LOG_MODE_RC, {"sub command", cmd});
	if (cmd == "add") {
		WCH_add_task();
	} else if (cmd == "delete") {
		WCH_delete_task();
	} else if (cmd == "list") {
		WCH_list_task();
	} else {
		string tmp;
		getline(cin, tmp);
		WCH_Error(WCH_ERRNO_OUT_OF_RANGE);
	}
}

void WCH_add_work() {
	// Add a new work.
	string work;
	cin >> work;
	WCH_work_list.insert(work);
	WCH_work_num++;
}

void WCH_done_work() {
	// Delete a work.
	string work;
	cin >> work;
	if (WCH_work_list.find(work) == WCH_work_list.end()) {
		WCH_Error(WCH_ERRNO_WORK_OPERATION);
	} else {
		WCH_work_list.erase(work);
		WCH_work_num--;
	}
}

void WCH_list_work() {
	// List all works.
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
	string cmd;
	cin >> cmd;
	WCH_printlog(WCH_LOG_MODE_RC, {"sub command", cmd});
	if (cmd == "add") {
		WCH_add_work();
	} else if (cmd == "done") {
		WCH_done_work();
	} else if (cmd == "list") {
		WCH_list_work();
	} else {
		string tmp;
		getline(cin, tmp);
		WCH_Error(WCH_ERRNO_OUT_OF_RANGE);
	}
}

void WCH_game() {
	// Guessing game.
	srand(time(NULL));
	int WCH_clock_num = rand() % 10000 + 1;
	string z = "0";
	try {
		while (stoi(z) != WCH_clock_num) {
			cout << "Please input your number (1 ~ 10000): ";
			cin >> z;
			if (z[0] == '-' || z[0] == '0' || (z.size() > 5 && z != "10000")) {
				cout << "Number out of range." << endl;
			} else if (stoi(z) > WCH_clock_num) {
				cout << "The answer is smaller." << endl;
			} else if (stoi(z) < WCH_clock_num) {
				cout << "The answer is bigger." << endl;
			}
		}
	} catch (...) {
		WCH_Error(WCH_ERRNO_OUT_OF_RANGE);
		return;
	}
	cout << "The number is " << WCH_clock_num << ". You WIN!" << endl;
	return;
}

void WCH_speedtest() {
	// Start a speed test with Python program.
	string tmp = "SPEEDTEST";
	tmp += WCH_Framework;
	tmp += ".EXE";
	system(tmp.c_str());
}

void WCH_pi() {
	// A sequence of function to make a screenshot.
	WCH_SetWindowStatus(false);
	WCH_PutPicture();
	WCH_SetWindowStatus(true);
	WCH_SaveImg();
}

void WCH_anti_idle_func() {
	// Enable anti-idle function.
	char ch;
	cout << "Are you sure to enable anti-idle function? If you want to disable it, press Ctrl + Down. (Y/N): ";
	cin >> ch;
	if (ch == 'Y' || ch == 'y') {
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

void WCH_unknown(string _command1) {
	// Make a response to unknown command.
	string _command2;
	getline(cin, _command2);
	cout << _command1 + _command2 << ": Command not found." << endl;
}

void WCH_trans() {
	// Translate string between Chinese / English.
	try {
		string info;
		string tmp;
		string res;
		getline(cin, info);
		tmp = info.substr(1, info.size() - 1);
		info = "TRANS";
		info += WCH_Framework;
		info += " -i \"";
		info += tmp;
		info += "\" > WCH_TRANS.tmp";
		system(info.c_str());
		WCH_Sleep(1000);
		WCH_cmd_line = false;
		fin.open("WCH_TRANS.tmp");
		getline(fin, res);
		cout << res << endl;
		fin.close();
		DeleteFile("WCH_TRANS.tmp");
		WCH_cmd_line = true;
	} catch (...) {
		WCH_Error(WCH_ERRNO_NETWORK_FAILURE);
		return;
	}
}

void WCH_ow() {
	// Get a random sentence.
	try {
		URLDownloadToFile(0, "https://v1.hitokoto.cn/?encode=text", "WCH_STDL.tmp", 0, 0);
		WCH_Sleep(1000);
		WCH_cmd_line = false;
		fin.open("WCH_STDL.tmp");
		string res;
		getline(fin, res);
		cout << UTF8ToANSI(res) << endl;
		fin.close();
		DeleteFile("WCH_STDL.tmp");
		WCH_cmd_line = true;
	} catch (...) {
		WCH_Error(WCH_ERRNO_NETWORK_FAILURE);
		return;
	}
}

void WCH_time() {
	// Print current time.
	char tmp[21];
	WCH_Time q = WCH_GetTime();
	sprintf(tmp, "%04d/%02d/%02d %02d:%02d:%02d", q.Year, q.Month, q.Day, q.Hour, q.Minute, q.Second);
	cout << tmp << endl;
}

void WCH_help() {
	// Print help information.
	cout << "Commands:" << endl;
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
	cout << "quit (Quit this program)" << endl;
}

#endif