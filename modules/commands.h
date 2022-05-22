/*
Web Class Helper Commands Module Header File 2.0.1
This source code file is under MIT License.
Copyright (c) 2022 Class Tools Develop Team
Contributors: jsh-jsh ren-yc hjl2011 Daijianghao
*/
#ifndef COMMANDS_H
#define COMMANDS_H
#include "file-process.h"
#include "init.h"
#include "functions.h"
#include "apis.h"
#include "variables.h"
#define fir first
#define sec second

extern const string WCH_WDName[7];
extern map <string, function <void ()> > WCH_command_support;
extern vector <string> WCH_command_list;
extern multimap <int, pair <int, string> > WCH_clock_list;
extern set <string> WCH_task_list;
extern set <string> WCH_work_list;
extern HWND WCH_hWnd;
extern int WCH_clock_num;
extern int WCH_task_num;
extern int WCH_work_num;
extern int WCH_ProgressBarCount;
extern int WCH_ProgressBarTot;
extern int WCH_InputTimes;
extern bool WCH_cmd_line;
extern bool WCH_anti_idle;
extern bool WCH_program_end;
extern bool WCH_wait_cmd;
extern string WCH_command;
extern string WCH_ProgressBarStr;
extern const string chg_fort[7] = {"", "大凶", "凶", "中平", "小吉", "中吉", "大吉"};
extern pair <string, string> chg_lucky[35], chg_unluc[35];
extern ifstream fin;
extern ofstream fout;
WCH_Time WCH_GetTime();
void WCH_Sleep(int _ms);
void WCH_Error(int _in);
void WCH_printlog(string _mode, string _info);
void WCH_read();
void WCH_save();
int WCH_GetNumDigits(int n);

void WCH_clear() {
	// Clear console information.
	if ((int)WCH_command_list.size() != 1) {
		WCH_Error(WCH_ERRNO_UNCORRECT);
		return;
	}
	system("cls");
}

