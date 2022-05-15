/*
Web Class Helper Main File 2.0.0
This source code file is under MIT License.
Copyright (c) 2022 Class Tools Develop Team
Contributors: jsh-jsh ren-yc
*/
#define WCH_VER "2.0.0"
#include "modules/file-process.h"
#include "modules/init.h"
#include "modules/commands.h"
#include "modules/functions.h"
#include "modules/apis.h"
#include "modules/variables.h"
using namespace std;

extern const string WCH_WDName[7];
extern map <string, function <void ()>> WCH_command_support;
extern vector <string> WCH_command_list;
extern multimap <int, pair <int, string>> WCH_clock_list;
extern set <string> WCH_task_list;
extern set <string> WCH_work_list;
extern HWND WCH_hWnd;
extern int WCH_clock_num;
extern int WCH_task_num;
extern int WCH_work_num;
extern int WCH_ProgressBarCount;
extern int WCH_ProgressBarTot;
extern int WCH_InputTimes;
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
void WCH_Error(int _in);
void WCH_printlog(string _mode, string _info);
void WCH_read();
void WCH_save();
int WCH_GetNumDigits(int n);

int main() {
	WCH_Init();
	while (true) {
		if (WCH_ShortCutKeyCheck() && !WCH_cmd_line) {
			WCH_SetWindowStatus(true);
			if (WCH_anti_idle) {
				WCH_SetTrayStatus(true);
				WCH_SetWindowSize(SW_NORMAL, GetActiveWindow());
				WCH_anti_idle = false;
				continue;
			}
		}
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
		}
	}
	return 0;
}