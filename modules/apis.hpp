/*
Web Class Helper APIs Module Header File 2.1.1
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

extern const array<wstring, 7> WCH_weekday_list;
extern const array<wstring, 2> WCH_language_list;
extern const map<wstring, function<void()>> WCH_command_support;
extern const set<tuple<wstring, wstring, wstring>> WCH_settings_support;
extern const set<wstring> WCH_language_support;
extern vector<wstring> WCH_command_list;
extern set<tuple<int32_t, int32_t, wstring>> WCH_clock_list;
extern set<wstring> WCH_task_list;
extern set<pair<wstring, wstring>> WCH_work_list;
extern wstring WCH_window_title;
extern wstring WCH_command;
extern wstring WCH_ProgressBarStr;
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

void WCH_printlog(wstring _mode, wstring _info);

void WCH_Sleep(int32_t _ms) {
	// Sleep.
	while (_ms > 0 && !WCH_program_end) {
		sleep_for(milliseconds(100));
		_ms -= 100;
	}
}

void WCH_PrintColor(uint16_t _mode) {
	// Change console text color.
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), _mode);
}

void WCH_PrintChar(size_t _times, wchar_t _c) {
	// Print characters.
	while (_times > 0 && !WCH_program_end) {
		_times--;
		wcout << _c;
	}
}

wstring StrToWstr(const string& str) {
	// Convert multiple byte string to wide string.
	wstring result;
	int32_t len = MultiByteToWideChar(CP_UTF8, 0, str.c_str(), (int32_t)str.size(), NULL, 0);
	TCHAR* buffer = new TCHAR[(size_t)len + 1];
	MultiByteToWideChar(CP_UTF8, 0, str.c_str(), (int32_t)str.size(), buffer, len);
	buffer[len] = '\0';
	result.append(buffer);
	delete[] buffer;
	return result;
}

string WstrToStr(const wstring& wstr) {
	// Convert wide string to multiple byte string.
	string result;
	int32_t len = WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), (int32_t)wstr.size(), NULL, 0, NULL, NULL);
	char* buffer = new char[(size_t)len + 1];
	WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), (int32_t)wstr.size(), buffer, len, NULL, NULL);
	buffer[len] = '\0';
	result.append(buffer);
	delete[] buffer;
	return result;
}

string UrlEncode(const string& _in) {
	// Get URL encode result.
	string _res = "";
	for (size_t i = 0; i < _in.size(); i++) {
		if (isalnum((uint8_t)_in[i]) || _in[i] == '-' || _in[i] == '_' || _in[i] == '.' || _in[i] == '~') {
			_res += _in[i];
		} else if (_in[i] == ' ') {
			_res += "+";
		} else {
			char _tmp1 = (uint8_t)_in[i] >> 4;
			char _tmp2 = (uint8_t)_in[i] % 16;
			_res += '%';
			_res += _tmp1 > 9 ? _tmp1 + 55 : _tmp1 + 48;
			_res += _tmp2 > 9 ? _tmp2 + 55 : _tmp2 + 48;
		}
	}
	return _res;
}

vector<wstring> WCH_split(const wstring& _in) {
	// Split CLI string.
	vector<wstring> _res;
	wstring _tmp;
	bool _flag = false;
	for (size_t i = 0; i < _in.size(); i++) {
		if (_in[i] == L' ' && !_flag && i != 0) {
			if (_in[i - 1] != L'"') {
				_res.push_back(_tmp);
				_tmp = L"";
			}
		} else if (_in[i] == L'"') {
			if (_flag) {
				_res.push_back(_tmp);
				_tmp = L"";
				_flag = false;
			} else {
				_flag = true;
			}
		} else {
			_tmp.push_back(_in[i]);
		}
	}
	if (_tmp != L"") {
		_res.push_back(_tmp);
	}
	if (_flag || find(_res.begin(), _res.end(), L"") != _res.end()) {
		_res.clear();
		_res.push_back(L"Incorrect");
	}
	if (_res.size() != 0) {
		if (_res.size() != 1) {
			wstring _debug = L"Input command array: \"" + _res[0] + L"\", ";
			for (size_t i = 1; i < _res.size() - 1; i++) {
				_debug.append(L"\"" + _res[i] + L"\", ");
			}
			_debug.append(L"\"" + _res[_res.size() - 1] + L"\"");
			WCH_printlog(WCH_LOG_STATUS_INFO, _debug);
		} else {
			WCH_printlog(WCH_LOG_STATUS_INFO, L"Input command array: \"" + _res[0] + L"\"");
		}
	}
	return _res;
}

WCH_Time WCH_GetTime() {
	// Get current time and return a WCH_Time object.
	WCH_Time NowTime;
	time_t rawtime;
	struct tm ptminfo;
	time(&rawtime);
	localtime_s(&ptminfo, &rawtime);
	NowTime.Year = ptminfo.tm_year + 1900;
	NowTime.Month = ptminfo.tm_mon + 1;
	NowTime.Day = ptminfo.tm_mday;
	NowTime.Hour = ptminfo.tm_hour;
	NowTime.Minute = ptminfo.tm_min;
	NowTime.Second = ptminfo.tm_sec;
	return NowTime;
}

wstring WCH_GetCompileTime() {
	// Get program compile time.
	wstring spi = StrToWstr(__DATE__);
	map<wstring, int32_t> mon;
	mon[L"Jan"] = 1;
	mon[L"Feb"] = 2;
	mon[L"Mar"] = 3;
	mon[L"Apr"] = 4;
	mon[L"May"] = 5;
	mon[L"Jun"] = 6;
	mon[L"Jul"] = 7;
	mon[L"Aug"] = 8;
	mon[L"Sep"] = 9;
	mon[L"Oct"] = 10;
	mon[L"Nov"] = 11;
	mon[L"Dec"] = 12;
	return format(L"{}/{:02}/{:02} {}", spi.substr(7, 4), mon[spi.substr(0, 3)], stoi(spi[4] == L' ' ? spi.substr(5, 1) : spi.substr(4, 2)), StrToWstr(__TIME__));
}

wstring WCH_GetExecDir() {
	// Get program executable directory.
	wstring _res = _wpgmptr;
	size_t _pos = 0;
	for (size_t i = 0; i < _res.size(); i++) {
		if (_res[i] == L'\\') {
			_pos = i;
		}
	}
	return _res.substr(0, _pos);
}

wstring WCH_GetUniIdent() {
	// Get unique identification. (Public IP)
	wstring FilePath = format(L"{}\\AppData\\Local\\Temp\\WCH_IDENT.tmp", _wgetenv(L"USERPROFILE"));
	wstring _in, _res;
	URLDownloadToFileW(NULL, L"https://api.ipify.org", FilePath.c_str(), 0, NULL);
	wfin.open(FilePath);
	wfin >> _in;
	wfin.close();
	DeleteFileW(FilePath.c_str());
	for (size_t i = 0; i < _in.size(); i++) {
		if (_in[i] != L'.') {
			_res.push_back(_in[i]);
		}
	}
	return L"11111" + to_wstring(stoll(_res) % 99991);
}

size_t WCH_GetWstrDisplaySize(const wstring& _in) {
	// Get display length of wide string.
	size_t _size = 0;
	for (size_t i = 0; i < _in.size(); i++) {
		if (iswascii(_in[i])) {
			_size++;
		} else {
			_size += 2;
		}
	}
	return _size;
}

void WCH_SetWindowStatus(bool flag) {
	// Set the window status by Windows API.
	ShowWindow(WCH_window_handle, flag);
	WCH_cmd_line = flag;
	WCH_printlog(WCH_LOG_STATUS_INFO, format(L"\"CONSOLE\" argument \"STATUS\" was set to {}", (flag == true ? L"\"SHOW\"" : L"\"HIDE\"")));
}

void WCH_SetTrayStatus(bool flag) {
	// Set the tray status by Windows API.
	ShowWindow(FindWindowW(L"Shell_trayWnd", NULL), (flag == true ? SW_SHOW : SW_HIDE));
	WCH_printlog(WCH_LOG_STATUS_INFO, format(L"\"TRAY\" argument \"STATUS\" was set to {}", (flag == true ? L"\"SHOW\"" : L"\"HIDE\"")));
}

void WCH_ShowTaskBarError() {
	// Show error on task bar icon.
	WCH_TBL->SetProgressValue(WCH_window_handle, 100, 100);
	WCH_TBL->SetProgressState(WCH_window_handle, TBPF_ERROR);
	WCH_Sleep(1000);
	WCH_TBL->SetProgressState(WCH_window_handle, TBPF_NOPROGRESS);
}

void WCH_InputCommandIncorrect() {
	// Print text for incorrect inputs.
	WCH_printlog(WCH_LOG_STATUS_WARN, L"Your input code is incorrect, please check and try again");
	wcout << StrToWstr(WCH_Language["InputCommandIncorrect"].asString()) << endl;
	thread T(WCH_ShowTaskBarError);
	T.detach();
}

void WCH_FileProcessingFailed() {
	// Print text for failed file processings.
	WCH_printlog(WCH_LOG_STATUS_ERROR, L"File processing failed. Please try reinstalling this program");
	wcout << StrToWstr(WCH_Language["FileProcessingFailed"].asString()) << endl;
	thread T(WCH_ShowTaskBarError);
	T.detach();
}

void WCH_NetworkError() {
	// Print text for network errors.
	WCH_printlog(WCH_LOG_STATUS_ERROR, L"An network error occurred, please check your network connection and try to update this program");
	wcout << StrToWstr(WCH_Language["NetworkError"].asString()) << endl;
	thread T(WCH_ShowTaskBarError);
	T.detach();
}

void WCH_CheckHotkey() {
	// Check when hot key is pressed.
	if (!WCH_program_end) {
		if (WCH_anti_idle) {
			if (StrToWstr(WCH_Settings["AntiIdleEndPrompt"].asString()) == L"True") {
				if (MessageBoxW(NULL, StrToWstr(WCH_Settings["AntiIdleEndContent"].asString()).c_str(), L"WCH WARN", MB_ICONWARNING | MB_YESNO | MB_TOPMOST) == IDNO) {
					return;
				}
			}
			WCH_anti_idle = false;
			WCH_SetTrayStatus(true);
		}
		if (WCH_count_down) {
			WCH_count_down = false;
		}
		WCH_SetWindowStatus(true);
	}
}

void WCH_PutPicture() {
	// Press PrintScreen. (Keyboard)
	keybd_event(VK_SNAPSHOT, 0, 0, 0);
	keybd_event(VK_SNAPSHOT, 0, KEYEVENTF_KEYUP, 0);
	WCH_printlog(WCH_LOG_STATUS_INFO, L"Copying screenshot to clipboard");
}

void WCH_SaveImg() {
	// Save screenshot to file.
	WCH_Time now = WCH_GetTime();
	wstring SavePathDir = StrToWstr(WCH_Settings["ScreenshotSavePath"].asString());
	wstring SavePath = format(L"{}{:04}{:02}{:02}{:02}{:02}{:02}.jpg", SavePathDir, now.Year, now.Month, now.Day, now.Hour, now.Minute, now.Second);
	if (_waccess(SavePathDir.c_str(), 0) != 0) {
		CreateDirectoryW(SavePathDir.c_str(), NULL);
	}
	HDC hdcScreen = ::GetDC(NULL);
	double dDpi = (double)GetDeviceCaps(GetDC(GetDesktopWindow()), DESKTOPHORZRES) / GetSystemMetrics(SM_CXSCREEN);
	int32_t nWidth = (int32_t)round(GetDeviceCaps(hdcScreen, HORZRES) * dDpi);
	int32_t nHeight = (int32_t)round(GetDeviceCaps(hdcScreen, VERTRES) * dDpi);
	HDC hMemDC;
	HBITMAP hBitmap, hOldBitmap;
	hMemDC = CreateCompatibleDC(hdcScreen);
	hBitmap = CreateCompatibleBitmap(hdcScreen, nWidth, nHeight);
	hOldBitmap = (HBITMAP)SelectObject(hMemDC, hBitmap);
	BitBlt(hMemDC, 0, 0, nWidth, nHeight, hdcScreen, 0, 0, SRCCOPY);
	GdiplusWrapper gdiplus {};
	gdiplus.SaveImage(hBitmap, SavePath.c_str(), L"image/jpeg");
	DeleteDC(hdcScreen);
	DeleteDC(hMemDC);
	DeleteObject(hBitmap);
	WCH_printlog(WCH_LOG_STATUS_INFO, L"Saving image to \"" + SavePath + L"\"");
}

void WCH_CheckAndDeleteFile(const wstring& _filename) {
	// Delete a file if it exists.
	if (_waccess(_filename.c_str(), 0) != -1) {
		DeleteFileW(_filename.c_str());
	}
}

bool WCH_FileIsBlank(const wstring& _filename) {
	// Check if a file is blank.
	if (_waccess(_filename.c_str(), 0) != -1) {
		wfin.open(_filename, ios::in);
		wstring _line;
		while (getline(wfin, _line)) {
			if (_line.size() != 0) {
				wfin.close();
				return false;
			}
		}
		wfin.close();
	}
	return true;
}

bool WCH_TaskKill(const wstring& name) {
	// Kill a task by system command.
	wstring FilePathNormal = format(L"{}\\AppData\\Local\\Temp\\WCH_SYSTEM_NORMAL.tmp", _wgetenv(L"USERPROFILE"));
	wstring FilePathError = format(L"{}\\AppData\\Local\\Temp\\WCH_SYSTEM_ERROR.tmp", _wgetenv(L"USERPROFILE"));
	_wsystem(format(L"TASKKILL /F /IM {} > \"{}\" 2> \"{}\"", name, FilePathNormal, FilePathError).c_str());
	bool _res = (!WCH_FileIsBlank(L"WCH_SYSTEM_NORMAL.tmp") && WCH_FileIsBlank(L"WCH_SYSTEM_ERROR.tmp"));
	DeleteFileW(FilePathNormal.c_str());
	DeleteFileW(FilePathError.c_str());
	return _res;
}

WCH_Version WCH_GetVersion(wstring _in) {
	// Get version from string.
	WCH_Version _res;
	size_t _pos = _in.find(L".");
	if (_pos != wstring::npos) {
		_res.X = stoi(_in.substr(0, _pos));
		_in = _in.substr(_pos + 1);
	}
	_pos = _in.find(L".");
	if (_pos != wstring::npos) {
		_res.Y = stoi(_in.substr(0, _pos));
		_in = _in.substr(_pos + 1);
	}
	_res.Z = stoi(_in);
	return _res;
}

size_t WCH_GetNumDigits(size_t _n) {
	// Get digits of a number.
	size_t _cnt = 1;
	while ((_n /= 10) != 0) {
		_cnt++;
	}
	return _cnt;
}

pair<bool, wstring> WCH_CheckConfigValid(const wstring& Key, const wstring& Value) {
	// Check if the settings key and the value of it match.
	wstring KeyType = L"String";
	bool flag = false;
	if (Value == L"True" || Value == L"False") {
		KeyType = L"Boolean";
	}
	if (Value.size() < 10) {
		for (size_t i = 0; i < Value.size(); i++) {
			if (!iswdigit(Value[i])) {
				break;
			}
			if (Value.size() == i + 1) {
				KeyType = L"Number";
			}
		}
	}
	for (auto it = WCH_settings_support.begin(); it != WCH_settings_support.end(); it++) {
		if (Key == get<0>(*it)) {
			if (KeyType != get<1>(*it)) {
				return make_pair(false, KeyType);
			} else {
				flag = true;
				break;
			}
		}
	}
	if (!flag) {
		return make_pair(false, KeyType);
	}
	if ((Key == L"ScreenshotSavePath" && Value[Value.size() - 1] != L'\\') || Key == L"Language" && find(WCH_language_list.begin(), WCH_language_list.end(), Value) == WCH_language_list.end()) {
		return make_pair(false, KeyType);
	}
	return make_pair(true, KeyType);
}

void WCH_PrintProgressBar(int32_t _sur, int32_t _n, bool _flag) {
	// Print a progress bar.
	wstring _ETAStr = format(L"{:02}:{:02}:{:02}", (int32_t)(_sur / 3600), (int32_t)((_sur % 3600) / 60), (int32_t)(_sur % 60));
	if (_flag) {
		wcout << "\r";
	}
	WCH_PrintColor(0x0A);
	for (int32_t i = 0; i < _n / 2; i++) {
		wcout << WCH_ProgressBarStr;
	}
	WCH_PrintColor(0x0C);
	for (int32_t i = _n / 2; i < 50; i++) {
		wcout << WCH_ProgressBarStr;
	}
	WCH_PrintColor(0x02);
	wcout << L" " << _n << L"%";
	WCH_PrintColor(0x07);
	wcout << L" ETA ";
	WCH_PrintColor(0x09);
	wcout << _ETAStr;
	WCH_PrintColor(0x07);
}

void WCH_ProgressBar() {
	// Progress bar.
	bool _cd = WCH_count_down;
	double _pro = 100.0 / WCH_ProgressBarTot;
	WCH_PrintProgressBar(WCH_ProgressBarTot, 0, false);
	WCH_TBL->SetProgressState(WCH_window_handle, TBPF_NORMAL);
	WCH_TBL->SetProgressValue(WCH_window_handle, 0, 100);
	for (int32_t i = WCH_ProgressBarTot - 1; i > 0 && !WCH_program_end && !(_cd ^ WCH_count_down); i--) {
		WCH_Sleep(1000);
		WCH_PrintProgressBar(i, (int32_t)((WCH_ProgressBarTot - i) * _pro), true);
		WCH_TBL->SetProgressValue(WCH_window_handle, (uint64_t)((WCH_ProgressBarTot - i) * _pro), 100);
	}
	WCH_Sleep(1000);
	WCH_PrintProgressBar(0, 100, true);
	WCH_TBL->SetProgressState(WCH_window_handle, TBPF_NOPROGRESS);
	wcout << endl;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
	// Window processing module.
	switch (message) {
		case WM_HOTKEY:
#ifdef _DEBUG
			WCH_printlog(WCH_LOG_STATUS_DEBUG, L"Entering \"WndProc()\": \"WM_HOTKEY\" & \"wParam = " + to_wstring(wParam) + L"\" & \"lParam = " + to_wstring(lParam) + L"\"");
#endif
			if (wParam == WCH_HOTKEY_SHOW) {
				WCH_CheckHotkey();
			}
			break;
		case WM_CREATE:
#ifdef _DEBUG
			WCH_printlog(WCH_LOG_STATUS_DEBUG, L"Entering \"WndProc()\": \"WM_CREATE\"");
#endif
			WCH_NID.cbSize = sizeof(WCH_NID);
			WCH_NID.hWnd = hWnd;
			WCH_NID.uID = 0;
			WCH_NID.uFlags = NIF_ICON | NIF_MESSAGE | NIF_TIP;
			WCH_NID.uCallbackMessage = WM_USER;
			WCH_NID.hIcon = (HICON)LoadImageW(NULL, (WCH_GetExecDir() + L"\\WCH.ico").c_str(), IMAGE_ICON, 0, 0, LR_LOADFROMFILE);
			wcscpy(WCH_NID.szTip, WCH_window_title.c_str());
			Shell_NotifyIconW(NIM_ADD, &WCH_NID);
			WCH_menu_handle = CreatePopupMenu();
			AppendMenuW(WCH_menu_handle, MF_STRING, WCH_MENU_SHOW, L"Ctrl + Down");
			AppendMenuW(WCH_menu_handle, MF_SEPARATOR, 0, NULL);
			AppendMenuW(WCH_menu_handle, MF_STRING, WCH_MENU_EXIT, L"Exit");
			break;
		case WM_USER:
			if (lParam == WM_LBUTTONDOWN) {
#ifdef _DEBUG
				WCH_printlog(WCH_LOG_STATUS_DEBUG, L"Entering \"WndProc()\": \"WM_USER\" & \"WM_LBUTTONDOWN\"");
#endif
				WCH_CheckHotkey();
			} else if (lParam == WM_RBUTTONDOWN) {
				POINT pt;
				int32_t xx;
				GetCursorPos(&pt);
				SetForegroundWindow(hWnd);
				xx = TrackPopupMenu(WCH_menu_handle, TPM_RETURNCMD, pt.x, pt.y, NULL, hWnd, NULL);
#ifdef _DEBUG
				WCH_printlog(WCH_LOG_STATUS_DEBUG, L"Entering \"WndProc()\": \"WM_USER\" & \"WM_RBUTTONDOWN\" & \"xx = " + to_wstring(xx) + L"\"");
#endif
				if (xx == WCH_MENU_SHOW) {
#ifdef _DEBUG
					WCH_printlog(WCH_LOG_STATUS_DEBUG, L"Entering \"WndProc()\": \"WM_USER\" & \"WM_RBUTTONDOWN\" & \"WCH_MENU_SHOW\"");
#endif
					WCH_CheckHotkey();
				} else if (xx == WCH_MENU_EXIT) {
#ifdef _DEBUG
					WCH_printlog(WCH_LOG_STATUS_DEBUG, L"Entering \"WndProc()\": \"WM_USER\" & \"WM_RBUTTONDOWN\" & \"WCH_MENU_EXIT\"");
#endif
					raise(SIGBREAK);
				} else if (xx == 0) {
					PostMessageW(hWnd, WM_LBUTTONDOWN, NULL, NULL);
				}
			}
			break;
		case WM_DESTROY:
#ifdef _DEBUG
			WCH_printlog(WCH_LOG_STATUS_DEBUG, L"Entering \"WndProc()\": \"WM_DESTROY\"");
#endif
			Shell_NotifyIconW(NIM_DELETE, &WCH_NID);
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

void WCH_ShowBugMessagebox(int32_t errorcode, const wstring& errormsg) {
	// Show messagebox to inform a bug to user.
	wcout << L"\a";
	WCH_TBL->SetProgressState(WCH_window_handle, TBPF_INDETERMINATE);
	if (MessageBoxW(NULL, (L"Oops! An error occurred.\nPlease inform our developers with the error message by opening a new Issue in our GitHub Repository.\nError message: " + to_wstring(errorcode) + L" " + errormsg + L"\nWould you like to visit the Issues page now?").c_str(), L"WCH ERROR", MB_ICONERROR | MB_YESNO) == IDYES) {
		_wsystem(L"START https://github.com/class-tools/Web-Class-Helper/issues/");
	}
	WCH_TBL->SetProgressState(WCH_window_handle, TBPF_NOPROGRESS);
}

void WCH_signalHandler() {
	// Signal handler.
	signal(SIGINT, []([[maybe_unused]] int32_t signum) {
		WCH_command_list.clear();
		WCH_command_list.push_back(L"exit");
		wcout << endl;
		exit(0);
	});
	signal(SIGBREAK, []([[maybe_unused]] int32_t signum) {
		WCH_command_list.clear();
		WCH_command_list.push_back(L"exit");
		wcout << endl;
		exit(0);
	});
	signal(SIGABRT, [](int32_t signum) {
		WCH_cmd_line = false;
		WCH_program_end = true;
		WCH_PrintColor(0x07);
		wcout << endl;
		WCH_printlog(WCH_LOG_STATUS_ERROR, L"Signal " + to_wstring(signum) + L" detected (Program aborted)");
		Sleep(500);
		WCH_SetWindowStatus(false);
		WCH_ShowBugMessagebox(signum, L"Program aborted");
		exit(signum);
	});
	signal(SIGFPE, [](int32_t signum) {
		WCH_cmd_line = false;
		WCH_program_end = true;
		WCH_PrintColor(0x07);
		wcout << endl;
		WCH_printlog(WCH_LOG_STATUS_ERROR, L"Signal " + to_wstring(signum) + L" detected (Operation overflow)");
		Sleep(500);
		WCH_SetWindowStatus(false);
		WCH_ShowBugMessagebox(signum, L"Operation overflow");
		exit(signum);
	});
	signal(SIGILL, [](int32_t signum) {
		WCH_cmd_line = false;
		WCH_program_end = true;
		WCH_PrintColor(0x07);
		wcout << endl;
		WCH_printlog(WCH_LOG_STATUS_ERROR, L"Signal " + to_wstring(signum) + L" detected (Illegal instruction)");
		Sleep(500);
		WCH_SetWindowStatus(false);
		WCH_ShowBugMessagebox(signum, L"Illegal instruction");
		exit(signum);
	});
	signal(SIGSEGV, [](int32_t signum) {
		WCH_cmd_line = false;
		WCH_program_end = true;
		WCH_PrintColor(0x07);
		wcout << endl;
		WCH_printlog(WCH_LOG_STATUS_ERROR, L"Signal " + to_wstring(signum) + L" detected (Access to illegal memory)");
		Sleep(500);
		WCH_SetWindowStatus(false);
		WCH_ShowBugMessagebox(signum, L"Access to illegal memory");
		exit(signum);
	});
}

#endif