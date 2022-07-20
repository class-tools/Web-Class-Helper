/*
Web Class Helper Main File 2.1.0
This source code file is under MIT License.
Copyright (c) 2022 Class Tools Develop Team
Contributors: jsh-jsh ren-yc
*/
#define WCH_VER_MAIN TEXT("2.1.0")
#define WCH_VER_TYPE 1
#include "modules/file-process.hpp"
#include "modules/init.hpp"
#include "modules/commands.hpp"
#include "modules/functions.hpp"
#include "modules/apis.hpp"
#include "modules/basic.hpp"

extern const wstring WCH_WDName[7];
extern map<wstring, function<void()>> WCH_command_support;
extern vector<wstring> WCH_command_list;
extern set<tuple<int, int, wstring>> WCH_clock_list;
extern set<wstring> WCH_task_list;
extern set<pair<wstring, wstring>> WCH_work_list;
extern wstring WCH_window_title;
extern HWND WCH_Win_hWnd;
extern HWND WCH_Tray_hWnd;
extern HMENU WCH_hMenu;
extern NOTIFYICONDATA WCH_NID;
extern ATL::CComPtr<ITaskbarList3> WCH_TBL;
extern int WCH_clock_num;
extern int WCH_task_num;
extern int WCH_work_num;
extern int WCH_clock_change;
extern int WCH_task_change;
extern int WCH_work_change;
extern int WCH_ProgressBarTot;
extern int WCH_InputTimes;
extern bool WCH_cmd_line;
extern bool WCH_anti_idle;
extern bool WCH_count_down;
extern bool WCH_program_end;
extern bool WCH_pre_start;
extern wstring WCH_command;
extern wstring WCH_ProgressBarStr;
extern ifstream fin;
extern wifstream wfin;
extern ofstream fout;
extern wofstream wfout;
extern Json::StreamWriterBuilder Json_SWB;
WCH_Time WCH_GetTime();
void WCH_Sleep(int _ms);
void WCH_printlog(wstring _mode, wstring _info);
void WCH_read();
bool WCH_save_func();
size_t WCH_GetNumDigits(size_t _n);

int main() {
	WCH_Init();
	while (true) {
		if (WCH_cmd_line) {
			WCH_CL_Init();
			if (WCH_command_support.find(WCH_command_list[0]) != WCH_command_support.end()) {
				WCH_command_support.find(WCH_command_list[0])->second();
			} else {
				WCH_PrintIncorrect();
			}
			wcout << endl;
		} else {
			WCH_Sleep(1000);
		}
	}
	return 0;
}