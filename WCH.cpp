/*
Web-Class-Helper 1.0.8
This source code file is under MIT License.
Copyright (c) 2022 Class Tools Develop Team
Contributors: jsh-jsh ren-yc
*/
#include <stdio.h>
#include <windows.h>
#include <conio.h>
#include <sstream>
#include <bits/stdc++.h>
#include "WCH.h"
using namespace std;

struct TalkDate
{
    int Year;
    int Month;
    int Day;
    int Hour;
    int Minute;
    int Second;
    int WeekDay;
    string Name;
};
multimap<int, pair<int, string>> mm;
TalkDate GetTime();
void PrintTime(TalkDate);
void PrintError();
void PrintOK();
void Init();
inline void in_data(string);
inline void out_data(string);
void HideWindow(bool);
bool JudgeKey();
void Game();
string op;
string UserName;
string Weekdayname[7] = {
    "Sunday.txt",
    "Monday.txt",
    "Tuesday.txt",
    "Wednesday.txt",
    "Thursday.txt",
    "Friday.txt",
    "Saturday.txt"};
int n;
bool fgh = true;
ifstream fin;
ofstream fout;

bool mode = false;
string lang = "Undefined";
string version = "Unknown";

int main()
{
    Init();
    system("mode con cols=100 lines=20");
    SetConsoleTitle(GetYaml("Title", lang).c_str());
    TalkDate q = GetTime();
    if (q.Month == 1 || q.Month == 2)
    {
        q.Month += 12;
        q.Year--;
    }
    int Week = (q.Day + 2 * q.Month + 3 * (q.Month + 1) / 5 + q.Year + q.Year / 4 - q.Year / 100 + q.Year / 400 + 1) % 7;
    HideWindow(1);
    fin.open(Weekdayname[Week].c_str());
    fin >> n;
    for (int i = 1; i <= n; i++)
    {
        int h = 0;
        int m = 0;
        string tname = "NULL";
        fin >> h >> m >> tname;
        mm.emplace(make_pair(h, make_pair(m, tname)));
    }
    cout << "Web-Class-Helper " + version << endl;
    cout << "Copyright (c) 2022 Class Tools Develop Team." << endl;
    cout << "Type 'help' to get help." << endl << endl;
    cout << "Username: ";
    cin >> UserName;
    printf("\n");
    while (op != "quit")
    {
        int h = GetTime().Hour;
        for (auto it = mm.equal_range(h).first; it != mm.equal_range(h).second; it++)
        {
            if ((it->second).first == GetTime().Minute && ((it->second).second).size() > 0)
            {
                printf("\a");
                MessageBox(NULL, ((it->second).second).c_str(), GetYaml("Title", lang).c_str(), MB_OK);
            }
        }
        if (JudgeKey())
        {
            HideWindow(1);
            fgh = true;
        }
        if (fgh)
        {
            GetPath(mode, UserName);
            cin >> op;
        }
        if (op == "add")
        {
            int h = 0;
            int m = 0;
            string tname = "NULL";
            scanf("%d%d", &h, &m);
            cin >> tname;
            if (h > 24 || m >= 60 || h < 1 || m < 0)
            {
                PrintError();
            }
            else
            {
                n++;
                mm.emplace(make_pair(h, make_pair(m, tname)));
                PrintOK();
            }
        }
        else if (op == "help")
        {
            cout << GetYaml("Quit", lang) << endl;
            cout << GetYaml("Add", lang) << endl;
            cout << GetYaml("Delete", lang) << endl;
            cout << GetYaml("Change", lang) << endl;
            cout << GetYaml("Ow", lang) << endl;
            cout << GetYaml("Hide", lang) << endl;
            cout << GetYaml("Game", lang) << endl;
            cout << GetYaml("Time", lang) << endl;
            cout << GetYaml("Pi", lang) << endl;
            cout << GetYaml("Mode", lang) << endl;
            cout << GetYaml("Cs", lang) << endl;
            cout << endl;
        }
        else if (op == "delete")
        {
            int h = 0;
            int m = 0;
            int flag = 0;
            string tname = "NULL";
            scanf("%d%d", &h, &m);
            cin >> tname;
            for (auto it = mm.equal_range(h).first; it != mm.equal_range(h).second; it++)
            {
                if ((it->second).first == m && ((it->second).second) == tname)
                {
                    mm.erase(it);
                    PrintOK();
                    flag = 1;
                    n--;
                }
            }
            if (!flag)
            {
                PrintError();
            }
        }
        else if (op == "change")
        {
            int h = 0;
            int m = 0;
            int flag = 0;
            string tname = "NULL";
            scanf("%d%d", &h, &m);
            cin >> tname;
            for (auto it = mm.equal_range(h).first; it != mm.equal_range(h).second; it++)
            {
                if ((it->second).first == m)
                {
                    ((it->second).second) = tname;
                    PrintOK();
                    flag = 1;
                }
            }
            if (!flag)
            {
                PrintError();
            }
        }
        else if (op == "ow")
        {
            GetGet();
        }
        else if (op == "hide")
        {
            HideWindow(0);
            fgh = false;
        }
        else if (op == "game")
        {
            Game();
        }
        else if (op == "time")
        {
            PrintTime(GetTime());
        }
        else if (op == "pi")
        {
            PutPicture();
        }
        else if (op == "mode")
        {
            string tmp;
            cin >> tmp;
            if (tmp == "cmd")
            {
                mode = true;
                PrintOK();
            }
            else if (tmp == "bash")
            {
                mode = false;
                PrintOK();
            }
            else
            {
                cout << GetYaml("UnknownMode", lang) << endl;
            }
        }
        else if (op == "cs")
        {
            system("python speedtest.py");
            PrintOK();
        }
        else if (op!="quit")
        {
            cout << GetYaml("Error", lang);
            char tmp;
            cin >> tmp;
            if (tmp == 'Y')
            {
                system(op.c_str());
            }
            cout << endl;
        }
    }
    if (n != 0)
    {
        fout.open(Weekdayname[Week].c_str());
        fout << n << endl;
        for (auto it = mm.begin(); it != mm.end(); it++)
        {
            fout << (it->first) << " " << (it->second).first << " " << (it->second).second << endl;
        }
    }
    return 0;
}
TalkDate GetTime()
{
    TalkDate NowTime;
    time_t rawtime;
    struct tm *ptminfo;
    time(&rawtime);
    ptminfo = localtime(&rawtime);
    NowTime.Year = ptminfo->tm_year + 1900;
    NowTime.Month = ptminfo->tm_mon + 1;
    NowTime.Day = ptminfo->tm_mday;
    NowTime.Hour = ptminfo->tm_hour;
    NowTime.Minute = ptminfo->tm_min;
    NowTime.Second = ptminfo->tm_sec;
    NowTime.Name = "NULL";
    return NowTime;
}

