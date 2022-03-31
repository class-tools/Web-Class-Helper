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

multimap <int, pair <int, string>> mm;

int n;
bool cmd_line = true;
bool mode = false;
string op;
string UserName;

int main() {
    WCH_Init(&n, &UserName, &mm);
    WCH_ReadData(&n, &mm);
    while (op != "quit") {
        WCH_check_clock(WCH_GetTime().Hour, &mm);
        if (WCH_ShortCutKeyCheck()) {
            WCH_SetWindowStatus(true);
            cmd_line = true;
        }
        if (cmd_line) {
            WCH_GetPath(mode, UserName);
            cin >> op;
        }
        if (op == "add") {
            WCH_add(&n, &mm);
        } else if (op == "delete") {
            WCH_delete(&n, &mm);
        } else if (op == "change") {
            WCH_change(&mm);
        } else if (op == "help") {
            WCH_help();
        } else if (op == "ow") {
            WCH_ow();
        } else if (op == "hide") {
            WCH_SetWindowStatus(false);
            cmd_line = false;
        } else if (op == "game") {
            WCH_game();
        } else if (op == "time") {
            char tmp[21];
            TalkDate a = WCH_GetTime();
            sprintf(tmp, "%04d/%02d/%02d %02d %02d %02d", a.Year, a.Month, a.Day, a.Hour, a.Minute, a.Second);
            cout << tmp << endl << endl;
        } else if (op == "pi") {
            WCH_pi();
        } else if (op == "mode") {
            WCH_mode(&mode);
        } else if (op == "speedtest") {
            system("start speedtest.exe");
        } else if (op != "quit") {
            WCH_unknown(&op);
        }
    }
    WCH_save(&n, &mm);
    return 0;
}
