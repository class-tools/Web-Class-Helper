/*
Web Class Helper Commands Module Header File 2.1.1
This source code file is under MIT License.
Copyright (c) 2022 Class Tools Develop Team
Contributors: jsh-jsh ren-yc hjl2011
*/
#ifndef COMMANDS_H
#define COMMANDS_H
#include "file-process.hpp"
#include "init.hpp"
#include "functions.hpp"
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

bool WCH_save_func(bool output);

void WCH_clear() {
	// Clear console information.
	if (WCH_command_list.size() != 1) {
		WCH_InputCommandIncorrect();
		return;
	}
	system("CLS");
	wcout << WCH_window_title << endl;
	wcout << StrToWstr(WCH_Language["Start"].asString()) << endl;
}

void WCH_exit() {
	// Exit.
	if (WCH_command_list.size() != 1) {
		WCH_InputCommandIncorrect();
		return;
	}
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
	WCH_printlog(WCH_LOG_STATUS_INFO, format(L"Exiting \"Web Class Helper {}{}\"", WCH_VER_MAIN, fullver));
	WCH_cmd_line = false;
	WCH_program_end = true;
	WCH_CheckAndDeleteFile(WCH_path_temp + L"\\WCH_SYSTEM_NORMAL.tmp");
	WCH_CheckAndDeleteFile(WCH_path_temp + L"\\WCH_SYSTEM_ERROR.tmp");
	WCH_CheckAndDeleteFile(WCH_path_temp + L"\\WCH_UPD.tmp");
	WCH_CheckAndDeleteFile(WCH_path_temp + L"\\WCH_TRANS.tmp");
	WCH_CheckAndDeleteFile(WCH_path_temp + L"\\WCH_OW.tmp");
	WCH_CheckAndDeleteFile(WCH_path_temp + L"\\WCH_FATE.tmp");
	WCH_CheckAndDeleteFile(WCH_path_temp + L"\\WCH_IDENT.tmp");
	SendMessageW(WCH_tray_handle, WM_DESTROY, NULL, NULL);
	_exit(0);
}

void WCH_hide() {
	// Hide the window.
	if (WCH_command_list.size() != 1) {
		WCH_InputCommandIncorrect();
		return;
	}
	WCH_SetWindowStatus(false);
}

void WCH_wiki() {
	// Visit the wiki page.
	if (WCH_command_list.size() != 1) {
		WCH_InputCommandIncorrect();
		return;
	}
	wcout << StrToWstr(WCH_Language["JumpWiki"].asString()) << endl;
	_wsystem(L"START https://github.com/class-tools/Web-Class-Helper/wiki/");
}

void WCH_update() {
	// Visit the website to update the program.
	if (WCH_command_list.size() != 1) {
		WCH_InputCommandIncorrect();
		return;
	}
	try {
		wcout << StrToWstr(WCH_Language["CheckUpdate"].asString()) << endl;
		WCH_ProgressBarTot = 5;
		thread T(WCH_ProgressBar);
		T.detach();
		wstring FilePath = WCH_path_temp + L"\\WCH_UPD.tmp";
		wstring url = L"https://class-tools.github.io/update/WCH?";
		random_device rd;
		mt19937 mtrand(rd());
		url.append(to_wstring(mtrand()));
		URLDownloadToFileW(0, url.c_str(), FilePath.c_str(), 0, 0);
		wstring res;
		wfin.open(FilePath);
		getline(wfin, res);
		wfin.close();
		WCH_Sleep(5000);
		if (WCH_FileIsBlank(FilePath)) {
			throw runtime_error("");
		}
		if (WCH_GetVersion(WCH_VER_MAIN) < WCH_GetVersion(res)) {
			wcout << StrToWstr(WCH_Language["FindUpdate"].asString()) << endl;
			_wsystem(L"START https://github.com/class-tools/Web-Class-Helper/releases/latest/");
			WCH_printlog(WCH_LOG_STATUS_INFO, L"Updating to version \"" + res + L"\"");
		} else {
			wcout << StrToWstr(WCH_Language["NoUpdate"].asString()) << endl;
			WCH_printlog(WCH_LOG_STATUS_INFO, L"Program version equals or is greater than \"" + res + L"\"");
		}
		DeleteFileW(FilePath.c_str());
	} catch (...) {
		WCH_NetworkError();
		return;
	}
}

