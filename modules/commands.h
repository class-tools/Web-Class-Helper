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
#include "functions.h"
#include "file-process.h"
#include "apis.h"
#include "variables.h"
using namespace std;

#ifdef URLDownloadToFile
#undef URLDownloadToFile
#endif

extern const string Weekdayname[7];
extern multimap <int, pair <int, string>> WCH_clock;
extern set <string> WCH_task_list;
extern HWND hwnd;
extern int WCH_clock_num;
extern bool cmd_line;
extern bool anti_idle;
extern bool mode;
extern string op;
extern string UserName;
extern ifstream fin;
extern ofstream fout;
WCH_Time WCH_GetTime();
void WCH_Error(string INFO);

typedef int(__stdcall *UDF)(LPVOID, LPCSTR, LPCSTR, DWORD, LPVOID);
UDF URLDownloadToFile = (UDF)(int*)GetProcAddress(LoadLibrary("urlmon.dll"), "URLDownloadToFileA");

void WCH_hide() {
	// Hide the window.
	WCH_SetWindowStatus(false);
}

void WCH_update() {
	// Visit the website to update the program.
	WCH_RunSystem("start https://github.com/class-tools/Web-Class-Helper/releases/latest/");
}

void WCH_add_clock() {
	// Add a new clock.
	int h = 0;
	int m = 0;
	string tname = "NULL";
	cin >> h >> m >> tname;
	if (h > 24 || m >= 60 || h < 1 || m < 0) {
		WCH_Error(WCH_ERRNO_OUT_OF_RANGE);
	} else {
		WCH_clock_num++;
		WCH_clock.emplace(make_pair(h, make_pair(m, tname)));
	}
}

