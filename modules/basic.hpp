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

using std::ignore;
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
using std::random_device;
using std::mt19937;
using std::chrono::milliseconds;
using std::make_pair;
using std::make_tuple;
using std::get;
using std::this_thread::sleep_for;

#define WCH_HOTKEY_SHOW 121
#define WCH_MENU_SHOW 11461
#define WCH_MENU_EXIT 11462
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
	int32_t GetEncoderClsid(const wchar_t* format, CLSID* pClsid) {
		UINT num = 0;
		UINT size = 0;
		UINT ret = -1;
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
#pragma warning(suppress : 6385)
			if (wcscmp(pImageCodecInfo[j].MimeType, format) == 0) {
				*pClsid = pImageCodecInfo[j].Clsid;
				ret = j;
			}
		}
		free(pImageCodecInfo);
		return ret;
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
	int32_t Year = 0;
	int32_t Month = 0;
	int32_t Day = 0;
	int32_t Hour = 0;
	int32_t Minute = 0;
	int32_t Second = 0;
	int32_t WeekDay = 0;
};

struct WCH_Data_Body {
	int32_t H = 0;
	int32_t M = 0;
};

struct WCH_Version {
	int32_t X = 0;
	int32_t Y = 0;
	int32_t Z = 0;
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

void WCH_clock();
void WCH_task();
void WCH_work();
void WCH_help();
void WCH_ow();
void WCH_hide();
void WCH_game();
void WCH_time();
void WCH_pi();
void WCH_trans();
void WCH_fate();
void WCH_focus();
void WCH_countdown();
void WCH_update();
void WCH_wiki();
void WCH_license();
void WCH_clear();
void WCH_config();
void WCH_develop();
void WCH_sysinfo();
void WCH_exit();
void WCH_restart();

const vector<wstring> WCH_list_weekday = {L"Sunday", L"Monday", L"Tuesday", L"Wednesday", L"Thursday", L"Friday", L"Saturday"};
const map<wstring, set<wstring>> WCH_choice_settings = {
	make_pair(L"Language", []() {
		set<wstring> _tmp = {L"en-US", L"zh-CN"};
		return _tmp;
	}()),
	make_pair(L"ScreenshotSaveMIME", []() {
		set<wstring> _tmp = {L"image/bmp", L"image/gif", L"image/jpeg", L"image/png", L"image/tiff"};
		return _tmp;
	}()),
};
const map<wstring, wstring> WCH_MIME_list = {
	make_pair(L"image/bmp", L".bmp"),
	make_pair(L"image/gif", L".gif"),
	make_pair(L"image/jpeg", L".jpg"),
	make_pair(L"image/png", L".png"),
	make_pair(L"image/tiff", L".tif")};
const map<wstring, function<void()>> WCH_support_command = {
	make_pair(L"clock", WCH_clock),
	make_pair(L"task", WCH_task),
	make_pair(L"work", WCH_work),
	make_pair(L"help", WCH_help),
	make_pair(L"ow", WCH_ow),
	make_pair(L"hide", WCH_hide),
	make_pair(L"game", WCH_game),
	make_pair(L"time", WCH_time),
	make_pair(L"pi", WCH_pi),
	make_pair(L"trans", WCH_trans),
	make_pair(L"fate", WCH_fate),
	make_pair(L"focus", WCH_focus),
	make_pair(L"countdown", WCH_countdown),
	make_pair(L"update", WCH_update),
	make_pair(L"wiki", WCH_wiki),
	make_pair(L"license", WCH_license),
	make_pair(L"clear", WCH_clear),
	make_pair(L"config", WCH_config),
	make_pair(L"develop", WCH_develop),
	make_pair(L"sysinfo", WCH_sysinfo),
	make_pair(L"exit", WCH_exit),
	make_pair(L"restart", WCH_restart)};
const set<tuple<wstring, wstring, wstring, bool>> WCH_support_settings = {
	make_tuple(L"CommandPrompt", L"String", L">>>", false),
	make_tuple(L"CountDownSoundPrompt", L"Boolean", L"False", false),
	make_tuple(L"FocusEndContent", L"String", L"Disable focus?", false),
	make_tuple(L"FocusEndPrompt", L"Boolean", L"False", false),
	make_tuple(L"FocusKillInterval", L"Number", L"60000", false),
	make_tuple(L"Language", L"String", L"en-US", true),
	make_tuple(L"ScreenshotSaveMIME", L"String", L"image/png", false),
	make_tuple(L"ScreenshotSavePath", L"String", format(L"{}\\Pictures\\", _wgetenv(L"USERPROFILE")), false)};
const set<wstring> WCH_support_language = {
	L"ProgramName",
	L"Start",
	L"Exit",
	L"PreviewWarning",
	L"InputCommandIncorrect",
	L"FileProcessingFailed",
	L"NetworkError",
	L"OSVersion",
	L"OSArchitecture",
	L"ProgramArchitecture",
	L"JumpWiki",
	L"CheckUpdate",
	L"FindUpdate",
	L"NoUpdate",
	L"ConfigWizardPrompt",
	L"Yes",
	L"No",
	L"Key",
	L"Value",
	L"Hour",
	L"Minute",
	L"Name",
	L"ProcessName",
	L"Tag",
	L"Unclassified",
	L"InputNumber",
	L"NumberOutOfRange",
	L"NumberSmaller",
	L"NumberBigger",
	L"NumberAnswer",
	L"NumberWin",
	L"NumberLose",
	L"Pi",
	L"CountDown",
	L"Focus",
	L"DataReading",
	L"WillRestart",
	L"BugMessagebox1",
	L"BugMessagebox2"};
const wstring WCH_progress_bar_str = IsWindows10OrGreater() ? L"━" : L"-";
const wstring WCH_path_data = format(L"{}\\AppData\\Local\\WCH", _wgetenv(L"USERPROFILE"));
const wstring WCH_path_temp = format(L"{}\\AppData\\Local\\Temp\\WCH", _wgetenv(L"USERPROFILE"));
vector<wstring> WCH_list_command;
set<tuple<int32_t, int32_t, wstring>> WCH_list_clock;
set<wstring> WCH_list_task;
set<pair<wstring, wstring>> WCH_list_work;
wstring WCH_title_window;
HWND WCH_handle_window;
HWND WCH_handle_tray;
HMENU WCH_handle_menu;
NOTIFYICONDATAW WCH_NID;
ATL::CComPtr<ITaskbarList3> WCH_TBL;
Json::Value WCH_Settings;
Json::Value WCH_Language;
int32_t WCH_num_clock;
int32_t WCH_num_task;
int32_t WCH_num_work;
int32_t WCH_progress_bar_duration;
bool WCH_cmd_line = true;
bool WCH_is_focus;
bool WCH_is_countdown;
bool WCH_program_end;
bool WCH_pre_start = true;
ifstream fin;
wifstream wfin;
ofstream fout;
wofstream wfout;
Json::Reader JSON_Reader;
Json::StreamWriterBuilder JSON_SWB;
unique_ptr<Json::StreamWriter> JSON_SW(JSON_SWB.newStreamWriter());
wstring WCH_path_program;

#endif