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

void WCH_read_clock() {
	// Read clock data.
	WCH_Time q = WCH_GetTime();
	wstring NowWeekDay = WCH_list_weekday[(q.Day + 2 * q.Month + 3 * (q.Month + 1) / 5 + q.Year + q.Year / 4 - q.Year / 100 + q.Year / 400 + 1) % 7];
	wstring FilePath = WCH_path_data + L"\\data\\clock.json";
	Json::Value val;
	fin.open(FilePath);
	if (!fin.is_open()) {
		return;
	}
	if (JSON_Reader.parse(fin, val)) {
		try {
			SPDLOG_INFO(format(L"Reading file \"{}\"", FilePath));
			WCH_num_clock = val[WstrToStr(NowWeekDay)].size();
			for (int32_t i = 0; i < WCH_num_clock; i++) {
				WCH_list_clock.insert(make_tuple(val[WstrToStr(NowWeekDay)][i][0].asInt(), val[WstrToStr(NowWeekDay)][i][1].asInt(), StrToWstr(val[WstrToStr(NowWeekDay)][i][2].asString())));
			}
		} catch (...) {
			goto ERR;
		}
	} else {
	ERR:
		SPDLOG_ERROR(format(L"Data in file \"{}\" corrupted", FilePath));
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
			SPDLOG_INFO(format(L"Reading file \"{}\"", FilePath));
			WCH_num_task = val.size();
			for (int32_t i = 0; i < WCH_num_task; i++) {
				WCH_list_task.insert(StrToWstr(val[i].asString()));
			}
		} catch (...) {
			goto ERR;
		}
	} else {
	ERR:
		SPDLOG_ERROR(format(L"Data in file \"{}\" corrupted", FilePath));
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
			SPDLOG_INFO(format(L"Reading file \"{}\"", FilePath));
			WCH_num_work = val.size();
			for (int32_t i = 0; i < WCH_num_work; i++) {
				WCH_list_work.insert(make_pair(StrToWstr(val[i][0].asString()), StrToWstr(val[i][1].asString())));
			}
		} catch (...) {
			goto ERR;
		}
	} else {
	ERR:
		SPDLOG_ERROR(format(L"Data in file \"{}\" corrupted", FilePath));
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
			if (!WCH_pre_start) {
				SPDLOG_INFO(format(L"Reading file \"{}\"", FilePath));
			}
			if (!WCH_Settings.isMember("StartTime")) {
				throw runtime_error("");
			}
			for (auto it = WCH_support_settings.begin(); it != WCH_support_settings.end(); it++) {
				if (!WCH_Settings.isMember(WstrToStr(get<0>(*it)))) {
					throw runtime_error("");
				} else if (!WCH_CheckConfigValid(get<0>(*it), StrToWstr(WCH_Settings[WstrToStr(get<0>(*it))].asString())).first) {
					throw runtime_error("");
				}
			}
			if (WCH_Settings.size() != WCH_support_settings.size() + 1) {
				throw runtime_error("");
			}
		} catch (...) {
			goto ERR;
		}
	} else {
	ERR:
		WCH_Settings.clear();
		WCH_Settings["StartTime"] = "00000000000000";
		for (auto it = WCH_support_settings.begin(); it != WCH_support_settings.end(); it++) {
			WCH_Settings[WstrToStr(get<0>(*it))] = WstrToStr(get<2>(*it));
		}
		if (!WCH_pre_start) {
			SPDLOG_ERROR(format(L"Data in file \"{}\" corrupted", FilePath));
		}
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
			SPDLOG_INFO(format(L"Reading file \"{}\"", FilePath));
			for (auto it = WCH_support_language.begin(); it != WCH_support_language.end(); it++) {
				if (!WCH_Language.isMember(WstrToStr(*it))) {
					throw runtime_error("");
				}
			}
		} catch (...) {
			goto ERR;
		}
	} else {
	ERR:
		SPDLOG_ERROR(format(L"Data in file \"{}\" corrupted", FilePath));
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
	wstring NowWeekDay = WCH_list_weekday[(q.Day + 2 * q.Month + 3 * (q.Month + 1) / 5 + q.Year + q.Year / 4 - q.Year / 100 + q.Year / 400 + 1) % 7];
	wstring FilePath = WCH_path_data + L"\\data\\clock.json";
	Json::Value val;
	fin.open(FilePath);
	if (fin.is_open()) {
		JSON_Reader.parse(fin, val);
	}
	fin.close();
	val[WstrToStr(NowWeekDay)].clear();
	for (auto it = WCH_list_clock.begin(); it != WCH_list_clock.end(); it++) {
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
		SPDLOG_INFO(format(L"Writing file \"{}\"", FilePath));
		fout.open(FilePath);
		JSON_SW->write(val, &fout);
		fout.close();
	} else {
		if (_waccess(FilePath.c_str(), 0) != -1) {
			SPDLOG_INFO(format(L"Deleting file \"{}\"", FilePath));
			DeleteFileW(FilePath.c_str());
		}
	}
}

void WCH_save_task() {
	// Save task list data.
	wstring FilePath = WCH_path_data + L"\\data\\task.json";
	Json::Value val;
	if (WCH_num_task == 0) {
		if (_waccess(FilePath.c_str(), 0) != -1) {
			SPDLOG_INFO(format(L"Deleting file \"{}\"", FilePath));
			DeleteFileW(FilePath.c_str());
		}
		return;
	}
	SPDLOG_INFO(format(L"Writing file \"{}\"", FilePath));
	for (auto it = WCH_list_task.begin(); it != WCH_list_task.end(); it++) {
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
	if (WCH_num_work == 0) {
		if (_waccess(FilePath.c_str(), 0) != -1) {
			SPDLOG_INFO(format(L"Deleting file \"{}\"", FilePath));
			DeleteFileW(FilePath.c_str());
		}
		return;
	}
	SPDLOG_INFO(format(L"Writing file \"{}\"", FilePath));
	for (auto it = WCH_list_work.begin(); it != WCH_list_work.end(); it++) {
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
	if (!WCH_pre_start) {
		SPDLOG_INFO(format(L"Writing file \"{}\"", FilePath));
	}
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