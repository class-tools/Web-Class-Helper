/*
Web-Class-Helper APIs Module Header File 1.1.0
This source code file is under MIT License.
Copyright (c) 2022 Class Tools Develop Team
Contributors: jsh-jsh ren-yc
*/
#ifndef APIS_H
#define APIS_H
#include <bits/stdc++.h>
#include <windows.h>
#include <conio.h>
#include <direct.h>
#include "commands.h"
#include "file-process.h"
#include "functions.h"
#include "variables.h"
using namespace std;

extern const string Weekdayname[7];
extern multimap <int, pair <int, string>> mm;
extern int WCH_clock_num;
extern bool cmd_line;
extern bool anti_idle;
extern bool mode;
extern string op;
extern string UserName;
extern ifstream fin;
extern ofstream fout;

string WCH_GetUserName() {
    char tmpc[256] = {0};
    DWORD Size = 256;
    GetUserName(tmpc, &Size);
    string tmps = tmpc;
    return tmps;
}

void WCH_SetWindowStatus(bool flag) {
    HWND hwnd = FindWindow("ConsoleWindowClass", NULL);
    if (hwnd) {
        ShowWindow(hwnd, flag);
    }
    cmd_line = flag;
}

void WCH_SetWindowPos(bool flag) {
    HWND hwnd = GetActiveWindow();
    ShowWindow(hwnd, (flag == true ? SW_MAXIMIZE : SW_MINIMIZE));
}

void WCH_SetTrayStatus(bool flag) {
    HWND hwnd = FindWindow("Shell_trayWnd", NULL);
    if (hwnd) {
        ShowWindow(hwnd, flag);
    }
}

#endif
