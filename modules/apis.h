/*
Web-Class-Helper APIs Module Header File 1.1.2
This source code file is under MIT License.
Copyright (c) 2022 Class Tools Develop Team
Contributors: jsh-jsh ren-yc
*/
#ifndef APIS_H
#define APIS_H
#include <bits/stdc++.h>
#include <io.h>
#include <thread>
#include <windows.h>
#include <wininet.h>
#include <tlhelp32.h>
#include <conio.h>
#include <direct.h>
#include <VersionHelpers.h>
#include "commands.h"
#include "functions.h"
#include "file-process.h"
#include "variables.h"
using namespace std;

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

void WCH_Sleep(int _ms) {
	// Sleep.
	while (_ms > 0 && !WCH_program_end) {
		Sleep(100);
		_ms -= 100;
	}
}

void WCH_PrintColor(WORD _mode) {
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), _mode);
}

void WCH_PrintChar(int _times, char _c) {
	// Print space.
	while (_times > 0 && !WCH_program_end) {
		_times--;
		cout << _c;
	}
}

wstring StrToWstr(string str) {
	wstring result;
	int len = MultiByteToWideChar(CP_ACP, 0, str.c_str(), (int)str.size(), NULL, 0);
	TCHAR *buffer = new TCHAR[len + 1];
	MultiByteToWideChar(CP_ACP, 0, str.c_str(), (int)str.size(), buffer, len);
	buffer[len] = '\0';
	result.append(buffer);
	delete[] buffer;
	return result;
}

string WstrToStr(wstring wstr) {
	string result;
	int len = WideCharToMultiByte(CP_ACP, 0, wstr.c_str(), (int)wstr.size(), NULL, 0, NULL, NULL);
	char* buffer = new char[len + 1];
	WideCharToMultiByte(CP_ACP, 0, wstr.c_str(), (int)wstr.size(), buffer, len, NULL, NULL);
	buffer[len] = '\0';
	result.append(buffer);
	delete[] buffer;
	return result;
}

DWORD WCH_GetPID(string name) {
	// Get PID by process name.
	DWORD pid = 0;
	PROCESSENTRY32 entry;
	entry.dwSize = sizeof(PROCESSENTRY32);
	HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (Process32First(snapshot, &entry) == TRUE) {
		while (Process32Next(snapshot, &entry) == TRUE) {
			if (WstrToStr(entry.szExeFile) == name) {
				HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, entry.th32ProcessID);
				pid = GetProcessId(hProcess);
				CloseHandle(hProcess);
			}
		}
	}
	CloseHandle(snapshot);
	return pid;
}

void WCH_SetWindowStatus(bool flag) {
	// Set the window status by Windows API.
	ShowWindow(WCH_hWnd, flag);
	WCH_cmd_line = flag;
	WCH_printlog(WCH_LOG_MODE_WD, {"CONSOLE", "STATUS", (flag == true ? "SHOW" : "HIDE")});
}

void WCH_SetTrayStatus(bool flag) {
	// Set the tray status by Windows API.
	ShowWindow(FindWindow(L"Shell_trayWnd", NULL), (flag == true ? SW_SHOW : SW_HIDE));
	WCH_printlog(WCH_LOG_MODE_WD, {"TRAY", "STATUS", (flag == true ? "SHOW" : "HIDE")});
}

void WCH_SetWindowSize(int mode, HWND hWnd) {
	// Set the window size by Windows API.
	ShowWindow(hWnd, mode);
	WCH_printlog(WCH_LOG_MODE_WD, {"CURWND", "SIZE", (mode == SW_MAXIMIZE ? "MAXIMIZE" : "NORMAL")});
}

bool WCH_TaskKill(string name) {
	// Kill a task by Windows API.
	HANDLE hProcess = OpenProcess(PROCESS_TERMINATE, FALSE, WCH_GetPID(name));
	if (hProcess) {
		TerminateProcess(hProcess, 0);
		CloseHandle(hProcess);
		return true;
	} else {
		return false;
	}
}

string UTF8ToANSI(string strUTF8) {
	UINT nLen = MultiByteToWideChar(CP_UTF8, 0, strUTF8.c_str(), -1, NULL, 0);
	WCHAR *wszBuffer = new WCHAR[nLen + 1];
	nLen = MultiByteToWideChar(CP_UTF8, 0, strUTF8.c_str(), -1, wszBuffer, nLen);
	wszBuffer[nLen] = 0;
	nLen = WideCharToMultiByte(936, 0, wszBuffer, -1, NULL, 0, NULL, 0);
	CHAR *szBuffer = new CHAR[nLen + 1];
	nLen = WideCharToMultiByte(936, 0, wszBuffer, -1, szBuffer, nLen, NULL, 0);
	szBuffer[nLen] = 0;
	strUTF8 = szBuffer;
	delete[] szBuffer;
	delete[] wszBuffer;
	return strUTF8;
}

