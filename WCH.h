#include <stdio.h>
#include <windows.h>
#include <conio.h>
#include <sstream>
#include <bits/stdc++.h>
using namespace std;
#ifdef URLDownloadToFile
#undef URLDownloadToFile
#endif
typedef int(__stdcall *UDF)(LPVOID, LPCSTR, LPCSTR, DWORD, LPVOID);
UDF URLDownloadToFile = (UDF)GetProcAddress(LoadLibrary("urlmon.dll"), "URLDownloadToFileA");
void UTF8ToANSI(char *str)
{
    int len = MultiByteToWideChar(CP_UTF8, 0, str, -1, 0, 0);
    WCHAR *wsz = new WCHAR[len + 1];
    len = MultiByteToWideChar(CP_UTF8, 0, str, -1, wsz, len);
    wsz[len] = 0;
    len = WideCharToMultiByte(CP_ACP, 0, wsz, -1, 0, 0, 0, 0);
    len = WideCharToMultiByte(CP_ACP, 0, wsz, -1, str, len, 0, 0);
    str[len] = 0;
    delete[] wsz;
}
HANDLE hOutput;
char name[32];
int cnt[8];
void GetGet()
{
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
    cout << file << endl;
    DeleteFile("download.tmp");
    delete[] file;
}