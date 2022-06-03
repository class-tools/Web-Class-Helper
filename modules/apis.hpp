/*
Web Class Helper APIs Module Header File 2.0.1
This source code file is under MIT License.
Copyright (c) 2022 Class Tools Develop Team
Contributors: jsh-jsh ren-yc
*/
#ifndef APIS_H
#define APIS_H
#include "file-process.hpp"
#include "init.hpp"
#include "commands.hpp"
#include "functions.hpp"
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

void WCH_Sleep(int _ms) {
	// Sleep.
	while (_ms > 0 && !WCH_program_end) {
		Sleep(100);
		_ms -= 100;
	}
}

void WCH_PrintColor(WORD _mode) {
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), _mode);
}

void WCH_PrintChar(int _times, char _c) {
	// Print space.
	while (_times > 0 && !WCH_program_end) {
		_times--;
		cout << _c;
	}
}

vector <string> WCH_split(const string &_in) {
	#ifdef _DEBUG
	WCH_printlog(WCH_LOG_STATUS_DEBUG, "Spliting string: \"" + _in + "\"");
	#endif
	vector <string> _res;
	string _tmp;
	stringstream _ss(_in);
	while (getline(_ss, _tmp, ' ')) {
		_res.push_back(_tmp);
	}
	#ifdef _DEBUG
	if ((int)_res.size() != 0) {
		if ((int)_res.size() != 1) {
			string _debug = "Splited result: \"" + _res[0] + "\", ";
			for (int i = 1; i < (int)_res.size() - 1; i++) {
				_debug += "\"" + _res[i] + "\", ";
			}
			_debug += "\"" + _res[(int)_res.size() - 1] + "\"";
			WCH_printlog(WCH_LOG_STATUS_DEBUG, _debug);
		} else {
			WCH_printlog(WCH_LOG_STATUS_DEBUG, "Splited result: \"" + _res[0] + "\"");
		}
	}
	#endif
	return _res;
}

wstring StrToWstr(string str) {
	wstring result;
	int len = MultiByteToWideChar(CP_ACP, 0, str.c_str(), (int)str.size(), NULL, 0);
	TCHAR *buffer = new TCHAR[len + 1];
	MultiByteToWideChar(CP_ACP, 0, str.c_str(), (int)str.size(), buffer, len);
	buffer[len] = '\0';
	result.append(buffer);
	delete[] buffer;
	return result;
}

string WstrToStr(wstring wstr) {
	string result;
	int len = WideCharToMultiByte(CP_ACP, 0, wstr.c_str(), (int)wstr.size(), NULL, 0, NULL, NULL);
	char* buffer = new char[len + 1];
	WideCharToMultiByte(CP_ACP, 0, wstr.c_str(), (int)wstr.size(), buffer, len, NULL, NULL);
	buffer[len] = '\0';
	result.append(buffer);
	delete[] buffer;
	return result;
}

void WCH_SetWindowStatus(bool flag) {
	// Set the window status by Windows API.
	ShowWindow(WCH_hWnd, flag);
	WCH_cmd_line = flag;
	WCH_printlog(WCH_LOG_STATUS_INFO, format("\"CONSOLE\" argument \"STATUS\" was set to {}", (flag == true ? "\"SHOW\"" : "\"HIDE\"")));
}

void WCH_SetTrayStatus(bool flag) {
	// Set the tray status by Windows API.
	ShowWindow(FindWindowW(L"Shell_trayWnd", NULL), (flag == true ? SW_SHOW : SW_HIDE));
	WCH_printlog(WCH_LOG_STATUS_INFO, format("\"TRAY\" argument \"STATUS\" was set to {}", (flag == true ? "\"SHOW\"" : "\"HIDE\"")));
}

void WCH_PutPicture() {
	// Press PrintScreen. (Keyboard)
	keybd_event(VK_SNAPSHOT, 0, 0, 0);
	keybd_event(VK_SNAPSHOT, 0, KEYEVENTF_KEYUP, 0);
	WCH_printlog(WCH_LOG_STATUS_INFO, "Copying screenshot to clipboard");
}

