/*
Web Class Helper Commands Module Header File 2.1.2
This source code file is under MIT License.
Copyright (c) 2022 - 2023 Class Tools Develop Team
Contributors: jsh-jsh ren-yc hjl2011
*/
#ifndef COMMANDS_H
#define COMMANDS_H
#include "file-process.hpp"
#include "init.hpp"
#include "functions.hpp"
#include "apis.hpp"
#include "basic.hpp"

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

void WCH_save();
void WCH_check_task_loop();

void WCH_clear() {
	// Clear console information.
	if (WCH_list_command.size() != 1) {
		WCH_InputCommandIncorrect();
		return;
	}
	system("CLS");
	wcout << WCH_title_window << endl;
	wcout << StrToWstr(WCH_Language["Start"].asString()) << endl;
}

void WCH_exit() {
	// Exit.
	if (WCH_list_command.size() != 1) {
		WCH_InputCommandIncorrect();
		return;
	}
	WCH_save();
	SPDLOG_INFO(format(L"Exiting \"Web Class Helper {}\"", WCH_version));
	WCH_cmd_line = false;
	WCH_program_end = true;
	WCH_CheckAndDeleteFile(WCH_path_temp + L"\\WCH_SYSTEM_NORMAL.tmp");
	WCH_CheckAndDeleteFile(WCH_path_temp + L"\\WCH_SYSTEM_ERROR.tmp");
	WCH_CheckAndDeleteFile(WCH_path_temp + L"\\WCH_UPD.tmp");
	WCH_CheckAndDeleteFile(WCH_path_temp + L"\\WCH_TRANS.tmp");
	WCH_CheckAndDeleteFile(WCH_path_temp + L"\\WCH_OW.tmp");
	WCH_CheckAndDeleteFile(WCH_path_temp + L"\\WCH_FATE.tmp");
	WCH_CheckAndDeleteFile(WCH_path_temp + L"\\WCH_IDENT.tmp");
	WCH_CheckAndDeleteFile(WCH_path_temp + L"\\WCH_HASH.tmp");
	WCH_CheckAndDeleteFile(WCH_path_temp + L"\\WCH_PWSH.tmp");
	SendMessageW(WCH_handle_tray, WM_DESTROY, NULL, NULL);
	_exit(0);
}

void WCH_restart() {
	// Restart the program.
	if (WCH_list_command.size() != 1) {
		WCH_InputCommandIncorrect();
		return;
	}
	_wsystem((L"START \"\" \"" + WCH_path_exec + L"\"").c_str());
	WCH_exit();
}

void WCH_hide() {
	// Hide the window.
	if (WCH_list_command.size() != 1) {
		WCH_InputCommandIncorrect();
		return;
	}
	WCH_SetWindowStatus(false);
}

void WCH_wiki() {
	// Visit the wiki page.
	if (WCH_list_command.size() != 1) {
		WCH_InputCommandIncorrect();
		return;
	}
	wcout << StrToWstr(WCH_Language["JumpWiki"].asString()) << endl;
	_wsystem(L"START https://github.com/class-tools/Web-Class-Helper/wiki/");
}

void WCH_update() {
	// Visit the website to update the program.
	if (WCH_list_command.size() != 1) {
		WCH_InputCommandIncorrect();
		return;
	}
	try {
		wcout << StrToWstr(WCH_Language["CheckUpdate"].asString()) << endl;
		WCH_progress_bar_duration = 5;
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
			SPDLOG_INFO(format(L"Updating to version \"{}\"", res));
		} else {
			wcout << StrToWstr(WCH_Language["NoUpdate"].asString()) << endl;
			SPDLOG_INFO(format(L"Program version equals or is greater than \"{}\"", res));
		}
		DeleteFileW(FilePath.c_str());
	} catch (...) {
		WCH_NetworkError();
		return;
	}
}