void WCH_license() {
	// Print the license.
	if (WCH_command_list.size() != 1) {
		WCH_InputCommandIncorrect();
		return;
	}
	wfin.open(WCH_GetExecDir() + L"\\LICENSE");
	if (!wfin.is_open()) {
		WCH_FileProcessingFailed();
		return;
	}
	wstring _res;
	while (getline(wfin, _res)) {
		wcout << _res << endl;
	}
	wfin.close();
}

void WCH_set_config() {
	// Set a value of settings.
	if (WCH_command_list.size() != 4) {
		WCH_InputCommandIncorrect();
		return;
	}
	pair<bool, wstring> res = WCH_CheckConfigValid(WCH_command_list[2], WCH_command_list[3]);
	if (!res.first) {
		WCH_InputCommandIncorrect();
		return;
	}
	WCH_Settings[WstrToStr(WCH_command_list[2])] = WstrToStr(WCH_command_list[3]);
	WCH_settings_change++;
	WCH_printlog(WCH_LOG_STATUS_INFO, L"The value of settings key \"" + WCH_command_list[1] + L"\" has been changed to \"" + WCH_command_list[3] + L"\" (Type: \"" + res.second + L"\")");
}

void WCH_list_config() {
	// List all configs.
	if (WCH_command_list.size() != 2) {
		WCH_InputCommandIncorrect();
		return;
	}
	size_t MAXK = 0;
	size_t MAXV = 0;
	for (auto it = WCH_settings_support.begin(); it != WCH_settings_support.end(); it++) {
		MAXK = max(MAXK, get<0>(*it).size());
		MAXV = max(MAXV, StrToWstr(WCH_Settings[WstrToStr(get<0>(*it))].asString()).size());
	}
	if (MAXK == 0 && MAXV == 0) {
		return;
	}
	MAXK = max(MAXK, WCH_GetWstrDisplaySize(StrToWstr(WCH_Language["Key"].asString())));
	MAXV = max(MAXV, WCH_GetWstrDisplaySize(StrToWstr(WCH_Language["Value"].asString())));
	wcout << StrToWstr(WCH_Language["Key"].asString());
	WCH_PrintChar(MAXK - WCH_GetWstrDisplaySize(StrToWstr(WCH_Language["Key"].asString())), L' ');
	wcout << L" | " + StrToWstr(WCH_Language["Value"].asString()) << endl;
	WCH_PrintChar(MAXK, L'-');
	wcout << L" | ";
	WCH_PrintChar(MAXV, L'-');
	wcout << endl;
	for (auto it = WCH_settings_support.begin(); it != WCH_settings_support.end(); it++) {
		wcout << get<0>(*it);
		WCH_PrintChar(MAXK - get<0>(*it).size(), L' ');
		wcout << L" | " << StrToWstr(WCH_Settings[WstrToStr(get<0>(*it))].asString()) << endl;
	}
}

void WCH_check_config() {
	// Config series command input.
	if (WCH_command_list.size() == 1) {
		WCH_InputCommandIncorrect();
		return;
	}
	transform(WCH_command_list[1].begin(), WCH_command_list[1].end(), WCH_command_list[1].begin(), ::tolower);
	if (WCH_command_list[1] == L"set") {
		WCH_set_config();
	} else if (WCH_command_list[1] == L"list") {
		WCH_list_config();
	} else {
		WCH_InputCommandIncorrect();
	}
}

void WCH_add_clock() {
	// Add a new clock.
	if (WCH_command_list.size() != 5) {
		WCH_InputCommandIncorrect();
		return;
	}
	try {
		int32_t h = stoi(WCH_command_list[2]);
		int32_t m = stoi(WCH_command_list[3]);
		wstring Tname = WCH_command_list[4];
		if (WCH_command_list.size() < 5 || h > 24 || m >= 60 || h < 1 || m < 0) {
			WCH_InputCommandIncorrect();
			return;
		} else {
			bool flag = false;
			for (auto it = WCH_clock_list.begin(); it != WCH_clock_list.end(); it++) {
				if (get<0>(*it) == h && get<1>(*it) == m && get<2>(*it) == Tname) {
					flag = true;
					break;
				}
			}
			if (!flag) {
				WCH_clock_num++;
				WCH_clock_change++;
				WCH_clock_list.insert(make_tuple(h, m, Tname));
			} else {
				throw runtime_error("");
			}
		}
	} catch (...) {
		WCH_InputCommandIncorrect();
	}
}