void WCH_SaveImg() {
	// Save screenshot to file.
	WCHAR cUserNameBuffer[256] = {0};
	DWORD dwUserNameSize = 256;
	GetUserNameW(cUserNameBuffer, &dwUserNameSize);
	WCH_Time now = WCH_GetTime();
	wstring SavePath = format(L"C:\\Users\\{}\\Pictures\\{:04}{:02}{:02}{:02}{:02}{:02}.jpg", cUserNameBuffer, now.Year, now.Month, now.Day, now.Hour, now.Minute, now.Second);
	HDC hdcScreen = ::GetDC(NULL);
	int oDpi = GetDpiForWindow(GetDesktopWindow());
	double dDpi = 0;
	switch (oDpi) {
		case 96:
			dDpi = 1;
			break;
		case 120:
			dDpi = 1.25;
			break;
		case 144:
			dDpi = 1.5;
			break;
		case 192:
			dDpi = 2;
			break;
		default:
			WCH_Error(WCH_ERROR_DPI_GET_FAILED);
			break;
	}
	int nBitPerPixel = GetDeviceCaps(hdcScreen, BITSPIXEL);
	int nWidth = (int)round(GetDeviceCaps(hdcScreen, HORZRES) * dDpi);
	int nHeight = (int)round(GetDeviceCaps(hdcScreen, VERTRES) * dDpi);
	HDC hMemDC;
	HBITMAP hBitmap, hOldBitmap;
	hMemDC = CreateCompatibleDC(hdcScreen);
	hBitmap = CreateCompatibleBitmap(hdcScreen, nWidth, nHeight);
	hOldBitmap = (HBITMAP)SelectObject(hMemDC, hBitmap);
	BitBlt(hMemDC, 0, 0, nWidth, nHeight, hdcScreen, 0, 0, SRCCOPY);
	GdiplusWrapper gdiplus;
	gdiplus.SaveImage(hBitmap, SavePath.c_str(), L"image/jpeg");
	DeleteDC(hdcScreen);
	DeleteDC(hMemDC);
	DeleteObject(hBitmap);
	WCH_printlog(WCH_LOG_STATUS_INFO, "Saving image to \"" + WstrToStr(SavePath) + "\"");
}

void WCH_CheckAndDeleteFile(wstring _filename) {
	// Delete a file if it exists.
	if (_waccess(_filename.c_str(), 0) != -1) {
		DeleteFileW(_filename.c_str());
	}
}

bool WCH_FileIsBlank(wstring _filename) {
	// Check if a file is blank.
	if (_waccess(_filename.c_str(), 0) != -1) {
		fin.open(_filename, ios::in);
		string _line;
		while (getline(fin, _line)) {
			if ((int)_line.size() != 0) {
				fin.close();
				return false;
			}
		}
		fin.close();
		return true;
	}
	return false;
}

bool WCH_TaskKill(string name) {
	// Kill a task by system command.
	system(("TASKKILL /F /IM " + name + " > WCH_SYSTEM_NORMAL.tmp 2> WCH_SYSTEM_ERROR.tmp").c_str());
	bool _res = (!WCH_FileIsBlank(L"WCH_SYSTEM_NORMAL.tmp") && WCH_FileIsBlank(L"WCH_SYSTEM_ERROR.tmp"));
	DeleteFileW(L"WCH_SYSTEM_NORMAL.tmp");
	DeleteFileW(L"WCH_SYSTEM_ERROR.tmp");
	return _res;
}

string UTF8ToANSI(string strUTF8) {
	UINT nLen = MultiByteToWideChar(CP_UTF8, 0, strUTF8.c_str(), -1, NULL, 0);
	WCHAR *wszBuffer = new WCHAR[nLen + 1];
	nLen = MultiByteToWideChar(CP_UTF8, 0, strUTF8.c_str(), -1, wszBuffer, nLen);
	wszBuffer[nLen] = 0;
	nLen = WideCharToMultiByte(936, 0, wszBuffer, -1, NULL, 0, NULL, 0);
	CHAR *szBuffer = new CHAR[nLen + 1];
	nLen = WideCharToMultiByte(936, 0, wszBuffer, -1, szBuffer, nLen, NULL, 0);
	szBuffer[nLen] = 0;
	strUTF8 = szBuffer;
	delete[] szBuffer;
	delete[] wszBuffer;
	return strUTF8;
}

bool WCH_CheckVersion(const WCH_Version _Fir, const WCH_Version _Sec) {
	if (_Fir.X < _Sec.X) {
		return true;
	} else if (_Fir.X > _Sec.X) {
		return false;
	} else {
		if (_Fir.Y < _Sec.Y) {
			return true;
		} else if (_Fir.Y > _Sec.Y) {
			return false;
		} else {
			if (_Fir.Z < _Sec.Z) {
				return true;
			} else {
				return false;
			}
		}
	}
}

