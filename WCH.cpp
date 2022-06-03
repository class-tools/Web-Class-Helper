/*
Web Class Helper Main File 2.0.1
This source code file is under MIT License.
Copyright (c) 2022 Class Tools Develop Team
Contributors: jsh-jsh ren-yc
*/
#define WCH_VER "2.0.1"
#include "resource.h"
#include "modules/file-process.h"
#include "modules/init.h"
#include "modules/commands.h"
#include "modules/functions.h"
#include "modules/apis.h"
#include "modules/basic.h"

extern const string WCH_WDName[7];
extern map <string, function <void ()>> WCH_command_support;
extern vector <string> WCH_command_list;
extern multimap <int, pair <int, string>> WCH_clock_list;
extern set <string> WCH_task_list;
extern set <string> WCH_work_list;
extern wstring WCH_window_title;
extern HWND WCH_hWnd;
extern HMENU WCH_hMenu;
extern int WCH_clock_num;
extern int WCH_task_num;
extern int WCH_work_num;
extern int WCH_clock_change;
extern int WCH_task_change;
extern int WCH_work_change;
extern int WCH_ProgressBarCount;
extern int WCH_ProgressBarTot;
extern int WCH_InputTimes;
extern bool WCH_cmd_line;
extern bool WCH_anti_idle;
extern bool WCH_program_end;
extern string WCH_command;
extern string WCH_ProgressBarStr;
extern ifstream fin;
extern ofstream fout;
WCH_Time WCH_GetTime();
void WCH_Sleep(int _ms);
void WCH_Error(int _in);
void WCH_printlog(string _mode, string _info);
void WCH_read();
bool WCH_save_func();
int WCH_GetNumDigits(int _n);

int main() {
	WCH_Init();
	while (true) {
		if (WCH_cmd_line) {
			WCH_CL_Init();
			if (WCH_command_support.find(WCH_command_list[0]) != WCH_command_support.end()) {
				WCH_printlog(WCH_LOG_STATUS_INFO, "Using command \"" + WCH_command + "\"");
				WCH_command_support.find(WCH_command_list[0]) -> second();
			} else {
				WCH_printlog(WCH_LOG_STATUS_INFO, "Using unknown command \"" + WCH_command + "\"");
				WCH_Error(WCH_ERRNO_UNCORRECT);
			}
			cout << endl;
		} else {
			WCH_Sleep(1000);
		}
	}
	return 0;
}