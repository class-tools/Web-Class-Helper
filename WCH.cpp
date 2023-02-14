/*
Web Class Helper Main File 2.1.2
This source code file is under MIT License.
Copyright (c) 2022 - 2023 Class Tools Develop Team
Contributors: jsh-jsh ren-yc
*/
#include "version.hpp"
#include "shasum.hpp"
#include "./modules/file-process.hpp"
#include "./modules/init.hpp"
#include "./modules/commands.hpp"
#include "./modules/functions.hpp"
#include "./modules/apis.hpp"
#include "./modules/basic.hpp"

extern const vector<wstring> WCH_list_weekday;
extern const map<wstring, set<wstring>> WCH_choice_settings;
extern const map<wstring, wstring> WCH_MIME_list;
extern const map<wstring, function<void()>> WCH_support_command;
extern const set<tuple<wstring, wstring, wstring, bool>> WCH_support_settings;
extern const wstring WCH_progress_bar_str;
extern const wstring WCH_path_data;
extern const wstring WCH_path_temp;
extern vector<wstring> WCH_list_command;
extern set<tuple<int32_t, int32_t, wstring>> WCH_list_clock;
extern set<wstring> WCH_list_task;
extern set<pair<wstring, wstring>> WCH_list_work;
extern wstring WCH_version;
extern wstring WCH_path_exec;
extern wstring WCH_title_window;
extern HWND WCH_handle_window;
extern HWND WCH_handle_tray;
extern HMENU WCH_handle_menu;
extern NOTIFYICONDATAW WCH_NID;
extern ATL::CComPtr<ITaskbarList3> WCH_TBL;
extern Json::Value WCH_Settings;
extern Json::Value WCH_Language;
extern int32_t WCH_num_clock;
extern int32_t WCH_num_task;
extern int32_t WCH_num_work;
extern int32_t WCH_progress_bar_duration;
extern bool WCH_cmd_line;
extern bool WCH_is_focus;
extern bool WCH_is_countdown;
extern bool WCH_program_end;
extern bool WCH_pre_start;
extern ifstream fin;
extern wifstream wfin;
extern ofstream fout;
extern wofstream wfout;
extern Json::Reader JSON_Reader;
extern Json::StreamWriterBuilder JSON_SWB;
extern unique_ptr<Json::StreamWriter> JSON_SW;
extern shared_ptr<spdlog::sinks::basic_file_sink_mt> LOG_sink;
extern shared_ptr<spdlog::logger> LOG_logger;

int32_t wmain(int32_t _argc, wchar_t* _argv[]) {
	WCH_Init();
	if (_argc > 1) {
		SPDLOG_WARN(format(L"Number of redundant command line arguments: {}.", _argc - 1));
	}
	WCH_path_exec = _argv[0];
	while (true) {
		if (WCH_cmd_line) {
			WCH_CL_Init();
			if (WCH_support_command.find(WCH_list_command[0]) != WCH_support_command.end()) {
				WCH_support_command.find(WCH_list_command[0])->second();
			} else {
				WCH_InputCommandIncorrect();
			}
			wcout << endl;
		} else {
			WCH_Sleep(1000);
		}
	}
	return 0;
}