void WCH_delete_clock() {
	// Delete a clock.
	if (WCH_command_list.size() != 5) {
		WCH_InputCommandIncorrect();
		return;
	}
	try {
		int32_t h = stoi(WCH_command_list[2]);
		int32_t m = stoi(WCH_command_list[3]);
		wstring Tname = WCH_command_list[4];
		bool flag = false;
		for (auto it = WCH_clock_list.begin(); it != WCH_clock_list.end(); it++) {
			if (get<0>(*it) == h && get<1>(*it) == m && get<2>(*it) == Tname) {
				WCH_clock_list.erase(it);
				flag = true;
				WCH_clock_num--;
				WCH_clock_change++;
				break;
			}
		}
		if (!flag) {
			throw runtime_error("");
		}
	} catch (...) {
		WCH_InputCommandIncorrect();
	}
}

void WCH_clear_clock() {
	// Clear clock list.
	if (WCH_command_list.size() != 2) {
		WCH_InputCommandIncorrect();
		return;
	}
	WCH_clock_list.clear();
	WCH_clock_num = 0;
	WCH_clock_change++;
}

void WCH_list_clock() {
	// List all clocks.
	if (WCH_command_list.size() != 2) {
		WCH_InputCommandIncorrect();
		return;
	}
	size_t MAXH = 0;
	size_t MAXM = 0;
	size_t MAXT = 0;
	for (auto it = WCH_clock_list.begin(); it != WCH_clock_list.end(); it++) {
		MAXH = max(MAXH, WCH_GetNumDigits((size_t)get<0>(*it)));
		MAXM = max(MAXM, WCH_GetNumDigits((size_t)get<1>(*it)));
		MAXT = max(MAXT, WCH_GetWstrDisplaySize(get<2>(*it)));
	}
	if (MAXH == 0 && MAXM == 0 && MAXT == 0) {
		return;
	}
	MAXH = max(MAXH, WCH_GetWstrDisplaySize(StrToWstr(WCH_Language["Hour"].asString())));
	MAXM = max(MAXM, WCH_GetWstrDisplaySize(StrToWstr(WCH_Language["Minute"].asString())));
	MAXT = max(MAXT, WCH_GetWstrDisplaySize(StrToWstr(WCH_Language["Name"].asString())));
	wcout << StrToWstr(WCH_Language["Hour"].asString());
	WCH_PrintChar(MAXH - WCH_GetWstrDisplaySize(StrToWstr(WCH_Language["Hour"].asString())), L' ');
	wcout << L" | " + StrToWstr(WCH_Language["Minute"].asString());
	WCH_PrintChar(MAXM - WCH_GetWstrDisplaySize(StrToWstr(WCH_Language["Minute"].asString())), L' ');
	wcout << L" | " + StrToWstr(WCH_Language["Name"].asString()) << endl;
	WCH_PrintChar(MAXH, L'-');
	wcout << L" | ";
	WCH_PrintChar(MAXM, L'-');
	wcout << L" | ";
	WCH_PrintChar(MAXT, L'-');
	wcout << endl;
	for (auto it = WCH_clock_list.begin(); it != WCH_clock_list.end(); it++) {
		wcout << get<0>(*it);
		WCH_PrintChar(MAXH - WCH_GetNumDigits(get<0>(*it)), L' ');
		wcout << L" | " << get<1>(*it);
		WCH_PrintChar(MAXM - WCH_GetNumDigits(get<1>(*it)), L' ');
		wcout << L" | " << get<2>(*it) << endl;
	}
}

void WCH_check_clock() {
	// Clock series command input.
	if (WCH_command_list.size() == 1) {
		WCH_InputCommandIncorrect();
		return;
	}
	transform(WCH_command_list[1].begin(), WCH_command_list[1].end(), WCH_command_list[1].begin(), ::tolower);
	if (WCH_command_list[1] == L"add") {
		WCH_add_clock();
	} else if (WCH_command_list[1] == L"delete") {
		WCH_delete_clock();
	} else if (WCH_command_list[1] == L"clear") {
		WCH_clear_clock();
	} else if (WCH_command_list[1] == L"list") {
		WCH_list_clock();
	} else {
		WCH_InputCommandIncorrect();
	}
}

