/*
Web Class Helper File Process Module Header File 2.1.0
This source code file is under MIT License.
Copyright (c) 2022 Class Tools Develop Team
Contributors: jsh-jsh ren-yc
*/
#ifndef FILE_PROCESS_H
#define FILE_PROCESS_H
#include "init.hpp"
#include "commands.hpp"
#include "functions.hpp"
#include "apis.hpp"
#include "basic.hpp"

extern const wstring WCH_WDName[7];
extern map<wstring, function<void()>> WCH_command_support;
extern set<tuple<wstring, wstring, wstring>> WCH_settings_support;
extern vector<wstring> WCH_command_list;
extern set<tuple<int, int, wstring>> WCH_clock_list;
extern set<wstring> WCH_task_list;
extern set<pair<wstring, wstring>> WCH_work_list;
extern wstring WCH_window_title;
extern wstring WCH_command;
extern wstring WCH_ProgressBarStr;
extern HWND WCH_Win_hWnd;
extern HWND WCH_Tray_hWnd;
extern HMENU WCH_hMenu;
extern NOTIFYICONDATA WCH_NID;
extern ATL::CComPtr<ITaskbarList3> WCH_TBL;
extern Json::Value WCH_Settings;
extern int WCH_clock_num;
extern int WCH_task_num;
extern int WCH_work_num;
extern int WCH_clock_change;
extern int WCH_task_change;
extern int WCH_work_change;
extern int WCH_settings_change;
extern int WCH_ProgressBarTot;
extern int WCH_InputTimes;
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
void WCH_Sleep(int _ms);
void WCH_printlog(wstring _mode, wstring _info);
void WCH_read_settings();
void WCH_read();
void WCH_save_settings();
bool WCH_save_func(bool output);
size_t WCH_GetNumDigits(size_t _n);

void WCH_printlog(wstring _mode, wstring _info) {
	// Print log.
	if (!WCH_pre_start) {
		WCH_Time now = WCH_GetTime();
		wfout.open(L"logs/latest.log", ios::app);
		wfout << format(L"[{:02}:{:02}:{:02}] {}: {}.", now.Hour, now.Minute, now.Second, _mode, _info) << endl;
		wfout.close();
	}
}

void WCH_read_clock() {
	// Read clock data.
	WCH_Time q = WCH_GetTime();
	wstring NowWeekDay = WCH_WDName[(q.Day + 2 * q.Month + 3 * (q.Month + 1) / 5 + q.Year + q.Year / 4 - q.Year / 100 + q.Year / 400 + 1) % 7];
	wstring FilePath = L"data/" + NowWeekDay + L".json";
	Json::Value val;
	fin.open(FilePath);
	if (!fin.is_open()) {
		return;
	}
	if (JSON_Reader.parse(fin, val)) {
		try {
			WCH_printlog(WCH_LOG_STATUS_INFO, L"Reading file \"" + FilePath + L"\"");
			WCH_clock_num = val.size();
			for (int i = 0; i < WCH_clock_num; i++) {
				WCH_clock_list.insert(make_tuple(val[i][0].asInt(), val[i][1].asInt(), StrToWstr(val[i][2].asString())));
			}
		} catch (...) {
			goto ERR;
		}
	} else {
	ERR:
		WCH_printlog(WCH_LOG_STATUS_ERROR, L"Data in file \"" + FilePath + L"\" corrupted");
	}
	fin.close();
}

void WCH_read_task() {
	// Read task data.
	wstring FilePath = L"data/task.json";
	Json::Value val;
	fin.open(FilePath);
	if (!fin.is_open()) {
		return;
	}
	if (JSON_Reader.parse(fin, val)) {
		try {
			WCH_printlog(WCH_LOG_STATUS_INFO, L"Reading file \"" + FilePath + L"\"");
			WCH_task_num = val.size();
			for (int i = 0; i < WCH_task_num; i++) {
				WCH_task_list.insert(StrToWstr(val[i].asString()));
			}
		} catch (...) {
			goto ERR;
		}
	} else {
	ERR:
		WCH_printlog(WCH_LOG_STATUS_ERROR, L"Data in file \"" + FilePath + L"\" corrupted");
	}
	fin.close();
}

void WCH_read_work() {
	// Read work data.
	wstring FilePath = L"data/work.json";
	Json::Value val;
	fin.open(FilePath);
	if (!fin.is_open()) {
		return;
	}
	if (JSON_Reader.parse(fin, val)) {
		try {
			WCH_printlog(WCH_LOG_STATUS_INFO, L"Reading file \"" + FilePath + L"\"");
			WCH_work_num = val.size();
			for (int i = 0; i < WCH_work_num; i++) {
				WCH_work_list.insert(make_pair(StrToWstr(val[i][0].asString()), StrToWstr(val[i][1].asString())));
			}
		} catch (...) {
			goto ERR;
		}
	} else {
	ERR:
		WCH_printlog(WCH_LOG_STATUS_ERROR, L"Data in file \"" + FilePath + L"\" corrupted");
	}
	fin.close();
}

