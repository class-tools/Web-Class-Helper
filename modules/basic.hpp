/*
Web Class Helper Basic Module Header File 2.1.0
This source code file is under MIT License.
Copyright (c) 2022 Class Tools Develop Team
Contributors: jsh-jsh ren-yc
*/
#ifndef BASIC_H
#define BASIC_H
#include <cassert>
#include <cctype>
#include <cerrno>
#include <cfloat>
#include <ciso646>
#include <climits>
#include <clocale>
#include <cmath>
#include <csetjmp>
#include <csignal>
#include <cstdarg>
#include <cstddef>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <ctime>
#include <cwchar>
#include <cwctype>
#include <ccomplex>
#include <cfenv>
#include <cinttypes>
#include <cstdalign>
#include <cstdbool>
#include <cstdint>
#include <ctgmath>
#include <cuchar>
#include <algorithm>
#include <bitset>
#include <complex>
#include <deque>
#include <exception>
#include <fstream>
#include <functional>
#include <iomanip>
#include <ios>
#include <iosfwd>
#include <iostream>
#include <istream>
#include <iterator>
#include <limits>
#include <list>
#include <locale>
#include <map>
#include <memory>
#include <new>
#include <numeric>
#include <ostream>
#include <queue>
#include <set>
#include <sstream>
#include <stack>
#include <stdexcept>
#include <streambuf>
#include <string>
#include <typeinfo>
#include <utility>
#include <valarray>
#include <vector>
#include <array>
#include <atomic>
#include <chrono>
#include <codecvt>
#include <condition_variable>
#include <forward_list>
#include <future>
#include <initializer_list>
#include <mutex>
#include <random>
#include <ratio>
#include <regex>
#include <scoped_allocator>
#include <system_error>
#include <thread>
#include <tuple>
#include <typeindex>
#include <type_traits>
#include <unordered_map>
#include <unordered_set>
#include <shared_mutex>
#include <any>
#include <charconv>
#include <filesystem>
#include <optional>
#include <memory_resource>
#include <string_view>
#include <variant>
#include <bit>
#include <compare>
#include <concepts>
#include <coroutine>
#include <numbers>
#include <ranges>
#include <span>
#include <stop_token>
#include <version>
#include <io.h>
#include <conio.h>
#include <direct.h>
#include <Windows.h>
#include <WinInet.h>
#include <TLHelp32.h>
#include <VersionHelpers.h>
#include <ShObjidl.h>
#include <gdiplus.h>
#include <atlbase.h>
#include <fcntl.h>
#include "json/json.h"

#pragma comment(lib, "winmm.lib")
#pragma comment(lib, "gdiplus.lib")
#pragma comment(lib, "urlmon.lib")
#pragma comment(lib, "ole32.lib")

using std::cin;
using std::wcin;
using std::cout;
using std::wcout;
using std::cerr;
using std::wcerr;
using std::endl;
using std::format;
using std::getline;
using std::find;
using std::to_string;
using std::to_wstring;
using std::string;
using std::wstring;
using std::stringstream;
using std::wstringstream;
using std::ifstream;
using std::wifstream;
using std::ofstream;
using std::wofstream;
using std::unique_ptr;
using std::locale;
using std::map;
using std::set;
using std::vector;
using std::tuple;
using std::function;
using std::pair;
using std::thread;
using std::runtime_error;
using std::exception;
using std::ios;
using std::chrono::milliseconds;
using std::make_pair;
using std::get;
using std::this_thread::sleep_for;

#define WCH_HOTKEY_SHOW 121
#define WCH_MENU_SHOW 11461
#define WCH_MENU_QUIT 11462
#define WCH_LOG_STATUS_INFO L"[INFO]"
#define WCH_LOG_STATUS_DEBUG L"[DEBUG]"
#define WCH_LOG_STATUS_WARN L"[WARN]"
#define WCH_LOG_STATUS_ERROR L"[ERROR]"

class GdiplusWrapper {
public:
	GdiplusWrapper() {
		Gdiplus::GdiplusStartupInput gdiplusStartupInput;
		GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);
	}
	~GdiplusWrapper() {
		Gdiplus::GdiplusShutdown(gdiplusToken);
	}
	int GetEncoderClsid(const WCHAR* format, CLSID* pClsid) {
		UINT num = 0;
		UINT size = 0;
		Gdiplus::ImageCodecInfo* pImageCodecInfo = NULL;
		Gdiplus::GetImageEncodersSize(&num, &size);
		if (size == 0) {
			return -1;
		}
		pImageCodecInfo = (Gdiplus::ImageCodecInfo*)(malloc(size));
		if (pImageCodecInfo == NULL) {
			return -1;
		}
		Gdiplus::GetImageEncoders(num, size, pImageCodecInfo);
		for (UINT j = 0; j < num; j++) {
			if (wcscmp(pImageCodecInfo[j].MimeType, format) == 0) {
				*pClsid = pImageCodecInfo[j].Clsid;
				free(pImageCodecInfo);
				return j;
			}
		}
		free(pImageCodecInfo);
		return -1;
	}
	void SaveImage(HBITMAP hBitmap, const WCHAR* filename, const WCHAR* format) {
		CLSID pngClsid;
		Gdiplus::Bitmap bitmap(hBitmap, NULL);
		GetEncoderClsid(format, &pngClsid);
		bitmap.Save(filename, &pngClsid);
	}
private:
	ULONG_PTR gdiplusToken;
};

struct WCH_Time {
	int Year {};
	int Month {};
	int Day {};
	int Hour {};
	int Minute {};
	int Second {};
	int WeekDay {};
};

struct WCH_Data_Body {
	int H {};
	int M {};
};

struct WCH_Version {
	int X {};
	int Y {};
	int Z {};
};

const wstring WCH_WDName[7] = {L"Sunday", L"Monday", L"Tuesday", L"Wednesday", L"Thursday", L"Friday", L"Saturday"};
map<wstring, function<void()>> WCH_command_support;
vector<wstring> WCH_command_list;
set<tuple<int, int, wstring>> WCH_clock_list;
set<wstring> WCH_task_list;
set<pair<wstring, wstring>> WCH_work_list;
wstring WCH_window_title;
HWND WCH_Win_hWnd;
HWND WCH_Tray_hWnd;
HMENU WCH_hMenu;
NOTIFYICONDATA WCH_NID;
ATL::CComPtr<ITaskbarList3> WCH_TBL;
int WCH_clock_num;
int WCH_task_num;
int WCH_work_num;
int WCH_clock_change;
int WCH_task_change;
int WCH_work_change;
int WCH_ProgressBarTot;
int WCH_InputTimes;
bool WCH_cmd_line = true;
bool WCH_anti_idle;
bool WCH_count_down;
bool WCH_program_end;
bool WCH_pre_start = true;
wstring WCH_command;
wstring WCH_ProgressBarStr;
ifstream fin;
wifstream wfin;
ofstream fout;
wofstream wfout;
Json::StreamWriterBuilder Json_SWB;
WCH_Time WCH_GetTime();
void WCH_Sleep(int _ms);
void WCH_printlog(wstring _mode, wstring _info);
void WCH_read();
bool WCH_save_func();
size_t WCH_GetNumDigits(size_t _n);

#endif