void WCH_add_task() {
	// Add a new task.
	if (WCH_command_list.size() != 3) {
		WCH_InputCommandIncorrect();
		return;
	}
	wstring task = WCH_command_list[2];
	if (WCH_task_list.find(task) != WCH_task_list.end()) {
		WCH_InputCommandIncorrect();
	} else {
		WCH_task_list.insert(task);
		WCH_task_num++;
		WCH_task_change++;
	}
}

void WCH_delete_task() {
	// Delete a task.
	if (WCH_command_list.size() != 3) {
		WCH_InputCommandIncorrect();
		return;
	}
	wstring task = WCH_command_list[2];
	if (WCH_task_list.find(task) == WCH_task_list.end()) {
		WCH_InputCommandIncorrect();
	} else {
		WCH_task_list.erase(task);
		WCH_task_num--;
		WCH_task_change++;
	}
}

void WCH_clear_task() {
	// Clear task list.
	if (WCH_command_list.size() != 2) {
		WCH_InputCommandIncorrect();
		return;
	}
	WCH_task_list.clear();
	WCH_task_num = 0;
	WCH_task_change++;
}

void WCH_list_task() {
	// List all tasks.
	if (WCH_command_list.size() != 2) {
		WCH_InputCommandIncorrect();
		return;
	}
	size_t MAX = 0;
	for (auto it = WCH_task_list.begin(); it != WCH_task_list.end(); it++) {
		MAX = max(MAX, WCH_GetWstrDisplaySize(*it));
	}
	if (MAX == 0) {
		return;
	}
	MAX = max(MAX, WCH_GetWstrDisplaySize(StrToWstr(WCH_Language["ProcessName"].asString())));
	wcout << StrToWstr(WCH_Language["ProcessName"].asString()) << endl;
	WCH_PrintChar(MAX, L'-');
	wcout << endl;
	for (auto it = WCH_task_list.begin(); it != WCH_task_list.end(); it++) {
		wcout << *it << endl;
	}
}

void WCH_check_task() {
	// Task series command input.
	if (WCH_command_list.size() == 1) {
		WCH_InputCommandIncorrect();
		return;
	}
	if (WCH_command_list[1] == L"add") {
		WCH_add_task();
	} else if (WCH_command_list[1] == L"delete") {
		WCH_delete_task();
	} else if (WCH_command_list[1] == L"clear") {
		WCH_clear_task();
	} else if (WCH_command_list[1] == L"list") {
		WCH_list_task();
	} else {
		WCH_InputCommandIncorrect();
	}
}

void WCH_add_work() {
	// Add a new work.
	if (WCH_command_list.size() != 3 && WCH_command_list.size() != 4) {
		WCH_InputCommandIncorrect();
		return;
	}
	wstring work = WCH_command_list[2];
	wstring tag = WCH_command_list.size() == 3 ? StrToWstr(WCH_Language["Unclassified"].asString()) : WCH_command_list[3];
	if (WCH_work_list.find(make_pair(work, tag)) != WCH_work_list.end()) {
		WCH_InputCommandIncorrect();
	} else {
		WCH_work_list.insert(make_pair(work, tag));
		WCH_work_num++;
		WCH_work_change++;
	}
}

void WCH_delete_work() {
	// Delete a work.
	if (WCH_command_list.size() != 3 && WCH_command_list.size() != 4) {
		WCH_InputCommandIncorrect();
		return;
	}
	wstring work = WCH_command_list[2];
	wstring tag = WCH_command_list.size() == 3 ? StrToWstr(WCH_Language["Unclassified"].asString()) : WCH_command_list[3];
	if (WCH_work_list.find(make_pair(work, tag)) == WCH_work_list.end()) {
		WCH_InputCommandIncorrect();
	} else {
		WCH_work_list.erase(make_pair(work, tag));
		WCH_work_num--;
		WCH_work_change++;
	}
}

void WCH_clear_work() {
	// Clear work list.
	if (WCH_command_list.size() != 2) {
		WCH_InputCommandIncorrect();
		return;
	}
	WCH_work_list.clear();
	WCH_work_num = 0;
	WCH_work_change++;
}

