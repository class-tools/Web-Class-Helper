/*
Web-Class-Helper Main File 1.1.0
This source code file is under MIT License.
Copyright (c) 2022 Class Tools Develop Team
Contributors: jsh-jsh ren-yc
*/
#define WCH_VER "1.1.0"
#include "modules/commands.h"
#include "modules/file-process.h"
#include "modules/functions.h"
using namespace std;

extern const string Weekdayname[7];
extern multimap <int, pair <int, string>> mm;
extern int WCH_clock_num;
extern bool cmd_line;
extern bool anti_idle;
extern bool mode;
extern string op;
extern string UserName;

int main() {
    WCH_Init();
    WCH_ReadData();
    while (op != "quit") {
        WCH_check_clock();
        if (WCH_ShortCutKeyCheck()) {
            WCH_SetWindowStatus(true);
            if (anti_idle == true) {
                WCH_SetTrayStatus(true);
            }
        }
        if (cmd_line) {
            WCH_GetPath(mode, UserName);
            cin >> op;
            if (op == "add") {
                WCH_add();
            } else if (op == "delete") {
                WCH_delete();
            } else if (op == "change") {
                WCH_change();
            } else if (op == "help") {
                WCH_help();
            } else if (op == "ow") {
                WCH_ow();
            } else if (op == "hide") {
                WCH_SetWindowStatus(false);
            } else if (op == "game") {
                WCH_game();
            } else if (op == "time") {
                char tmp[21];
                WCH_Time a = WCH_GetTime();
                sprintf(tmp, "%04d/%02d/%02d %02d %02d %02d", a.Year, a.Month, a.Day, a.Hour, a.Minute, a.Second);
                cout << tmp << endl << endl;
            } else if (op == "pi") {
                WCH_pi();
            } else if (op == "mode") {
                WCH_mode();
            } else if (op == "speedtest") {
                WCH_speedtest();
            } else if (op == "anti-idle") {
                WCH_anti_idle();
            } else if (op != "quit") {
                WCH_unknown();
            }
            cout << endl;
        }
    }
    WCH_save();
    return 0;
}
