/*
Web Class Helper Initialization Module Header File 2.1.2
This source code file is under MIT License.
Copyright (c) 2022 - 2023 Class Tools Develop Team
Contributors: jsh-jsh ren-yc hjl2011
*/
void WCH_read();
void WCH_read_settings();
void WCH_save_settings();

void WCH_Init_Dir() {
	// Initialization for directory.
	if (!std::filesystem::is_directory(WCH_path_data)) {
		std::filesystem::create_directory(WCH_path_data);
	}
	if (!std::filesystem::is_directory(WCH_path_data + L"\\data")) {
		std::filesystem::create_directory(WCH_path_data + L"\\data");
	}
	if (!std::filesystem::is_directory(WCH_path_data + L"\\logs")) {
		std::filesystem::create_directory(WCH_path_data + L"\\logs");
	}
	if (!std::filesystem::is_directory(WCH_path_temp)) {
		std::filesystem::create_directory(WCH_path_temp);
	}
}

void WCH_Init_Bind() {
	// Initialization for bind.
	atexit(WCH_exit);
	WCH_signalHandler();
	std::ignore = CoInitializeEx(nullptr, COINIT_APARTMENTTHREADED | COINIT_DISABLE_OLE1DDE);
	std::ignore = WCH_TBL.CoCreateInstance(CLSID_TaskbarList);
	JSON_SWB.settings_ = []() {
		Json::Value def;
		Json::StreamWriterBuilder::setDefaults(&def);
		def["emitUTF8"] = true;
		return def;
	}();
	WCH_handle_window = GetConsoleWindow();
	std::ignore = _setmode(_fileno(stdin), _O_WTEXT);
	std::ignore = _setmode(_fileno(stdout), _O_WTEXT);
	wfin.imbue(std::locale(".UTF-8", LC_CTYPE));
	wfout.imbue(std::locale(".UTF-8", LC_CTYPE));
	_wsystem(L"CHCP 65001 > NUL");
}

void WCH_Init_Log() {
	// Initialization for log.
	WCH_Time now = WCH_GetTime();
	WCH_read_settings();
	if (std::filesystem::is_regular_file(WCH_path_data + L"\\logs\\latest.log")) {
		std::filesystem::rename(WCH_path_data + L"\\logs\\latest.log", WCH_path_data + L"\\logs\\" + StrToWstr(WCH_Settings["StartTime"].asString()) + L".log");
	}
	WCH_Settings["StartTime"] = WstrToStr(fmt::format(L"{:04}{:02}{:02}{:02}{:02}{:02}", now.Year, now.Month, now.Day, now.Hour, now.Minute, now.Second));
	LOG_sink = make_shared<spdlog::sinks::basic_file_sink_mt>(WCH_path_data + L"\\logs\\latest.log");
	LOG_sink->set_pattern("[%Y-%m-%d %H:%M:%S.%f] [%l] [%s:%# %!] [Process %P] [Thread %t]: %v.");
	LOG_logger = make_shared<spdlog::logger>("WCH", LOG_sink);
	LOG_logger->flush_on(spdlog::level::debug);
	spdlog::register_logger(LOG_logger);
#ifdef _DEBUG
	spdlog::set_level(spdlog::level::debug);
#else
	spdlog::set_level(spdlog::level::info);
#endif
	spdlog::set_default_logger(LOG_logger);
	WCH_save_settings();
	WCH_pre_start = false;
	WCH_version = WCH_VER_MAIN;
#if WCH_VER_TYPE != 0
	#if WCH_VER_TYPE == 1
	WCH_version.append(L" Alpha");
	#elif WCH_VER_TYPE == 2
	WCH_version.append(L" Beta");
	#elif WCH_VER_TYPE == 3
	WCH_version.append(L" Rc");
	#endif
	WCH_version.append(fmt::format(L" {}", WCH_VER_BUILD));
#endif
	WCH_version.append(fmt::format(L" ({})", StrToWstr(WCH_Architecture)));
	SPDLOG_INFO(fmt::format(L"Starting \"Web Class Helper {}\"", WCH_version));
}

void WCH_Init_Var() {
	// Initialization for variable.
	WCH_title_window = fmt::format(L"{} {}", StrToWstr(WCH_Language["ProgramName"].asString()), WCH_version);
#if WCH_VER_TYPE != 0
	WCH_SetWindowStatus(false);
	if (MessageBoxW(NULL, (StrToWstr(WCH_Language["PreviewWarning"].asString()) + WCH_GetCompileTime()).c_str(), L"WCH WARN", MB_ICONWARNING | MB_YESNO | MB_TOPMOST) == IDNO) {
		WCH_CheckAndDeleteFile(WCH_path_data + L"\\logs\\latest.log");
		_exit(0);
	}
	WCH_SetWindowStatus(true);
#endif
}

void WCH_Init_Win() {
	// Initialization for window.
	SetConsoleTitleW(WCH_title_window.c_str());
	WCH_TBL->SetProgressState(WCH_handle_window, TBPF_NOPROGRESS);
	SetWindowLongPtrW(WCH_handle_window, GWL_STYLE, GetWindowLongPtrW(WCH_handle_window, GWL_STYLE) & ~WS_SIZEBOX & ~WS_MAXIMIZEBOX & ~WS_MINIMIZEBOX);
}

void WCH_Init_Loop() {
	// Initialization for multiple thread loop.
	std::thread T1(WCH_check_clock_loop);
	T1.detach();
	std::thread T2(WCH_message_loop);
	T2.detach();
	std::wcout << StrToWstr(WCH_Language["DataReading"].asString()) << std::endl;
	WCH_progress_bar_duration = 3;
	std::thread T3(WCH_ProgressBar);
	T3.detach();
	WCH_Sleep(4000);
	_wsystem(L"CLS");
}

void WCH_Init() {
	// Initialize the whole program.
	_wsystem(L"CLS");
	WCH_Init_Dir();
	WCH_Init_Bind();
	WCH_Init_Log();
	WCH_read();
	WCH_Init_Var();
	WCH_Init_Win();
	WCH_Init_Loop();
	std::wcout << WCH_title_window << std::endl;
	std::wcout << StrToWstr(WCH_Language["Start"].asString()) << std::endl;
	std::wcout << std::endl;
}