void WCH_list_work() {
	// List all works.
	if (WCH_command_list.size() != 2) {
		WCH_InputCommandIncorrect();
		return;
	}
	size_t MAXN = 0;
	size_t MAXT = 0;
	for (auto it = WCH_work_list.begin(); it != WCH_work_list.end(); it++) {
		MAXN = max(MAXN, WCH_GetWstrDisplaySize(it->first));
		MAXT = max(MAXT, WCH_GetWstrDisplaySize(it->second));
	}
	if (MAXN == 0 && MAXT == 0) {
		return;
	}
	MAXN = max(MAXN, WCH_GetWstrDisplaySize(StrToWstr(WCH_Language["Name"].asString())));
	MAXT = max(MAXT, WCH_GetWstrDisplaySize(StrToWstr(WCH_Language["Tag"].asString())));
	wcout << StrToWstr(WCH_Language["Name"].asString());
	WCH_PrintChar(MAXN - WCH_GetWstrDisplaySize(StrToWstr(WCH_Language["Name"].asString())), L' ');
	wcout << L" | " + StrToWstr(WCH_Language["Tag"].asString()) << endl;
	WCH_PrintChar(MAXN, L'-');
	wcout << L" | ";
	WCH_PrintChar(MAXT, L'-');
	wcout << endl;
	for (auto it = WCH_work_list.begin(); it != WCH_work_list.end(); it++) {
		wcout << it->first;
		WCH_PrintChar(MAXN - WCH_GetWstrDisplaySize(it->first), L' ');
		wcout << L" | " << it->second << endl;
	}
}

void WCH_check_work() {
	// Work series command input.
	if (WCH_command_list.size() == 1) {
		WCH_InputCommandIncorrect();
		return;
	}
	transform(WCH_command_list[1].begin(), WCH_command_list[1].end(), WCH_command_list[1].begin(), ::tolower);
	if (WCH_command_list[1] == L"add") {
		WCH_add_work();
	} else if (WCH_command_list[1] == L"delete") {
		WCH_delete_work();
	} else if (WCH_command_list[1] == L"clear") {
		WCH_clear_work();
	} else if (WCH_command_list[1] == L"list") {
		WCH_list_work();
	} else {
		WCH_InputCommandIncorrect();
	}
}

void WCH_game() {
	// Guessing game.
	if (WCH_command_list.size() != 1) {
		WCH_InputCommandIncorrect();
		return;
	}
	random_device rd;
	mt19937 mtrand(rd());
	int32_t inp = 0, ans = mtrand() % 10000 + 1;
	bool flag = true;
	wstring z = L"0";
	vector<wstring> zv;
	zv.push_back(L"0");
	while (true) {
		wcout << StrToWstr(WCH_Language["InputNumber"].asString());
		getline(wcin, z);
		zv = WCH_split(z);
		if (zv.size() == 0) {
			flag = false;
			break;
		}
		try {
			inp = stoi(zv[0]);
		} catch (...) {
			wcout << StrToWstr(WCH_Language["NumberOutOfRange"].asString()) << endl;
			continue;
		}
		if (inp <= 0 || inp > 10000) {
			wcout << StrToWstr(WCH_Language["NumberOutOfRange"].asString()) << endl;
			continue;
		}
		if (inp > ans) {
			wcout << StrToWstr(WCH_Language["NumberSmaller"].asString()) << endl;
		} else if (inp < ans) {
			wcout << StrToWstr(WCH_Language["NumberBigger"].asString()) << endl;
		} else {
			break;
		}
	}
	wcout << StrToWstr(WCH_Language["NumberAnswer"].asString()) + L" " << ans;
	if (flag) {
		wcout << StrToWstr(WCH_Language["NumberWin"].asString()) << endl;
	} else {
		wcout << StrToWstr(WCH_Language["NumberLose"].asString()) << endl;
	}
}

void WCH_pi() {
	// A sequence of function to make a screenshot.
	if (WCH_command_list.size() != 1) {
		WCH_InputCommandIncorrect();
		return;
	}
	WCH_SetWindowStatus(false);
	WCH_Sleep(500);
	WCH_PutPicture();
	WCH_SaveImg();
	WCH_Sleep(500);
	WCH_SetWindowStatus(true);
	wcout << StrToWstr(WCH_Language["Pi"].asString()) << endl;
}

