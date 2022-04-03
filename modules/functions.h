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
#include <conio.h>
#include <direct.h>
#include "commands.h"
#include "file-process.h"
#include "variables.h"
#ifdef URLDownloadToFile
#undef URLDownloadToFile
#endif
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

typedef int(__stdcall *UDF)(LPVOID, LPCSTR, LPCSTR, DWORD, LPVOID);
UDF URLDownloadToFile = (UDF)GetProcAddress(LoadLibrary("urlmon.dll"), "URLDownloadToFileA");

void WCH_SetWindowStatus(bool flag) {
    HWND hwnd = FindWindow("ConsoleWindowClass", NULL);
    if (hwnd) {
        ShowWindow(hwnd, flag);
    }
    cmd_line = flag;
}

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
    SetConsoleTitle("Web Class Helper");
    WCH_Time q = WCH_GetTime();
    if (q.Month == 1 || q.Month == 2) {
        q.Month += 12;
        q.Year--;
    }
    WCH_SetWindowStatus(true);
    string tmp = WCH_VER;
    cout << "Web Class Helper " + tmp << endl;
    cout << "Copyright (c) 2022 Class Tools Develop Team." << endl;
    cout << "Type 'help' to get help." << endl << endl;
    cout << "Please input your username: ";
    cin >> UserName;
    cout << endl;
}

void WCH_Error(string INFO) {
    if (INFO == "OOR") {
        cout << "Your input code is out of range, please check and try again." << endl;
    }
    if (INFO == "NF") {
        cout << "An network error occurred, please check your network connection and try to update this program." << endl;
    }
    if (INFO == "CO") {
        cout << "Cannot Operate the clock, please try to restart this program." << endl;
    }
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
void WCH_PleaseYouChooseName(bool f) {
	HWND hwnd=GetForegroundWindow();
	if(f==1){
		ShowWindow(hwnd,SW_MINIMIZE);//Min
	}
	else{
		ShowWindow(hwnd,SW_MAXIMIZE);//Max
	}
}

void WCH_check_clock() {
    WCH_Time NOW = WCH_GetTime();
    for (auto it = mm.equal_range(NOW.Hour).first; it != mm.equal_range(NOW.Hour).second; it++) {
        if ((it -> second).first == NOW.Minute && ((it -> second).second).size() > 0) {
            cout << "\a";
            MessageBox(NULL, ((it -> second).second).c_str(), "Web Class Helper Clock", MB_OK);
        }
    }
}

void WCH_ReadData() {
    WCH_Time q = WCH_GetTime();
    fin.open(Weekdayname[(q.Day + 2 * q.Month + 3 * (q.Month + 1) / 5 + q.Year + q.Year / 4 - q.Year / 100 + q.Year / 400 + 1) % 7].c_str());
    fin >> WCH_clock_num;
    for (int i = 1; i <= WCH_clock_num; i++) {
        int h = 0;
        int m = 0;
        string tname = "NULL";
        fin >> h >> m >> tname;
        mm.emplace(make_pair(h, make_pair(m, tname)));
    }
}

void WCH_save() {
    if (WCH_clock_num != 0) {
        WCH_Time q = WCH_GetTime();
        fout.open(Weekdayname[(q.Day + 2 * q.Month + 3 * (q.Month + 1) / 5 + q.Year + q.Year / 4 - q.Year / 100 + q.Year / 400 + 1) % 7].c_str());
        fout << WCH_clock_num << endl;
        for (auto it = mm.begin(); it != mm.end(); it++) {
            fout << (it -> first) << " " << (it -> second).first << " " << (it -> second).second << endl;
        }
    }
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

void WCH_ow() {
    try {
        int len;
        DWORD unused;
        char url[128], *file;
        HANDLE hFile;
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
        delete[] file;
    }
    catch (...) {
        WCH_Error(WCH_ERRNO_NETWORK_FAILURE);
        return;
    }
}

void WCH_GetPath(bool mode, string UserName) {
    char ExeFile[256];
    _getcwd(ExeFile, 256);
    if (mode) {
        cout << ExeFile << "> ";
    } else {
        string tmp;
        tmp = ExeFile;
        tmp.replace(1, 2, "\\");
        for (long long unsigned int i = 0; i < tmp.size(); i++) {
            if (isupper(tmp[i])) {
                tmp[i] = tolower(tmp[i]);
            }
        }
        cout << UserName << "@\\" << tmp << "$ ";
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

void WCH_SetTrayStatus(bool flag) {
    string className = "Shell_trayWnd";
    HWND wnd = FindWindow(className.c_str(), NULL);
    ShowWindow(wnd,flag);
}

void WCH_SaveImg() {
    system("START IMG.EXE");
    Sleep(500);
}

#endif
