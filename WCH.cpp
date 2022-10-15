/*
Web Class Helper Main File 2.1.1
This source code file is under MIT License.
Copyright (c) 2022 Class Tools Develop Team
Contributors: jsh-jsh ren-yc
*/
#define WCH_VER_MAIN TEXT("2.1.1")
#define WCH_VER_TYPE 1
#if WCH_VER_TYPE != 0
	#define WCH_VER_BUILD 1
#endif
#include "./modules/file-process.hpp"
#include "./modules/init.hpp"
#include "./modules/commands.hpp"
#include "./modules/functions.hpp"
#include "./modules/apis.hpp"
#include "./modules/basic.hpp"

extern const array<wstring, 7> WCH_weekday_list;
extern const array<wstring, 2> WCH_language_list;
extern map<wstring, function<void()>> WCH_command_support;
extern set<tuple<wstring, wstring, wstring>> WCH_settings_support;
extern set<wstring> WCH_language_support;
extern vector<wstring> WCH_command_list;
extern set<tuple<int32_t, int32_t, wstring>> WCH_clock_list;
extern set<wstring> WCH_task_list;
extern set<pair<wstring, wstring>> WCH_work_list;
extern wstring WCH_window_title;
extern wstring WCH_command;
extern wstring WCH_ProgressBarStr;
extern HWND WCH_window_handle;
extern HWND WCH_tray_handle;
extern HMENU WCH_menu_handle;
extern NOTIFYICONDATA WCH_NID;
extern ATL::CComPtr<ITaskbarList3> WCH_TBL;
extern Json::Value WCH_Settings;
extern Json::Value WCH_Language;
extern int32_t WCH_clock_num;
extern int32_t WCH_task_num;
extern int32_t WCH_work_num;
extern int32_t WCH_clock_change;
extern int32_t WCH_task_change;
extern int32_t WCH_work_change;
extern int32_t WCH_settings_change;
extern int32_t WCH_ProgressBarTot;
extern int32_t WCH_InputTimes;
extern bool WCH_cmd_line;
extern bool WCH_anti_idle;
extern bool WCH_count_down;
extern bool WCH_program_end;
extern bool WCH_pre_start;
extern ifstream fin;
extern wifstream wfin;
extern ofstream fout;
extern wofstream wfout;
extern Json::Reader JSON_Reader;
extern Json::StreamWriterBuilder JSON_SWB;
extern unique_ptr<Json::StreamWriter> JSON_SW;
WCH_Time WCH_GetTime();
void WCH_Sleep(int32_t _ms);
void WCH_printlog(wstring _mode, wstring _info);
void WCH_read_settings();
void WCH_read();
void WCH_save_settings();
bool WCH_save_func(bool output);
size_t WCH_GetNumDigits(size_t _n);

int32_t wmain([[maybe_unused]] int argc, [[maybe_unused]] wchar_t* argv[]) {
	WCH_Init();
	while (true) {
		if (WCH_cmd_line) {
			WCH_CL_Init();
			if (WCH_command_support.find(WCH_command_list[0]) != WCH_command_support.end()) {
				WCH_command_support.find(WCH_command_list[0])->second();
			} else {
				WCH_InputCodeIncorrect();
			}
			wcout << endl;
		} else {
			WCH_Sleep(1000);
		}
	}
	return 0;
}