void WCH_count_down_func() {
	// Start a count-down timer.
	if (WCH_command_list.size() != 4) {
		WCH_InputCommandIncorrect();
		return;
	}
	try {
		int32_t h = stoi(WCH_command_list[1]);
		int32_t m = stoi(WCH_command_list[2]);
		int32_t s = stoi(WCH_command_list[3]);
		if ((h == 0 && m == 0 && s == 0) || (h < 0 || m < 0 || s < 0) || (h >= 24 || m >= 60 || s >= 60)) {
			throw runtime_error("");
		}
		WCH_count_down = true;
		wcout << StrToWstr(WCH_Language["CountDown"].asString()) << endl;
		WCH_ProgressBarTot = h * 3600 + m * 60 + s;
		WCH_ProgressBar();
		if (StrToWstr(WCH_Settings["CountDownSoundPrompt"].asString()) == L"True") {
			wcout << L"\a";
		}
		WCH_count_down = false;
	} catch (...) {
		WCH_InputCommandIncorrect();
		WCH_count_down = false;
	}
}

void WCH_check_task_loop() {
	// Check if the running task is in the task list. (Another thread)
	while (WCH_anti_idle && !WCH_program_end) {
		for (auto it = WCH_task_list.begin(); it != WCH_task_list.end(); it++) {
			if (WCH_TaskKill(*it)) {
				WCH_printlog(WCH_LOG_STATUS_INFO, L"Successfully killed \"" + *it + L"\"");
			} else {
				WCH_printlog(WCH_LOG_STATUS_INFO, L"Failed to kill \"" + *it + L"\"");
			}
		}
		WCH_Sleep(3000);
	}
}

void WCH_anti_idle_func() {
	// Enable anti-idle function.
	if (WCH_command_list.size() != 1) {
		WCH_InputCommandIncorrect();
		return;
	}
	if (MessageBoxW(NULL, StrToWstr(WCH_Language["AntiIdle"].asString()).c_str(), L"WCH WARN", MB_ICONWARNING | MB_YESNO | MB_TOPMOST) == IDYES) {
		WCH_SetWindowStatus(false);
		WCH_anti_idle = true;
		WCH_Sleep(500);
		WCH_SetTrayStatus(false);
		WCH_Sleep(500);
		thread T(WCH_check_task_loop);
		T.detach();
	}
}

void WCH_trans() {
	// Translate string between Chinese / English.
	if (WCH_command_list.size() != 2) {
		WCH_InputCommandIncorrect();
		return;
	}
	try {
		wstring FilePath = WCH_path_temp + L"\\WCH_TRANS.tmp";
		URLDownloadToFileW(NULL, (L"http://fanyi.youdao.com/openapi.do?keyfrom=xujiangtao&key=1490852988&type=data&doctype=json&version=1.1&only=translate&q=" + StrToWstr(UrlEncode(WstrToStr((WCH_command_list[1]))))).c_str(), FilePath.c_str(), 0, NULL);
		if (WCH_FileIsBlank(FilePath)) {
			throw runtime_error("");
		}
		Json::Value val;
		fin.open(FilePath);
		if (!JSON_Reader.parse(fin, val)) {
			throw runtime_error("");
		}
		if (!val.isMember("translation")) {
			throw runtime_error("");
		}
		wcout << StrToWstr(val["translation"][0].asString()) << endl;
		fin.close();
		DeleteFileW(FilePath.c_str());
	} catch (...) {
		WCH_NetworkError();
		return;
	}
}

void WCH_ow() {
	// Get a random sentence.
	if (WCH_command_list.size() != 1) {
		WCH_InputCommandIncorrect();
		return;
	}
	try {
		wstring FilePath = WCH_path_temp + L"\\WCH_OW.tmp";
		URLDownloadToFileW(0, L"https://v1.hitokoto.cn/?encode=text", FilePath.c_str(), 0, 0);
		if (WCH_FileIsBlank(FilePath)) {
			throw runtime_error("");
		}
		fin.open(FilePath);
		string res;
		getline(fin, res);
		wcout << StrToWstr(res) << endl;
		fin.close();
		DeleteFileW(FilePath.c_str());
	} catch (...) {
		WCH_NetworkError();
		return;
	}
}

