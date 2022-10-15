/*
Web Class Helper Basic Module Header File 2.1.1
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
#include <climits>
#include <clocale>
#include <cmath>
#include <codecvt>
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
#include <string_view>
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
using std::vformat;
using std::getline;
using std::find;
using std::to_string;
using std::to_wstring;
using std::string;
using std::string_view;
using std::wstring;
using std::wstring_view;
using std::stringstream;
using std::wstringstream;
using std::ifstream;
using std::wifstream;
using std::ofstream;
using std::wofstream;
using std::unique_ptr;
using std::locale;
using std::array;
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
using std::make_tuple;
using std::get;
using std::this_thread::sleep_for;

#define WCH_HOTKEY_SHOW 121
#define WCH_MENU_SHOW 11461
#define WCH_MENU_EXIT 11462
#define WCH_LOG_STATUS_INFO TEXT("[INFO]")
#define WCH_LOG_STATUS_DEBUG TEXT("[DEBUG]")
#define WCH_LOG_STATUS_WARN TEXT("[WARN]")
#define WCH_LOG_STATUS_ERROR TEXT("[ERROR]")

class GdiplusWrapper {
public:
	GdiplusWrapper() {
		Gdiplus::GdiplusStartupInput gdiplusStartupInput;
		GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);
	}
	~GdiplusWrapper() {
		Gdiplus::GdiplusShutdown(gdiplusToken);
	}
	int32_t GetEncoderClsid(const wchar_t* format, CLSID* pClsid) {
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
	void SaveImage(HBITMAP hBitmap, const wchar_t* filename, const wchar_t* format) {
		CLSID pngClsid;
		Gdiplus::Bitmap bitmap(hBitmap, NULL);
		GetEncoderClsid(format, &pngClsid);
		bitmap.Save(filename, &pngClsid);
	}
private:
	ULONG_PTR gdiplusToken;
};

struct WCH_Time {
	int32_t Year {};
	int32_t Month {};
	int32_t Day {};
	int32_t Hour {};
	int32_t Minute {};
	int32_t Second {};
	int32_t WeekDay {};
};

struct WCH_Data_Body {
	int32_t H {};
	int32_t M {};
};

struct WCH_Version {
	int32_t X {};
	int32_t Y {};
	int32_t Z {};
	bool operator==(const WCH_Version& _in) {
		return (X == _in.X && Y == _in.Y && Z == _in.Z);
	}
	bool operator<(const WCH_Version& _in) {
		if (X < _in.X) {
			return true;
		} else if (X > _in.X) {
			return false;
		} else {
			if (Y < _in.Y) {
				return true;
			} else if (Y > _in.Y) {
				return false;
			} else {
				if (Z < _in.Z) {
					return true;
				} else {
					return false;
				}
			}
		}
	}
};

const array<wstring, 7> WCH_weekday_list {L"Sunday", L"Monday", L"Tuesday", L"Wednesday", L"Thursday", L"Friday", L"Saturday"};
const array<wstring, 2> WCH_language_list {L"en-US", L"zh-CN"};
map<wstring, function<void()>> WCH_command_support;
set<tuple<wstring, wstring, wstring>> WCH_settings_support;
set<wstring> WCH_language_support;
vector<wstring> WCH_command_list;
set<tuple<int32_t, int32_t, wstring>> WCH_clock_list;
set<wstring> WCH_task_list;
set<pair<wstring, wstring>> WCH_work_list;
wstring WCH_window_title;
wstring WCH_command;
wstring WCH_ProgressBarStr;
HWND WCH_window_handle;
HWND WCH_tray_handle;
HMENU WCH_menu_handle;
NOTIFYICONDATA WCH_NID;
ATL::CComPtr<ITaskbarList3> WCH_TBL;
Json::Value WCH_Settings;
Json::Value WCH_Language;
int32_t WCH_clock_num;
int32_t WCH_task_num;
int32_t WCH_work_num;
int32_t WCH_clock_change;
int32_t WCH_task_change;
int32_t WCH_work_change;
int32_t WCH_settings_change;
int32_t WCH_ProgressBarTot;
int32_t WCH_InputTimes;
bool WCH_cmd_line = true;
bool WCH_anti_idle;
bool WCH_count_down;
bool WCH_program_end;
bool WCH_pre_start = true;
ifstream fin;
wifstream wfin;
ofstream fout;
wofstream wfout;
Json::Reader JSON_Reader;
Json::StreamWriterBuilder JSON_SWB;
unique_ptr<Json::StreamWriter> JSON_SW(JSON_SWB.newStreamWriter());
WCH_Time WCH_GetTime();
void WCH_Sleep(int32_t _ms);
void WCH_printlog(wstring _mode, wstring _info);
void WCH_read_settings();
void WCH_read();
void WCH_save_settings();
bool WCH_save_func(bool output);
size_t WCH_GetNumDigits(size_t _n);

#endif