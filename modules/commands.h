/*
Web-Class-Helper Commands Module Header File 1.1.0
This source code file is under MIT License.
Copyright (c) 2022 Class Tools Develop Team
Contributors: jsh-jsh ren-yc
*/
#ifndef COMMANDS_H
#define COMMANDS_H
#include <bits/stdc++.h>
#include <windows.h>
#include <conio.h>
#include <direct.h>
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

void WCH_add() {
    int h = 0;
    int m = 0;
    string tname = "NULL";
    cin >> h >> m >> tname;
    if (h > 24 || m >= 60 || h < 1 || m < 0) {
        WCH_Error(WCH_ERRNO_OUT_OF_RANGE);
    } else {
        WCH_clock_num++;
        mm.emplace(make_pair(h, make_pair(m, tname)));
    }
}

void WCH_delete() {
    int h = 0;
    int m = 0;
    int flag = 0;
    string tname = "NULL";
    cin >> h >> m >> tname;
    for (auto it = mm.equal_range(h).first; it != mm.equal_range(h).second; it++) {
        if ((it -> second).first == m && ((it -> second).second) == tname) {
            mm.erase(it);
            flag = 1;
            WCH_clock_num--;
        }
    }
    if (!flag) {
        WCH_Error(WCH_ERRNO_CLOCK_OPERATION);
        return;
    }
}

void WCH_change() {
    int h = 0;
    int m = 0;
    bool flag = false;
    string tname = "NULL";
    cin >> h >> m >> tname;
    for (auto it = mm.equal_range(h).first; it != mm.equal_range(h).second; it++) {
        if ((it -> second).first == m) {
            ((it -> second).second) = tname;
            flag = true;
        }
    }
    if (!flag) {
        WCH_Error(WCH_ERRNO_CLOCK_OPERATION);
        return;
    }
}

void WCH_game() {
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
    system("START SPEEDTEST.EXE");
}

void WCH_pi() {
    WCH_SetWindowStatus(false);
    WCH_PutPicture();
    WCH_SetWindowStatus(true);
    WCH_SaveImg();
}

void WCH_mode() {
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
    char ch;
    cout << "Are you sure to enable anti-idle function? If you want to disable it, press Ctrl + Down. (Y/N): ";
    cin >> ch;
    if (ch == 'Y') {
    	anti_idle=true;
        WCH_SetWindowStatus(false);
        WCH_SetTrayStatus(false);
        WCH_PleaseYouChooseName(false);
    }
}

void WCH_trans() {
    string info;
    getline(cin, info);
    info = "START /B TRANS.EXE -i \"" + info + "\"";
    system(info.c_str());
    cmd_line = false;
    Sleep(2000);
    cmd_line = true;
}

void WCH_unknown() {
    printf("Is it a system command? (Y/N): ");
    char tmp;
    cin >> tmp;
    if (tmp == 'Y') {
        system(op.c_str());
    }
}

void WCH_help() {
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
    cout << "anti-idle (Enable anti-idle mode)" << endl;
    cout << "trans {info} (Translate {info} that was input after spaces between Chinese and English)" << endl;
    cout << "quit (Quit this program)" << endl;
}

#endif
