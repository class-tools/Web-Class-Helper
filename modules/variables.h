/*
Web-Class-Helper Commands Module Header File 1.1.2
This source code file is under MIT License.
Copyright (c) 2022 Class Tools Develop Team
Contributors: jsh-jsh ren-yc
*/
#ifndef VARIABLES_H
#define VARIABLES_H
#include <bits/stdc++.h>
#include <windows.h>
#include <wininet.h>
#include <tlhelp32.h>
#include <conio.h>
#include <direct.h>
#include "commands.h"
#include "functions.h"
#include "file-process.h"
#include "apis.h"
using namespace std;

#define WCH_ERRNO_OUT_OF_RANGE "OOR"
#define WCH_ERRNO_NETWORK_FAILURE "NF"
#define WCH_ERRNO_CLOCK_OPERATION "CO"
#define WCH_ERRNO_TASK_OPERATION "TO"
#define WCH_LOG_MODE_ST -1
#define WCH_LOG_MODE_ERROR 0
#define WCH_LOG_MODE_RC 1
#define WCH_LOG_MODE_RW 2
#define WCH_LOG_MODE_KT 3
#define WCH_LOG_STATUS_INFO "[INFO]"
#define WCH_LOG_STATUS_ERROR "[ERROR]"

struct WCH_Time {
	int Year;
	int Month;
	int Day;
	int Hour;
	int Minute;
	int Second;
	int WeekDay;
	string Name;
};
struct WCH_Data_Body {
	int H;
	int M;
};
const string Weekdayname[7] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};
multimap <int, pair <int, string>> WCH_clock;
set <string> WCH_task_list;
HWND hwnd;
int WCH_clock_num;
bool cmd_line = true;
bool anti_idle = false;
bool mode = false;
string op;
string UserName;
ifstream fin;
ofstream fout;
WCH_Time WCH_GetTime();
void WCH_Error(string INFO);

#endif