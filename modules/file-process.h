/*
Web Class Helper File Process Module Header File 2.0.1
This source code file is under MIT License.
Copyright (c) 2022 Class Tools Develop Team
Contributors: jsh-jsh ren-yc
*/
#ifndef FILE_PROCESS_H
#define FILE_PROCESS_H
#include "init.h"
#include "commands.h"
#include "functions.h"
#include "apis.h"
#include "basic.h"

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

void WCH_printlog(string _mode, string _info) {
	// Print log.
	WCH_Time now = WCH_GetTime();
	fout.open("logs/latest.log", ios::app);
	fout << format("[{:02}:{:02}:{:02}] {}: {}.", now.Hour, now.Minute, now.Second, _mode, _info) << endl;
	fout.close();
}

void WCH_read_clock() {
	// Read clock data.
	WCH_Time q = WCH_GetTime();
	string NowWeekDay = WCH_WDName[(q.Day + 2 * q.Month + 3 * (q.Month + 1) / 5 + q.Year + q.Year / 4 - q.Year / 100 + q.Year / 400 + 1) % 7];
	string FilePath = "data/" + NowWeekDay + ".dat";
	fin.open(FilePath);
	if (!fin.is_open()) {
		return;
	}
	WCH_printlog(WCH_LOG_STATUS_INFO, "Reading file \"" + FilePath + "\"");
	fin >> WCH_clock_num;
	fin.ignore();
	for (int i = 1; i <= WCH_clock_num; i++) {
		string tmp1;
		getline(fin, tmp1);
		vector <string> tmp2 = WCH_split(tmp1);
		int H = stoi(tmp2[0]);
		int M = stoi(tmp2[1]);
		string Tname = tmp1.substr((int)tmp2[0].size() + (int)tmp2[1].size() + 2, tmp1.size() - 1);
		WCH_clock_list.emplace(make_pair(H, make_pair(M, Tname)));
	}
	fin.close();
}

void WCH_read_task() {
	// Read task data.
	string FilePath = "data/task.dat";
	fin.open(FilePath);
	if (!fin.is_open()) {
		return;
	}
	WCH_printlog(WCH_LOG_STATUS_INFO, "Reading file \"" + FilePath + "\"");
	fin >> WCH_task_num;
	fin.ignore();
	for (int i = 1; i <= WCH_task_num; i++) {
		string TaskName;
		getline(fin, TaskName);
		WCH_task_list.insert(TaskName);
	}
	fin.close();
}

void WCH_read_work() {
	// Read work data.
	string FilePath = "data/work.dat";
	fin.open(FilePath);
	if (!fin.is_open()) {
		return;
	}
	WCH_printlog(WCH_LOG_STATUS_INFO, "Reading file \"" + FilePath + "\"");
	fin >> WCH_work_num;
	fin.ignore();
	for (int i = 1; i <= WCH_work_num; i++) {
		string WorkName;
		getline(fin, WorkName);
		WCH_work_list.insert(WorkName);
	}
	fin.close();
}

void WCH_read() {
	// Read data.
	WCH_cmd_line = false;
	cout << "Reading data..." << endl;
	WCH_read_clock();
	WCH_read_task();
	WCH_read_work();
	WCH_ProgressBarTot = 3;
	thread T(WCH_ProgressBar);
	T.detach();
	WCH_Sleep(4000);
	WCH_cmd_line = true;
	system("cls");
}

void WCH_save_clock() {
	// Save clock data.
	WCH_Time q = WCH_GetTime();
	string NowWeekDay = WCH_WDName[(q.Day + 2 * q.Month + 3 * (q.Month + 1) / 5 + q.Year + q.Year / 4 - q.Year / 100 + q.Year / 400 + 1) % 7];
	string FilePath = "data/" + NowWeekDay + ".dat";
	if (WCH_clock_num == 0) {
		if (_access(FilePath.c_str(), 0) != -1) {
			WCH_printlog(WCH_LOG_STATUS_INFO, "Deleting file \"" + FilePath + "\"");
			DeleteFileW(StrToWstr(FilePath).c_str());
		}
		return;
	}
	WCH_printlog(WCH_LOG_STATUS_INFO, "Writing file \"" + FilePath + "\"");
	fout.open(FilePath);
	fout << WCH_clock_num << endl;
	for (int i = 0; i <= 24; i++) {
		for (auto it = WCH_clock_list.equal_range(i).first; it != WCH_clock_list.equal_range(i).second; it++) {
			fout << i << " " << (it -> second).first << " " << (it -> second).second << endl;
		}
	}
	fout.close();
}

void WCH_save_task() {
	// Save task list data.
	string FilePath = "data/task.dat";
	if (WCH_task_num == 0) {
		if (_access(FilePath.c_str(), 0) != -1) {
			WCH_printlog(WCH_LOG_STATUS_INFO, "Deleting file \"" + FilePath + "\"");
			DeleteFileW(StrToWstr(FilePath).c_str());
		}
		return;
	}
	WCH_printlog(WCH_LOG_STATUS_INFO, "Writing file \"" + FilePath + "\"");
	fout.open(FilePath);
	fout << WCH_task_num << endl;
	for (auto it = WCH_task_list.begin(); it != WCH_task_list.end(); it++) {
		fout << *it << endl;
	}
	fout.close();
}

void WCH_save_work() {
	// Save task list data.
	string FilePath = "data/work.dat";
	if (WCH_work_num == 0) {
		if (_access(FilePath.c_str(), 0) != -1) {
			WCH_printlog(WCH_LOG_STATUS_INFO, "Deleting file \"" + FilePath + "\"");
			DeleteFileW(StrToWstr(FilePath).c_str());
		}
		return;
	}
	WCH_printlog(WCH_LOG_STATUS_INFO, "Writing file \"" + FilePath + "\"");
	fout.open(FilePath);
	fout << WCH_work_num << endl;
	for (auto it = WCH_work_list.begin(); it != WCH_work_list.end(); it++) {
		fout << *it << endl;
	}
	fout.close();
}

bool WCH_save_func() {
	// Save data. (Function)
	bool NeedSave = (WCH_clock_change != 0 || WCH_task_change != 0 || WCH_work_change != 0);
	if (NeedSave) {
		cout << "Saving data..." << endl;
		WCH_ProgressBarTot = WCH_clock_change + WCH_task_change + WCH_work_change;
	}
	WCH_save_clock();
	WCH_save_task();
	WCH_save_work();
	if (NeedSave) {
		thread T(WCH_ProgressBar);
		T.detach();
		WCH_Sleep(WCH_ProgressBarTot * 1000);
		WCH_Sleep(1000);
	}
	return NeedSave;
}

#endif