int WCH_GetNumDigits(int _n) {
	int _cnt = 1;
	while ((_n /= 10) != 0) {
		_cnt++;
	}
	return _cnt;
}

void WCH_PrintProgressBar(int _sur, int _n, bool _flag) {
	// Print a progress bar.
	string _ETAStr = format("{:02}:{:02}:{:02}", (int)(_sur / 3600), (int)((_sur % 3600) / 60), (int)(_sur % 60));
	if (_flag) {
		for (int i = 0; i < WCH_ProgressBarCount; i++) {
			cout << "\b";
		}
	}
	WCH_PrintColor(0x0A);
	for (int i = 0; i < _n / 2; i++) {
		cout << WCH_ProgressBarStr;
	}
	WCH_PrintColor(0x0C);
	for (int i = _n / 2; i < 50; i++) {
		cout << WCH_ProgressBarStr;
	}
	WCH_PrintColor(0x02);
	cout << " " << _n << "%";
	WCH_PrintColor(0x07);
	cout << " ETA ";
	WCH_PrintColor(0x09);
	cout << _ETAStr;
	WCH_PrintColor(0x07);
	WCH_ProgressBarCount = 65 + WCH_GetNumDigits(_n);
}

void WCH_ProgressBar() {
	// Progress bar.
	WCH_PrintProgressBar(WCH_ProgressBarTot, 0, false);
	for (int i = WCH_ProgressBarTot; i > 1 && !WCH_program_end; i--) {
		WCH_Sleep(1000);
		WCH_PrintProgressBar(i, 100 / i, true);
	}
	WCH_Sleep(1000);
	WCH_PrintProgressBar(0, 100, true);
	cout << endl;
}

void WCH_signalHandler() {
	// Signal handler.
	signal(SIGINT, [](int signum) {
		string tmp = "ERROR";
		tmp += to_string(WCH_Framework);
		tmp += ".EXE";
		WCH_cmd_line = false;
		WCH_program_end = true;
		WCH_PrintColor(0x07);
		cout << endl;
		WCH_save();
		WCH_Sleep(100);
		WCH_printlog(WCH_LOG_MODE_ERROR, {"Signal " + to_string(signum) + " detected (Program interrupted)"});
		system((tmp + " " + to_string(signum) + " \"Program interrupted\"").c_str());
		exit(signum);
	});
	signal(SIGFPE, [](int signum) {
		string tmp = "ERROR";
		tmp += to_string(WCH_Framework);
		tmp += ".EXE";
		WCH_cmd_line = false;
		WCH_program_end = true;
		WCH_PrintColor(0x07);
		cout << endl;
		WCH_save();
		WCH_Sleep(100);
		WCH_printlog(WCH_LOG_MODE_ERROR, {"Signal " + to_string(signum) + " detected (Operation overflow)"});
		system((tmp + " " + to_string(signum) + " \"Operation overflow\"").c_str());
		exit(signum);
	});
	signal(SIGILL, [](int signum) {
		string tmp = "ERROR";
		tmp += to_string(WCH_Framework);
		tmp += ".EXE";
		WCH_cmd_line = false;
		WCH_program_end = true;
		WCH_PrintColor(0x07);
		cout << endl;
		WCH_save();
		WCH_Sleep(100);
		WCH_printlog(WCH_LOG_MODE_ERROR, {"Signal " + to_string(signum) + " detected (Illegal instruction)"});
		system((tmp + " " + to_string(signum) + " \"Illegal instruction\"").c_str());
		exit(signum);
	});
	signal(SIGSEGV, [](int signum) {
		string tmp = "ERROR";
		tmp += to_string(WCH_Framework);
		tmp += ".EXE";
		WCH_cmd_line = false;
		WCH_program_end = true;
		WCH_PrintColor(0x07);
		cout << endl;
		WCH_save();
		WCH_Sleep(100);
		WCH_printlog(WCH_LOG_MODE_ERROR, {"Signal " + to_string(signum) + " detected (Access to illegal memory)"});
		system((tmp + " " + to_string(signum) + " \"Access to illegal memory\"").c_str());
		exit(signum);
	});
}

#endif