void WCH_quit() {
	// Quit.
	if ((int)WCH_command_list.size() != 1) {
		WCH_Error(WCH_ERRNO_UNCORRECT);
		return;
	}
	WCH_program_end = true;
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
		DeleteFile(L"WCH_UPD.tmp");
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
		WCH_Sleep(15000);
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
		WCH_SetWindowSize(SW_MAXIMIZE, GetActiveWindow());
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
			DeleteFile(L"WCH_TRANS.tmp");
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
			DeleteFile(L"WCH_STDL.tmp");
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

void init_fate() {
    // Init the sentenses of the fate
    chg_lucky[0].fir = "万事皆凶";
    chg_lucky[0].sec = "";
    chg_lucky[1].fir = "写作文";
    chg_lucky[1].sec = "非常有文采";
    chg_lucky[2].fir = "搞基";
    chg_lucky[2].sec = "友谊地久天长";
    chg_lucky[3].fir = "扶老奶奶过马路";;
    chg_lucky[3].sec = "RP++";
    chg_lucky[4].fir = "开电脑";
    chg_lucky[4].sec = "电脑的状态也很好";
    chg_lucky[5].fir = "刷题";
    chg_lucky[5].sec = "成为虐题狂魔";
    chg_lucky[6].fir = "重构代码";
    chg_lucky[6].sec = "代码质量明显提高";
    chg_lucky[7].fir = "写作业";
    chg_lucky[7].sec = "都会写，写的全对";
    chg_lucky[8].fir = "参加模拟赛";
    chg_lucky[8].sec = "可以AK虐全场";
    chg_lucky[9].fir = "睡觉";
    chg_lucky[9].sec = "养足精力，明日再战";
    chg_lucky[10].fir = "打chunithm";
    chg_lucky[10].sec = "你虹了";
    chg_lucky[11].fir = "祭祀";
    chg_lucky[11].sec = "获得祖宗的庇护";
    chg_lucky[12].fir = "膜拜大神";
    chg_lucky[12].sec = "接受神犇光环照耀";
    chg_lucky[13].fir = "洗澡";
    chg_lucky[13].sec = "你多久没洗澡了？";
    chg_lucky[14].fir = "继续完成WA的题";
    chg_lucky[14].sec = "下一次就可以AC了";
    chg_lucky[15].fir = "熬夜";
    chg_lucky[15].sec = "事情终究是可以完成的";
    chg_lucky[16].fir = "泡妹子";
    chg_lucky[16].sec = "肯定能牵手！";
    chg_lucky[17].fir = "考试";
    chg_lucky[17].sec = "考的全会，蒙的全对";
    chg_lucky[18].fir = "背诵课文";
    chg_lucky[18].sec = "看一遍就背下来了";
    chg_lucky[19].fir = "体育锻炼";
    chg_lucky[19].sec = "身体棒棒哒";
    chg_lucky[20].fir = "吃饭";
    chg_lucky[20].sec = "人是铁饭是钢";
    chg_lucky[21].fir = "上课";
    chg_lucky[21].sec = "100%消化";
    chg_lucky[22].fir = "装弱";
    chg_lucky[22].sec = "谦虚最好了";
    chg_lucky[23].fir = "学习珂学";
    chg_lucky[23].sec = "珂朵莉太可爱了";
    chg_lucky[24].fir = "纳财";
    chg_lucky[24].sec = "要到好多Money";
    chg_lucky[25].fir = "上B站";
    chg_lucky[25].sec = "愉悦身心";
    chg_lucky[26].fir = "发朋友圈";
    chg_lucky[26].sec = "分享是种美德";
    chg_lucky[27].fir = "装逼";
    chg_lucky[27].sec = "获得众人敬仰";
    chg_lucky[28].fir = "玩网游";
    chg_lucky[28].sec = "犹如神助";
    chg_lucky[29].fir = "打sdvx";
    chg_lucky[29].sec = "您暴了";
    chg_lucky[30].fir = "出行";
    chg_lucky[30].sec = "一路顺风";
    chg_unluc[0].fir = "万事皆宜";
    chg_unluc[0].sec = "";
    chg_unluc[1].fir = "写作文";
    chg_unluc[1].sec = "可能会离题";
    chg_unluc[2].fir = "搞基";
    chg_unluc[2].sec = "会被掰弯";
    chg_unluc[3].fir = "扶老奶奶过马路";
    chg_unluc[3].sec = "会被讹";
    chg_unluc[4].fir = "开电脑";
    chg_unluc[4].sec = "意外的死机故障不可避免";
    chg_unluc[5].fir = "刷题";
    chg_unluc[5].sec = "容易WA";
    chg_unluc[6].fir = "重构代码";
    chg_unluc[6].sec = "越改越乱";
    chg_unluc[7].fir = "写作业";
    chg_unluc[7].sec = "上课讲了这些了吗";
    chg_unluc[8].fir = "参加模拟赛";
    chg_unluc[8].sec = "注意爆零";
    chg_unluc[9].fir = "睡觉";
    chg_unluc[9].sec = "翻来覆去睡不着";
    chg_unluc[10].fir = "打chunithm";
    chg_unluc[10].sec = "今天状态不好";
    chg_unluc[11].fir = "祭祀";
    chg_unluc[11].sec = "祖宗不知干啥就不鸟你";
    chg_unluc[12].fir = "膜拜大神";
    chg_unluc[12].sec = "被大神鄙视";
    chg_unluc[13].fir = "洗澡";
    chg_unluc[13].sec = "当心着凉";
    chg_unluc[14].fir = "继续完成WA的题";
    chg_unluc[14].sec = "然而变成了TLE";
    chg_unluc[15].fir = "熬夜";
    chg_unluc[15].sec = "爆肝";
    chg_unluc[16].fir = "泡妹子";
    chg_unluc[16].sec = "肯定能牵手！";
    chg_unluc[17].fir = "考试";
    chg_unluc[17].sec = "作弊会被抓";
    chg_unluc[18].fir = "背诵课文";
    chg_unluc[18].sec = "记忆力只有50Byte";
    chg_unluc[19].fir = "体育锻炼";
    chg_unluc[19].sec = "消耗的能量全吃回来了";
    chg_unluc[20].fir = "吃饭";
    chg_unluc[20].sec = "小心变胖啊";
    chg_unluc[21].fir = "上课";
    chg_unluc[21].sec = "反正你听不懂";
    chg_unluc[22].fir = "装弱";
    chg_unluc[22].sec = "被看穿";
    chg_unluc[23].fir = "学习珂学";
    chg_unluc[23].sec = "珂朵莉不知干啥不理你";
    chg_unluc[24].fir = "纳财";
    chg_unluc[24].sec = "然而今天并没有财运";
    chg_unluc[25].fir = "上B站";
    chg_unluc[25].sec = "会被教练发现";
    chg_unluc[26].fir = "发朋友圈";
    chg_unluc[26].sec = "会被当做卖面膜的";
    chg_unluc[27].fir = "装逼";
    chg_unluc[27].sec = "被识破";
    chg_unluc[28].fir = "玩网游";
    chg_unluc[28].sec = "匹配到一群猪队友";
    chg_unluc[29].fir = "打sdvx";
    chg_unluc[29].sec = "今天状态不好";
    chg_unluc[30].fir = "出行";
    chg_unluc[30].sec = "路途可能坎坷";
}

void WCH_calcu_fate() {
    // Get daily fate
    init_fate();
    int fate, lucky, unluc;
    fin.open("data/fate.dat");
    WCH_Time now, past;
    int fate_cnt = 0;
    now = WCH_GetTime();
    if (fin.is_open()) {
        fin >> past.Year >> past.Month >> past.Day >> fate_cnt;
    }
    if (!fin.is_open()||!(now.Year == past.Year && now.Month == past.Month && now.Day == past.Day)) {
        fin.close();
        fout.open("data/fate.dat");
        fout << now.Year << " " << now.Month << " " << now.Day << " " << ++fate_cnt << endl;
        fate = WCH_rand(1,6);
        fout << "               §" << chg_fort[fate] << "§" << endl;
        for (int i = 1; i <= 2; i++) {
            int rand_luc, rand_unl;
            if (fate == 1) rand_luc = (i == 1 ? 0 : 31);
            else rand_luc = WCH_rand(1, 30);
            if (fate == 6) rand_unl = (i == 1 ? 0 : 31);
            else rand_unl = WCH_rand(1, 30);
            fout << (fate == 1 ? "    " : "宜：" ) << chg_lucky[rand_luc].fir;
            fout << WCH_print_space(20 - (int)chg_lucky[rand_luc].fir.size()) << (fate == 6 ?"    " : " 凶：") << chg_unluc[rand_unl].fir << endl;
            fout << "    " << chg_lucky[rand_luc].sec;
            fout << WCH_print_space(20 - (int)chg_lucky[rand_luc].sec.size()) << "     " << chg_unluc[rand_unl].sec << endl;
        }
        fout.close();
    }
    fin.close();
    fin.open("data/fate.dat");
    string res;
    getline(fin, res);
    cout << WCH_print_space(11) << now.Year << " 年 " << now.Month << " 月 " << now.Day << " 日 " << endl;
    cout << WCH_print_space(9) << "你已经在 WCH 打卡了 " << fate_cnt << " 天" << endl;
    while (getline(fin, res)) {
        cout << res << endl;
    }
    fin.close();
}

#endif
