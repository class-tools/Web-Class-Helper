/*
Web Class Helper Functions Module Header File 2.1.2
This source code file is under MIT License.
Copyright (c) 2022 - 2023 Class Tools Develop Team
Contributors: jsh-jsh ren-yc
*/
void WCH_check_clock_loop() {
	// Check if the time equals to the clock. (Another thread)
	WCH_Sleep((60 - WCH_GetTime().Second) * 1000);
	while (!WCH_program_end) {
		WCH_Time NOW = WCH_GetTime();
		for (auto it = WCH_list_clock.begin(); it != WCH_list_clock.end(); it++) {
			if (get<0>(*it) == NOW.Hour && get<1>(*it) == NOW.Minute && get<2>(*it).size() > 0) {
				if (StrToWstr(WCH_Settings["CountDownSoundPrompt"].asString()) == L"True") {
					std::wcout << L"\a";
				}
				bool _tmp = WCH_cmd_line;
				if (_tmp) {
					WCH_SetWindowStatus(false);
				}
				MessageBoxW(NULL, (get<2>(*it)).c_str(), L"WCH CLOCK", MB_OK | MB_TOPMOST);
				if (!WCH_is_focus || _tmp) {
					WCH_SetWindowStatus(true);
				}
			}
		}
		WCH_Sleep(60000);
	}
}

void WCH_message_loop() {
	// Message loop.
	MSG msg = {};
	WNDCLASS wndclass = {};
	wndclass.style = CS_HREDRAW | CS_VREDRAW;
	wndclass.lpfnWndProc = WndProc;
	wndclass.cbClsExtra = 0;
	wndclass.cbWndExtra = 0;
	wndclass.hInstance = NULL;
	wndclass.hIcon = LoadIconW(NULL, IDI_APPLICATION);
	wndclass.hCursor = LoadCursorW(NULL, IDC_ARROW);
	wndclass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wndclass.lpszMenuName = NULL;
	wndclass.lpszClassName = WCH_title_window.c_str();
	RegisterClassW(&wndclass);
	WCH_handle_tray = CreateWindowExW(WS_EX_TOOLWINDOW, WCH_title_window.c_str(), WCH_title_window.c_str(), WS_POPUP, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, NULL, NULL, NULL, NULL);
	ShowWindow(WCH_handle_tray, SW_HIDE);
	UpdateWindow(WCH_handle_tray);
	RegisterHotKey(WCH_handle_tray, WCH_HOTKEY_SHOW, MOD_CONTROL, VK_DOWN);
	while (GetMessageW(&msg, NULL, 0, 0)) {
		TranslateMessage(&msg);
		DispatchMessageW(&msg);
	}
}

void WCH_check_task_loop() {
	// Check if the running task is in the task list. (Another thread)
	while (WCH_is_focus && !WCH_program_end) {
		for (auto it = WCH_list_task.begin(); it != WCH_list_task.end(); it++) {
			if (WCH_TaskKill(*it)) {
				SPDLOG_INFO(fmt::format(L"Successfully killed \"{}\"", *it));
			} else {
				SPDLOG_INFO(fmt::format(L"Failed to kill \"{}\"", *it));
			}
		}
		WCH_Sleep(stoi(StrToWstr(WCH_Settings["FocusKillInterval"].asString())));
	}
}

void WCH_CL_Init() {
	// Initialize the command line.
BEGIN:
	std::wstring _in;
	std::wcout << StrToWstr(WCH_Settings["CommandPrompt"].asString()) << L" ";
	std::getline(std::wcin, _in);
	if (std::wcin.eof()) {
		raise(SIGINT);
	}
	WCH_list_command = WCH_split(_in);
	if (WCH_list_command.size() == 0) {
		std::wcout << std::endl;
		goto BEGIN;
	}
	transform(WCH_list_command[0].begin(), WCH_list_command[0].end(), WCH_list_command[0].begin(), ::towlower);
}
