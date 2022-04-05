/*
Web-Class-Helper Function Module Header File 1.1.0
This source code file is under MIT License.
Copyright (c) 2022 Class Tools Develop Team
Contributors: jsh-jsh ren-yc
*/
#ifndef FUNCTIONS_H
#define FUNCTIONS_H
#include <bits/stdc++.h>
#include <windows.h>
#include <wininet.h>
#include <conio.h>
#include <direct.h>
#include "commands.h"
#include "file-process.h"
#include "apis.h"
#include "variables.h"
using namespace std;

extern const string Weekdayname[7];
extern multimap <int, pair <int, string> > mm;
extern int WCH_clock_num;
extern bool cmd_line;
extern bool anti_idle;
extern bool mode;
extern string op;
extern string UserName;
extern ifstream fin;
extern ofstream fout;
WCH_Time WCH_GetTime();

WCH_Time WCH_GetTime() {
	WCH_Time NowTime;
	time_t rawtime;
	struct tm *ptminfo;
	time(&rawtime);
	ptminfo = localtime(&rawtime);
	NowTime.Year = ptminfo -> tm_year + 1900;
	NowTime.Month = ptminfo -> tm_mon + 1;
	NowTime.Day = ptminfo -> tm_mday;
	NowTime.Hour = ptminfo -> tm_hour;
	NowTime.Minute = ptminfo -> tm_min;
	NowTime.Second = ptminfo -> tm_sec;
	NowTime.Name = "NULL";
	return NowTime;
}

void WCH_Init() {
	WCH_Time q = WCH_GetTime();
	char tmp[21];
	sprintf(tmp, "./logs/%04d%02d%02d%02d%02d%02d.log", q.Year, q.Month, q.Day, q.Hour, q.Minute, q.Second);
	rename("./logs/latest.log", tmp);
	WCH_printlog(WCH_LOG_MODE_ST, {"s"});
	sprintf(tmp, "Web Class Helper (%s)", WCH_Framework);
	SetConsoleTitle(tmp);
	UserName = WCH_GetUserName();
	if (q.Month == 1 || q.Month == 2) {
		q.Month += 12;
		q.Year--;
	}
	WCH_SetWindowStatus(true);
	cout << "Web Class Helper " << WCH_VER << " (" << WCH_Framework << ")" << endl;
	cout << "Copyright (c) 2022 Class Tools Develop Team." << endl;
	cout << "Type 'help' to get help." << endl;
	cout << endl;
}

void WCH_Error(string INFO) {
	string tmp;
	if (INFO == "OOR") {
		tmp = "Your input code is out of range, please check and try again";
	}
	if (INFO == "NF") {
		tmp = "An network error occurred, please check your network connection and try to update this program";
	}
	if (INFO == "CO") {
		tmp = "Cannot Operate the clock, please try to restart this program";
	}
	cout << tmp << "." << endl;
	WCH_printlog(WCH_LOG_MODE_ERROR, {tmp});
}

bool WCH_ShortCutKeyCheck() {
	int a = GetKeyState(VK_CONTROL);
	int b = GetKeyState(VK_DOWN);
	if (a < 0 && b < 0) {
		return true;
	} else {
		return false;
	}
}

void WCH_check_clock() {
	Sleep((60 - WCH_GetTime().Second) * 1000);
	while (true) {
		WCH_Time NOW = WCH_GetTime();
		for (auto it = mm.equal_range(NOW.Hour).first; it != mm.equal_range(NOW.Hour).second; it++) {
			if ((it -> second).first == NOW.Minute && ((it -> second).second).size() > 0) {
				cout << "\a";
				MessageBox(NULL, ((it -> second).second).c_str(), "Web Class Helper Clock", MB_OK);
			}
		}
		Sleep(60000);
	}
}

void WCH_CL_Init() {
	WCH_GetPath(mode, UserName);
	cin >> op;
}

void WCH_PutPicture() {
	keybd_event(VK_SNAPSHOT, 0, 0, 0);
	keybd_event(VK_SNAPSHOT, 0, KEYEVENTF_KEYUP, 0);
	cout << "The picture is in the clipboard and be saved in your Pictures folder." << endl << endl;
}

void WCH_SaveImg() {
	system("START IMG.EXE");
	Sleep(500);
}

#endif
