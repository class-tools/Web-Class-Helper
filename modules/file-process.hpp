/*
Web Class Helper File Processing Module Header File 2.1.1
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

extern const array<wstring, 7> WCH_weekday_list;
extern const array<wstring, 2> WCH_language_list;
extern const map<wstring, function<void()>> WCH_command_support;
extern const set<tuple<wstring, wstring, wstring, bool>> WCH_settings_support;
extern const set<wstring> WCH_language_support;
extern const wstring WCH_progress_bar_str;
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
extern NOTIFYICONDATAW WCH_NID;
extern ATL::CComPtr<ITaskbarList3> WCH_TBL;
extern Json::Value WCH_Settings;
extern Json::Value WCH_Language;
extern int32_t WCH_clock_num;
extern int32_t WCH_task_num;
extern int32_t WCH_work_num;
extern int32_t WCH_progress_bar_duration;
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

void WCH_printlog(wstring _mode, wstring _info) {
	// Print log.
	if (!WCH_pre_start) {
		WCH_Time now = WCH_GetTime();
		wfout.open(WCH_path_data + L"\\logs\\latest.log", ios::app);
		wfout << format(L"[{:02}:{:02}:{:02}] {}: {}.", now.Hour, now.Minute, now.Second, _mode, _info) << endl;
		wfout.close();
	}
}

void WCH_read_clock() {
	// Read clock data.
	WCH_Time q = WCH_GetTime();
	wstring NowWeekDay = WCH_weekday_list[(q.Day + 2 * q.Month + 3 * (q.Month + 1) / 5 + q.Year + q.Year / 4 - q.Year / 100 + q.Year / 400 + 1) % 7];
	wstring FilePath = WCH_path_data + L"\\data\\clock.json";
	Json::Value val;
	fin.open(FilePath);
	if (!fin.is_open()) {
		return;
	}
	if (JSON_Reader.parse(fin, val)) {
		try {
			WCH_printlog(WCH_LOG_STATUS_INFO, L"Reading file \"" + FilePath + L"\"");
			WCH_clock_num = val[WstrToStr(NowWeekDay)].size();
			for (int32_t i = 0; i < WCH_clock_num; i++) {
				WCH_clock_list.insert(make_tuple(val[WstrToStr(NowWeekDay)][i][0].asInt(), val[WstrToStr(NowWeekDay)][i][1].asInt(), StrToWstr(val[WstrToStr(NowWeekDay)][i][2].asString())));
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
	wstring FilePath = WCH_path_data + L"\\data\\task.json";
	Json::Value val;
	fin.open(FilePath);
	if (!fin.is_open()) {
		return;
	}
	if (JSON_Reader.parse(fin, val)) {
		try {
			WCH_printlog(WCH_LOG_STATUS_INFO, L"Reading file \"" + FilePath + L"\"");
			WCH_task_num = val.size();
			for (int32_t i = 0; i < WCH_task_num; i++) {
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
	wstring FilePath = WCH_path_data + L"\\data\\work.json";
	Json::Value val;
	fin.open(FilePath);
	if (!fin.is_open()) {
		return;
	}
	if (JSON_Reader.parse(fin, val)) {
		try {
			WCH_printlog(WCH_LOG_STATUS_INFO, L"Reading file \"" + FilePath + L"\"");
			WCH_work_num = val.size();
			for (int32_t i = 0; i < WCH_work_num; i++) {
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
	wstring FilePath = WCH_path_data + L"\\settings.json";
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
			if (WCH_Settings.isMember("Language")) {
				if (find(WCH_language_list.begin(), WCH_language_list.end(), StrToWstr(WCH_Settings["Language"].asString())) == WCH_language_list.end()) {
					throw runtime_error("");
				}
			}
			for (auto it = WCH_settings_support.begin(); it != WCH_settings_support.end(); it++) {
				if (!WCH_Settings.isMember(WstrToStr(get<0>(*it)))) {
					throw runtime_error("");
				} else if (!WCH_CheckConfigValid(get<0>(*it), StrToWstr(WCH_Settings[WstrToStr(get<0>(*it))].asString())).first) {
					throw runtime_error("");
				}
			}
			if (WCH_Settings.size() != WCH_settings_support.size() + 1) {
				throw runtime_error("");
			}
		} catch (...) {
			goto ERR;
		}
	} else {
	ERR:
		WCH_Settings.clear();
		WCH_Settings["StartTime"] = "00000000000000";
		for (auto it = WCH_settings_support.begin(); it != WCH_settings_support.end(); it++) {
			WCH_Settings[WstrToStr(get<0>(*it))] = WstrToStr(get<2>(*it));
		}
		WCH_printlog(WCH_LOG_STATUS_ERROR, L"Data in file \"" + FilePath + L"\" corrupted");
	}
	fin.close();
}

void WCH_read_language() {
	// Read language data.
	wstring FilePath = WCH_GetExecDir() + L"\\resources\\" + StrToWstr(WCH_Settings["Language"].asString()) + L"\\interactive.json";
	fin.open(FilePath);
	if (!fin.is_open()) {
		goto ERR;
	}
	if (JSON_Reader.parse(fin, WCH_Language)) {
		try {
			WCH_printlog(WCH_LOG_STATUS_INFO, L"Reading file \"" + FilePath + L"\"");
			for (auto it = WCH_language_support.begin(); it != WCH_language_support.end(); it++) {
				if (!WCH_Language.isMember(WstrToStr(*it))) {
					throw runtime_error("");
				}
			}
		} catch (...) {
			goto ERR;
		}
	} else {
	ERR:
		WCH_printlog(WCH_LOG_STATUS_ERROR, L"Data in file \"" + FilePath + L"\" corrupted");
		WCH_FileProcessingFailed();
		raise(SIGBREAK);
	}
	fin.close();
}

void WCH_read() {
	// Read data.
	WCH_cmd_line = false;
	WCH_read_clock();
	WCH_read_task();
	WCH_read_work();
	WCH_read_settings();
	WCH_read_language();
	WCH_cmd_line = true;
}

void WCH_save_clock() {
	// Save clock data.
	WCH_Time q = WCH_GetTime();
	wstring NowWeekDay = WCH_weekday_list[(q.Day + 2 * q.Month + 3 * (q.Month + 1) / 5 + q.Year + q.Year / 4 - q.Year / 100 + q.Year / 400 + 1) % 7];
	wstring FilePath = WCH_path_data + L"\\data\\clock.json";
	Json::Value val;
	fin.open(FilePath);
	if (fin.is_open()) {
		JSON_Reader.parse(fin, val);
	}
	fin.close();
	val[WstrToStr(NowWeekDay)].clear();
	for (auto it = WCH_clock_list.begin(); it != WCH_clock_list.end(); it++) {
		Json::Value sval;
		sval.append(get<0>(*it));
		sval.append(get<1>(*it));
		sval.append(WstrToStr(get<2>(*it)));
		val[WstrToStr(NowWeekDay)].append(sval);
	}
	if (val[WstrToStr(NowWeekDay)].size() == 0) {
		val.removeMember(WstrToStr(NowWeekDay));
	}
	if (val.size() != 0) {
		WCH_printlog(WCH_LOG_STATUS_INFO, L"Writing file \"" + FilePath + L"\"");
		fout.open(FilePath);
		JSON_SW->write(val, &fout);
		fout.close();
	} else {
		if (_waccess(FilePath.c_str(), 0) != -1) {
			WCH_printlog(WCH_LOG_STATUS_INFO, L"Deleting file \"" + FilePath + L"\"");
			DeleteFileW(FilePath.c_str());
		}
	}
}

void WCH_save_task() {
	// Save task list data.
	wstring FilePath = WCH_path_data + L"\\data\\task.json";
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
	wstring FilePath = WCH_path_data + L"\\data\\work.json";
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
	wstring FilePath = WCH_path_data + L"\\settings.json";
	WCH_printlog(WCH_LOG_STATUS_INFO, L"Writing file \"" + FilePath + L"\"");
	fout.open(FilePath);
	JSON_SW->write(WCH_Settings, &fout);
	fout.close();
}

void WCH_save() {
	// Save data.
	WCH_save_clock();
	WCH_save_task();
	WCH_save_work();
	WCH_save_settings();
}

#endif