WCH_Version WCH_GetVersion(string _in) {
	// Get version from string.
	WCH_Version _res;
	int _pos = (int)_in.find(".");
	if (_pos != string::npos) {
		_res.X = stoi(_in.substr(0, _pos));
		_in = _in.substr(_pos + 1);
	}
	_pos = (int)_in.find(".");
	if (_pos != string::npos) {
		_res.Y = stoi(_in.substr(0, _pos));
		_in = _in.substr(_pos + 1);
	}
	_res.Z = (int)stoi(_in);
	return _res;
}

int WCH_GetNumDigits(int _n) {
	// Get digits of a number.
	int _cnt = 1;
	while ((_n /= 10) != 0) {
		_cnt++;
	}
	return _cnt;
}

void WCH_PrintProgressBar(int _sur, int _n, bool _flag) {
	// Print a progress bar.
	string _ETAStr = format("{:02}:{:02}:{:02}", (int)(_sur / 3600), (int)((_sur % 3600) / 60), (int)(_sur % 60));
	if (_flag) {
		WCH_PrintChar(WCH_ProgressBarCount, '\b');
	}
	WCH_PrintColor(0x0A);
	for (int i = 0; i < _n / 2; i++) {
		cout << WCH_ProgressBarStr;
	}
	WCH_PrintColor(0x0C);
	for (int i = _n / 2; i < 50; i++) {
		cout << WCH_ProgressBarStr;
	}
	WCH_PrintColor(0x02);
	cout << " " << _n << "%";
	WCH_PrintColor(0x07);
	cout << " ETA ";
	WCH_PrintColor(0x09);
	cout << _ETAStr;
	WCH_PrintColor(0x07);
	WCH_ProgressBarCount = 65 + WCH_GetNumDigits(_n);
}

void WCH_ProgressBar() {
	// Progress bar.
	int _pro = 100 / WCH_ProgressBarTot;
	WCH_PrintProgressBar(WCH_ProgressBarTot, 0, false);
	for (int i = WCH_ProgressBarTot - 1; i > 0 && !WCH_program_end; i--) {
		WCH_Sleep(1000);
		WCH_PrintProgressBar(i, (WCH_ProgressBarTot - i) * _pro, true);
	}
	WCH_Sleep(1000);
	WCH_PrintProgressBar(0, 100, true);
	cout << endl;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
	// Window processing module.
	NOTIFYICONDATA nid {};
	switch (message) {
		case WM_CREATE:
			#ifdef _DEBUG
			WCH_printlog(WCH_LOG_STATUS_DEBUG, "Entering \"WndProc()\": \"WM_CREATE\"");
			#endif
			nid.cbSize = sizeof(nid);
			nid.hWnd = hWnd;
			nid.uID = 0;
			nid.uFlags = NIF_ICON | NIF_MESSAGE | NIF_TIP;
			nid.uCallbackMessage = WM_USER;
			nid.hIcon = (HICON)LoadImageW(NULL, L"WCH.ico", IMAGE_ICON, 0, 0, LR_LOADFROMFILE);
			wcscpy(nid.szTip, WCH_window_title.c_str());
			Shell_NotifyIconW(NIM_ADD, &nid);
			WCH_hMenu = CreatePopupMenu();
			AppendMenuW(WCH_hMenu, MF_STRING, WCH_IDM_SHOWHIDE, L"Show");
			AppendMenuW(WCH_hMenu, MF_SEPARATOR, 0, NULL);
			AppendMenuW(WCH_hMenu, MF_STRING, WCH_IDM_EXIT, L"Quit");
			break;
		case WM_USER:
			if (lParam == WM_LBUTTONDOWN) {
				#ifdef _DEBUG
				WCH_printlog(WCH_LOG_STATUS_DEBUG, "Entering \"WndProc()\": \"WM_USER\" & \"WM_LBUTTONDOWN\"");
				#endif
				WCH_SetWindowStatus(true);
			} else if (lParam == WM_RBUTTONDOWN) {
				POINT pt;
				int xx;
				GetCursorPos(&pt);
				SetForegroundWindow(hWnd);
				xx = TrackPopupMenu(WCH_hMenu, TPM_RETURNCMD, pt.x, pt.y, NULL, hWnd, NULL);
				#ifdef _DEBUG
				WCH_printlog(WCH_LOG_STATUS_DEBUG, "Entering \"WndProc()\": \"WM_USER\" & \"WM_RBUTTONDOWN\" & \"xx = " + to_string(xx) + "\"");
				#endif
				if (xx == IDR_SHOW) {
					#ifdef _DEBUG
					WCH_printlog(WCH_LOG_STATUS_DEBUG, "Entering \"WndProc()\": \"WM_USER\" & \"WM_RBUTTONDOWN\" & \"IDR_SHOW\"");
					#endif
					WCH_SetWindowStatus(true);
				} else if (xx == IDR_QUIT) {
					#ifdef _DEBUG
					WCH_printlog(WCH_LOG_STATUS_DEBUG, "Entering \"WndProc()\": \"WM_USER\" & \"WM_RBUTTONDOWN\" & \"IDR_QUIT\"");
					#endif
					WCH_command_list.clear();
					WCH_command_list.push_back("quit");
					cout << endl;
					exit(0);
				} else if (xx == 0) {
					PostMessageW(hWnd, WM_LBUTTONDOWN, NULL, NULL);
				}
			}
			break;
		case WM_DESTROY:
			#ifdef _DEBUG
			WCH_printlog(WCH_LOG_STATUS_DEBUG, "Entering \"WndProc()\": \"WM_DESTROY\"");
			#endif
			Shell_NotifyIconW(NIM_DELETE, &nid);
			PostQuitMessage(0);
			break;
		default:
			if (message == RegisterWindowMessageW(L"TaskbarCreated")) {
				SendMessageW(hWnd, WM_CREATE, wParam, lParam);
			}
			break;
	}
	return DefWindowProcW(hWnd, message, wParam, lParam);
}

