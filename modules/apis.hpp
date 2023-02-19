/*
Web Class Helper APIs Module Header File 2.1.2
This source code file is under MIT License.
Copyright (c) 2022 - 2023 Class Tools Develop Team
Contributors: jsh-jsh ren-yc
*/
#ifndef APIS_H
#define APIS_H
#include "file-process.hpp"
#include "init.hpp"
#include "commands.hpp"
#include "functions.hpp"
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

size_t WCH_NewlineCount(const wstring& _in) {
	// Count newline characters in a wide string.
	size_t _cnt = 0;
	for (size_t i = 0; i < _in.size(); i++) {
		if (_in[i] == L'\n') {
			_cnt++;
		}
	}
	return _cnt;
}

void WCH_ClearConsole() {
	// Clear console.
	CONSOLE_SCREEN_BUFFER_INFO _csbi = {};
	COORD _crd = {0, 0};
	DWORD _nocw = 0;
	GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &_csbi);
	FillConsoleOutputCharacterW(GetStdHandle(STD_OUTPUT_HANDLE), L' ', _csbi.dwSize.X * _csbi.dwSize.Y, _crd, &_nocw);
	GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &_csbi);
	FillConsoleOutputAttribute(GetStdHandle(STD_OUTPUT_HANDLE), _csbi.wAttributes, _csbi.dwSize.X * _csbi.dwSize.Y, _crd, &_nocw);
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), _crd);
}