void PrintTime(TalkDate a)
{
    printf("%d/%02d/%02d %02d %02d %02d\n\n", a.Year, a.Month, a.Day, a.Hour, a.Minute, a.Second);
}

void PrintError()
{
    cout << GetYaml("PrintError", lang) << endl << endl;
}

inline void in_data(string fname)
{
    freopen(fname.c_str(), "r", stdin);
}

inline void out_data(string fname)
{
    freopen(fname.c_str(), "w", stdout);
}

void HideWindow(bool ju)
{
    HWND hwnd = FindWindow("ConsoleWindowClass", NULL);
    if (hwnd)
    {
        ShowWindow(hwnd, ju);
    }
}

bool JudgeKey()
{
    int a = GetKeyState(VK_CONTROL);
    int b = GetKeyState(VK_DOWN);
    if (a < 0 && b < 0)
    {
        return true;
    }
    else
    {
        return false;
    }
}

void Game()
{
    srand(time(0));
    int n = rand() % 10000 + 1;
    int z = 0;
    while (z != n)
    {
        cout << GetYaml("InputNumber", lang);
        cin >> z;
        if (z > n)
        {
            cout << GetYaml("Smaller", lang) << endl;
        }
        if (z < n)
        {
            cout << GetYaml("Bigger", lang) << endl;
        }
        if (z == -100000)
        {
            return;
        }
    }
    printf(GetYaml("Win", lang).c_str(), n);
    return;
}

void PrintOK()
{
    printf("OK!\n\n");
}

void Init()
{
    lang = GetYaml("Language", "None");
    mode = (GetYaml("CommandMode", "None") == "bash" ? false : true);
    version = GetYaml("Version", "None");
}
