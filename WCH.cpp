/*
Web-Class-Helper Main File 1.1.2
This source code file is under MIT License.
Copyright (c) 2022 Class Tools Develop Team
Contributors: jsh-jsh ren-yc
*/
#define WCH_VER "1.1.2"
#define WCH_Framework "64"
#include "modules/commands.h"
#include "modules/functions.h"
#include "modules/file-process.h"
#include "modules/apis.h"
#include "modules/variables.h"
using namespace std;

extern const string Weekdayname[7];
extern multimap <int, pair <int, string>> WCH_clock;
extern set <string> WCH_task_list;
extern HWND hwnd;
extern int WCH_clock_num;
extern int WCH_task_num;
extern int WCH_ProcessBarCount;
extern int WCH_ProcessBarTot;
extern bool cmd_line;
extern bool anti_idle;
extern bool isend;
extern string op;
extern ifstream fin;
extern ofstream fout;
WCH_Time WCH_GetTime();
void WCH_Error(string INFO);
void WCH_printlog(int w, initializer_list <string> other);
int WCH_GetNumDigits(int n);

int main() {
	WCH_Init();
	WCH_read();
	while (op != "quit") {
		if (WCH_ShortCutKeyCheck() && !cmd_line) {
			WCH_SetWindowStatus(true);
			if (anti_idle == true) {
				WCH_SetTrayStatus(true);
				WCH_SetWindowSize(SW_NORMAL, GetActiveWindow());
				anti_idle = false;
			}
		}
		if (cmd_line) {
			WCH_CL_Init();
			WCH_printlog(WCH_LOG_MODE_RC, {op});
			if (op == "clock") {
				WCH_check_clock();
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
			} else if (op == "speedtest") {
				WCH_speedtest();
			} else if (op == "trans") {
				WCH_trans();
			} else if (op == "anti-idle") {
				WCH_anti_idle();
			} else if (op == "task") {
				WCH_check_task();
			} else if (op == "update") {
				WCH_update();
			} else if (op == "license") {
				WCH_license();
			} else if (op != "quit") {
				WCH_printlog(WCH_LOG_MODE_RC, {"unknown"});
				WCH_unknown(op);
			}
			cout << endl;
		}
	}
	return 0;
}
