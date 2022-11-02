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
#pragma warning(push)
#pragma warning(disable : 6385)
		for (UINT j = 0; j < num; j++) {
			if (wcscmp(pImageCodecInfo[j].MimeType, format) == 0) {
				*pClsid = pImageCodecInfo[j].Clsid;
				ret = j;
			}
		}
		free(pImageCodecInfo);
#pragma warning(pop)
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

void WCH_check_clock();
void WCH_check_task();
void WCH_check_work();
void WCH_help();
void WCH_ow();
void WCH_hide();
void WCH_game();
void WCH_time();
void WCH_pi();
void WCH_trans();
void WCH_fate();
void WCH_anti_idle_func();
void WCH_count_down_func();
void WCH_update();
void WCH_wiki();
void WCH_license();
void WCH_clear();
void WCH_check_config();
void WCH_save_cmd();
void WCH_exit();

const array<wstring, 7> WCH_weekday_list = {L"Sunday", L"Monday", L"Tuesday", L"Wednesday", L"Thursday", L"Friday", L"Saturday"};
const array<wstring, 2> WCH_language_list = {L"en-US", L"zh-CN"};
const map<wstring, wstring> WCH_MIME_list = {
	make_pair(L"image/bmp", L".bmp"),
	make_pair(L"image/gif", L".gif"),
	make_pair(L"image/jpeg", L".jpg"),
	make_pair(L"image/png", L".png"),
	make_pair(L"image/tiff", L".tif")};
const map<wstring, function<void()>> WCH_command_support = {
	make_pair(L"clock", WCH_check_clock),
	make_pair(L"task", WCH_check_task),
	make_pair(L"work", WCH_check_work),
	make_pair(L"help", WCH_help),
	make_pair(L"ow", WCH_ow),
	make_pair(L"hide", WCH_hide),
	make_pair(L"game", WCH_game),
	make_pair(L"time", WCH_time),
	make_pair(L"pi", WCH_pi),
	make_pair(L"trans", WCH_trans),
	make_pair(L"fate", WCH_fate),
	make_pair(L"anti-idle", WCH_anti_idle_func),
	make_pair(L"count-down", WCH_count_down_func),
	make_pair(L"update", WCH_update),
	make_pair(L"wiki", WCH_wiki),
	make_pair(L"license", WCH_license),
	make_pair(L"clear", WCH_clear),
	make_pair(L"config", WCH_check_config),
	make_pair(L"save", WCH_save_cmd),
	make_pair(L"exit", WCH_exit)};
const set<tuple<wstring, wstring, wstring>> WCH_settings_support = {
	make_tuple(L"AntiIdleEndContent", L"String", L"Disable anti-idle?"),
	make_tuple(L"AntiIdleEndPrompt", L"Boolean", L"False"),
	make_tuple(L"AutoSave", L"Boolean", L"True"),
	make_tuple(L"AutoSaveTime", L"Number", L"60000"),
	make_tuple(L"CommandPrompt", L"String", L">>>"),
	make_tuple(L"CountDownSoundPrompt", L"Boolean", L"False"),
	make_tuple(L"Language", L"String", L"en-US"),
	make_tuple(L"ScreenshotSaveMIME", L"String", L"image/png"),
	make_tuple(L"ScreenshotSavePath", L"String", format(L"{}\\Pictures\\", _wgetenv(L"USERPROFILE")))};
const set<wstring> WCH_language_support = {
	L"ProgramName",
	L"Start",
	L"AlreadyRunning",
	L"InternalPreview",
	L"PublicPreview",
	L"ReleaseCandidate",
	L"Build",
	L"Show",
	L"Exit",
	L"PreviewWarning",
	L"InputCommandIncorrect",
	L"FileProcessingFailed",
	L"NetworkError",
	L"JumpWiki",
	L"CheckUpdate",
	L"FindUpdate",
	L"NoUpdate",
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
	L"AntiIdle",
	L"DataReading",
	L"DataSaving",
	L"DataSaved",
	L"DataNone"};
const wstring WCH_progress_bar = IsWindows10OrGreater() ? L"━" : L"-";
const wstring WCH_path_data = format(L"{}\\AppData\\Local\\WCH", _wgetenv(L"USERPROFILE"));
const wstring WCH_path_temp = format(L"{}\\AppData\\Local\\Temp", _wgetenv(L"USERPROFILE"));
vector<wstring> WCH_command_list;
set<tuple<int32_t, int32_t, wstring>> WCH_clock_list;
set<wstring> WCH_task_list;
set<pair<wstring, wstring>> WCH_work_list;
wstring WCH_window_title;
HWND WCH_window_handle;
HWND WCH_tray_handle;
HMENU WCH_menu_handle;
NOTIFYICONDATAW WCH_NID;
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

#endif