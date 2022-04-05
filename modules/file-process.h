/*
Web-Class-Helper File Process Module Header File 1.1.0
This source code file is under MIT License.
Copyright (c) 2022 Class Tools Develop Team
Contributors: jsh-jsh ren-yc
*/
#ifndef FILE_PROCESS_H
#define FILE_PROCESS_H
#include <bits/stdc++.h>
#include <windows.h>
#include <wininet.h>
#include <conio.h>
#include <direct.h>
#include "commands.h"
#include "functions.h"
#include "variables.h"
#include "apis.h"
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

void WCH_ReadData() {
    WCH_Time q = WCH_GetTime();
    string FilePath = "./data/" + Weekdayname[(q.Day + 2 * q.Month + 3 * (q.Month + 1) / 5 + q.Year + q.Year / 4 - q.Year / 100 + q.Year / 400 + 1) % 7] + ".dat";
    fin.open(FilePath);
    if (!fin.is_open()) {
        return;
    }
    fin >> WCH_clock_num;
    for (int i = 1; i <= WCH_clock_num; i++) {
        int H, M;
        string Tname;
        fin >> H >> M >> Tname;
        mm.emplace(make_pair(H, make_pair(M, Tname)));
    }
    fin.close();
}

void WCH_save() {
    if (WCH_clock_num == 0) {
        return;
    }
    WCH_Time q = WCH_GetTime();
    string FilePath = "./data/" + Weekdayname[(q.Day + 2 * q.Month + 3 * (q.Month + 1) / 5 + q.Year + q.Year / 4 - q.Year / 100 + q.Year / 400 + 1) % 7] + ".dat";
    fout.open(FilePath, ios::binary);
    fout << WCH_clock_num << endl;
    for (auto it = mm.begin(); it != mm.end(); it++) {
        fout << (it -> first) << " " << (it -> second).first << " " << (it -> second).second << endl;
    }
    fout.close();
}

void UTF8ToANSI(char *str) {
    int len = MultiByteToWideChar(CP_UTF8, 0, str, -1, 0, 0);
    WCHAR *wsz = new WCHAR[len + 1];
    len = MultiByteToWideChar(CP_UTF8, 0, str, -1, wsz, len);
    wsz[len] = 0;
    len = WideCharToMultiByte(CP_ACP, 0, wsz, -1, 0, 0, 0, 0);
    len = WideCharToMultiByte(CP_ACP, 0, wsz, -1, str, len, 0, 0);
    str[len] = 0;
    delete[] wsz;
}

#endif