void WCH_delete_clock() {
	// Delete a clock.
	int h = 0;
	int m = 0;
	bool flag = false;
	string tname = "NULL";
	try {
		cin >> h >> m >> tname;
		for (auto it = WCH_clock.equal_range(h).first; it != WCH_clock.equal_range(h).second; it++) {
			if ((it -> second).first == m && (it -> second).second == tname) {
				WCH_clock.erase(it);
				flag = true;
				WCH_clock_num--;
				break;
			}
		}
	}
	catch (...) {
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
	string tname = "NULL";
	try {
		cin >> h >> m >> tname;
		for (auto it = WCH_clock.equal_range(h).first; it != WCH_clock.equal_range(h).second; it++) {
			if ((it -> second).first == m) {
				(it -> second).second = tname;
				flag = true;
			}
		}
	}
	catch (...) {
		goto Error;
	}
	if (!flag) {
		Error: WCH_Error(WCH_ERRNO_CLOCK_OPERATION);
		return;
	}
}

void WCH_check_clock() {
	// Clock series command input.
	string cmd;
	cin >> cmd;
	if (cmd == "add") {
		WCH_add_clock();
	} else if (cmd == "delete") {
		WCH_delete_clock();
	} else if (cmd == "change") {
		WCH_change_clock();
	} else {
		WCH_Error(WCH_ERRNO_OUT_OF_RANGE);
	}
}

void WCH_add_task() {
	// Add a new task.
	string task;
	string tmp;
	getline(cin, task);
	tmp = task.substr(1, task.size() - 1);
	WCH_task_list.insert(tmp);
}

void WCH_delete_task() {
	// Delete a task.
	string task;
	string tmp;
	getline(cin, task);
	tmp = task.substr(1, task.size() - 1);
	if (WCH_task_list.find(tmp) == WCH_task_list.end()) {
		WCH_Error(WCH_ERRNO_TASK_OPERATION);
	} else {
		WCH_task_list.erase(tmp);
	}
}

void WCH_check_task() {
	// Task series command input.
	string cmd;
	cin >> cmd;
	if (cmd == "add") {
		WCH_add_task();
	} else if (cmd == "delete") {
		WCH_delete_task();
	} else {
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
	}
	catch (...) {
		WCH_Error(WCH_ERRNO_OUT_OF_RANGE);
		return;
	}
	cout << "The number is " << WCH_clock_num << ". You WIN!" << endl;
	return;
}

void WCH_speedtest() {
	// Start a speed test with Python program.
	WCH_RunSystem("START SPEEDTEST.EXE");
}

void WCH_pi() {
	// A sequence of function to make a screenshot.
	WCH_SetWindowStatus(false);
	WCH_PutPicture();
	WCH_SetWindowStatus(true);
	WCH_SaveImg();
}

void WCH_mode() {
	// Switch command-line mode.
	string tmp;
	cin >> tmp;
	if (tmp == "cmd") {
		mode = true;
	} else if (tmp == "bash") {
		mode = false;
	} else {
		cout << "Unknown mode." << endl;
	}
}

void WCH_anti_idle() {
	// Enable anti-idle function.
	char ch;
	cout << "Are you sure to enable anti-idle function? If you want to disable it, press Ctrl + Down. (Y/N): ";
	cin >> ch;
	if (ch == 'Y' || ch == 'y') {
		WCH_SetWindowStatus(false);
		anti_idle = true;
		Sleep(500);
		WCH_SetTrayStatus(false);
		Sleep(500);
		WCH_SetWindowPos(false);
		Sleep(500);
		WCH_check_task_loop();
	}
}

void WCH_unknown(string op) {
	// Make a response to unknown command.
	cout << op << ": Command not found." << endl;
}

void WCH_trans() {
	// Translate string between Chinese / English.
	try {
		string info;
		string res;
		getline(cin, info);
		info = "TRANS -i \"" + info.substr(1, info.size() - 1) + "\" > WCH_TRANS.tmp";
		WCH_RunSystem(info);
		cmd_line = false;
		Sleep(2000);
		fin.open("WCH_TRANS.tmp");
		while (fin >> res) {
			cout << res << " ";
		}
		cout << endl;
		fin.close();
		DeleteFile("WCH_TRANS.tmp");
		cmd_line = true;
	}
	catch (...) {
		WCH_Error(WCH_ERRNO_NETWORK_FAILURE);
		return;
	}
}

void WCH_ow() {
	// Get a random sentence.
	try {
		int len;
		char url[128], *file;
		HANDLE hFile;
		DWORD unused;
		char ss[128] = "https://v1.hitokoto.cn/?encode=text";
		sprintf(url, ss);
		URLDownloadToFile(0, url, "WCH_STDL.tmp", 0, 0);
		hFile = CreateFile("WCH_STDL.tmp", GENERIC_READ, FILE_SHARE_READ, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
		len = GetFileSize(hFile, 0);
		file = new char[len + 3];
		ReadFile(hFile, file, len, &unused, 0);
		file[len] = file[len + 1] = 0;
		CloseHandle(hFile);
		UTF8ToANSI(file);
		cout << file << endl;
		DeleteFile("WCH_STDL.tmp");
	}
	catch (...) {
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
	cout << "add {hour} {minute} {name} (Add clock at {hour}:{minute})" << endl;
	cout << "delete hour {minute} {name} (Delete clock at {hour}:{minute})" << endl;
	cout << "change hour {minute} {name} (Change clock at {hour}:{minute})" << endl;
	cout << "help (Get help output)" << endl;
	cout << "ow (Get a sentence) **From web**" << endl;
	cout << "hide (Hide the command line window)" << endl;
	cout << "game (Guessing game)" << endl;
	cout << "time (Get time at once)" << endl;
	cout << "pi (Make a screenshot)" << endl;
	cout << "mode {cmd-mode} (Switch command line mode to {cmd-mode} cmd / bash)" << endl;
	cout << "speedtest (Start a speed test with a GUI window)" << endl;
	cout << "trans {info} (Translate a word between English / Chinese) **End with \"#\"**" << endl;
	cout << "anti-idle (Enable anti-idle mode)" << endl;
	cout << "update (Visit the releases page in default browser)" << endl;
	cout << "quit (Quit this program)" << endl;
}

#endif