void WCH_fate() {
	// Get fate.
	if (WCH_command_list.size() != 1) {
		WCH_InputCommandIncorrect();
		return;
	}
	try {
		wstring FilePath = WCH_path_temp + L"\\WCH_FATE.tmp";
		URLDownloadToFileW(NULL, (L"https://api.fanlisky.cn/api/qr-fortune/get/" + StrToWstr(UrlEncode(WstrToStr((WCH_GetUniIdent()))))).c_str(), FilePath.c_str(), 0, NULL);
		if (WCH_FileIsBlank(FilePath)) {
			throw runtime_error("");
		}
		Json::Value val;
		fin.open(FilePath);
		if (!JSON_Reader.parse(fin, val)) {
			throw runtime_error("");
		}
		if (!val.isMember("data")) {
			throw runtime_error("");
		}
		if (val["status"].asString() == "20011") {
			wcout << StrToWstr(val["data"]["fortuneSummary"].asString()) << endl;
			wcout << StrToWstr(val["data"]["luckyStar"].asString()) << endl;
			wcout << StrToWstr(val["data"]["signText"].asString()) << endl;
			wcout << StrToWstr(val["data"]["unSignText"].asString()) << endl;
		}
		fin.close();
		DeleteFileW(FilePath.c_str());
	} catch (...) {
		WCH_NetworkError();
		return;
	}
}

void WCH_time() {
	// Print current time.
	if (WCH_command_list.size() != 1) {
		WCH_InputCommandIncorrect();
		return;
	}
	WCH_Time q = WCH_GetTime();
	wcout << format(L"{:04}/{:02}/{:02} {:02}:{:02}:{:02}", q.Year, q.Month, q.Day, q.Hour, q.Minute, q.Second) << endl;
}