void WCH_license() {
	// Print the license.
	if (WCH_list_command.size() != 1) {
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

void WCH_sysinfo() {
	// Get system information.
	if (WCH_list_command.size() != 1) {
		WCH_InputCommandIncorrect();
		return;
	}
	tuple<uint32_t, uint32_t, uint32_t> res = WCH_GetSystemVersion();
	wcout << format(L"{}: {}.{}.{}", StrToWstr(WCH_Language["OSVersion"].asString()), get<0>(res), get<1>(res), get<2>(res)) << endl;
	wcout << format(L"{}: {}", StrToWstr(WCH_Language["OSArchitecture"].asString()), WCH_GetSystemArchitecture()) << endl;
	wcout << format(L"{}: {}", StrToWstr(WCH_Language["ProgramArchitecture"].asString()), WCH_Framework) << endl;
}

void WCH_develop_opendata() {
	// Open data folder in AppData.
	if (WCH_list_command.size() != 2) {
		WCH_InputCommandIncorrect();
		return;
	}
	_wsystem((L"EXPLORER \"" + WCH_path_data + L"\"").c_str());
}

void WCH_develop_opentemp() {
	// Open temp folder in AppData.
	if (WCH_list_command.size() != 2) {
		WCH_InputCommandIncorrect();
		return;
	}
	_wsystem((L"EXPLORER \"" + WCH_path_temp + L"\"").c_str());
}

void WCH_develop() {
	// Develop series command input.
	if (WCH_list_command.size() == 1) {
		WCH_InputCommandIncorrect();
		return;
	}
	transform(WCH_list_command[1].begin(), WCH_list_command[1].end(), WCH_list_command[1].begin(), ::towlower);
	if (WCH_list_command[1] == L"opendata") {
		WCH_develop_opendata();
	} else if (WCH_list_command[1] == L"opentemp") {
		WCH_develop_opentemp();
	} else {
		WCH_InputCommandIncorrect();
	}
}

void WCH_config_set() {
	// Set a value of settings.
	if (WCH_list_command.size() != 4) {
		WCH_InputCommandIncorrect();
		return;
	}
	pair<bool, wstring> res = WCH_CheckConfigValid(WCH_list_command[2], WCH_list_command[3]);
	if (!res.first) {
		WCH_InputCommandIncorrect();
		return;
	}
	WCH_Settings[WstrToStr(WCH_list_command[2])] = WstrToStr(WCH_list_command[3]);
	SPDLOG_INFO(format(L"The value of settings key \"{}\" has been changed to \"{}\" (Type: \"{}\")", WCH_list_command[2], WCH_list_command[3], res.second));
	for (auto it = WCH_support_settings.begin(); it != WCH_support_settings.end(); it++) {
		if (get<0>(*it) == WCH_list_command[2] && get<3>(*it)) {
			MessageBoxW(NULL, StrToWstr(WCH_Language["WillRestart"].asString()).c_str(), L"WCH WARN", MB_ICONWARNING | MB_TOPMOST);
			WCH_list_command.clear();
			WCH_list_command.push_back(L"restart");
			wcout << endl;
			WCH_restart();
		}
	}
}

void WCH_config_list() {
	// List all configs.
	if (WCH_list_command.size() != 2) {
		WCH_InputCommandIncorrect();
		return;
	}
	size_t MAXK = 0;
	size_t MAXV = 0;
	for (auto it = WCH_support_settings.begin(); it != WCH_support_settings.end(); it++) {
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
	for (auto it = WCH_support_settings.begin(); it != WCH_support_settings.end(); it++) {
		wcout << get<0>(*it);
		WCH_PrintChar(MAXK - get<0>(*it).size(), L' ');
		wcout << L" | " << StrToWstr(WCH_Settings[WstrToStr(get<0>(*it))].asString()) << endl;
	}
}

void WCH_config_wizard() {
	// Wizard to config settings.
	if (WCH_list_command.size() != 2) {
		WCH_InputCommandIncorrect();
		return;
	}
	wstring FilePath = WCH_GetExecDir() + L"\\resources\\" + StrToWstr(WCH_Settings["Language"].asString()) + L"\\config.json";
	Json::Value valcfg;
	SPDLOG_INFO(format(L"Reading file \"{}\"", FilePath));
	fin.open(FilePath);
	if (!fin.is_open() || WCH_GetFileHash(FilePath) != (StrToWstr(WCH_Settings["Language"].asString()) == L"en-US" ? SHASUM_enUS_config : SHASUM_zhCN_config)) {
		SPDLOG_ERROR(format(L"Data in file \"{}\" corrupted", FilePath));
		WCH_FileProcessingFailed();
		raise(SIGBREAK);
	}
	ignore = JSON_Reader.parse(fin, valcfg);
	fin.close();
	int32_t cnt = 0;
	bool flag = false;
	for (auto it = WCH_support_settings.begin(); it != WCH_support_settings.end(); it++, cnt++) {
		wcout << format(L"{}: {}", StrToWstr(valcfg["Title"][0].asString()), get<0>(*it)) << endl;
		wcout << format(L"{}: {}", StrToWstr(valcfg["Title"][1].asString()), get<1>(*it)) << endl;
		wcout << format(L"{}: {}", StrToWstr(valcfg["Title"][2].asString()), StrToWstr(valcfg["Content"][cnt].asString())) << endl;
		wcout << format(L"{}: {}", StrToWstr(valcfg["Title"][4].asString()), StrToWstr(WCH_Language[get<3>(*it) ? "Yes" : "No"].asString())) << endl;
		wcout << format(L"{}: {}", StrToWstr(valcfg["Title"][3].asString()), get<2>(*it)) << endl;
		wcout << format(L"{}: {}", StrToWstr(valcfg["Title"][5].asString()), StrToWstr(WCH_Settings[WstrToStr(get<0>(*it))].asString())) << endl;
		if (WCH_choice_settings.find(get<0>(*it)) != WCH_choice_settings.end()) {
			wcout << StrToWstr(valcfg["Title"][6].asString()) << L":" << endl;
			for (auto ite = WCH_choice_settings.find(get<0>(*it))->second.begin(); ite != WCH_choice_settings.find(get<0>(*it))->second.end(); ite++) {
				wcout << L" - " << *ite << endl;
			}
		}
		wcout << StrToWstr(WCH_Language["ConfigWizardPrompt"].asString()) << endl;
		wstring _in;
		wcout << StrToWstr(WCH_Settings["CommandPrompt"].asString()) << L" ";
		if (cnt == 0) {
			size_t _nlc = WCH_NewlineCount(get<2>(*it)) + (WCH_choice_settings.find(get<0>(*it)) != WCH_choice_settings.end() ? WCH_choice_settings.find(get<0>(*it))->second.size() + 1 : 0);
			CONSOLE_SCREEN_BUFFER_INFO _csbi = {};
			COORD _crd = {};
			GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &_csbi);
			_crd.X = _csbi.dwCursorPosition.X;
			_crd.Y = (int16_t)_nlc + 7;
			WCH_PrintChar((size_t)_csbi.dwSize.Y - _nlc - 8, L'\n');
			SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), _crd);
		}
	BEGIN:
		getline(wcin, _in);
		if (wcin.eof()) {
			raise(SIGINT);
		}
		if (_in.size() != 0) {
			pair<bool, wstring> res = WCH_CheckConfigValid(get<0>(*it), _in);
			if (!res.first) {
				WCH_InputCommandIncorrect();
				wcout << endl;
				wcout << StrToWstr(WCH_Settings["CommandPrompt"].asString()) << L" ";
				goto BEGIN;
			}
			WCH_Settings[WstrToStr(get<0>(*it))] = WstrToStr(_in);
			SPDLOG_INFO(format(L"The value of settings key \"{}\" has been changed to \"{}\" (Type: \"{}\")", get<0>(*it), _in, res.second));
			for (auto ite = WCH_support_settings.begin(); ite != WCH_support_settings.end(); ite++) {
				if (get<0>(*ite) == get<0>(*it) && get<3>(*ite)) {
					flag = true;
				}
			}
		}
		WCH_ClearConsole();
	}
	if (flag) {
		MessageBoxW(NULL, StrToWstr(WCH_Language["WillRestart"].asString()).c_str(), L"WCH WARN", MB_ICONWARNING | MB_TOPMOST);
		WCH_list_command.clear();
		WCH_list_command.push_back(L"restart");
		wcout << endl;
		WCH_restart();
	}
}