void WCH_read_settings() {
	// Read settings data.
	wstring FilePath = L"settings.json";
	fin.open(FilePath);
	if (!fin.is_open()) {
		goto ERR;
	}
	if (JSON_Reader.parse(fin, WCH_Settings)) {
		try {
			WCH_printlog(WCH_LOG_STATUS_INFO, L"Reading file \"" + FilePath + L"\"");
			if (!WCH_Settings.isMember("StartTime")) {
				throw runtime_error("");
			}
			for (auto it = WCH_settings_support.begin(); it != WCH_settings_support.end(); it++) {
				if (!WCH_Settings.isMember(WstrToStr(get<0>(*it)))) {
					throw runtime_error("");
				}
			}
		} catch (...) {
			goto ERR;
		}
	} else {
	ERR:
		WCH_Settings["StartTime"] = "00000000000000";
		for (auto it = WCH_settings_support.begin(); it != WCH_settings_support.end(); it++) {
			WCH_Settings[WstrToStr(get<0>(*it))] = WstrToStr(get<2>(*it));
		}
		WCH_printlog(WCH_LOG_STATUS_ERROR, L"Data in file \"" + FilePath + L"\" corrupted");
	}
	fin.close();
}

void WCH_read() {
	// Read data.
	WCH_cmd_line = false;
	wcout << L"Reading data..." << endl;
	WCH_read_clock();
	WCH_read_task();
	WCH_read_work();
	WCH_read_settings();
	WCH_ProgressBarTot = 3;
	thread T(WCH_ProgressBar);
	T.detach();
	WCH_Sleep(4000);
	WCH_cmd_line = true;
	_wsystem(L"CLS");
}

void WCH_save_clock() {
	// Save clock data.
	WCH_Time q = WCH_GetTime();
	wstring NowWeekDay = WCH_WDName[(q.Day + 2 * q.Month + 3 * (q.Month + 1) / 5 + q.Year + q.Year / 4 - q.Year / 100 + q.Year / 400 + 1) % 7];
	wstring FilePath = L"data/" + NowWeekDay + L".json";
	Json::Value val;
	if (WCH_clock_num == 0) {
		if (_waccess(FilePath.c_str(), 0) != -1) {
			WCH_printlog(WCH_LOG_STATUS_INFO, L"Deleting file \"" + FilePath + L"\"");
			DeleteFileW(FilePath.c_str());
		}
		return;
	}
	WCH_printlog(WCH_LOG_STATUS_INFO, L"Writing file \"" + FilePath + L"\"");
	for (auto it = WCH_clock_list.begin(); it != WCH_clock_list.end(); it++) {
		Json::Value sval;
		sval.append(get<0>(*it));
		sval.append(get<1>(*it));
		sval.append(WstrToStr(get<2>(*it)));
		val.append(sval);
	}
	fout.open(FilePath);
	JSON_SW->write(val, &fout);
	fout.close();
}

void WCH_save_task() {
	// Save task list data.
	wstring FilePath = L"data/task.json";
	Json::Value val;
	if (WCH_task_num == 0) {
		if (_waccess(FilePath.c_str(), 0) != -1) {
			WCH_printlog(WCH_LOG_STATUS_INFO, L"Deleting file \"" + FilePath + L"\"");
			DeleteFileW(FilePath.c_str());
		}
		return;
	}
	WCH_printlog(WCH_LOG_STATUS_INFO, L"Writing file \"" + FilePath + L"\"");
	for (auto it = WCH_task_list.begin(); it != WCH_task_list.end(); it++) {
		val.append(WstrToStr(*it));
	}
	fout.open(FilePath);
	JSON_SW->write(val, &fout);
	fout.close();
}

void WCH_save_work() {
	// Save task list data.
	wstring FilePath = L"data/work.json";
	Json::Value val;
	if (WCH_work_num == 0) {
		if (_waccess(FilePath.c_str(), 0) != -1) {
			WCH_printlog(WCH_LOG_STATUS_INFO, L"Deleting file \"" + FilePath + L"\"");
			DeleteFileW(FilePath.c_str());
		}
		return;
	}
	WCH_printlog(WCH_LOG_STATUS_INFO, L"Writing file \"" + FilePath + L"\"");
	for (auto it = WCH_work_list.begin(); it != WCH_work_list.end(); it++) {
		Json::Value sval;
		sval.append(WstrToStr(it->first));
		sval.append(WstrToStr(it->second));
		val.append(sval);
	}
	fout.open(FilePath);
	JSON_SW->write(val, &fout);
	fout.close();
}

void WCH_save_settings() {
	// Save settings data.
	wstring FilePath = L"settings.json";
	WCH_printlog(WCH_LOG_STATUS_INFO, L"Writing file \"" + FilePath + L"\"");
	fout.open(FilePath);
	JSON_SW->write(WCH_Settings, &fout);
	fout.close();
}

bool WCH_save_func(bool output) {
	// Save data. (Function)
	if (WCH_clock_change != 0 || WCH_task_change != 0 || WCH_work_change != 0 || WCH_settings_change != 0) {
		if (output) {
			wcout << L"Saving data..." << endl;
			WCH_ProgressBarTot = WCH_clock_change + WCH_task_change + WCH_work_change + WCH_settings_change;
		}
		WCH_clock_change = 0;
		WCH_task_change = 0;
		WCH_work_change = 0;
		WCH_settings_change = 0;
		WCH_save_clock();
		WCH_save_task();
		WCH_save_work();
		WCH_save_settings();
		if (output) {
			thread T(WCH_ProgressBar);
			T.detach();
			WCH_Sleep((WCH_ProgressBarTot + 1) * 1000);
		}
		return true;
	} else {
		return false;
	}
}

#endif