void WCH_help() {
	// Print help information.
	if (WCH_command_list.size() > 2) {
		WCH_InputCommandIncorrect();
		return;
	}
	wstring FilePath = WCH_GetExecDir() + L"\\resources\\" + StrToWstr(WCH_Settings["Language"].asString()) + L"\\help.json";
	Json::Value val;
	WCH_printlog(WCH_LOG_STATUS_INFO, L"Reading file \"" + FilePath + L"\"");
	fin.open(FilePath);
	if (!JSON_Reader.parse(fin, val)) {
		WCH_printlog(WCH_LOG_STATUS_ERROR, L"Data in file \"" + FilePath + L"\" corrupted");
		WCH_FileProcessingFailed();
		return;
	}
	fin.close();
	if (WCH_command_list.size() == 1) {
		if (val.isMember("index")) {
			for (auto it = val["index"].begin(); it != val["index"].end(); it++) {
				wcout << StrToWstr((*it).asString()) << endl;
			}
		} else {
			WCH_printlog(WCH_LOG_STATUS_ERROR, L"Data in file \"" + FilePath + L"\" corrupted");
			WCH_FileProcessingFailed();
		}
	} else {
		transform(WCH_command_list[1].begin(), WCH_command_list[1].end(), WCH_command_list[1].begin(), ::tolower);
		if (val.isMember(WstrToStr(WCH_command_list[1]))) {
			for (auto it = val[WstrToStr(WCH_command_list[1])].begin(); it != val[WstrToStr(WCH_command_list[1])].end(); it++) {
				wcout << StrToWstr((*it).asString()) << endl;
			}
			if (WCH_command_list[1] == L"config") {
				FilePath = WCH_GetExecDir() + L"\\resources\\" + StrToWstr(WCH_Settings["Language"].asString()) + L"\\config.json";
				Json::Value valcfg;
				WCH_printlog(WCH_LOG_STATUS_INFO, L"Reading file \"" + FilePath + L"\"");
				fin.open(FilePath);
				if (!JSON_Reader.parse(fin, valcfg)) {
					WCH_printlog(WCH_LOG_STATUS_ERROR, L"Data in file \"" + FilePath + L"\" corrupted");
					WCH_FileProcessingFailed();
					return;
				} else {
					if (valcfg["Title"].size() != 5 || valcfg["Content"].size() != WCH_settings_support.size()) {
						WCH_printlog(WCH_LOG_STATUS_ERROR, L"Data in file \"" + FilePath + L"\" corrupted");
						WCH_FileProcessingFailed();
						return;
					}
				}
				fin.close();
				size_t MAXKN = 0;
				size_t MAXVT = 0;
				size_t MAXDE = 0;
				size_t MAXDV = 0;
				size_t MAXRR = 0;
				int32_t cnt = 0;
				for (auto it = WCH_settings_support.begin(); it != WCH_settings_support.end(); it++, cnt++) {
					MAXKN = max(MAXKN, WCH_GetWstrDisplaySize(get<0>(*it)));
					MAXVT = max(MAXVT, WCH_GetWstrDisplaySize(get<1>(*it)));
					MAXDE = max(MAXDE, WCH_GetWstrDisplaySize(StrToWstr(valcfg["Content"][cnt][0].asString())));
					MAXDV = max(MAXDV, WCH_GetWstrDisplaySize(get<2>(*it)));
					MAXRR = max(MAXRR, WCH_GetWstrDisplaySize(StrToWstr(valcfg["Content"][cnt][1].asString())));
				}
				if (MAXKN == 0 && MAXVT == 0 && MAXDE == 0 && MAXDV == 0 && MAXRR == 0) {
					return;
				}
				MAXKN = max(MAXKN, WCH_GetWstrDisplaySize(StrToWstr(valcfg["Title"][0].asString())));
				MAXVT = max(MAXVT, WCH_GetWstrDisplaySize(StrToWstr(valcfg["Title"][1].asString())));
				MAXDE = max(MAXDE, WCH_GetWstrDisplaySize(StrToWstr(valcfg["Title"][2].asString())));
				MAXDV = max(MAXDV, WCH_GetWstrDisplaySize(StrToWstr(valcfg["Title"][3].asString())));
				MAXRR = max(MAXRR, WCH_GetWstrDisplaySize(StrToWstr(valcfg["Title"][4].asString())));
				wcout << StrToWstr(valcfg["Title"][0].asString());
				WCH_PrintChar(MAXKN - WCH_GetWstrDisplaySize(StrToWstr(valcfg["Title"][0].asString())), L' ');
				wcout << L" | " + StrToWstr(valcfg["Title"][1].asString());
				WCH_PrintChar(MAXVT - WCH_GetWstrDisplaySize(StrToWstr(valcfg["Title"][1].asString())), L' ');
				wcout << L" | " + StrToWstr(valcfg["Title"][2].asString());
				WCH_PrintChar(MAXDE - WCH_GetWstrDisplaySize(StrToWstr(valcfg["Title"][2].asString())), L' ');
				wcout << L" | " + StrToWstr(valcfg["Title"][3].asString());
				WCH_PrintChar(MAXDV - WCH_GetWstrDisplaySize(StrToWstr(valcfg["Title"][3].asString())), L' ');
				wcout << L" | " + StrToWstr(valcfg["Title"][4].asString()) << endl;
				WCH_PrintChar(MAXKN, L'-');
				wcout << L" | ";
				WCH_PrintChar(MAXVT, L'-');
				wcout << L" | ";
				WCH_PrintChar(MAXDE, L'-');
				wcout << L" | ";
				WCH_PrintChar(MAXDV, L'-');
				wcout << L" | ";
				WCH_PrintChar(MAXRR, L'-');
				wcout << endl;
				cnt = 0;
				for (auto it = WCH_settings_support.begin(); it != WCH_settings_support.end(); it++, cnt++) {
					wcout << get<0>(*it);
					WCH_PrintChar(MAXKN - WCH_GetWstrDisplaySize(get<0>(*it)), L' ');
					wcout << L" | " << get<1>(*it);
					WCH_PrintChar(MAXVT - WCH_GetWstrDisplaySize(get<1>(*it)), L' ');
					wcout << L" | " << StrToWstr(valcfg["Content"][cnt][0].asString());
					WCH_PrintChar(MAXDE - WCH_GetWstrDisplaySize(StrToWstr(valcfg["Content"][cnt][0].asString())), L' ');
					wcout << L" | " << get<2>(*it);
					WCH_PrintChar(MAXDV - WCH_GetWstrDisplaySize(get<2>(*it)), L' ');
					wcout << L" | " << StrToWstr(valcfg["Content"][cnt][1].asString()) << endl;
				}
			}
		} else {
			if (WCH_command_support.find(WCH_command_list[1]) != WCH_command_support.end()) {
				WCH_printlog(WCH_LOG_STATUS_ERROR, L"Data in file \"" + FilePath + L"\" corrupted");
				WCH_FileProcessingFailed();
			} else {
				WCH_InputCommandIncorrect();
			}
		}
	}
}

void WCH_save_cmd() {
	// Save data. (Command)
	if (WCH_command_list.size() != 1) {
		WCH_InputCommandIncorrect();
		return;
	}
	if (WCH_save_func(true)) {
		wcout << StrToWstr(WCH_Language["DataSaved"].asString()) << endl;
	} else {
		wcout << StrToWstr(WCH_Language["DataNone"].asString()) << endl;
	}
}

#endif