void WCH_config() {
	// Config series command input.
	if (WCH_list_command.size() == 1) {
		WCH_InputCommandIncorrect();
		return;
	}
	transform(WCH_list_command[1].begin(), WCH_list_command[1].end(), WCH_list_command[1].begin(), ::towlower);
	if (WCH_list_command[1] == L"set") {
		WCH_config_set();
	} else if (WCH_list_command[1] == L"list") {
		WCH_config_list();
	} else if (WCH_list_command[1] == L"wizard") {
		WCH_config_wizard();
	} else {
		WCH_InputCommandIncorrect();
	}
}

void WCH_clock_add() {
	// Add a new clock.
	if (WCH_list_command.size() != 5) {
		WCH_InputCommandIncorrect();
		return;
	}
	try {
		int32_t h = stoi(WCH_list_command[2]);
		int32_t m = stoi(WCH_list_command[3]);
		wstring Tname = WCH_list_command[4];
		if (WCH_list_command.size() < 5 || h > 24 || m >= 60 || h < 1 || m < 0) {
			WCH_InputCommandIncorrect();
			return;
		} else {
			bool flag = false;
			for (auto it = WCH_list_clock.begin(); it != WCH_list_clock.end(); it++) {
				if (get<0>(*it) == h && get<1>(*it) == m && get<2>(*it) == Tname) {
					flag = true;
					break;
				}
			}
			if (!flag) {
				WCH_num_clock++;
				WCH_list_clock.insert(make_tuple(h, m, Tname));
			} else {
				throw runtime_error("");
			}
		}
	} catch (...) {
		WCH_InputCommandIncorrect();
	}
}

