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

extern const array<wstring, 7> WCH_weekday_list;
extern const array<wstring, 2> WCH_language_list;
extern const map<wstring, function<void()>> WCH_command_support;
extern const set<tuple<wstring, wstring, wstring>> WCH_settings_support;
extern const set<wstring> WCH_language_support;
extern const wstring WCH_progress_bar;
extern const wstring WCH_path_data;
extern const wstring WCH_path_temp;
extern vector<wstring> WCH_command_list;
extern set<tuple<int32_t, int32_t, wstring>> WCH_clock_list;
extern set<wstring> WCH_task_list;
extern set<pair<wstring, wstring>> WCH_work_list;
extern wstring WCH_window_title;
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
}

void WCH_Init_Bind() {
	// Initialization for bind.
	atexit(WCH_exit);
	WCH_signalHandler();
	assert(CoInitializeEx(nullptr, COINIT_APARTMENTTHREADED | COINIT_DISABLE_OLE1DDE) == S_OK);
	assert(WCH_TBL.CoCreateInstance(CLSID_TaskbarList) == S_OK);
	JSON_SWB.settings_ = []() {
		Json::Value def;
		Json::StreamWriterBuilder::setDefaults(&def);
		def["emitUTF8"] = true;
		return def;
	}();
	WCH_window_handle = GetConsoleWindow();
	assert(_setmode(_fileno(stdin), _O_WTEXT) != -1);
	assert(_setmode(_fileno(stdout), _O_WTEXT) != -1);
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
	wstring vertype = L"";
	WCH_window_title = StrToWstr(WCH_Language["ProgramName"].asString()) + L" ";
	WCH_window_title.append(WCH_VER_MAIN);
#if WCH_VER_TYPE != 0
	#if WCH_VER_TYPE == 1
	WCH_window_title.append(L" " + StrToWstr(WCH_Language["InternalPreview"].asString()));
	vertype = StrToWstr(WCH_Language["InternalPreview"].asString());
	#elif WCH_VER_TYPE == 2
	WCH_window_title.append(L" " + StrToWstr(WCH_Language["PublicPreview"].asString()));
	vertype = StrToWstr(WCH_Language["PublicPreview"].asString());
	#elif WCH_VER_TYPE == 3
	WCH_window_title.append(L" " + StrToWstr(WCH_Language["ReleaseCandidate"].asString()));
	vertype = StrToWstr(WCH_Language["ReleaseCandidate"].asString());
	#endif
	WCH_window_title.append(L" " + StrToWstr(WCH_Language["Build"].asString()) + L" " + to_wstring(WCH_VER_BUILD));
	WCH_SetWindowStatus(false);
	if (MessageBoxW(NULL, (StrToWstr(WCH_Language["PreviewWarning"].asString()) + WCH_GetCompileTime()).c_str(), L"WCH WARN", MB_ICONWARNING | MB_YESNO | MB_TOPMOST) == IDNO) {
		WCH_CheckAndDeleteFile(WCH_path_data + L"\\logs\\latest.log");
		_exit(0);
	}
	WCH_SetWindowStatus(true);
#endif
	WCH_window_title.append(L" (");
	WCH_window_title.append(WCH_Framework);
	WCH_window_title.append(L")");
}

void WCH_Init_Win() {
	// Initialization for window.
	if (FindWindowW(NULL, WCH_window_title.c_str()) != NULL) {
		WCH_TBL->SetProgressState(WCH_window_handle, TBPF_INDETERMINATE);
		MessageBoxW(NULL, StrToWstr(WCH_Language["AlreadyRunning"].asString()).c_str(), WCH_window_title.c_str(), MB_ICONERROR | MB_TOPMOST);
		WCH_TBL->SetProgressState(WCH_window_handle, TBPF_NOPROGRESS);
		WCH_printlog(WCH_LOG_STATUS_WARN, L"Application is already running");
		raise(SIGBREAK);
	}
	SetConsoleTitleW(WCH_window_title.c_str());
	WCH_TBL->SetProgressState(WCH_window_handle, TBPF_NOPROGRESS);
}

void WCH_Init_Loop() {
	// Initialization for multiple thread loop.
	thread T1(WCH_check_clock_loop);
	T1.detach();
	thread T2(WCH_safety_input_loop);
	T2.detach();
	thread T3(WCH_message_loop);
	T3.detach();
	if (StrToWstr(WCH_Settings["AutoSave"].asString()) == L"True") {
		thread T4(WCH_AutoSave_loop);
		T4.detach();
	}
	wcout << StrToWstr(WCH_Language["DataReading"].asString()) << endl;
	WCH_ProgressBarTot = 3;
	thread T5(WCH_ProgressBar);
	T5.detach();
	WCH_Sleep(4000);
	_wsystem(L"CLS");
}

void WCH_Init() {
	// Initialize the whole program.
	WCH_Init_Dir();
	WCH_Init_Bind();
	WCH_Init_Log();
	WCH_read();
	WCH_Init_Var();
	WCH_Init_Win();
	WCH_Init_Loop();
	wcout << WCH_window_title << endl;
	wcout << StrToWstr(WCH_Language["Start"].asString()) << endl;
	wcout << endl;
}

#endif