/*
Web Class Helper Main File 2.0.1
This source code file is under MIT License.
Copyright (c) 2022 Class Tools Develop Team
Contributors: jsh-jsh ren-yc
*/
#define WCH_VER L"2.0.1"
#include "yaml-cpp/yaml.h"
#include "json/json.h"
#include "modules/file-process.hpp"
#include "modules/init.hpp"
#include "modules/commands.hpp"
#include "modules/functions.hpp"
#include "modules/apis.hpp"
#include "modules/basic.hpp"

extern const wstring WCH_WDName[7];
extern map <wstring, function <void ()>> WCH_command_support;
extern vector <wstring> WCH_command_list;
extern multimap <int, pair <int, wstring>> WCH_clock_list;
extern set <wstring> WCH_task_list;
extern set <wstring> WCH_work_list;
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
extern wstring WCH_command;
extern wstring WCH_ProgressBarStr;
extern ifstream fin;
extern wifstream wfin;
extern ofstream fout;
extern wofstream wfout;
WCH_Time WCH_GetTime();
void WCH_Sleep(int _ms);
void WCH_printlog(wstring _mode, wstring _info);
void WCH_read();
bool WCH_save_func();
int WCH_GetNumDigits(int _n);

int main() {
	WCH_Init();
	while (true) {
		if (WCH_cmd_line) {
			WCH_CL_Init();
			if (WCH_command_support.find(WCH_command_list[0]) != WCH_command_support.end()) {
				WCH_printlog(WCH_LOG_STATUS_INFO, L"Using command \"" + WCH_command + L"\"");
				WCH_command_support.find(WCH_command_list[0]) -> second();
			} else {
				WCH_printlog(WCH_LOG_STATUS_INFO, L"Using unknown command \"" + WCH_command + L"\"");
				WCH_printlog(WCH_LOG_STATUS_WARN, L"Your input code is uncorrect, please check and try again");
				wcout << L"Your input code is uncorrect, please check and try again." << endl;
			}
			wcout << endl;
		} else {
			WCH_Sleep(1000);
		}
	}
	return 0;
}