void WCH_clock_delete() {
	// Delete a clock.
	if (WCH_list_command.size() != 5) {
		WCH_InputCommandIncorrect();
		return;
	}
	try {
		int32_t h = stoi(WCH_list_command[2]);
		int32_t m = stoi(WCH_list_command[3]);
		wstring Tname = WCH_list_command[4];
		bool flag = false;
		for (auto it = WCH_list_clock.begin(); it != WCH_list_clock.end(); it++) {
			if (get<0>(*it) == h && get<1>(*it) == m && get<2>(*it) == Tname) {
				WCH_list_clock.erase(it);
				flag = true;
				WCH_num_clock--;
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

void WCH_clock_clear() {
	// Clear clock list.
	if (WCH_list_command.size() != 2) {
		WCH_InputCommandIncorrect();
		return;
	}
	WCH_list_clock.clear();
	WCH_num_clock = 0;
}

void WCH_clock_list() {
	// List all clocks.
	if (WCH_list_command.size() != 2) {
		WCH_InputCommandIncorrect();
		return;
	}
	size_t MAXH = 0;
	size_t MAXM = 0;
	size_t MAXT = 0;
	for (auto it = WCH_list_clock.begin(); it != WCH_list_clock.end(); it++) {
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
	for (auto it = WCH_list_clock.begin(); it != WCH_list_clock.end(); it++) {
		wcout << get<0>(*it);
		WCH_PrintChar(MAXH - WCH_GetNumDigits(get<0>(*it)), L' ');
		wcout << L" | " << get<1>(*it);
		WCH_PrintChar(MAXM - WCH_GetNumDigits(get<1>(*it)), L' ');
		wcout << L" | " << get<2>(*it) << endl;
	}
}

void WCH_clock() {
	// Clock series command input.
	if (WCH_list_command.size() == 1) {
		WCH_InputCommandIncorrect();
		return;
	}
	transform(WCH_list_command[1].begin(), WCH_list_command[1].end(), WCH_list_command[1].begin(), ::towlower);
	if (WCH_list_command[1] == L"add") {
		WCH_clock_add();
	} else if (WCH_list_command[1] == L"delete") {
		WCH_clock_delete();
	} else if (WCH_list_command[1] == L"clear") {
		WCH_clock_clear();
	} else if (WCH_list_command[1] == L"list") {
		WCH_clock_list();
	} else {
		WCH_InputCommandIncorrect();
	}
}

void WCH_task_add() {
	// Add a new task.
	if (WCH_list_command.size() != 3) {
		WCH_InputCommandIncorrect();
		return;
	}
	wstring task = WCH_list_command[2];
	if (WCH_list_task.find(task) != WCH_list_task.end()) {
		WCH_InputCommandIncorrect();
	} else {
		WCH_list_task.insert(task);
		WCH_num_task++;
	}
}

void WCH_task_delete() {
	// Delete a task.
	if (WCH_list_command.size() != 3) {
		WCH_InputCommandIncorrect();
		return;
	}
	wstring task = WCH_list_command[2];
	if (WCH_list_task.find(task) == WCH_list_task.end()) {
		WCH_InputCommandIncorrect();
	} else {
		WCH_list_task.erase(task);
		WCH_num_task--;
	}
}

void WCH_task_clear() {
	// Clear task list.
	if (WCH_list_command.size() != 2) {
		WCH_InputCommandIncorrect();
		return;
	}
	WCH_list_task.clear();
	WCH_num_task = 0;
}

void WCH_task_list() {
	// List all tasks.
	if (WCH_list_command.size() != 2) {
		WCH_InputCommandIncorrect();
		return;
	}
	size_t MAX = 0;
	for (auto it = WCH_list_task.begin(); it != WCH_list_task.end(); it++) {
		MAX = max(MAX, WCH_GetWstrDisplaySize(*it));
	}
	if (MAX == 0) {
		return;
	}
	MAX = max(MAX, WCH_GetWstrDisplaySize(StrToWstr(WCH_Language["ProcessName"].asString())));
	wcout << StrToWstr(WCH_Language["ProcessName"].asString()) << endl;
	WCH_PrintChar(MAX, L'-');
	wcout << endl;
	for (auto it = WCH_list_task.begin(); it != WCH_list_task.end(); it++) {
		wcout << *it << endl;
	}
}

void WCH_task() {
	// Task series command input.
	if (WCH_list_command.size() == 1) {
		WCH_InputCommandIncorrect();
		return;
	}
	if (WCH_list_command[1] == L"add") {
		WCH_task_add();
	} else if (WCH_list_command[1] == L"delete") {
		WCH_task_delete();
	} else if (WCH_list_command[1] == L"clear") {
		WCH_task_clear();
	} else if (WCH_list_command[1] == L"list") {
		WCH_task_list();
	} else {
		WCH_InputCommandIncorrect();
	}
}

void WCH_work_add() {
	// Add a new work.
	if (WCH_list_command.size() != 3 && WCH_list_command.size() != 4) {
		WCH_InputCommandIncorrect();
		return;
	}
	wstring work = WCH_list_command[2];
	wstring tag = WCH_list_command.size() == 3 ? StrToWstr(WCH_Language["Unclassified"].asString()) : WCH_list_command[3];
	if (WCH_list_work.find(make_pair(work, tag)) != WCH_list_work.end()) {
		WCH_InputCommandIncorrect();
	} else {
		WCH_list_work.insert(make_pair(work, tag));
		WCH_num_work++;
	}
}

void WCH_work_delete() {
	// Delete a work.
	if (WCH_list_command.size() != 3 && WCH_list_command.size() != 4) {
		WCH_InputCommandIncorrect();
		return;
	}
	wstring work = WCH_list_command[2];
	wstring tag = WCH_list_command.size() == 3 ? StrToWstr(WCH_Language["Unclassified"].asString()) : WCH_list_command[3];
	if (WCH_list_work.find(make_pair(work, tag)) == WCH_list_work.end()) {
		WCH_InputCommandIncorrect();
	} else {
		WCH_list_work.erase(make_pair(work, tag));
		WCH_num_work--;
	}
}

void WCH_work_clear() {
	// Clear work list.
	if (WCH_list_command.size() != 2) {
		WCH_InputCommandIncorrect();
		return;
	}
	WCH_list_work.clear();
	WCH_num_work = 0;
}

void WCH_work_list() {
	// List all works.
	if (WCH_list_command.size() != 2) {
		WCH_InputCommandIncorrect();
		return;
	}
	size_t MAXN = 0;
	size_t MAXT = 0;
	for (auto it = WCH_list_work.begin(); it != WCH_list_work.end(); it++) {
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
	for (auto it = WCH_list_work.begin(); it != WCH_list_work.end(); it++) {
		wcout << it->first;
		WCH_PrintChar(MAXN - WCH_GetWstrDisplaySize(it->first), L' ');
		wcout << L" | " << it->second << endl;
	}
}

void WCH_work() {
	// Work series command input.
	if (WCH_list_command.size() == 1) {
		WCH_InputCommandIncorrect();
		return;
	}
	transform(WCH_list_command[1].begin(), WCH_list_command[1].end(), WCH_list_command[1].begin(), ::towlower);
	if (WCH_list_command[1] == L"add") {
		WCH_work_add();
	} else if (WCH_list_command[1] == L"delete") {
		WCH_work_delete();
	} else if (WCH_list_command[1] == L"clear") {
		WCH_work_clear();
	} else if (WCH_list_command[1] == L"list") {
		WCH_work_list();
	} else {
		WCH_InputCommandIncorrect();
	}
}

void WCH_game() {
	// Guessing game.
	if (WCH_list_command.size() != 1) {
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
	wcout << StrToWstr(WCH_Language["NumberAnswer"].asString()) << L" " << ans;
	if (flag) {
		wcout << StrToWstr(WCH_Language["NumberWin"].asString()) << endl;
	} else {
		wcout << StrToWstr(WCH_Language["NumberLose"].asString()) << endl;
	}
}

void WCH_prtscn() {
	// A sequence of function to make a screenshot.
	if (WCH_list_command.size() != 1) {
		WCH_InputCommandIncorrect();
		return;
	}
	WCH_SetWindowStatus(false);
	WCH_Sleep(500);
	WCH_PutPicture();
	WCH_SaveImg();
	WCH_Sleep(500);
	WCH_SetWindowStatus(true);
	wcout << StrToWstr(WCH_Language["Prtscn"].asString()) << endl;
}

void WCH_countdown() {
	// Start a count down timer.
	if (WCH_list_command.size() != 4) {
		WCH_InputCommandIncorrect();
		return;
	}
	try {
		int32_t h = stoi(WCH_list_command[1]);
		int32_t m = stoi(WCH_list_command[2]);
		int32_t s = stoi(WCH_list_command[3]);
		if ((h == 0 && m == 0 && s == 0) || (h < 0 || m < 0 || s < 0) || (h >= 24 || m >= 60 || s >= 60)) {
			throw runtime_error("");
		}
		WCH_is_countdown = true;
		wcout << StrToWstr(WCH_Language["CountDown"].asString()) << endl;
		WCH_progress_bar_duration = h * 3600 + m * 60 + s;
		WCH_ProgressBar();
		if (StrToWstr(WCH_Settings["CountDownSoundPrompt"].asString()) == L"True") {
			wcout << L"\a";
		}
		WCH_is_countdown = false;
	} catch (...) {
		WCH_InputCommandIncorrect();
		WCH_is_countdown = false;
	}
}

void WCH_focus() {
	// Enable focus function.
	if (WCH_list_command.size() != 1) {
		WCH_InputCommandIncorrect();
		return;
	}
	if (MessageBoxW(NULL, StrToWstr(WCH_Language["Focus"].asString()).c_str(), L"WCH WARN", MB_ICONWARNING | MB_YESNO | MB_TOPMOST) == IDYES) {
		WCH_SetWindowStatus(false);
		WCH_is_focus = true;
		WCH_Sleep(500);
		WCH_SetTrayStatus(false);
		WCH_Sleep(500);
		thread T(WCH_check_task_loop);
		T.detach();
	}
}

void WCH_trans() {
	// Translate string between Chinese / English.
	if (WCH_list_command.size() != 2) {
		WCH_InputCommandIncorrect();
		return;
	}
	try {
		wstring FilePath = WCH_path_temp + L"\\WCH_TRANS.tmp";
		URLDownloadToFileW(NULL, (L"http://fanyi.youdao.com/openapi.do?keyfrom=xujiangtao&key=1490852988&type=data&doctype=json&version=1.1&only=translate&q=" + StrToWstr(UrlEncode(WstrToStr((WCH_list_command[1]))))).c_str(), FilePath.c_str(), 0, NULL);
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
	if (WCH_list_command.size() != 1) {
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
	if (WCH_list_command.size() != 1) {
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
	if (WCH_list_command.size() != 1) {
		WCH_InputCommandIncorrect();
		return;
	}
	WCH_Time q = WCH_GetTime();
	wcout << format(L"{:04}/{:02}/{:02} {:02}:{:02}:{:02}", q.Year, q.Month, q.Day, q.Hour, q.Minute, q.Second) << endl;
}

void WCH_help() {
	// Print help information.
	if (WCH_list_command.size() > 2) {
		WCH_InputCommandIncorrect();
		return;
	}
	wstring FilePath = WCH_GetExecDir() + L"\\resources\\" + StrToWstr(WCH_Settings["Language"].asString()) + L"\\help.json";
	Json::Value val;
	SPDLOG_INFO(format(L"Reading file \"{}\"", FilePath));
	fin.open(FilePath);
	if (!fin.is_open() || WCH_GetFileHash(FilePath) != (StrToWstr(WCH_Settings["Language"].asString()) == L"en-US" ? SHASUM_enUS_help : SHASUM_zhCN_help)) {
		SPDLOG_ERROR(format(L"Data in file \"{}\" corrupted", FilePath));
		WCH_FileProcessingFailed();
		raise(SIGBREAK);
	}
	ignore = JSON_Reader.parse(fin, val);
	fin.close();
	if (WCH_list_command.size() == 1) {
		for (auto it = val["index"].begin(); it != val["index"].end(); it++) {
			wcout << StrToWstr((*it).asString()) << endl;
		}
	} else {
		if (WCH_support_command.find(WCH_list_command[1]) == WCH_support_command.end()) {
			WCH_InputCommandIncorrect();
			return;
		}
		transform(WCH_list_command[1].begin(), WCH_list_command[1].end(), WCH_list_command[1].begin(), ::towlower);
		for (auto it = val[WstrToStr(WCH_list_command[1])].begin(); it != val[WstrToStr(WCH_list_command[1])].end(); it++) {
			wcout << StrToWstr((*it).asString()) << endl;
		}
		if (WCH_list_command[1] == L"config") {
			FilePath = WCH_GetExecDir() + L"\\resources\\" + StrToWstr(WCH_Settings["Language"].asString()) + L"\\config.json";
			Json::Value valcfg;
			SPDLOG_INFO(format(L"Reading file \"{}\"", FilePath));
			fin.open(FilePath);
			if (!fin.is_open() || WCH_GetFileHash(FilePath) != (StrToWstr(WCH_Settings["Language"].asString()) == L"en-US" ? SHASUM_enUS_config : SHASUM_zhCN_config)) {
				SPDLOG_ERROR(format(L"Data in file \"{}\" corrupted", FilePath));
				WCH_FileProcessingFailed();
				raise(SIGBREAK);
			}
			ignore = JSON_Reader.parse(fin, valcfg);
			fin.close();
			size_t MAXKN = 0;
			size_t MAXVT = 0;
			size_t MAXDE = 0;
			size_t MAXDV = 0;
			size_t MAXRR = 0;
			int32_t cnt = 0;
			for (auto it = WCH_support_settings.begin(); it != WCH_support_settings.end(); it++, cnt++) {
				MAXKN = max(MAXKN, WCH_GetWstrDisplaySize(get<0>(*it)));
				MAXVT = max(MAXVT, WCH_GetWstrDisplaySize(get<1>(*it)));
				MAXDE = max(MAXDE, WCH_GetWstrDisplaySize(StrToWstr(valcfg["Content"][cnt].asString())));
				MAXDV = max(MAXDV, WCH_GetWstrDisplaySize(get<2>(*it)));
				MAXRR = max(MAXRR, WCH_GetWstrDisplaySize(StrToWstr(WCH_Language[get<3>(*it) ? "Yes" : "No"].asString())));
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
			for (auto it = WCH_support_settings.begin(); it != WCH_support_settings.end(); it++, cnt++) {
				wcout << get<0>(*it);
				WCH_PrintChar(MAXKN - WCH_GetWstrDisplaySize(get<0>(*it)), L' ');
				wcout << L" | " << get<1>(*it);
				WCH_PrintChar(MAXVT - WCH_GetWstrDisplaySize(get<1>(*it)), L' ');
				wcout << L" | " << StrToWstr(valcfg["Content"][cnt].asString());
				WCH_PrintChar(MAXDE - WCH_GetWstrDisplaySize(StrToWstr(valcfg["Content"][cnt].asString())), L' ');
				wcout << L" | " << get<2>(*it);
				WCH_PrintChar(MAXDV - WCH_GetWstrDisplaySize(get<2>(*it)), L' ');
				wcout << L" | " << StrToWstr(WCH_Language[get<3>(*it) ? "Yes" : "No"].asString()) << endl;
			}
		}
	}
}

#endif
