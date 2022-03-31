/*
Web-Class-Helper Function Module Header File 1.1.0
This source code file is under MIT License.
Copyright (c) 2022 Class Tools Develop Team
Contributors: jsh-jsh ren-yc
*/
#pragma once
#include <bits/stdc++.h>
#include <windows.h>
#include <conio.h>
#include <direct.h>
#include "file-process.h"
#include "commands.h"
#ifdef URLDownloadToFile
#undef URLDownloadToFile
#endif
using namespace std;

struct TalkDate {
    int Year;
    int Month;
    int Day;
    int Hour;
    int Minute;
    int Second;
    int WeekDay;
    string Name;
};

typedef int(__stdcall *UDF)(LPVOID, LPCSTR, LPCSTR, DWORD, LPVOID);
UDF URLDownloadToFile = (UDF)GetProcAddress(LoadLibrary("urlmon.dll"), "URLDownloadToFileA");

HANDLE hOutput;
char name[32];
int cnt[8];

void WCH_SetWindowStatus(bool flag) {
    HWND hwnd = FindWindow("ConsoleWindowClass", NULL);
    if (hwnd) {
        ShowWindow(hwnd, flag);
    }
}

TalkDate WCH_GetTime() {
    TalkDate NowTime;
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

void WCH_Init(int *n, string *UserName, multimap <int, pair <int, string>> *mm) {
    system("mode con cols=100 lines=20");
    SetConsoleTitle("Web Class Helper");
    TalkDate q = WCH_GetTime();
    if (q.Month == 1 || q.Month == 2) {
        q.Month += 12;
        q.Year--;
    }
    WCH_SetWindowStatus(true);
    cout << strcat((char*)"Web Class Helper ", WCH_VER) << endl;
    cout << "Copyright (c) 2022 Class Tools Develop Team." << endl;
    cout << "Type 'help' to get help." << endl << endl;
    cout << "Please input your username: " << endl;
    cin >> *UserName;
    cout << endl;
}

void WCH_Error() {
    cout << "This input or code is wrong. Please see your code." << endl;
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
    int len;
    DWORD unused;
    char url[128], *file;
    HANDLE hFile;
    hOutput = GetStdHandle(STD_OUTPUT_HANDLE);
    char ss[128] = "https://v1.hitokoto.cn/?encode=text";
    sprintf(url, ss);
    URLDownloadToFile(0, url, "download.tmp", 0, 0);
    hFile = CreateFile("download.tmp", GENERIC_READ, FILE_SHARE_READ, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
    len = GetFileSize(hFile, 0);
    file = new char[len + 3];
    ReadFile(hFile, file, len, &unused, 0);
    file[len] = file[len + 1] = 0;
    CloseHandle(hFile);
    UTF8ToANSI(file);
    memset(cnt, 0, sizeof(cnt));
    cout << file << endl << endl;
    DeleteFile("download.tmp");
    delete[] file;
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

void WCH_PutPicture() {
    keybd_event(VK_SNAPSHOT, 0, 0, 0);
    keybd_event(VK_SNAPSHOT, 0, KEYEVENTF_KEYUP, 0);
    cout << "The picture is in the clipboard and be saved in your Pictures folder." << endl << endl;
}

void WCH_SaveImg() {
    system("start img.exe");
    Sleep(500);
}