void WCH_ShowBugMessagebox(int errorcode, wstring errormsg) {
	// Show messagebox to inform a bug to user.
	cout << "\a";
	if (MessageBoxW(NULL, (L"Oops! An error occurred.\nPlease inform our developers with the error message by open a new Issue in our GitHub Repository.\nError message: " + to_wstring(errorcode) + L" " + errormsg + L"\nWould you like to visit the Issues page now?").c_str(), L"WCH ERROR", MB_ICONERROR | MB_YESNO) == IDYES) {
		system("start resources/website/issues.url");
	}
}

void WCH_signalHandler() {
	// Signal handler.
	signal(SIGINT, [](int signum) {
		WCH_cmd_line = false;
		WCH_program_end = true;
		WCH_PrintColor(0x07);
		cout << endl;
		WCH_printlog(WCH_LOG_STATUS_ERROR, "Signal " + to_string(signum) + " detected (Program interrupted)");
		Sleep(500);
		WCH_SetWindowStatus(false);
		WCH_ShowBugMessagebox(signum, L"Program interrupted");
		exit(signum);
	});
	signal(SIGABRT, [](int signum) {
		WCH_cmd_line = false;
		WCH_program_end = true;
		WCH_PrintColor(0x07);
		cout << endl;
		WCH_printlog(WCH_LOG_STATUS_ERROR, "Signal " + to_string(signum) + " detected (Program aborted)");
		Sleep(500);
		WCH_SetWindowStatus(false);
		WCH_ShowBugMessagebox(signum, L"Program aborted");
		exit(signum);
	});
	signal(SIGFPE, [](int signum) {
		WCH_cmd_line = false;
		WCH_program_end = true;
		WCH_PrintColor(0x07);
		cout << endl;
		WCH_printlog(WCH_LOG_STATUS_ERROR, "Signal " + to_string(signum) + " detected (Operation overflow)");
		Sleep(500);
		WCH_SetWindowStatus(false);
		WCH_ShowBugMessagebox(signum, L"Operation overflow");
		exit(signum);
	});
	signal(SIGILL, [](int signum) {
		WCH_cmd_line = false;
		WCH_program_end = true;
		WCH_PrintColor(0x07);
		cout << endl;
		WCH_printlog(WCH_LOG_STATUS_ERROR, "Signal " + to_string(signum) + " detected (Illegal instruction)");
		Sleep(500);
		WCH_SetWindowStatus(false);
		WCH_ShowBugMessagebox(signum, L"Illegal instruction");
		exit(signum);
	});
	signal(SIGSEGV, [](int signum) {
		WCH_cmd_line = false;
		WCH_program_end = true;
		WCH_PrintColor(0x07);
		cout << endl;
		WCH_printlog(WCH_LOG_STATUS_ERROR, "Signal " + to_string(signum) + " detected (Access to illegal memory)");
		Sleep(500);
		WCH_SetWindowStatus(false);
		WCH_ShowBugMessagebox(signum, L"Access to illegal memory");
		exit(signum);
	});
}

#endif