/*
Web-Class-Helper Main File 1.1.2
This source code file is under MIT License.
Copyright (c) 2022 Class Tools Develop Team
Contributors: jsh-jsh ren-yc
*/
#define WCH_VER "1.1.2"
#define WCH_Framework "64"
#define WCH_DisplayFramework "64"
#include "modules/commands.h"
#include "modules/functions.h"
#include "modules/file-process.h"
#include "modules/apis.h"
#include "modules/variables.h"
using namespace std;

extern const string WCH_WDName[7];
extern multimap <int, pair <int, string>> WCH_clock_list;
extern set <string> WCH_task_list;
extern set <string> WCH_work_list;
extern HWND WCH_hWnd;
extern int WCH_clock_num;
extern int WCH_task_num;
extern int WCH_work_num;
extern int WCH_ProgressBarCount;
extern int WCH_ProgressBarTot;
extern bool WCH_cmd_line;
extern bool WCH_anti_idle;
extern bool WCH_program_end;
extern bool WCH_wait_cmd;
extern string WCH_command;
extern string WCH_ProgressBarStr;
extern ifstream fin;
extern ofstream fout;
extern wifstream wfin;
extern wofstream wfout;
WCH_Time WCH_GetTime();
void WCH_Sleep(int _ms);
void WCH_Error(string INFO);
void WCH_printlog(int w, initializer_list <string> other);
void WCH_read();
void WCH_save();
int WCH_GetNumDigits(int n);

int main() {
	WCH_Init();
	while (WCH_command != "quit") {
		if (WCH_ShortCutKeyCheck() && !WCH_cmd_line) {
			WCH_SetWindowStatus(true);
			if (WCH_anti_idle == true) {
				WCH_SetTrayStatus(true);
				WCH_SetWindowSize(SW_NORMAL, GetActiveWindow());
				WCH_anti_idle = false;
			}
		}
		if (WCH_cmd_line) {
			WCH_CL_Init();
			WCH_printlog(WCH_LOG_MODE_RC, {"command", WCH_command});
			if (WCH_command == "clock") {
				WCH_check_clock();
			} else if (WCH_command == "task") {
				WCH_check_task();
			} else if (WCH_command == "work") {
				WCH_check_work();
			} else if (WCH_command == "help") {
				WCH_help();
			} else if (WCH_command == "ow") {
				WCH_ow();
			} else if (WCH_command == "hide") {
				WCH_hide();
			} else if (WCH_command == "game") {
				WCH_game();
			} else if (WCH_command == "time") {
				WCH_time();
			} else if (WCH_command == "pi") {
				WCH_pi();
			} else if (WCH_command == "speedtest") {
				WCH_speedtest();
			} else if (WCH_command == "trans") {
				WCH_trans();
			} else if (WCH_command == "anti-idle") {
				WCH_anti_idle_func();
			} else if (WCH_command == "update") {
				WCH_update();
			} else if (WCH_command == "license") {
				WCH_license();
			} else if (WCH_command != "quit") {
				if (WCH_cmd_line) {
					WCH_printlog(WCH_LOG_MODE_RC, {"command", "unknown"});
					WCH_unknown(WCH_command);
				}
			}
			cout << endl;
		}
	}
	return 0;
}