wstring StrToWstr(const string& str) {
	// Convert multiple byte string to wide string.
	wstring result;
	int32_t len = MultiByteToWideChar(CP_UTF8, 0, str.c_str(), (int32_t)str.size(), NULL, 0);
	wchar_t* buffer = new wchar_t[(size_t)len + 1];
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

vector<wstring> WCH_split(const wstring& _input) {
	// Split CLI string.
	vector<wstring> _res;
	wstring _tmp, _in;
	bool _flag = false;
	wchar_t last_ch;
	for (auto cur : _input) {
		if (cur == '"') {
			_flag = true;
			_in.push_back(cur);
		} else if (_flag) {
			if (cur == '"') _flag = false;
			_in.push_back(cur);
		} else if (cur != ' ') {
			_in.push_back(cur);
		} else {
			if (_in.empty()) {
				last_ch = ' ';
			} else {
				last_ch = _in.back();
			}
			if (last_ch != ' ') _in.push_back(cur);
		}
	}
	_flag = false;
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
			wstring _debug = format(L"Input command array: \"{}\", ", _res[0]);
			for (size_t i = 1; i < _res.size() - 1; i++) {
				_debug.append(format(L"\"{}\", ", _res[i]));
			}
			_debug.append(format(L"\"{}\"", _res[_res.size() - 1]));
			SPDLOG_INFO(_debug);
		} else {
			SPDLOG_INFO(format(L"Input command array: \"{}\"", _res[0]));
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
	wstring FilePath = WCH_path_temp + L"\\WCH_IDENT.tmp";
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

tuple<uint32_t, uint32_t, uint32_t> WCH_GetSystemVersion() {
	// Get system version.
	uint32_t _version = 0;
#pragma warning(suppress : 4996 28159)
	_version = GetVersion();
	return make_tuple((uint32_t)(LOBYTE(LOWORD(_version))), (uint32_t)(HIBYTE(LOWORD(_version))), (uint32_t)(HIWORD(_version)));
}

wstring WCH_GetSystemArchitecture() {
	// Get system architecture.
	SYSTEM_INFO _sysinfo = {};
	GetNativeSystemInfo(&_sysinfo);
	switch (_sysinfo.wProcessorArchitecture) {
		case PROCESSOR_ARCHITECTURE_INTEL:
			return L"x86";
		case PROCESSOR_ARCHITECTURE_AMD64:
			return L"x64";
		case PROCESSOR_ARCHITECTURE_ARM:
			return L"ARM";
		case PROCESSOR_ARCHITECTURE_ARM64:
			return L"ARM64";
		case PROCESSOR_ARCHITECTURE_IA64:
			return L"IA64";
		case PROCESSOR_ARCHITECTURE_UNKNOWN:
			return L"Unknown";
		default:
			return L"Unknown";
	}
}

bool WCH_GetPowerShellCompatibility() {
	// Get compatibility of PowerShell.
	wstring FilePath = WCH_path_temp + L"\\WCH_PWSH.tmp";
	wstring _res = L"";
	_wsystem((L"PWSH -? > NUL 2> NUL && ECHO %ERRORLEVEL% > \"" + FilePath + L"\"").c_str());
	wfin.open(FilePath);
	wfin >> _res;
	wfin.close();
	DeleteFileW(FilePath.c_str());
	return _res == L"0";
}

wstring WCH_GetFileHash(wstring _in) {
	// Get SHA512 hash of file.
	wstring FilePath = WCH_path_temp + L"\\WCH_HASH.tmp";
	wstring _res = L"";
	_wsystem(((WCH_GetPowerShellCompatibility() ? L"PWSH" : L"POWERSHELL") + (L" -COMMAND \"(Get-FileHash \\\"" + _in) + L"\\\" -Algorithm SHA512).Hash | Out-File \"" + FilePath + L"\" -Encoding ASCII").c_str());
	wfin.open(FilePath);
	wfin >> _res;
	wfin.close();
	DeleteFileW(FilePath.c_str());
	return _res;
}

void WCH_SetWindowStatus(bool flag) {
	// Set the window status by Windows API.
	ShowWindow(WCH_handle_window, (flag ? SW_SHOWNORMAL : SW_HIDE));
	WCH_cmd_line = flag;
	SPDLOG_INFO(format(L"\"CONSOLE\" argument \"STATUS\" was set to {}", (flag ? L"\"SHOW\"" : L"\"HIDE\"")));
}

void WCH_SetTrayStatus(bool flag) {
	// Set the tray status by Windows API.
	ShowWindow(FindWindowW(L"Shell_trayWnd", NULL), (flag ? SW_SHOWNORMAL : SW_HIDE));
	SPDLOG_INFO(format(L"\"TRAY\" argument \"STATUS\" was set to {}", (flag ? L"\"SHOW\"" : L"\"HIDE\"")));
}

void WCH_ShowTaskBarError() {
	// Show error on task bar icon.
	WCH_TBL->SetProgressValue(WCH_handle_window, 100, 100);
	WCH_TBL->SetProgressState(WCH_handle_window, TBPF_ERROR);
	WCH_Sleep(1000);
	WCH_TBL->SetProgressState(WCH_handle_window, TBPF_NOPROGRESS);
}

void WCH_InputCommandIncorrect() {
	// Print text for incorrect inputs.
	SPDLOG_WARN(L"Your input code is incorrect, please check and try again");
	wcout << StrToWstr(WCH_Language["InputCommandIncorrect"].asString()) << endl;
	thread T(WCH_ShowTaskBarError);
	T.detach();
}

void WCH_FileProcessingFailed() {
	// Print text for failed file processings.
	SPDLOG_ERROR(L"File processing failed. Please try reinstalling this program");
	wcout << StrToWstr(WCH_Language["FileProcessingFailed"].asString()) << endl;
	thread T(WCH_ShowTaskBarError);
	T.detach();
}

void WCH_NetworkError() {
	// Print text for network errors.
	SPDLOG_ERROR(L"An network error occurred, please check your network connection and try to update this program");
	wcout << StrToWstr(WCH_Language["NetworkError"].asString()) << endl;
	thread T(WCH_ShowTaskBarError);
	T.detach();
}

void WCH_CheckHotkey() {
	// Check when hot key is pressed.
	if (!WCH_program_end) {
		if (WCH_is_focus) {
			if (StrToWstr(WCH_Settings["FocusEndPrompt"].asString()) == L"True") {
				if (MessageBoxW(NULL, StrToWstr(WCH_Settings["FocusEndContent"].asString()).c_str(), L"WCH WARN", MB_ICONWARNING | MB_YESNO | MB_TOPMOST) == IDNO) {
					return;
				}
			}
			WCH_is_focus = false;
			WCH_SetTrayStatus(true);
		}
		if (WCH_is_countdown) {
			WCH_is_countdown = false;
		}
		WCH_SetWindowStatus(true);
	}
}

void WCH_PutPicture() {
	// Press PrintScreen. (Keyboard)
	keybd_event(VK_SNAPSHOT, 0, 0, 0);
	keybd_event(VK_SNAPSHOT, 0, KEYEVENTF_KEYUP, 0);
	SPDLOG_INFO(L"Copying screenshot to clipboard");
}

void WCH_SaveImg() {
	// Save screenshot to file.
	WCH_Time now = WCH_GetTime();
	wstring SavePathDir = StrToWstr(WCH_Settings["ScreenshotSavePath"].asString());
	wstring SavePath = format(L"{}{:04}{:02}{:02}{:02}{:02}{:02}{}", SavePathDir, now.Year, now.Month, now.Day, now.Hour, now.Minute, now.Second, WCH_MIME_list.find(StrToWstr(WCH_Settings["ScreenshotSaveMIME"].asString()))->second);
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
	GdiplusWrapper gdiplus = {};
	gdiplus.SaveImage(hBitmap, SavePath.c_str(), StrToWstr(WCH_Settings["ScreenshotSaveMIME"].asString()).c_str());
	DeleteDC(hdcScreen);
	DeleteDC(hMemDC);
	DeleteObject(hBitmap);
	SPDLOG_INFO(format(L"Saving image to \"{}\"", SavePath));
	if (StrToWstr(WCH_Settings["ScreenshotOpen"].asString()) == L"True") {
		_wsystem(SavePath.c_str());
	}
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
	wstring FilePathNormal = WCH_path_temp + L"\\WCH_SYSTEM_NORMAL.tmp";
	wstring FilePathError = WCH_path_temp + L"\\WCH_SYSTEM_ERROR.tmp";
	_wsystem(format(L"TASKKILL /F /IM {} > \"{}\" 2> \"{}\"", name, FilePathNormal, FilePathError).c_str());
	bool _res = (!WCH_FileIsBlank(FilePathNormal) && WCH_FileIsBlank(FilePathError));
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
	for (auto it = WCH_support_settings.begin(); it != WCH_support_settings.end(); it++) {
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
	if (Key == L"ScreenshotSavePath" && Value[Value.size() - 1] != L'\\') {
		return make_pair(false, KeyType);
	}
	if (WCH_choice_settings.find(Key) != WCH_choice_settings.end()) {
		if (WCH_choice_settings.find(Key)->second.find(Value) == WCH_choice_settings.find(Key)->second.end()) {
			return make_pair(false, KeyType);
		}
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
		wcout << WCH_progress_bar_str;
	}
	WCH_PrintColor(0x0C);
	for (int32_t i = _n / 2; i < 50; i++) {
		wcout << WCH_progress_bar_str;
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
	bool _cd = WCH_is_countdown;
	double _pro = 100.0 / WCH_progress_bar_duration;
	WCH_PrintProgressBar(WCH_progress_bar_duration, 0, false);
	WCH_TBL->SetProgressState(WCH_handle_window, TBPF_NORMAL);
	WCH_TBL->SetProgressValue(WCH_handle_window, 0, 100);
	for (int32_t i = WCH_progress_bar_duration - 1; i > 0 && !WCH_program_end && !(_cd ^ WCH_is_countdown); i--) {
		WCH_Sleep(1000);
		WCH_PrintProgressBar(i, (int32_t)((WCH_progress_bar_duration - i) * _pro), true);
		WCH_TBL->SetProgressValue(WCH_handle_window, (uint64_t)(((uint64_t)WCH_progress_bar_duration - (uint64_t)i) * _pro), 100);
	}
	WCH_Sleep(1000);
	WCH_PrintProgressBar(0, 100, true);
	WCH_TBL->SetProgressState(WCH_handle_window, TBPF_NOPROGRESS);
	wcout << endl;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
	// Window processing module.
	switch (message) {
		case WM_HOTKEY:
			SPDLOG_DEBUG(format(L"Entering \"WndProc()\": \"WM_HOTKEY\" & \"wParam = {}\" & \"lParam = {}\"", wParam, lParam));
			if (wParam == WCH_HOTKEY_SHOW) {
				WCH_CheckHotkey();
			}
			break;
		case WM_CREATE:
			SPDLOG_DEBUG(L"Entering \"WndProc()\": \"WM_CREATE\"");
			WCH_NID.cbSize = sizeof(WCH_NID);
			WCH_NID.hWnd = hWnd;
			WCH_NID.uID = 0;
			WCH_NID.uFlags = NIF_ICON | NIF_MESSAGE | NIF_TIP;
			WCH_NID.uCallbackMessage = WM_USER;
			WCH_NID.hIcon = (HICON)LoadImageW(NULL, (WCH_GetExecDir() + L"\\WCH.ico").c_str(), IMAGE_ICON, 0, 0, LR_LOADFROMFILE);
			wcscpy(WCH_NID.szTip, WCH_title_window.c_str());
			Shell_NotifyIconW(NIM_ADD, &WCH_NID);
			WCH_handle_menu = CreatePopupMenu();
			AppendMenuW(WCH_handle_menu, MF_STRING, WCH_MENU_SHOW, L"Ctrl + Down");
			AppendMenuW(WCH_handle_menu, MF_SEPARATOR, 0, NULL);
			AppendMenuW(WCH_handle_menu, MF_STRING, WCH_MENU_EXIT, StrToWstr(WCH_Language["Exit"].asString()).c_str());
			break;
		case WM_USER:
			if (lParam == WM_LBUTTONDOWN) {
				SPDLOG_DEBUG(L"Entering \"WndProc()\": \"WM_USER\" & \"WM_LBUTTONDOWN\"");
				WCH_CheckHotkey();
			} else if (lParam == WM_RBUTTONDOWN) {
				POINT pt;
				int32_t xx;
				GetCursorPos(&pt);
				SetForegroundWindow(hWnd);
				xx = TrackPopupMenu(WCH_handle_menu, TPM_RETURNCMD, pt.x, pt.y, NULL, hWnd, NULL);
				SPDLOG_DEBUG(format(L"Entering \"WndProc()\": \"WM_USER\" & \"WM_RBUTTONDOWN\" & \"xx = {}\"", to_wstring(xx)));
				if (xx == WCH_MENU_SHOW) {
					SPDLOG_DEBUG(L"Entering \"WndProc()\": \"WM_USER\" & \"WM_RBUTTONDOWN\" & \"WCH_MENU_SHOW\"");
					WCH_CheckHotkey();
				} else if (xx == WCH_MENU_EXIT) {
					SPDLOG_DEBUG(L"Entering \"WndProc()\": \"WM_USER\" & \"WM_RBUTTONDOWN\" & \"WCH_MENU_EXIT\"");
					raise(SIGBREAK);
				} else if (xx == 0) {
					PostMessageW(hWnd, WM_LBUTTONDOWN, NULL, NULL);
				}
			}
			break;
		case WM_DESTROY:
			SPDLOG_DEBUG(L"Entering \"WndProc()\": \"WM_DESTROY\"");
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
	WCH_TBL->SetProgressState(WCH_handle_window, TBPF_INDETERMINATE);
	if (MessageBoxW(NULL, (StrToWstr(WCH_Language["BugMessagebox1"].asString()) + to_wstring(errorcode) + L" " + errormsg + StrToWstr(WCH_Language["BugMessagebox2"].asString())).c_str(), L"WCH ERROR", MB_ICONERROR | MB_YESNO) == IDYES) {
		_wsystem(L"START https://github.com/class-tools/Web-Class-Helper/issues/new/choose");
	}
	WCH_TBL->SetProgressState(WCH_handle_window, TBPF_NOPROGRESS);
}

void WCH_signalHandler() {
	// Signal handler.
	signal(SIGINT, []([[maybe_unused]] int32_t signum) {
		WCH_list_command.clear();
		WCH_list_command.push_back(L"exit");
		wcout << endl;
		exit(0);
	});
	signal(SIGBREAK, []([[maybe_unused]] int32_t signum) {
		WCH_list_command.clear();
		WCH_list_command.push_back(L"exit");
		wcout << endl;
		exit(0);
	});
	signal(SIGABRT, [](int32_t signum) {
		WCH_cmd_line = false;
		WCH_program_end = true;
		WCH_PrintColor(0x07);
		wcout << endl;
		SPDLOG_CRITICAL(format(L"Signal {} detected (Program aborted)", signum));
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
		SPDLOG_CRITICAL(format(L"Signal {} detected (Operation overflow)", signum));
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
		SPDLOG_CRITICAL(format(L"Signal {} detected (Illegal instruction)", signum));
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
		SPDLOG_CRITICAL(format(L"Signal {} detected (Access to illegal memory)", signum));
		Sleep(500);
		WCH_SetWindowStatus(false);
		WCH_ShowBugMessagebox(signum, L"Access to illegal memory");
		exit(signum);
	});
}

#endif
