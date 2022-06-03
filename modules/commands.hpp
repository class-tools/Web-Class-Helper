/*
Web Class Helper Commands Module Header File 2.0.1
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

extern const string WCH_WDName[7];
extern map <string, function <void ()>> WCH_command_support;
extern vector <string> WCH_command_list;
extern multimap <int, pair <int, string>> WCH_clock_list;
extern set <string> WCH_task_list;
extern set <string> WCH_work_list;
extern wstring WCH_window_title;
extern HWND WCH_hWnd;
extern HMENU WCH_hMenu;
extern int WCH_clock_num;
extern int WCH_task_num;
extern int WCH_work_num;
extern int WCH_clock_change;
extern int WCH_task_change;
extern int WCH_work_change;
extern int WCH_ProgressBarCount;
extern int WCH_ProgressBarTot;
extern int WCH_InputTimes;
extern bool WCH_cmd_line;
extern bool WCH_anti_idle;
extern bool WCH_program_end;
extern string WCH_command;
extern string WCH_ProgressBarStr;
extern ifstream fin;
extern ofstream fout;
WCH_Time WCH_GetTime();
void WCH_Sleep(int _ms);
void WCH_Error(int _in);
void WCH_printlog(string _mode, string _info);
void WCH_read();
bool WCH_save_func();
int WCH_GetNumDigits(int _n);

void WCH_clear() {
	// Clear console information.
	if ((int)WCH_command_list.size() != 1) {
		WCH_Error(WCH_ERRNO_UNCORRECT);
		return;
	}
	system("cls");
	cout << "Web Class Helper " << WCH_VER << " (x" << to_string(WCH_DisplayFramework) << ")" << endl;
	cout << "Copyright (c) 2022 Class Tools Develop Team." << endl;
	cout << "Type \"help\", \"update\" or \"license\" for more information." << endl;
}

void WCH_quit() {
	// Quit.
	if ((int)WCH_command_list.size() != 1) {
		WCH_Error(WCH_ERRNO_UNCORRECT);
		return;
	}
	WCH_printlog(WCH_LOG_STATUS_INFO, "Exiting \"Web Class Helper (x" + to_string(WCH_DisplayFramework) + ")\"");
	WCH_cmd_line = false;
	WCH_program_end = true;
	WCH_CheckAndDeleteFile(L"WCH_SYSTEM_NORMAL.tmp");
	WCH_CheckAndDeleteFile(L"WCH_SYSTEM_ERROR.tmp");
	WCH_CheckAndDeleteFile(L"WCH_UPD.tmp");
	WCH_CheckAndDeleteFile(L"WCH_TRANS.tmp");
	WCH_CheckAndDeleteFile(L"WCH_STDL.tmp");
	exit(0);
}

void WCH_hide() {
	// Hide the window.
	if ((int)WCH_command_list.size() != 1) {
		WCH_Error(WCH_ERRNO_UNCORRECT);
		return;
	}
	WCH_SetWindowStatus(false);
}

void WCH_wiki() {
	// Visit the wiki page.
	if ((int)WCH_command_list.size() != 1) {
		WCH_Error(WCH_ERRNO_UNCORRECT);
		return;
	}
	cout << "Jumping to wiki page..." << endl;
	system("start resources/website/wiki.url");
}

void WCH_update() {
	// Visit the website to update the program.
	if ((int)WCH_command_list.size() != 1) {
		WCH_Error(WCH_ERRNO_UNCORRECT);
		return;
	}
	try {
		cout << "Checking update..." << endl;
		WCH_ProgressBarTot = 5;
		thread T(WCH_ProgressBar);
		T.detach();
		wstring url = L"https://class-tools.gq/update/WCH?";
		srand((unsigned)time(NULL));
		url += to_wstring(rand());
		URLDownloadToFileW(0, url.c_str(), L"WCH_UPD.tmp", 0, 0);
		string res;
		fin.open("WCH_UPD.tmp");
		getline(fin, res);
		fin.close();
		WCH_Sleep(5000);
		if (res == "") {
			throw runtime_error("WCH_ERRNO_NETWORK_FAILURE: \"Check Update\"");
		}
		if (WCH_CheckVersion(WCH_GetVersion(WCH_VER), WCH_GetVersion(res))) {
			cout << "Program version is less than latest released version, jumping to releases page..." << endl;
			system("start resources/website/releases.url");
			WCH_printlog(WCH_LOG_STATUS_INFO, "Updating to version \"" + res + "\"");
		} else {
			cout << "Program version equals or is greater than latest released version." << endl;
			WCH_printlog(WCH_LOG_STATUS_INFO, "Program version equals or is greater than \"" + res + "\"");
		}
		DeleteFileW(L"WCH_UPD.tmp");
	} catch (...) {
		WCH_Error(WCH_ERRNO_NETWORK_FAILURE);
		return;
	}
}

void WCH_license() {
	// Print the license.
	if ((int)WCH_command_list.size() != 1) {
		WCH_Error(WCH_ERRNO_UNCORRECT);
		return;
	}
	fin.open("LICENSE");
	if (!fin.is_open()) {
		WCH_Error(WCH_ERRNO_FILE_NOT_FOUND);
		return;
	}
	string _res;
	while (getline(fin, _res)) {
		cout << _res << endl;
	}
	fin.close();
}

void WCH_add_clock() {
	// Add a new clock.
	if ((int)WCH_command_list.size() < 5) {
		WCH_Error(WCH_ERRNO_UNCORRECT);
		return;
	}
	try {
		int h = stoi(WCH_command_list[2]);
		int m = stoi(WCH_command_list[3]);
		string Tname = WCH_command.substr(12 + (int)WCH_command_list[2].size() + (int)WCH_command_list[3].size(), WCH_command.size() - 1);
		if ((int)WCH_command_list.size() < 5 || h > 24 || m >= 60 || h < 1 || m < 0) {
			WCH_Error(WCH_ERRNO_UNCORRECT);
			return;
		} else {
			bool flag = false;
			for (auto it = WCH_clock_list.equal_range(h).first; it != WCH_clock_list.equal_range(h).second; it++) {
				if ((it -> second).first == m) {
					flag = true;
					break;
				}
			}
			if (!flag) {
				WCH_clock_num++;
				WCH_clock_change++;
				WCH_clock_list.emplace(make_pair(h, make_pair(m, Tname)));
			} else {
				WCH_Error(WCH_ERRNO_UNCORRECT);
			}
		}
	} catch (...) {
		WCH_Error(WCH_ERRNO_UNCORRECT);
	}
}

void WCH_delete_clock() {
	// Delete a clock.
	if ((int)WCH_command_list.size() < 5) {
		WCH_Error(WCH_ERRNO_UNCORRECT);
		return;
	}
	try {
		int h = stoi(WCH_command_list[2]);
		int m = stoi(WCH_command_list[3]);
		string Tname = WCH_command.substr(15 + (int)WCH_command_list[2].size() + (int)WCH_command_list[3].size(), WCH_command.size() - 1);
		bool flag = false;
		for (auto it = WCH_clock_list.equal_range(h).first; it != WCH_clock_list.equal_range(h).second; it++) {
			if ((it -> second).first == m && (it -> second).second == Tname) {
				WCH_clock_list.erase(it);
				flag = true;
				WCH_clock_num--;
				WCH_clock_change++;
				break;
			}
		}
		if (!flag) {
			WCH_Error(WCH_ERRNO_CLOCK_OPERATION);
		}
	} catch (...) {
		WCH_Error(WCH_ERRNO_CLOCK_OPERATION);
	}
}

void WCH_change_clock() {
	// Change a clock.
	if ((int)WCH_command_list.size() < 5) {
		WCH_Error(WCH_ERRNO_UNCORRECT);
		return;
	}
	try {
		int h = stoi(WCH_command_list[2]);
		int m = stoi(WCH_command_list[3]);
		string Tname = WCH_command.substr(15 + (int)WCH_command_list[2].size() + (int)WCH_command_list[3].size(), WCH_command.size() - 1);
		bool flag = false;
		for (auto it = WCH_clock_list.equal_range(h).first; it != WCH_clock_list.equal_range(h).second; it++) {
			if ((it -> second).first == m) {
				(it -> second).second = Tname;
				flag = true;
				WCH_clock_change++;
			}
		}
		if (!flag) {
			WCH_Error(WCH_ERRNO_CLOCK_OPERATION);
		}
	} catch (...) {
		WCH_Error(WCH_ERRNO_CLOCK_OPERATION);
	}
}

void WCH_clear_clock() {
	// Clear clock list.
	if ((int)WCH_command_list.size() != 2) {
		WCH_Error(WCH_ERRNO_UNCORRECT);
		return;
	}
	WCH_clock_list.clear();
	WCH_clock_num = 0;
	WCH_clock_change++;
}

void WCH_list_clock() {
	// List all tasks.
	if ((int)WCH_command_list.size() != 2) {
		WCH_Error(WCH_ERRNO_UNCORRECT);
		return;
	}
	int MAXH = -1;
	int MAXM = -1;
	int MAXT = -1;
	for (auto it = WCH_clock_list.begin(); it != WCH_clock_list.end(); it++) {
		MAXH = max(MAXH, WCH_GetNumDigits(it -> first));
		MAXM = max(MAXM, WCH_GetNumDigits((it -> second).first));
		MAXT = max(MAXT, (int)((it -> second).second).size());
	}
	if (MAXH == -1 && MAXM == -1 && MAXT == -1) {
		return;
	}
	MAXH = max(MAXH, 4);
	MAXM = max(MAXM, 6);
	MAXT = max(MAXT, 4);
	cout << "Hour";
	WCH_PrintChar(MAXH - 4, ' ');
	cout << " | Minute";
	WCH_PrintChar(MAXM - 6, ' ');
	cout << " | Name";
	WCH_PrintChar(MAXT - 4, ' ');
	cout << endl;
	WCH_PrintChar(MAXH, '-');
	cout << " | ";
	WCH_PrintChar(MAXM, '-');
	cout << " | ";
	WCH_PrintChar(MAXT, '-');
	cout << endl;
	for (int i = 0; i <= 24; i++) {
		for (auto it = WCH_clock_list.equal_range(i).first; it != WCH_clock_list.equal_range(i).second; it++) {
			cout << i;
			WCH_PrintChar(MAXH - WCH_GetNumDigits(i), ' ');
			cout << " | " << (it -> second).first;
			WCH_PrintChar(MAXM - WCH_GetNumDigits((it -> second).first), ' ');
			cout << " | " << (it -> second).second << endl;
		}
	}
}

void WCH_check_clock() {
	// Clock series command input.
	if (WCH_command_list.size() == 1) {
		WCH_Error(WCH_ERRNO_UNCORRECT);
		return;
	}
	transform(WCH_command_list[1].begin(), WCH_command_list[1].end(), WCH_command_list[1].begin(), ::tolower);
	if (WCH_command_list[1] == "add") {
		WCH_add_clock();
	} else if (WCH_command_list[1] == "delete") {
		WCH_delete_clock();
	} else if (WCH_command_list[1] == "clear") {
		WCH_clear_clock();
	} else if (WCH_command_list[1] == "change") {
		WCH_change_clock();
	} else if (WCH_command_list[1] == "list") {
		WCH_list_clock();
	} else {
		WCH_Error(WCH_ERRNO_UNCORRECT);
	}
}

void WCH_add_task() {
	// Add a new task.
	if ((int)WCH_command_list.size() < 3) {
		WCH_Error(WCH_ERRNO_UNCORRECT);
		return;
	}
	string task = WCH_command.substr(9, WCH_command.size() - 1);
	transform(task.begin(), task.end(), task.begin(), ::tolower);
	if (WCH_task_list.find(task) != WCH_task_list.end()) {
		WCH_Error(WCH_ERRNO_WORK_OPERATION);
	} else {
		WCH_task_list.insert(task);
		WCH_task_num++;
		WCH_task_change++;
	}
}

void WCH_delete_task() {
	// Delete a task.
	if ((int)WCH_command_list.size() < 3) {
		WCH_Error(WCH_ERRNO_UNCORRECT);
		return;
	}
	string task = WCH_command.substr(12, WCH_command.size() - 1);
	transform(task.begin(), task.end(), task.begin(), ::tolower);
	if (WCH_task_list.find(task) == WCH_task_list.end()) {
		WCH_Error(WCH_ERRNO_TASK_OPERATION);
	} else {
		WCH_task_list.erase(task);
		WCH_task_num--;
		WCH_task_change++;
	}
}

void WCH_clear_task() {
	// Clear task list.
	if ((int)WCH_command_list.size() != 2) {
		WCH_Error(WCH_ERRNO_UNCORRECT);
		return;
	}
	WCH_task_list.clear();
	WCH_task_num = 0;
	WCH_task_change++;
}

void WCH_list_task() {
	// List all tasks.
	if ((int)WCH_command_list.size() != 2) {
		WCH_Error(WCH_ERRNO_UNCORRECT);
		return;
	}
	int MAX = -1;
	for (auto it = WCH_task_list.begin(); it != WCH_task_list.end(); it++) {
		MAX = max(MAX, (int)(*it).size());
	}
	if (MAX == -1) {
		return;
	}
	MAX = max(MAX, 12);
	cout << "Process Name" << endl;
	WCH_PrintChar(MAX, '-');
	cout << endl;
	for (auto it = WCH_task_list.begin(); it != WCH_task_list.end(); it++) {
		cout << *it << endl;
	}
}

void WCH_check_task() {
	// Task series command input.
	if ((int)WCH_command_list.size() == 1) {
		WCH_Error(WCH_ERRNO_UNCORRECT);
		return;
	}
	transform(WCH_command_list[1].begin(), WCH_command_list[1].end(), WCH_command_list[1].begin(), ::tolower);
	if (WCH_command_list[1] == "add") {
		WCH_add_task();
	} else if (WCH_command_list[1] == "delete") {
		WCH_delete_task();
	} else if (WCH_command_list[1] == "clear") {
		WCH_clear_task();
	} else if (WCH_command_list[1] == "list") {
		WCH_list_task();
	} else {
		WCH_Error(WCH_ERRNO_UNCORRECT);
	}
}

void WCH_add_work() {
	// Add a new work.
	if ((int)WCH_command_list.size() < 3) {
		WCH_Error(WCH_ERRNO_UNCORRECT);
		return;
	}
	string work = WCH_command.substr(9, WCH_command.size() - 1);
	if (WCH_work_list.find(work) != WCH_work_list.end()) {
		WCH_Error(WCH_ERRNO_WORK_OPERATION);
	} else {
		WCH_work_list.insert(work);
		WCH_work_num++;
		WCH_work_change++;
	}
}

void WCH_delete_work() {
	// Delete a work.
	if ((int)WCH_command_list.size() < 3) {
		WCH_Error(WCH_ERRNO_UNCORRECT);
		return;
	}
	string work = WCH_command.substr(10, WCH_command.size() - 1);
	if (WCH_work_list.find(work) == WCH_work_list.end()) {
		WCH_Error(WCH_ERRNO_WORK_OPERATION);
	} else {
		WCH_work_list.erase(work);
		WCH_work_num--;
		WCH_work_change++;
	}
}

void WCH_clear_work() {
	// Clear work list.
	if ((int)WCH_command_list.size() != 2) {
		WCH_Error(WCH_ERRNO_UNCORRECT);
		return;
	}
	WCH_work_list.clear();
	WCH_work_num = 0;
	WCH_work_change++;
}

void WCH_list_work() {
	// List all works.
	if ((int)WCH_command_list.size() != 2) {
		WCH_Error(WCH_ERRNO_UNCORRECT);
		return;
	}
	int MAX = -1;
	for (auto it = WCH_work_list.begin(); it != WCH_work_list.end(); it++) {
		MAX = max(MAX, (int)(*it).size());
	}
	if (MAX == -1) {
		return;
	}
	MAX = max(MAX, 4);
	cout << "Name" << endl;
	WCH_PrintChar(MAX, '-');
	cout << endl;
	for (auto it = WCH_work_list.begin(); it != WCH_work_list.end(); it++) {
		cout << *it << endl;
	}
}

void WCH_check_work() {
	// Work series command input.
	if (WCH_command_list.size() == 1) {
		WCH_Error(WCH_ERRNO_UNCORRECT);
		return;
	}
	transform(WCH_command_list[1].begin(), WCH_command_list[1].end(), WCH_command_list[1].begin(), ::tolower);
	if (WCH_command_list[1] == "add") {
		WCH_add_work();
	} else if (WCH_command_list[1] == "delete") {
		WCH_delete_work();
	} else if (WCH_command_list[1] == "clear") {
		WCH_clear_work();
	} else if (WCH_command_list[1] == "list") {
		WCH_list_work();
	} else {
		WCH_Error(WCH_ERRNO_UNCORRECT);
	}
}

void WCH_game() {
	// Guessing game.
	if ((int)WCH_command_list.size() != 1) {
		WCH_Error(WCH_ERRNO_UNCORRECT);
		return;
	}
	srand((unsigned)time(NULL));
	int ans = rand() % 10000 + 1;
	bool flag = true;
	string z = "0";
	vector <string> zv;
	zv.push_back("0");
	try {
		while (stoi(zv[0]) != ans) {
			BEGIN: cout << "Please input your number (1 ~ 10000): ";
			getline(cin, z);
			zv = WCH_split(z);
			if ((int)zv.size() != 1) {
				if ((int)zv.size() > 1) {
					cout << "Number out of range." << endl;
					zv.clear();
					zv.push_back("0");
				} else {
					flag = false;
					goto END;
				}
			} else if (zv[0][0] == '-' || zv[0][0] == '0' || (zv[0].size() >= 5 && zv[0] != "10000")) {
				cout << "Number out of range." << endl;
			} else if (stoi(zv[0]) > ans) {
				cout << "The answer is smaller." << endl;
			} else if (stoi(zv[0]) < ans) {
				cout << "The answer is bigger." << endl;
			}
		}
	} catch (...) {
		cout << "Number out of range." << endl;
		goto BEGIN;
	}
	END: cout << "The number is " << ans << ".";
	if (flag) {
		cout << " You WIN!";
	}
	cout << endl;
}

void WCH_speedtest() {
	// Start a speed test with Python program.
	if ((int)WCH_command_list.size() != 1) {
		WCH_Error(WCH_ERRNO_UNCORRECT);
		return;
	}
	string tmp = "SPEEDTEST";
	tmp += to_string(WCH_Framework);
	tmp += ".EXE";
	system(tmp.c_str());
}

void WCH_pi() {
	// A sequence of function to make a screenshot.
	if ((int)WCH_command_list.size() != 1) {
		WCH_Error(WCH_ERRNO_UNCORRECT);
		return;
	}
	WCH_SetWindowStatus(false);
	WCH_Sleep(500);
	WCH_PutPicture();
	WCH_SaveImg();
	WCH_Sleep(500);
	WCH_SetWindowStatus(true);
	cout << "The picture is in the clipboard and be saved in your Pictures folder." << endl;
}

void WCH_check_task_loop() {
	// Check if the running task is in the task list. (Another thread)
	while (WCH_anti_idle && !WCH_program_end) {
		for (auto it = WCH_task_list.begin(); it != WCH_task_list.end(); it++) {
			if (WCH_TaskKill(*it)) {
				WCH_printlog(WCH_LOG_STATUS_INFO, "Successfully killed \"" + *it + "\"");
			} else {
				WCH_printlog(WCH_LOG_STATUS_INFO, "Failed to kill \"" + *it + "\"");
			}
		}
		WCH_Sleep(3000);
	}
}

void WCH_anti_idle_func() {
	// Enable anti-idle function.
	if ((int)WCH_command_list.size() != 1) {
		WCH_Error(WCH_ERRNO_UNCORRECT);
		return;
	}
	string ch;
	cout << "Are you sure to enable anti-idle function? If you want to disable it, press Ctrl + Down. (Y/N): ";
	getline(cin, ch);
	if (ch == "Y" || ch == "y") {
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
	if ((int)WCH_command_list.size() == 1) {
		WCH_Error(WCH_ERRNO_UNCORRECT);
		return;
	}
	try {
		string info;
		string res;
		info = "TRANS";
		info += to_string(WCH_Framework);
		info += " -i \"";
		info += WCH_command.substr(5, WCH_command.size() - 1);
		info += "\" > WCH_TRANS.tmp";
		system(info.c_str());
		WCH_Sleep(1000);
		WCH_cmd_line = false;
		if (_access("WCH_TRANS.tmp", 0) != -1) {
			fin.open("WCH_TRANS.tmp");
			getline(fin, res);
			cout << res << endl;
			fin.close();
			DeleteFileW(L"WCH_TRANS.tmp");
		} else {
			throw runtime_error("WCH_ERRNO_NETWORK_FAILURE: \"Youdao Translation\"");
		}
		WCH_cmd_line = true;
	} catch (...) {
		WCH_Error(WCH_ERRNO_NETWORK_FAILURE);
		return;
	}
}

void WCH_ow() {
	// Get a random sentence.
	if ((int)WCH_command_list.size() != 1) {
		WCH_Error(WCH_ERRNO_UNCORRECT);
		return;
	}
	try {
		URLDownloadToFileW(0, L"https://v1.hitokoto.cn/?encode=text", L"WCH_STDL.tmp", 0, 0);
		WCH_Sleep(1000);
		WCH_cmd_line = false;
		if (_access("WCH_STDL.tmp", 0) != -1) {
			fin.open("WCH_STDL.tmp");
			string res;
			getline(fin, res);
			cout << UTF8ToANSI(res) << endl;
			fin.close();
			DeleteFileW(L"WCH_STDL.tmp");
		} else {
			throw runtime_error("WCH_ERRNO_NETWORK_FAILURE: \"Random Sentence\"");
		}
		WCH_cmd_line = true;
	} catch (...) {
		WCH_Error(WCH_ERRNO_NETWORK_FAILURE);
		return;
	}
}

void WCH_time() {
	// Print current time.
	if ((int)WCH_command_list.size() != 1) {
		WCH_Error(WCH_ERRNO_UNCORRECT);
		return;
	}
	WCH_Time q = WCH_GetTime();
	cout << format("{:04}/{:02}/{:02} {:02}:{:02}:{:02}", q.Year, q.Month, q.Day, q.Hour, q.Minute, q.Second) << endl;
}

void WCH_help() {
	// Print help information.
	if ((int)WCH_command_list.size() == 1) {
		if (_access("resources/help/index.dat", 0) != -1) {
			string _res;
			fin.open("resources/help/index.dat");
			while (getline(fin, _res)) {
				cout << _res << endl;
			}
			fin.close();
		} else {
			WCH_Error(WCH_ERRNO_FILE_NOT_FOUND);
		}
	} else {
		transform(WCH_command_list[1].begin(), WCH_command_list[1].end(), WCH_command_list[1].begin(), ::tolower);
		if ((int)WCH_command_list.size() == 2 && _access(("resources/help/" + WCH_command_list[1] + ".dat").c_str(), 0) != -1) {
			string _res;
			fin.open(("resources/help/" + WCH_command_list[1] + ".dat").c_str());
			while (getline(fin, _res)) {
				cout << _res << endl;
			}
			fin.close();
		} else {
			if (WCH_command_support.find(WCH_command_list[1]) != WCH_command_support.end()) {
				WCH_Error(WCH_ERRNO_FILE_NOT_FOUND);
			} else {
				WCH_Error(WCH_ERRNO_UNCORRECT);
			}
			return;
		}
	}
}

void WCH_save_cmd() {
	// Save data. (Command)
	if ((int)WCH_command_list.size() != 1) {
		WCH_Error(WCH_ERRNO_UNCORRECT);
		return;
	}
	if (WCH_save_func()) {
		cout << "Successfully saved all data." << endl;
	} else {
		cout << "No data to save." << endl;
	}
}

#endif
