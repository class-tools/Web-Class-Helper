/*
Web-Class-Helper Main File 1.1.0
This source code file is under MIT License.
Copyright (c) 2022 Class Tools Develop Team
Contributors: jsh-jsh ren-yc
*/
#define WCH_VER "1.1.0"
#define WCH_Framework "x64"
#include "modules/commands.h"
#include "modules/functions.h"
#include "modules/file-process.h"
#include "modules/apis.h"
#include "modules/variables.h"
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
WCH_Time WCH_GetTime();

int main() {
	WCH_Init();
	WCH_ReadData();
	atexit(WCH_save);
	thread T(WCH_check_clock);
	T.detach();
	while (op != "quit") {
		if (WCH_ShortCutKeyCheck() && !cmd_line) {
			WCH_SetWindowStatus(true);
			if (anti_idle == true) {
				WCH_SetTrayStatus(true);
				WCH_SetWindowPos(true);
			}
		}
		if (cmd_line) {
			WCH_CL_Init();
			WCH_printlog(WCH_LOG_MODE_RC, {op});
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
				WCH_hide();
			} else if (op == "game") {
				WCH_game();
			} else if (op == "time") {
				WCH_time();
			} else if (op == "pi") {
				WCH_pi();
			} else if (op == "mode") {
				WCH_mode();
			} else if (op == "speedtest") {
				WCH_speedtest();
			} else if (op == "anti-idle") {
				WCH_anti_idle();
			} else if (op == "trans") {
				WCH_trans();
			} else if (op == "update") {
				WCH_update();
			} else if (op != "quit") {
				WCH_printlog(WCH_LOG_MODE_RC, {"unknown"});
				WCH_unknown(op);
			}
			cout << endl;
		}
	}
	WCH_save();
	return 0;
}
