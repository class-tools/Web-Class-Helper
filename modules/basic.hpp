/*
Web Class Helper Basic Module Header File 2.1.2
This source code file is under MIT License.
Copyright (c) 2022 - 2023 Class Tools Develop Team
Contributors: jsh-jsh ren-yc
*/
#include <csignal>
#include <cstdint>
#include <cstdlib>
#include <chrono>
#include <locale>
#include <utility>
#include <filesystem>
#include <ios>
#include <fstream>
#include <iostream>
#include <map>
#include <set>
#include <tuple>
#include <vector>
#include <thread>
#include <random>
#include <io.h>
#include <Windows.h>
#include <WinInet.h>
#include <TLHelp32.h>
#include <VersionHelpers.h>
#include <ShObjidl.h>
#include <gdiplus.h>
#include <atlbase.h>
#include <fcntl.h>

#pragma warning(push)
#pragma warning(disable : 26437 26451 26495 26498 26800)
#define SPDLOG_WCHAR_FILENAMES
#define SPDLOG_ACTIVE_LEVEL SPDLOG_LEVEL_DEBUG
#include <fmt/format.h>
#include <json/json.h>
#include <openssl/sha.h>
#include <spdlog/spdlog.h>
#include <spdlog/sinks/basic_file_sink.h>
#pragma warning(pop)

#pragma comment(lib, "winmm.lib")
#pragma comment(lib, "gdiplus.lib")
#pragma comment(lib, "urlmon.lib")
#pragma comment(lib, "ole32.lib")

#define WCH_HOTKEY_SHOW 121
#define WCH_MENU_SHOW 11461
#define WCH_MENU_EXIT 11462

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
void WCH_open();
void WCH_ow();
void WCH_hide();
void WCH_game();
void WCH_time();
void WCH_prtscn();
void WCH_trans();
void WCH_fate();
void WCH_focus();
void WCH_countdown();
void WCH_update();
void WCH_license();
void WCH_clear();
void WCH_config();
void WCH_develop();
void WCH_sysinfo();
void WCH_exit();
void WCH_restart();

const std::vector<std::wstring> WCH_list_weekday = {L"Sunday", L"Monday", L"Tuesday", L"Wednesday", L"Thursday", L"Friday", L"Saturday"};
const std::map<std::wstring, std::set<std::wstring>> WCH_choice_settings = {
	make_pair(L"Language", []() {
		std::set<std::wstring> _tmp = {L"en-US", L"zh-CN"};
		return _tmp;
	}()),
	make_pair(L"ScreenshotSaveMIME", []() {
		std::set<std::wstring> _tmp = {L"image/bmp", L"image/gif", L"image/jpeg", L"image/png", L"image/tiff"};
		return _tmp;
	}()),
};
const std::map<std::wstring, std::wstring> WCH_MIME_list = {
	std::make_pair(L"image/bmp", L".bmp"),
	std::make_pair(L"image/gif", L".gif"),
	std::make_pair(L"image/jpeg", L".jpg"),
	std::make_pair(L"image/png", L".png"),
	std::make_pair(L"image/tiff", L".tif")};
const std::map<std::wstring, std::function<void()>> WCH_support_command = {
	std::make_pair(L"clock", WCH_clock),
	std::make_pair(L"task", WCH_task),
	std::make_pair(L"work", WCH_work),
	std::make_pair(L"help", WCH_help),
	std::make_pair(L"open", WCH_open),
	std::make_pair(L"ow", WCH_ow),
	std::make_pair(L"hide", WCH_hide),
	std::make_pair(L"game", WCH_game),
	std::make_pair(L"time", WCH_time),
	std::make_pair(L"prtscn", WCH_prtscn),
	std::make_pair(L"trans", WCH_trans),
	std::make_pair(L"fate", WCH_fate),
	std::make_pair(L"focus", WCH_focus),
	std::make_pair(L"countdown", WCH_countdown),
	std::make_pair(L"update", WCH_update),
	std::make_pair(L"license", WCH_license),
	std::make_pair(L"clear", WCH_clear),
	std::make_pair(L"config", WCH_config),
	std::make_pair(L"develop", WCH_develop),
	std::make_pair(L"sysinfo", WCH_sysinfo),
	std::make_pair(L"exit", WCH_exit),
	std::make_pair(L"restart", WCH_restart)};
const std::set<std::tuple<std::wstring, std::wstring, std::wstring, bool>> WCH_support_settings = {
	std::make_tuple(L"CommandPrompt", L"String", L">>>", false),
	std::make_tuple(L"CountDownSoundPrompt", L"Boolean", L"False", false),
	std::make_tuple(L"FocusEndContent", L"String", L"Disable focus?", false),
	std::make_tuple(L"FocusEndPrompt", L"Boolean", L"False", false),
	std::make_tuple(L"FocusKillInterval", L"Number", L"60000", false),
	std::make_tuple(L"Language", L"String", L"en-US", true),
	std::make_tuple(L"ScreenshotOpen", L"Boolean", L"False", false),
	std::make_tuple(L"ScreenshotSaveMIME", L"String", L"image/png", false),
	std::make_tuple(L"ScreenshotSavePath", L"String", fmt::format(L"{}\\Pictures\\", _wgetenv(L"USERPROFILE")), false)};
const std::wstring WCH_progress_bar_str = IsWindows10OrGreater() ? L"━" : L"-";
const std::wstring WCH_path_data = fmt::format(L"{}\\AppData\\Local\\WCH", _wgetenv(L"USERPROFILE"));
const std::wstring WCH_path_temp = fmt::format(L"{}\\WCH", std::filesystem::temp_directory_path().c_str());
std::vector<std::wstring> WCH_list_command;
std::set<std::tuple<int32_t, int32_t, std::wstring>> WCH_list_clock;
std::set<std::wstring> WCH_list_task;
std::set<std::pair<std::wstring, std::wstring>> WCH_list_work;
std::wstring WCH_version;
std::wstring WCH_path_exec;
std::wstring WCH_title_window;
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
std::ifstream fin;
std::wifstream wfin;
std::ofstream fout;
std::wofstream wfout;
Json::Reader JSON_Reader;
Json::StreamWriterBuilder JSON_SWB;
std::unique_ptr<Json::StreamWriter> JSON_SW(JSON_SWB.newStreamWriter());
std::shared_ptr<spdlog::sinks::basic_file_sink_mt> LOG_sink;
std::shared_ptr<spdlog::logger> LOG_logger;
