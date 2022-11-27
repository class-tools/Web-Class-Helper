/*
Web Class Helper Initialization Module Header File 2.1.1
This source code file is under MIT License.
Copyright (c) 2022 Class Tools Develop Team
Contributors: jsh-jsh ren-yc hjl2011
*/
#ifndef INIT_H
#define INIT_H
#include "functions.hpp"
#include "file-process.hpp"
#include "commands.hpp"
#include "apis.hpp"
#include "basic.hpp"

extern const vector<wstring> WCH_list_weekday;
extern const map<wstring, set<wstring>> WCH_choice_settings;
extern const map<wstring, wstring> WCH_MIME_list;
extern const map<wstring, function<void()>> WCH_support_command;
extern const set<tuple<wstring, wstring, wstring, bool>> WCH_support_settings;
extern const set<wstring> WCH_support_language;
extern const wstring WCH_progress_bar_str;
extern const wstring WCH_path_data;
extern const wstring WCH_path_temp;
extern vector<wstring> WCH_list_command;
extern set<tuple<int32_t, int32_t, wstring>> WCH_list_clock;
extern set<wstring> WCH_list_task;
extern set<pair<wstring, wstring>> WCH_list_work;
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

void WCH_read();
void WCH_read_settings();
void WCH_save_settings();

void WCH_Init_Dir() {
	// Initialization for directory.
	if (_waccess(WCH_path_data.c_str(), 0) != 0) {
		CreateDirectoryW(WCH_path_data.c_str(), NULL);
	}
	if (_waccess((WCH_path_data + L"\\data").c_str(), 0) != 0) {
		CreateDirectoryW((WCH_path_data + L"\\data").c_str(), NULL);
	}
	if (_waccess((WCH_path_data + L"\\logs").c_str(), 0) != 0) {
		CreateDirectoryW((WCH_path_data + L"\\logs").c_str(), NULL);
	}
	if (_waccess(WCH_path_temp.c_str(), 0) != 0) {
		CreateDirectoryW(WCH_path_temp.c_str(), NULL);
	}
}

void WCH_Init_Bind() {
	// Initialization for bind.
	atexit(WCH_exit);
	WCH_signalHandler();
	ignore = CoInitializeEx(nullptr, COINIT_APARTMENTTHREADED | COINIT_DISABLE_OLE1DDE);
	ignore = WCH_TBL.CoCreateInstance(CLSID_TaskbarList);
	JSON_SWB.settings_ = []() {
		Json::Value def;
		Json::StreamWriterBuilder::setDefaults(&def);
		def["emitUTF8"] = true;
		return def;
	}();
	WCH_handle_window = GetConsoleWindow();
	ignore = _setmode(_fileno(stdin), _O_WTEXT);
	ignore = _setmode(_fileno(stdout), _O_WTEXT);
	wfin.imbue(locale(".UTF-8", LC_CTYPE));
	wfout.imbue(locale(".UTF-8", LC_CTYPE));
	_wsystem(L"CHCP 65001 > NUL");
}

void WCH_Init_Log() {
	// Initialization for log.
	WCH_Time now = WCH_GetTime();
	WCH_read_settings();
	if (_waccess((WCH_path_data + L"\\logs\\latest.log").c_str(), 0) != -1) {
		ignore = _wrename((WCH_path_data + L"\\logs\\latest.log").c_str(), (WCH_path_data + L"\\logs\\" + StrToWstr(WCH_Settings["StartTime"].asString()) + L".log").c_str());
	}
	WCH_Settings["StartTime"] = WstrToStr(format(L"{:04}{:02}{:02}{:02}{:02}{:02}", now.Year, now.Month, now.Day, now.Hour, now.Minute, now.Second));
	WCH_save_settings();
	WCH_pre_start = false;
	wstring fullver = L" ";
#if WCH_VER_TYPE != 0
	#if WCH_VER_TYPE == 1
	fullver += L"Internal Preview";
	#elif WCH_VER_TYPE == 2
	fullver += L"Public Preview";
	#elif WCH_VER_TYPE == 3
	fullver += L"Release Candidate";
	#endif
	fullver += L" Build " + to_wstring(WCH_VER_BUILD);
#else
	fullver = L"";
#endif
	WCH_printlog(WCH_LOG_STATUS_INFO, format(L"Starting \"Web Class Helper {}{}\"", WCH_VER_MAIN, fullver));
}

void WCH_Init_Var() {
	// Initialization for variable.
	WCH_title_window = StrToWstr(WCH_Language["ProgramName"].asString()) + L" ";
	WCH_title_window.append(WCH_VER_MAIN);
#if WCH_VER_TYPE != 0
	#if WCH_VER_TYPE == 1
	WCH_title_window.append(L" Alpha");
	#elif WCH_VER_TYPE == 2
	WCH_title_window.append(L" Beta");
	#elif WCH_VER_TYPE == 3
	WCH_title_window.append(L" Rc");
	#endif
	WCH_title_window.append(L" " + to_wstring(WCH_VER_BUILD));
	WCH_SetWindowStatus(false);
	if (MessageBoxW(NULL, (StrToWstr(WCH_Language["PreviewWarning"].asString()) + WCH_GetCompileTime()).c_str(), L"WCH WARN", MB_ICONWARNING | MB_YESNO | MB_TOPMOST) == IDNO) {
		WCH_CheckAndDeleteFile(WCH_path_data + L"\\logs\\latest.log");
		_exit(0);
	}
	WCH_SetWindowStatus(true);
#endif
	WCH_title_window.append(L" (");
	WCH_title_window.append(WCH_Framework);
	WCH_title_window.append(L")");
	wchar_t Path[512];
	GetModuleFileName(NULL, Path, sizeof(Path) - 1);
	string tmp(Path, Path + wcslen(Path));
	WCH_program_path = tmp;
}

void WCH_Init_Win() {
	// Initialization for window.
	SetConsoleTitleW(WCH_title_window.c_str());
	WCH_TBL->SetProgressState(WCH_handle_window, TBPF_NOPROGRESS);
	SetWindowLongPtrW(WCH_handle_window, GWL_STYLE, GetWindowLongPtrW(WCH_handle_window, GWL_STYLE) & ~WS_SIZEBOX & ~WS_MAXIMIZEBOX & ~WS_MINIMIZEBOX);
}

void WCH_Init_Loop() {
	// Initialization for multiple thread loop.
	thread T1(WCH_check_clock_loop);
	T1.detach();
	thread T2(WCH_message_loop);
	T2.detach();
	wcout << StrToWstr(WCH_Language["DataReading"].asString()) << endl;
	WCH_progress_bar_duration = 3;
	thread T3(WCH_ProgressBar);
	T3.detach();
	WCH_Sleep(4000);
	_wsystem(L"CLS");
}

void WCH_Init() {
	// Initialize the whole program.
	_wsystem(L"CLS");
	WCH_Init_Dir();
	WCH_Init_Bind();
	WCH_Init_Log();
	WCH_read();
	WCH_Init_Var();
	WCH_Init_Win();
	WCH_Init_Loop();
	wcout << WCH_title_window << endl;
	wcout << StrToWstr(WCH_Language["Start"].asString()) << endl;
	wcout << endl;
}

#endif