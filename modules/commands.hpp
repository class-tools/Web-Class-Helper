/*
Web Class Helper Commands Module Header File 2.1.0
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

void WCH_clear() {
	// Clear console information.
	if (WCH_command_list.size() != 1) {
		WCH_PrintIncorrect();
		return;
	}
	system("cls");
	wcout << WCH_window_title << endl;
	wcout << L"Copyright (c) 2022 Class Tools Develop Team." << endl;
	wcout << L"Type \"help\", \"update\" or \"license\" for more information." << endl;
}

void WCH_quit() {
	// Quit.
	if (WCH_command_list.size() != 1) {
		WCH_PrintIncorrect();
		return;
	}
	WCH_printlog(WCH_LOG_STATUS_INFO, L"Exiting \"" + WCH_window_title + L"\"");
	WCH_cmd_line = false;
	WCH_program_end = true;
	WCH_CheckAndDeleteFile(L"WCH_SYSTEM_NORMAL.tmp");
	WCH_CheckAndDeleteFile(L"WCH_SYSTEM_ERROR.tmp");
	WCH_CheckAndDeleteFile(L"WCH_UPD.tmp");
	WCH_CheckAndDeleteFile(L"WCH_TRANS.tmp");
	WCH_CheckAndDeleteFile(L"WCH_OW.tmp");
	WCH_CheckAndDeleteFile(L"WCH_FATE.tmp");
	WCH_CheckAndDeleteFile(L"WCH_IDENT.tmp");
	SendMessageW(WCH_Tray_hWnd, WM_DESTROY, NULL, NULL);
	_exit(0);
}

void WCH_hide() {
	// Hide the window.
	if (WCH_command_list.size() != 1) {
		WCH_PrintIncorrect();
		return;
	}
	WCH_SetWindowStatus(false);
}

void WCH_wiki() {
	// Visit the wiki page.
	if (WCH_command_list.size() != 1) {
		WCH_PrintIncorrect();
		return;
	}
	wcout << L"Jumping to wiki page..." << endl;
	_wsystem(L"START resources/website/wiki.url");
}

void WCH_update() {
	// Visit the website to update the program.
	if (WCH_command_list.size() != 1) {
		WCH_PrintIncorrect();
		return;
	}
	try {
		wcout << L"Checking update..." << endl;
		WCH_ProgressBarTot = 5;
		thread T(WCH_ProgressBar);
		T.detach();
		wstring url = L"https://class-tools.github.io/update/WCH?";
		srand((unsigned)time(NULL));
		url.append(to_wstring(rand()));
		URLDownloadToFileW(0, url.c_str(), L"WCH_UPD.tmp", 0, 0);
		wstring res;
		wfin.open(L"WCH_UPD.tmp");
		getline(wfin, res);
		wfin.close();
		WCH_Sleep(5000);
		if (WCH_FileIsBlank(L"WCH_UPD.tmp")) {
			throw runtime_error("");
		}
		if (WCH_CheckVersion(WCH_GetVersion(WCH_VER_MAIN), WCH_GetVersion(res))) {
			wcout << L"Program version is less than latest released version, jumping to releases page..." << endl;
			_wsystem(L"START resources/website/releases.url");
			WCH_printlog(WCH_LOG_STATUS_INFO, L"Updating to version \"" + res + L"\"");
		} else {
			wcout << L"Program version equals or is greater than latest released version." << endl;
			WCH_printlog(WCH_LOG_STATUS_INFO, L"Program version equals or is greater than \"" + res + L"\"");
		}
		DeleteFileW(L"WCH_UPD.tmp");
	} catch (...) {
		WCH_PrintNetworkErr();
		return;
	}
}

void WCH_license() {
	// Print the license.
	if (WCH_command_list.size() != 1) {
		WCH_PrintIncorrect();
		return;
	}
	wfin.open(L"LICENSE");
	if (!wfin.is_open()) {
		WCH_printlog(WCH_LOG_STATUS_ERROR, L"File processing failed. Please try reinstalling this program");
		wcout << L"File processing failed. Please try reinstalling this program." << endl;
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
		WCH_PrintIncorrect();
		return;
	}
	wstring KeyType = L"String";
	bool flag = false;
	if (WCH_command_list[3] == L"True" || WCH_command_list[3] == L"False") {
		KeyType = L"Boolean";
	}
	if (WCH_command_list[3].size() < 10) {
		for (size_t i = 0; i < WCH_command_list[3].size(); i++) {
			if (!iswdigit(WCH_command_list[3][i])) {
				break;
			}
			if (WCH_command_list[3].size() == i + 1) {
				KeyType = L"Number";
			}
		}
	}
	for (auto it = WCH_settings_support.begin(); it != WCH_settings_support.end(); it++) {
		if (WCH_command_list[2] == get<0>(*it)) {
			if (KeyType != get<1>(*it)) {
				WCH_PrintIncorrect();
				return;
			} else {
				flag = true;
				break;
			}
		}
	}
	if (!flag) {
		WCH_PrintIncorrect();
		return;
	}
	if (WCH_command_list[2] == L"ScreenshotSavePath" && WCH_command_list[3][WCH_command_list[3].size() - 1] != L'\\') {
		WCH_PrintIncorrect();
		return;
	}
	WCH_Settings[WstrToStr(WCH_command_list[2])] = WstrToStr(WCH_command_list[3]);
	WCH_settings_change++;
	WCH_printlog(WCH_LOG_STATUS_INFO, L"The value of settings key \"" + WCH_command_list[1] + L"\" has been changed to \"" + WCH_command_list[3] + L"\" (Type: \"" + KeyType + L"\")");
}

void WCH_list_config() {
	// List all configs.
	if (WCH_command_list.size() != 2) {
		WCH_PrintIncorrect();
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
	MAXK = max(MAXK, 3);
	MAXV = max(MAXV, 5);
	wcout << L"Key";
	WCH_PrintChar(MAXK - 3, L' ');
	wcout << L" | Value" << endl;
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
		WCH_PrintIncorrect();
		return;
	}
	transform(WCH_command_list[1].begin(), WCH_command_list[1].end(), WCH_command_list[1].begin(), ::tolower);
	if (WCH_command_list[1] == L"set") {
		WCH_set_config();
	} else if (WCH_command_list[1] == L"list") {
		WCH_list_config();
	} else {
		WCH_PrintIncorrect();
	}
}

void WCH_add_clock() {
	// Add a new clock.
	if (WCH_command_list.size() != 5) {
		WCH_PrintIncorrect();
		return;
	}
	try {
		int h = stoi(WCH_command_list[2]);
		int m = stoi(WCH_command_list[3]);
		wstring Tname = WCH_command_list[4];
		if (WCH_command_list.size() < 5 || h > 24 || m >= 60 || h < 1 || m < 0) {
			WCH_PrintIncorrect();
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
		WCH_PrintIncorrect();
	}
}

void WCH_delete_clock() {
	// Delete a clock.
	if (WCH_command_list.size() != 5) {
		WCH_PrintIncorrect();
		return;
	}
	try {
		int h = stoi(WCH_command_list[2]);
		int m = stoi(WCH_command_list[3]);
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
		WCH_printlog(WCH_LOG_STATUS_ERROR, L"Cannot operate the list, please try to restart this program");
		wcout << L"Cannot operate the list, please try to restart this program." << endl;
	}
}

void WCH_clear_clock() {
	// Clear clock list.
	if (WCH_command_list.size() != 2) {
		WCH_PrintIncorrect();
		return;
	}
	WCH_clock_list.clear();
	WCH_clock_num = 0;
	WCH_clock_change++;
}

void WCH_list_clock() {
	// List all clocks.
	if (WCH_command_list.size() != 2) {
		WCH_PrintIncorrect();
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
	MAXH = max(MAXH, 4);
	MAXM = max(MAXM, 6);
	MAXT = max(MAXT, 4);
	wcout << L"Hour";
	WCH_PrintChar(MAXH - 4, L' ');
	wcout << L" | Minute";
	WCH_PrintChar(MAXM - 6, L' ');
	wcout << L" | Name" << endl;
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
		WCH_PrintIncorrect();
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
		WCH_PrintIncorrect();
	}
}

void WCH_add_task() {
	// Add a new task.
	if (WCH_command_list.size() != 3) {
		WCH_PrintIncorrect();
		return;
	}
	wstring task = WCH_command_list[2];
	if (WCH_task_list.find(task) != WCH_task_list.end()) {
		WCH_printlog(WCH_LOG_STATUS_ERROR, L"Cannot operate the list, please try to restart this program");
		wcout << L"Cannot operate the list, please try to restart this program." << endl;
	} else {
		WCH_task_list.insert(task);
		WCH_task_num++;
		WCH_task_change++;
	}
}

void WCH_delete_task() {
	// Delete a task.
	if (WCH_command_list.size() != 3) {
		WCH_PrintIncorrect();
		return;
	}
	wstring task = WCH_command_list[2];
	if (WCH_task_list.find(task) == WCH_task_list.end()) {
		WCH_printlog(WCH_LOG_STATUS_ERROR, L"Cannot operate the list, please try to restart this program");
		wcout << L"Cannot operate the list, please try to restart this program." << endl;
	} else {
		WCH_task_list.erase(task);
		WCH_task_num--;
		WCH_task_change++;
	}
}

void WCH_clear_task() {
	// Clear task list.
	if (WCH_command_list.size() != 2) {
		WCH_PrintIncorrect();
		return;
	}
	WCH_task_list.clear();
	WCH_task_num = 0;
	WCH_task_change++;
}

void WCH_list_task() {
	// List all tasks.
	if (WCH_command_list.size() != 2) {
		WCH_PrintIncorrect();
		return;
	}
	size_t MAX = 0;
	for (auto it = WCH_task_list.begin(); it != WCH_task_list.end(); it++) {
		MAX = max(MAX, WCH_GetWstrDisplaySize(*it));
	}
	if (MAX == 0) {
		return;
	}
	MAX = max(MAX, 12);
	wcout << L"Process Name" << endl;
	WCH_PrintChar(MAX, L'-');
	wcout << endl;
	for (auto it = WCH_task_list.begin(); it != WCH_task_list.end(); it++) {
		wcout << *it << endl;
	}
}

void WCH_check_task() {
	// Task series command input.
	if (WCH_command_list.size() == 1) {
		WCH_PrintIncorrect();
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
		WCH_PrintIncorrect();
	}
}

void WCH_add_work() {
	// Add a new work.
	if (WCH_command_list.size() != 4) {
		WCH_PrintIncorrect();
		return;
	}
	wstring work = WCH_command_list[2];
	wstring tag = WCH_command_list[3];
	if (WCH_work_list.find(make_pair(work, tag)) != WCH_work_list.end()) {
		WCH_printlog(WCH_LOG_STATUS_ERROR, L"Cannot operate the list, please try to restart this program");
		wcout << L"Cannot operate the list, please try to restart this program." << endl;
	} else {
		WCH_work_list.insert(make_pair(work, tag));
		WCH_work_num++;
		WCH_work_change++;
	}
}

void WCH_delete_work() {
	// Delete a work.
	if (WCH_command_list.size() != 4) {
		WCH_PrintIncorrect();
		return;
	}
	wstring work = WCH_command_list[2];
	wstring tag = WCH_command_list[3];
	if (WCH_work_list.find(make_pair(work, tag)) == WCH_work_list.end()) {
		WCH_printlog(WCH_LOG_STATUS_ERROR, L"Cannot operate the list, please try to restart this program");
		wcout << L"Cannot operate the list, please try to restart this program." << endl;
	} else {
		WCH_work_list.erase(make_pair(work, tag));
		WCH_work_num--;
		WCH_work_change++;
	}
}

void WCH_clear_work() {
	// Clear work list.
	if (WCH_command_list.size() != 2) {
		WCH_PrintIncorrect();
		return;
	}
	WCH_work_list.clear();
	WCH_work_num = 0;
	WCH_work_change++;
}

void WCH_list_work() {
	// List all works.
	if (WCH_command_list.size() != 2) {
		WCH_PrintIncorrect();
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
	MAXN = max(MAXN, 4);
	MAXT = max(MAXT, 3);
	wcout << L"Name";
	WCH_PrintChar(MAXN - 4, L' ');
	wcout << L" | Tag" << endl;
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
		WCH_PrintIncorrect();
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
		WCH_PrintIncorrect();
	}
}

void WCH_game() {
	// Guessing game.
	if (WCH_command_list.size() != 1) {
		WCH_PrintIncorrect();
		return;
	}
	srand((unsigned)time(NULL));
	int inp = 0, ans = rand() % 10000 + 1;
	bool digitflag = true, winflag = true;
	wstring z = L"0";
	vector<wstring> zv;
	zv.push_back(L"0");
	while (true) {
		wcout << L"Please input your number (1 ~ 10000): ";
		getline(wcin, z);
		zv = WCH_split(z);
		if (zv.size() == 0) {
			winflag = false;
			break;
		}
		try {
			inp = stoi(zv[0]);
		} catch (...) {
			wcout << L"Number out of range." << endl;
			continue;
		}
		if (inp <= 0 || inp > 10000) {
			wcout << L"Number out of range." << endl;
			continue;
		}
		if (inp > ans) {
			wcout << L"The answer is smaller." << endl;
		} else if (inp < ans) {
			wcout << L"The answer is bigger." << endl;
		} else {
			break;
		}
	}
	wcout << L"The number is " << ans << L".";
	if (winflag) {
		wcout << L" You WIN!";
	}
	wcout << endl;
}

void WCH_pi() {
	// A sequence of function to make a screenshot.
	if (WCH_command_list.size() != 1) {
		WCH_PrintIncorrect();
		return;
	}
	WCH_SetWindowStatus(false);
	WCH_Sleep(500);
	WCH_PutPicture();
	WCH_SaveImg();
	WCH_Sleep(500);
	WCH_SetWindowStatus(true);
	wcout << L"The picture is in the clipboard and be saved in your Pictures folder." << endl;
}

void WCH_count_down_func() {
	// Start a count-down timer.
	if (WCH_command_list.size() != 4) {
		WCH_PrintIncorrect();
		return;
	}
	try {
		int h = stoi(WCH_command_list[1]);
		int m = stoi(WCH_command_list[2]);
		int s = stoi(WCH_command_list[3]);
		if ((h == 0 && m == 0 && s == 0) || (h < 0 || m < 0 || s < 0) || (h >= 24 || m >= 60 || s >= 60)) {
			throw runtime_error("");
		}
		WCH_count_down = true;
		wcout << L"Starting count down timer..." << endl;
		WCH_ProgressBarTot = h * 3600 + m * 60 + s;
		WCH_ProgressBar();
		if (StrToWstr(WCH_Settings["CountDownSoundPrompt"].asString()) == L"True") {
			wcout << L"\a";
		}
		WCH_count_down = false;
	} catch (...) {
		WCH_PrintIncorrect();
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
		WCH_PrintIncorrect();
		return;
	}
	wstring ch;
	wcout << L"Are you sure to enable anti-idle function? If you want to disable it, press Ctrl + Down. (Y/N): ";
	getline(wcin, ch);
	if (ch == L"Y" || ch == L"y") {
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
		WCH_PrintIncorrect();
		return;
	}
	try {
		URLDownloadToFileW(NULL, (L"http://fanyi.youdao.com/openapi.do?keyfrom=xujiangtao&key=1490852988&type=data&doctype=json&version=1.1&only=translate&q=" + StrToWstr(UrlEncode(WstrToStr((WCH_command_list[1]))))).c_str(), L"WCH_TRANS.tmp", 0, NULL);
		if (WCH_FileIsBlank(L"WCH_TRANS.tmp")) {
			throw runtime_error("");
		}
		Json::Value val;
		fin.open(L"WCH_TRANS.tmp");
		if (!JSON_Reader.parse(fin, val)) {
			throw runtime_error("");
		}
		if (!val.isMember("translation")) {
			throw runtime_error("");
		}
		wcout << StrToWstr(val["translation"][0].asString()) << endl;
		fin.close();
		DeleteFileW(L"WCH_TRANS.tmp");
	} catch (...) {
		WCH_PrintNetworkErr();
		return;
	}
}

void WCH_ow() {
	// Get a random sentence.
	if (WCH_command_list.size() != 1) {
		WCH_PrintIncorrect();
		return;
	}
	try {
		URLDownloadToFileW(0, L"https://v1.hitokoto.cn/?encode=text", L"WCH_OW.tmp", 0, 0);
		if (WCH_FileIsBlank(L"WCH_OW.tmp")) {
			throw runtime_error("");
		}
		fin.open(L"WCH_OW.tmp");
		string res;
		getline(fin, res);
		wcout << StrToWstr(res) << endl;
		fin.close();
		DeleteFileW(L"WCH_OW.tmp");
	} catch (...) {
		WCH_PrintNetworkErr();
		return;
	}
}

void WCH_fate() {
	// Get fate.
	if (WCH_command_list.size() != 1) {
		WCH_PrintIncorrect();
		return;
	}
	try {
		URLDownloadToFileW(NULL, (L"https://api.fanlisky.cn/api/qr-fortune/get/" + StrToWstr(UrlEncode(WstrToStr((WCH_GetUniIdent()))))).c_str(), L"WCH_FATE.tmp", 0, NULL);
		if (WCH_FileIsBlank(L"WCH_FATE.tmp")) {
			throw runtime_error("");
		}
		Json::Value val;
		fin.open(L"WCH_FATE.tmp");
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
		DeleteFileW(L"WCH_FATE.tmp");
	} catch (...) {
		WCH_PrintNetworkErr();
		return;
	}
}

void WCH_time() {
	// Print current time.
	if (WCH_command_list.size() != 1) {
		WCH_PrintIncorrect();
		return;
	}
	WCH_Time q = WCH_GetTime();
	wcout << format(L"{:04}/{:02}/{:02} {:02}:{:02}:{:02}", q.Year, q.Month, q.Day, q.Hour, q.Minute, q.Second) << endl;
}

void WCH_help() {
	// Print help information.
	if (WCH_command_list.size() == 1) {
		if (_waccess(L"resources/help/index.dat", 0) != -1) {
			wstring _res;
			wfin.open(L"resources/help/index.dat");
			while (getline(wfin, _res)) {
				wcout << _res << endl;
			}
			wfin.close();
		} else {
			WCH_printlog(WCH_LOG_STATUS_ERROR, L"File processing failed. Please try reinstalling this program");
			wcout << L"File processing failed. Please try reinstalling this program." << endl;
		}
	} else if (WCH_command_list.size() == 2) {
		transform(WCH_command_list[1].begin(), WCH_command_list[1].end(), WCH_command_list[1].begin(), ::tolower);
		if (_waccess((L"resources/help/" + WCH_command_list[1] + L".dat").c_str(), 0) != -1) {
			wstring _res;
			wfin.open((L"resources/help/" + WCH_command_list[1] + L".dat").c_str());
			while (getline(wfin, _res)) {
				wcout << _res << endl;
			}
			wfin.close();
		} else {
			if (WCH_command_support.find(WCH_command_list[1]) != WCH_command_support.end()) {
				WCH_printlog(WCH_LOG_STATUS_ERROR, L"File processing failed. Please try reinstalling this program");
				wcout << L"File processing failed. Please try reinstalling this program." << endl;
			} else {
				WCH_PrintIncorrect();
			}
			return;
		}
	} else {
		WCH_PrintIncorrect();
	}
}

void WCH_save_cmd() {
	// Save data. (Command)
	if (WCH_command_list.size() != 1) {
		WCH_PrintIncorrect();
		return;
	}
	if (WCH_save_func(true)) {
		wcout << L"Successfully saved all data." << endl;
	} else {
		wcout << L"No data to save." << endl;
	}
}

#endif