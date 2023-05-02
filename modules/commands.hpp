/*
Web Class Helper Commands Module Header File 2.1.2
This source code file is under MIT License.
Copyright (c) 2022 - 2023 Class Tools Develop Team
Contributors: jsh-jsh ren-yc hjl2011
*/
void WCH_save();
void WCH_check_task_loop();

void WCH_open_wiki() {
	// Visit the wiki page.
	if (WCH_list_command.size() != 2) {
		WCH_InputCommandIncorrect();
		return;
	}
	std::wcout << StrToWstr(WCH_Language["JumpWiki"].asString()) << std::endl;
	_wsystem(L"START https://github.com/class-tools/Web-Class-Helper/wiki/");
}

void WCH_open_releases() {
	// Visit the releases page.
	if (WCH_list_command.size() != 2) {
		WCH_InputCommandIncorrect();
		return;
	}
	std::wcout << StrToWstr(WCH_Language["JumpReleases"].asString()) << std::endl;
	_wsystem(L"START https://github.com/class-tools/Web-Class-Helper/releases/");
}

void WCH_open_code() {
	// Visit the code page (main page).
	if (WCH_list_command.size() != 2) {
		WCH_InputCommandIncorrect();
		return;
	}
	std::wcout << StrToWstr(WCH_Language["JumpCode"].asString()) << std::endl;
	_wsystem(L"START https://github.com/class-tools/Web-Class-Helper/");
}

void WCH_open_screenshot() {
	// Open screenshot save folder.
	if (WCH_list_command.size() != 2) {
		WCH_InputCommandIncorrect();
		return;
	}
	std::wcout << StrToWstr(WCH_Language["OpenScreenshotSaveFolder"].asString()) << std::endl;
	_wsystem((L"START \"\" \"" + StrToWstr(WCH_Settings["ScreenshotSavePath"].asString()) + L"\"").c_str());
}

void WCH_open() {
	// Open series command input.
	if (WCH_list_command.size() == 1) {
		WCH_InputCommandIncorrect();
		return;
	}
	transform(WCH_list_command[1].begin(), WCH_list_command[1].end(), WCH_list_command[1].begin(), ::towlower);
	if (WCH_list_command[1] == L"wiki") {
		WCH_open_wiki();
	} else if (WCH_list_command[1] == L"releases") {
		WCH_open_releases();
	} else if (WCH_list_command[1] == L"code") {
		WCH_open_code();
	} else if (WCH_list_command[1] == L"screenshot") {
		WCH_open_screenshot();
	} else {
		WCH_InputCommandIncorrect();
	}
}

void WCH_clear() {
	// Clear console information.
	if (WCH_list_command.size() != 1) {
		WCH_InputCommandIncorrect();
		return;
	}
	system("CLS");
	std::wcout << WCH_title_window << std::endl;
	std::wcout << StrToWstr(WCH_Language["Start"].asString()) << std::endl;
}

void WCH_exit() {
	// Exit.
	if (WCH_list_command.size() != 1) {
		WCH_InputCommandIncorrect();
		return;
	}
	WCH_save();
	SPDLOG_INFO(fmt::format(L"Exiting \"Web Class Helper {}\"", WCH_version));
	WCH_cmd_line = false;
	WCH_program_end = true;
	WCH_CheckAndDeleteFile(WCH_path_temp + L"\\WCH_SYSTEM_NORMAL.tmp");
	WCH_CheckAndDeleteFile(WCH_path_temp + L"\\WCH_SYSTEM_ERROR.tmp");
	WCH_CheckAndDeleteFile(WCH_path_temp + L"\\WCH_UPD.tmp");
	WCH_CheckAndDeleteFile(WCH_path_temp + L"\\WCH_TRANS.tmp");
	WCH_CheckAndDeleteFile(WCH_path_temp + L"\\WCH_OW.tmp");
	WCH_CheckAndDeleteFile(WCH_path_temp + L"\\WCH_FATE.tmp");
	WCH_CheckAndDeleteFile(WCH_path_temp + L"\\WCH_IDENT.tmp");
	WCH_CheckAndDeleteFile(WCH_path_temp + L"\\WCH_HASH.tmp");
	WCH_CheckAndDeleteFile(WCH_path_temp + L"\\WCH_PWSH.tmp");
	SendMessageW(WCH_handle_tray, WM_DESTROY, NULL, NULL);
	_exit(0);
}

void WCH_restart() {
	// Restart the program.
	if (WCH_list_command.size() != 1) {
		WCH_InputCommandIncorrect();
		return;
	}
	_wsystem((L"START \"\" \"" + WCH_path_exec + L"\"").c_str());
	WCH_exit();
}

void WCH_hide() {
	// Hide the window.
	if (WCH_list_command.size() != 1) {
		WCH_InputCommandIncorrect();
		return;
	}
	WCH_SetWindowStatus(false);
}

void WCH_update() {
	// Visit the website to update the program.
	if (WCH_list_command.size() != 1) {
		WCH_InputCommandIncorrect();
		return;
	}
	try {
		std::wcout << StrToWstr(WCH_Language["CheckUpdate"].asString()) << std::endl;
		std::wstring FilePath = WCH_path_temp + L"\\WCH_UPD.tmp";
		std::wstring url = L"https://class-tools.github.io/update/WCH?";
		std::random_device rd;
		std::mt19937 mtrand(rd());
		url.append(std::to_wstring(mtrand()));
		URLDownloadToFileW(0, url.c_str(), FilePath.c_str(), 0, 0);
		std::wstring res;
		wfin.open(FilePath);
		std::getline(wfin, res);
		wfin.close();
		if (WCH_FileIsBlank(FilePath)) {
			throw std::runtime_error("");
		}
		if (WCH_GetVersion(WCH_VER_MAIN) < WCH_GetVersion(res)) {
			std::wcout << StrToWstr(WCH_Language["FindUpdate"].asString()) << std::endl;
			_wsystem(L"START https://github.com/class-tools/Web-Class-Helper/releases/latest/");
			SPDLOG_INFO(fmt::format(L"Updating to version \"{}\"", res));
		} else {
			std::wcout << StrToWstr(WCH_Language["NoUpdate"].asString()) << std::endl;
			SPDLOG_INFO(fmt::format(L"Program version equals or is greater than \"{}\"", res));
		}
		DeleteFileW(FilePath.c_str());
	} catch (...) {
		WCH_NetworkError();
		return;
	}
}

void WCH_license() {
	// Print the license.
	if (WCH_list_command.size() != 1) {
		WCH_InputCommandIncorrect();
		return;
	}
	wfin.open(WCH_GetExecDir() + L"\\LICENSE");
	if (!wfin.is_open()) {
		WCH_FileProcessingFailed();
		return;
	}
	std::wstring _res;
	while (std::getline(wfin, _res)) {
		std::wcout << _res << std::endl;
	}
	wfin.close();
}

void WCH_sysinfo() {
	// Get system information.
	if (WCH_list_command.size() != 1) {
		WCH_InputCommandIncorrect();
		return;
	}
	std::tuple<uint32_t, uint32_t, uint32_t> res = WCH_GetSystemVersion();
	std::wcout << fmt::format(L"{}: {}.{}.{}", StrToWstr(WCH_Language["OSVersion"].asString()), get<0>(res), get<1>(res), get<2>(res)) << std::endl;
	std::wcout << fmt::format(L"{}: {}", StrToWstr(WCH_Language["OSArchitecture"].asString()), WCH_GetSystemArchitecture()) << std::endl;
	std::wcout << fmt::format(L"{}: {}", StrToWstr(WCH_Language["ProgramArchitecture"].asString()), StrToWstr(WCH_Architecture)) << std::endl;
}

void WCH_develop_opendata() {
	// Open data folder in AppData.
	if (WCH_list_command.size() != 2) {
		WCH_InputCommandIncorrect();
		return;
	}
	_wsystem((L"EXPLORER \"" + WCH_path_data + L"\"").c_str());
}

void WCH_develop_opentemp() {
	// Open temp folder in AppData.
	if (WCH_list_command.size() != 2) {
		WCH_InputCommandIncorrect();
		return;
	}
	_wsystem((L"EXPLORER \"" + WCH_path_temp + L"\"").c_str());
}

void WCH_develop() {
	// Develop series command input.
	if (WCH_list_command.size() == 1) {
		WCH_InputCommandIncorrect();
		return;
	}
	transform(WCH_list_command[1].begin(), WCH_list_command[1].end(), WCH_list_command[1].begin(), ::towlower);
	if (WCH_list_command[1] == L"opendata") {
		WCH_develop_opendata();
	} else if (WCH_list_command[1] == L"opentemp") {
		WCH_develop_opentemp();
	} else {
		WCH_InputCommandIncorrect();
	}
}

void WCH_config_set() {
	// Set a value of settings.
	if (WCH_list_command.size() != 4) {
		WCH_InputCommandIncorrect();
		return;
	}
	std::pair<bool, std::wstring> res = WCH_CheckConfigValid(WCH_list_command[2], WCH_list_command[3]);
	if (!res.first) {
		WCH_InputCommandIncorrect();
		return;
	}
	WCH_Settings[WstrToStr(WCH_list_command[2])] = WstrToStr(WCH_list_command[3]);
	SPDLOG_INFO(fmt::format(L"The value of settings key \"{}\" has been changed to \"{}\" (Type: \"{}\")", WCH_list_command[2], WCH_list_command[3], res.second));
	for (auto it = WCH_support_settings.begin(); it != WCH_support_settings.end(); it++) {
		if (get<0>(*it) == WCH_list_command[2] && get<3>(*it)) {
			MessageBoxW(NULL, StrToWstr(WCH_Language["WillRestart"].asString()).c_str(), L"WCH WARN", MB_ICONWARNING | MB_TOPMOST);
			WCH_list_command.clear();
			WCH_list_command.push_back(L"restart");
			std::wcout << std::endl;
			WCH_restart();
		}
	}
}

void WCH_config_list() {
	// List all configs.
	if (WCH_list_command.size() != 2) {
		WCH_InputCommandIncorrect();
		return;
	}
	size_t MAXK = 0;
	size_t MAXV = 0;
	for (auto it = WCH_support_settings.begin(); it != WCH_support_settings.end(); it++) {
		MAXK = max(MAXK, get<0>(*it).size());
		MAXV = max(MAXV, StrToWstr(WCH_Settings[WstrToStr(get<0>(*it))].asString()).size());
	}
	if (MAXK == 0 && MAXV == 0) {
		return;
	}
	MAXK = max(MAXK, WCH_GetWstrDisplaySize(StrToWstr(WCH_Language["Key"].asString())));
	MAXV = max(MAXV, WCH_GetWstrDisplaySize(StrToWstr(WCH_Language["Value"].asString())));
	std::wcout << StrToWstr(WCH_Language["Key"].asString());
	WCH_PrintChar(MAXK - WCH_GetWstrDisplaySize(StrToWstr(WCH_Language["Key"].asString())), L' ');
	std::wcout << L" | " + StrToWstr(WCH_Language["Value"].asString()) << std::endl;
	WCH_PrintChar(MAXK, L'-');
	std::wcout << L" | ";
	WCH_PrintChar(MAXV, L'-');
	std::wcout << std::endl;
	for (auto it = WCH_support_settings.begin(); it != WCH_support_settings.end(); it++) {
		std::wcout << get<0>(*it);
		WCH_PrintChar(MAXK - get<0>(*it).size(), L' ');
		std::wcout << L" | " << StrToWstr(WCH_Settings[WstrToStr(get<0>(*it))].asString()) << std::endl;
	}
}

void WCH_config_wizard() {
	// Wizard to config settings.
	if (WCH_list_command.size() != 2) {
		WCH_InputCommandIncorrect();
		return;
	}
	std::wstring FilePath = WCH_GetExecDir() + L"\\resources\\" + StrToWstr(WCH_Settings["Language"].asString()) + L"\\config.json";
	Json::Value valcfg;
	SPDLOG_INFO(fmt::format(L"Reading file \"{}\"", FilePath));
	if (!WCH_CheckFileHash(FilePath, StrToWstr(WCH_Settings["Language"].asString()) == L"en-US" ? SHASUM_enUS_config : SHASUM_zhCN_config)) {
		SPDLOG_CRITICAL(fmt::format(L"Data in file \"{}\" corrupted", FilePath));
		WCH_FileProcessingFailed();
		raise(SIGINT);
	}
	fin.open(FilePath);
	std::ignore = JSON_Reader.parse(fin, valcfg);
	fin.close();
	int32_t cnt = 0;
	bool flag = false;
	for (auto it = WCH_support_settings.begin(); it != WCH_support_settings.end(); it++, cnt++) {
		std::wcout << fmt::format(L"{}: {}", StrToWstr(valcfg["Title"][0].asString()), get<0>(*it)) << std::endl;
		std::wcout << fmt::format(L"{}: {}", StrToWstr(valcfg["Title"][1].asString()), get<1>(*it)) << std::endl;
		std::wcout << fmt::format(L"{}: {}", StrToWstr(valcfg["Title"][2].asString()), StrToWstr(valcfg["Content"][cnt].asString())) << std::endl;
		std::wcout << fmt::format(L"{}: {}", StrToWstr(valcfg["Title"][4].asString()), StrToWstr(WCH_Language[get<3>(*it) ? "Yes" : "No"].asString())) << std::endl;
		std::wcout << fmt::format(L"{}: {}", StrToWstr(valcfg["Title"][3].asString()), get<2>(*it)) << std::endl;
		std::wcout << fmt::format(L"{}: {}", StrToWstr(valcfg["Title"][5].asString()), StrToWstr(WCH_Settings[WstrToStr(get<0>(*it))].asString())) << std::endl;
		if (WCH_choice_settings.find(get<0>(*it)) != WCH_choice_settings.end()) {
			std::wcout << StrToWstr(valcfg["Title"][6].asString()) << L":" << std::endl;
			for (auto ite = WCH_choice_settings.find(get<0>(*it))->second.begin(); ite != WCH_choice_settings.find(get<0>(*it))->second.end(); ite++) {
				std::wcout << L" - " << *ite << std::endl;
			}
		}
		std::wcout << StrToWstr(WCH_Language["ConfigWizardPrompt"].asString()) << std::endl;
		std::wstring _in;
		std::wcout << StrToWstr(WCH_Settings["CommandPrompt"].asString()) << L" ";
		if (cnt == 0) {
			size_t _nlc = WCH_NewlineCount(get<2>(*it)) + (WCH_choice_settings.find(get<0>(*it)) != WCH_choice_settings.end() ? WCH_choice_settings.find(get<0>(*it))->second.size() + 1 : 0);
			CONSOLE_SCREEN_BUFFER_INFO _csbi = {};
			COORD _crd = {};
			GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &_csbi);
			_crd.X = _csbi.dwCursorPosition.X;
			_crd.Y = (int16_t)_nlc + 7;
			WCH_PrintChar((size_t)_csbi.dwSize.Y - _nlc - 8, L'\n');
			SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), _crd);
		}
	BEGIN:
		std::getline(std::wcin, _in);
		if (std::wcin.eof()) {
			raise(SIGINT);
		}
		if (_in.size() != 0) {
			std::pair<bool, std::wstring> res = WCH_CheckConfigValid(get<0>(*it), _in);
			if (!res.first) {
				WCH_InputCommandIncorrect();
				std::wcout << std::endl;
				std::wcout << StrToWstr(WCH_Settings["CommandPrompt"].asString()) << L" ";
				goto BEGIN;
			}
			WCH_Settings[WstrToStr(get<0>(*it))] = WstrToStr(_in);
			SPDLOG_INFO(fmt::format(L"The value of settings key \"{}\" has been changed to \"{}\" (Type: \"{}\")", get<0>(*it), _in, res.second));
			for (auto ite = WCH_support_settings.begin(); ite != WCH_support_settings.end(); ite++) {
				if (get<0>(*ite) == get<0>(*it) && get<3>(*ite)) {
					flag = true;
				}
			}
		}
		WCH_ClearConsole();
	}
	if (flag) {
		MessageBoxW(NULL, StrToWstr(WCH_Language["WillRestart"].asString()).c_str(), L"WCH WARN", MB_ICONWARNING | MB_TOPMOST);
		WCH_list_command.clear();
		WCH_list_command.push_back(L"restart");
		std::wcout << std::endl;
		WCH_restart();
	}
}

void WCH_config_reset() {
	// Reset configuration to default.
	if (WCH_list_command.size() != 2) {
		WCH_InputCommandIncorrect();
		return;
	}
	bool flag = false;
	for (auto it = WCH_support_settings.begin(); it != WCH_support_settings.end(); it++) {
		if (get<0>(*it) == L"Language" && StrToWstr(WCH_Settings[WstrToStr(get<0>(*it))].asString()) != get<2>(*it)) {
			flag = true;
		}
		WCH_Settings[WstrToStr(get<0>(*it))] = WstrToStr(get<2>(*it));
	}
	if (flag) {
		MessageBoxW(NULL, StrToWstr(WCH_Language["WillRestart"].asString()).c_str(), L"WCH WARN", MB_ICONWARNING | MB_TOPMOST);
		WCH_list_command.clear();
		WCH_list_command.push_back(L"restart");
		std::wcout << std::endl;
		WCH_restart();
	}
}

void WCH_config() {
	// Config series command input.
	if (WCH_list_command.size() == 1) {
		WCH_InputCommandIncorrect();
		return;
	}
	transform(WCH_list_command[1].begin(), WCH_list_command[1].end(), WCH_list_command[1].begin(), ::towlower);
	if (WCH_list_command[1] == L"std::set") {
		WCH_config_set();
	} else if (WCH_list_command[1] == L"list") {
		WCH_config_list();
	} else if (WCH_list_command[1] == L"wizard") {
		WCH_config_wizard();
	} else if (WCH_list_command[1] == L"reset") {
		WCH_config_reset();
	} else {
		WCH_InputCommandIncorrect();
	}
}

void WCH_clock_add() {
	// Add a new clock.
	if (WCH_list_command.size() != 5) {
		WCH_InputCommandIncorrect();
		return;
	}
	try {
		int32_t h = stoi(WCH_list_command[2]);
		int32_t m = stoi(WCH_list_command[3]);
		std::wstring Tname = WCH_list_command[4];
		if (WCH_list_command.size() < 5 || h > 24 || m >= 60 || h < 1 || m < 0) {
			WCH_InputCommandIncorrect();
			return;
		} else {
			bool flag = false;
			for (auto it = WCH_list_clock.begin(); it != WCH_list_clock.end(); it++) {
				if (get<0>(*it) == h && get<1>(*it) == m && get<2>(*it) == Tname) {
					flag = true;
					break;
				}
			}
			if (!flag) {
				WCH_num_clock++;
				WCH_list_clock.insert(make_tuple(h, m, Tname));
			} else {
				throw std::runtime_error("");
			}
		}
	} catch (...) {
		WCH_InputCommandIncorrect();
	}
}

void WCH_clock_delete() {
	// Delete a clock.
	if (WCH_list_command.size() != 5) {
		WCH_InputCommandIncorrect();
		return;
	}
	try {
		int32_t h = stoi(WCH_list_command[2]);
		int32_t m = stoi(WCH_list_command[3]);
		std::wstring Tname = WCH_list_command[4];
		bool flag = false;
		for (auto it = WCH_list_clock.begin(); it != WCH_list_clock.end(); it++) {
			if (get<0>(*it) == h && get<1>(*it) == m && get<2>(*it) == Tname) {
				WCH_list_clock.erase(it);
				flag = true;
				WCH_num_clock--;
				break;
			}
		}
		if (!flag) {
			throw std::runtime_error("");
		}
	} catch (...) {
		WCH_InputCommandIncorrect();
	}
}

void WCH_clock_clear() {
	// Clear clock list.
	if (WCH_list_command.size() != 2) {
		WCH_InputCommandIncorrect();
		return;
	}
	WCH_list_clock.clear();
	WCH_num_clock = 0;
}

void WCH_clock_list() {
	// List all clocks.
	if (WCH_list_command.size() != 2) {
		WCH_InputCommandIncorrect();
		return;
	}
	size_t MAXH = 0;
	size_t MAXM = 0;
	size_t MAXT = 0;
	for (auto it = WCH_list_clock.begin(); it != WCH_list_clock.end(); it++) {
		MAXH = max(MAXH, WCH_GetNumDigits((size_t)get<0>(*it)));
		MAXM = max(MAXM, WCH_GetNumDigits((size_t)get<1>(*it)));
		MAXT = max(MAXT, WCH_GetWstrDisplaySize(get<2>(*it)));
	}
	if (MAXH == 0 && MAXM == 0 && MAXT == 0) {
		return;
	}
	MAXH = max(MAXH, WCH_GetWstrDisplaySize(StrToWstr(WCH_Language["Hour"].asString())));
	MAXM = max(MAXM, WCH_GetWstrDisplaySize(StrToWstr(WCH_Language["Minute"].asString())));
	MAXT = max(MAXT, WCH_GetWstrDisplaySize(StrToWstr(WCH_Language["Name"].asString())));
	std::wcout << StrToWstr(WCH_Language["Hour"].asString());
	WCH_PrintChar(MAXH - WCH_GetWstrDisplaySize(StrToWstr(WCH_Language["Hour"].asString())), L' ');
	std::wcout << L" | " + StrToWstr(WCH_Language["Minute"].asString());
	WCH_PrintChar(MAXM - WCH_GetWstrDisplaySize(StrToWstr(WCH_Language["Minute"].asString())), L' ');
	std::wcout << L" | " + StrToWstr(WCH_Language["Name"].asString()) << std::endl;
	WCH_PrintChar(MAXH, L'-');
	std::wcout << L" | ";
	WCH_PrintChar(MAXM, L'-');
	std::wcout << L" | ";
	WCH_PrintChar(MAXT, L'-');
	std::wcout << std::endl;
	for (auto it = WCH_list_clock.begin(); it != WCH_list_clock.end(); it++) {
		std::wcout << get<0>(*it);
		WCH_PrintChar(MAXH - WCH_GetNumDigits(get<0>(*it)), L' ');
		std::wcout << L" | " << get<1>(*it);
		WCH_PrintChar(MAXM - WCH_GetNumDigits(get<1>(*it)), L' ');
		std::wcout << L" | " << get<2>(*it) << std::endl;
	}
}

void WCH_clock() {
	// Clock series command input.
	if (WCH_list_command.size() == 1) {
		WCH_InputCommandIncorrect();
		return;
	}
	transform(WCH_list_command[1].begin(), WCH_list_command[1].end(), WCH_list_command[1].begin(), ::towlower);
	if (WCH_list_command[1] == L"add") {
		WCH_clock_add();
	} else if (WCH_list_command[1] == L"delete") {
		WCH_clock_delete();
	} else if (WCH_list_command[1] == L"clear") {
		WCH_clock_clear();
	} else if (WCH_list_command[1] == L"list") {
		WCH_clock_list();
	} else {
		WCH_InputCommandIncorrect();
	}
}

void WCH_task_add() {
	// Add a new task.
	if (WCH_list_command.size() != 3) {
		WCH_InputCommandIncorrect();
		return;
	}
	std::wstring task = WCH_list_command[2];
	if (WCH_list_task.find(task) != WCH_list_task.end()) {
		WCH_InputCommandIncorrect();
	} else {
		WCH_list_task.insert(task);
		WCH_num_task++;
	}
}

void WCH_task_delete() {
	// Delete a task.
	if (WCH_list_command.size() != 3) {
		WCH_InputCommandIncorrect();
		return;
	}
	std::wstring task = WCH_list_command[2];
	if (WCH_list_task.find(task) == WCH_list_task.end()) {
		WCH_InputCommandIncorrect();
	} else {
		WCH_list_task.erase(task);
		WCH_num_task--;
	}
}

void WCH_task_clear() {
	// Clear task list.
	if (WCH_list_command.size() != 2) {
		WCH_InputCommandIncorrect();
		return;
	}
	WCH_list_task.clear();
	WCH_num_task = 0;
}

void WCH_task_list() {
	// List all tasks.
	if (WCH_list_command.size() != 2) {
		WCH_InputCommandIncorrect();
		return;
	}
	size_t MAX = 0;
	for (auto it = WCH_list_task.begin(); it != WCH_list_task.end(); it++) {
		MAX = max(MAX, WCH_GetWstrDisplaySize(*it));
	}
	if (MAX == 0) {
		return;
	}
	MAX = max(MAX, WCH_GetWstrDisplaySize(StrToWstr(WCH_Language["ProcessName"].asString())));
	std::wcout << StrToWstr(WCH_Language["ProcessName"].asString()) << std::endl;
	WCH_PrintChar(MAX, L'-');
	std::wcout << std::endl;
	for (auto it = WCH_list_task.begin(); it != WCH_list_task.end(); it++) {
		std::wcout << *it << std::endl;
	}
}

void WCH_task() {
	// Task series command input.
	if (WCH_list_command.size() == 1) {
		WCH_InputCommandIncorrect();
		return;
	}
	if (WCH_list_command[1] == L"add") {
		WCH_task_add();
	} else if (WCH_list_command[1] == L"delete") {
		WCH_task_delete();
	} else if (WCH_list_command[1] == L"clear") {
		WCH_task_clear();
	} else if (WCH_list_command[1] == L"list") {
		WCH_task_list();
	} else {
		WCH_InputCommandIncorrect();
	}
}

void WCH_work_add() {
	// Add a new work.
	if (WCH_list_command.size() != 3 && WCH_list_command.size() != 4) {
		WCH_InputCommandIncorrect();
		return;
	}
	std::wstring work = WCH_list_command[2];
	std::wstring tag = WCH_list_command.size() == 3 ? StrToWstr(WCH_Language["Unclassified"].asString()) : WCH_list_command[3];
	if (WCH_list_work.find(make_pair(work, tag)) != WCH_list_work.end()) {
		WCH_InputCommandIncorrect();
	} else {
		WCH_list_work.insert(make_pair(work, tag));
		WCH_num_work++;
	}
}

void WCH_work_delete() {
	// Delete a work.
	if (WCH_list_command.size() != 3 && WCH_list_command.size() != 4) {
		WCH_InputCommandIncorrect();
		return;
	}
	std::wstring work = WCH_list_command[2];
	std::wstring tag = WCH_list_command.size() == 3 ? StrToWstr(WCH_Language["Unclassified"].asString()) : WCH_list_command[3];
	if (WCH_list_work.find(make_pair(work, tag)) == WCH_list_work.end()) {
		WCH_InputCommandIncorrect();
	} else {
		WCH_list_work.erase(make_pair(work, tag));
		WCH_num_work--;
	}
}

void WCH_work_clear() {
	// Clear work list.
	if (WCH_list_command.size() != 2) {
		WCH_InputCommandIncorrect();
		return;
	}
	WCH_list_work.clear();
	WCH_num_work = 0;
}

void WCH_work_list() {
	// List all works.
	if (WCH_list_command.size() != 2) {
		WCH_InputCommandIncorrect();
		return;
	}
	size_t MAXN = 0;
	size_t MAXT = 0;
	for (auto it = WCH_list_work.begin(); it != WCH_list_work.end(); it++) {
		MAXN = max(MAXN, WCH_GetWstrDisplaySize(it->first));
		MAXT = max(MAXT, WCH_GetWstrDisplaySize(it->second));
	}
	if (MAXN == 0 && MAXT == 0) {
		return;
	}
	MAXN = max(MAXN, WCH_GetWstrDisplaySize(StrToWstr(WCH_Language["Name"].asString())));
	MAXT = max(MAXT, WCH_GetWstrDisplaySize(StrToWstr(WCH_Language["Tag"].asString())));
	std::wcout << StrToWstr(WCH_Language["Name"].asString());
	WCH_PrintChar(MAXN - WCH_GetWstrDisplaySize(StrToWstr(WCH_Language["Name"].asString())), L' ');
	std::wcout << L" | " + StrToWstr(WCH_Language["Tag"].asString()) << std::endl;
	WCH_PrintChar(MAXN, L'-');
	std::wcout << L" | ";
	WCH_PrintChar(MAXT, L'-');
	std::wcout << std::endl;
	for (auto it = WCH_list_work.begin(); it != WCH_list_work.end(); it++) {
		std::wcout << it->first;
		WCH_PrintChar(MAXN - WCH_GetWstrDisplaySize(it->first), L' ');
		std::wcout << L" | " << it->second << std::endl;
	}
}

void WCH_work() {
	// Work series command input.
	if (WCH_list_command.size() == 1) {
		WCH_InputCommandIncorrect();
		return;
	}
	transform(WCH_list_command[1].begin(), WCH_list_command[1].end(), WCH_list_command[1].begin(), ::towlower);
	if (WCH_list_command[1] == L"add") {
		WCH_work_add();
	} else if (WCH_list_command[1] == L"delete") {
		WCH_work_delete();
	} else if (WCH_list_command[1] == L"clear") {
		WCH_work_clear();
	} else if (WCH_list_command[1] == L"list") {
		WCH_work_list();
	} else {
		WCH_InputCommandIncorrect();
	}
}

void WCH_game() {
	// Guessing game.
	if (WCH_list_command.size() != 1) {
		WCH_InputCommandIncorrect();
		return;
	}
	std::random_device rd;
	std::mt19937 mtrand(rd());
	int32_t inp = 0, ans = mtrand() % 10000 + 1;
	bool flag = true;
	std::wstring z = L"0";
	std::vector<std::wstring> zv;
	zv.push_back(L"0");
	while (true) {
		std::wcout << StrToWstr(WCH_Language["InputNumber"].asString());
		std::getline(std::wcin, z);
		zv = WCH_split(z);
		if (zv.size() == 0) {
			flag = false;
			break;
		}
		try {
			inp = stoi(zv[0]);
		} catch (...) {
			std::wcout << StrToWstr(WCH_Language["NumberOutOfRange"].asString()) << std::endl;
			continue;
		}
		if (inp <= 0 || inp > 10000) {
			std::wcout << StrToWstr(WCH_Language["NumberOutOfRange"].asString()) << std::endl;
			continue;
		}
		if (inp > ans) {
			std::wcout << StrToWstr(WCH_Language["NumberSmaller"].asString()) << std::endl;
		} else if (inp < ans) {
			std::wcout << StrToWstr(WCH_Language["NumberBigger"].asString()) << std::endl;
		} else {
			break;
		}
	}
	std::wcout << StrToWstr(WCH_Language["NumberAnswer"].asString()) << L" " << ans;
	if (flag) {
		std::wcout << StrToWstr(WCH_Language["NumberWin"].asString()) << std::endl;
	} else {
		std::wcout << StrToWstr(WCH_Language["NumberLose"].asString()) << std::endl;
	}
}

void WCH_prtscn() {
	// A sequence of function to make a screenshot.
	if (WCH_list_command.size() != 1) {
		WCH_InputCommandIncorrect();
		return;
	}
	WCH_SetWindowStatus(false);
	WCH_Sleep(500);
	WCH_PutPicture();
	WCH_SaveImg();
	WCH_Sleep(500);
	WCH_SetWindowStatus(true);
	std::wcout << StrToWstr(WCH_Language["Prtscn"].asString()) << std::endl;
}

void WCH_countdown() {
	// Start a count down timer.
	if (WCH_list_command.size() != 4) {
		WCH_InputCommandIncorrect();
		return;
	}
	try {
		int32_t h = stoi(WCH_list_command[1]);
		int32_t m = stoi(WCH_list_command[2]);
		int32_t s = stoi(WCH_list_command[3]);
		if ((h == 0 && m == 0 && s == 0) || (h < 0 || m < 0 || s < 0) || (h >= 24 || m >= 60 || s >= 60)) {
			throw std::runtime_error("");
		}
		WCH_is_countdown = true;
		std::wcout << StrToWstr(WCH_Language["CountDown"].asString()) << std::endl;
		WCH_progress_bar_duration = h * 3600 + m * 60 + s;
		WCH_ProgressBar();
		if (StrToWstr(WCH_Settings["CountDownSoundPrompt"].asString()) == L"True") {
			std::wcout << L"\a";
		}
		WCH_is_countdown = false;
	} catch (...) {
		WCH_InputCommandIncorrect();
		WCH_is_countdown = false;
	}
}

void WCH_focus() {
	// Enable focus function.
	if (WCH_list_command.size() != 1) {
		WCH_InputCommandIncorrect();
		return;
	}
	if (MessageBoxW(NULL, StrToWstr(WCH_Language["Focus"].asString()).c_str(), L"WCH WARN", MB_ICONWARNING | MB_YESNO | MB_TOPMOST) == IDYES) {
		WCH_SetWindowStatus(false);
		WCH_is_focus = true;
		WCH_Sleep(500);
		WCH_SetTrayStatus(false);
		WCH_Sleep(500);
		std::thread T(WCH_check_task_loop);
		T.detach();
	}
}

void WCH_trans() {
	// Translate string between Chinese / English.
	if (WCH_list_command.size() != 2) {
		WCH_InputCommandIncorrect();
		return;
	}
	try {
		std::wstring FilePath = WCH_path_temp + L"\\WCH_TRANS.tmp";
		URLDownloadToFileW(NULL, (L"http://fanyi.youdao.com/openapi.do?keyfrom=xujiangtao&key=1490852988&type=data&doctype=json&version=1.1&only=translate&q=" + StrToWstr(UrlEncode(WstrToStr((WCH_list_command[1]))))).c_str(), FilePath.c_str(), 0, NULL);
		if (WCH_FileIsBlank(FilePath)) {
			throw std::runtime_error("");
		}
		Json::Value val;
		fin.open(FilePath);
		if (!JSON_Reader.parse(fin, val)) {
			throw std::runtime_error("");
		}
		if (!val.isMember("translation")) {
			throw std::runtime_error("");
		}
		std::wcout << StrToWstr(val["translation"][0].asString()) << std::endl;
		fin.close();
		DeleteFileW(FilePath.c_str());
	} catch (...) {
		WCH_NetworkError();
		return;
	}
}

void WCH_ow() {
	// Get a random sentence.
	if (WCH_list_command.size() != 1) {
		WCH_InputCommandIncorrect();
		return;
	}
	try {
		std::wstring FilePath = WCH_path_temp + L"\\WCH_OW.tmp";
		URLDownloadToFileW(0, L"https://v1.hitokoto.cn/?encode=text", FilePath.c_str(), 0, 0);
		if (WCH_FileIsBlank(FilePath)) {
			throw std::runtime_error("");
		}
		fin.open(FilePath);
		std::string res;
		std::getline(fin, res);
		std::wcout << StrToWstr(res) << std::endl;
		fin.close();
		DeleteFileW(FilePath.c_str());
	} catch (...) {
		WCH_NetworkError();
		return;
	}
}

void WCH_fate() {
	// Get fate.
	if (WCH_list_command.size() != 1) {
		WCH_InputCommandIncorrect();
		return;
	}
	try {
		std::wstring FilePath = WCH_path_temp + L"\\WCH_FATE.tmp";
		URLDownloadToFileW(NULL, (L"https://api.fanlisky.cn/api/qr-fortune/get/" + StrToWstr(UrlEncode(WstrToStr((WCH_GetUniIdent()))))).c_str(), FilePath.c_str(), 0, NULL);
		if (WCH_FileIsBlank(FilePath)) {
			throw std::runtime_error("");
		}
		Json::Value val;
		fin.open(FilePath);
		if (!JSON_Reader.parse(fin, val)) {
			throw std::runtime_error("");
		}
		if (!val.isMember("data")) {
			throw std::runtime_error("");
		}
		if (val["status"].asString() == "20011") {
			std::wcout << StrToWstr(val["data"]["fortuneSummary"].asString()) << std::endl;
			std::wcout << StrToWstr(val["data"]["luckyStar"].asString()) << std::endl;
			std::wcout << StrToWstr(val["data"]["signText"].asString()) << std::endl;
			std::wcout << StrToWstr(val["data"]["unSignText"].asString()) << std::endl;
		}
		fin.close();
		DeleteFileW(FilePath.c_str());
	} catch (...) {
		WCH_NetworkError();
		return;
	}
}

void WCH_time() {
	// Print current time.
	if (WCH_list_command.size() != 1) {
		WCH_InputCommandIncorrect();
		return;
	}
	WCH_Time q = WCH_GetTime();
	std::wcout << fmt::format(L"{:04}/{:02}/{:02} {:02}:{:02}:{:02}", q.Year, q.Month, q.Day, q.Hour, q.Minute, q.Second) << std::endl;
}

void WCH_help() {
	// Print help information.
	if (WCH_list_command.size() > 2) {
		WCH_InputCommandIncorrect();
		return;
	}
	std::wstring FilePath = WCH_GetExecDir() + L"\\resources\\" + StrToWstr(WCH_Settings["Language"].asString()) + L"\\help.json";
	Json::Value val;
	SPDLOG_INFO(fmt::format(L"Reading file \"{}\"", FilePath));
	if (!WCH_CheckFileHash(FilePath, StrToWstr(WCH_Settings["Language"].asString()) == L"en-US" ? SHASUM_enUS_help : SHASUM_zhCN_help)) {
		SPDLOG_CRITICAL(fmt::format(L"Data in file \"{}\" corrupted", FilePath));
		WCH_FileProcessingFailed();
		raise(SIGINT);
	}
	fin.open(FilePath);
	std::ignore = JSON_Reader.parse(fin, val);
	fin.close();
	if (WCH_list_command.size() == 1) {
		for (auto it = val["index"].begin(); it != val["index"].end(); it++) {
			std::wcout << StrToWstr((*it).asString()) << std::endl;
		}
	} else {
		if (WCH_support_command.find(WCH_list_command[1]) == WCH_support_command.end()) {
			WCH_InputCommandIncorrect();
			return;
		}
		transform(WCH_list_command[1].begin(), WCH_list_command[1].end(), WCH_list_command[1].begin(), ::towlower);
		for (auto it = val[WstrToStr(WCH_list_command[1])].begin(); it != val[WstrToStr(WCH_list_command[1])].end(); it++) {
			std::wcout << StrToWstr((*it).asString()) << std::endl;
		}
		if (WCH_list_command[1] == L"config") {
			FilePath = WCH_GetExecDir() + L"\\resources\\" + StrToWstr(WCH_Settings["Language"].asString()) + L"\\config.json";
			Json::Value valcfg;
			SPDLOG_INFO(fmt::format(L"Reading file \"{}\"", FilePath));
			if (!WCH_CheckFileHash(FilePath, StrToWstr(WCH_Settings["Language"].asString()) == L"en-US" ? SHASUM_enUS_config : SHASUM_zhCN_config)) {
				SPDLOG_CRITICAL(fmt::format(L"Data in file \"{}\" corrupted", FilePath));
				WCH_FileProcessingFailed();
				raise(SIGINT);
			}
			fin.open(FilePath);
			std::ignore = JSON_Reader.parse(fin, valcfg);
			fin.close();
			size_t MAXKN = 0;
			size_t MAXVT = 0;
			size_t MAXDE = 0;
			size_t MAXDV = 0;
			size_t MAXRR = 0;
			int32_t cnt = 0;
			for (auto it = WCH_support_settings.begin(); it != WCH_support_settings.end(); it++, cnt++) {
				MAXKN = max(MAXKN, WCH_GetWstrDisplaySize(get<0>(*it)));
				MAXVT = max(MAXVT, WCH_GetWstrDisplaySize(get<1>(*it)));
				MAXDE = max(MAXDE, WCH_GetWstrDisplaySize(StrToWstr(valcfg["Content"][cnt].asString())));
				MAXDV = max(MAXDV, WCH_GetWstrDisplaySize(get<2>(*it)));
				MAXRR = max(MAXRR, WCH_GetWstrDisplaySize(StrToWstr(WCH_Language[get<3>(*it) ? "Yes" : "No"].asString())));
			}
			if (MAXKN == 0 && MAXVT == 0 && MAXDE == 0 && MAXDV == 0 && MAXRR == 0) {
				return;
			}
			MAXKN = max(MAXKN, WCH_GetWstrDisplaySize(StrToWstr(valcfg["Title"][0].asString())));
			MAXVT = max(MAXVT, WCH_GetWstrDisplaySize(StrToWstr(valcfg["Title"][1].asString())));
			MAXDE = max(MAXDE, WCH_GetWstrDisplaySize(StrToWstr(valcfg["Title"][2].asString())));
			MAXDV = max(MAXDV, WCH_GetWstrDisplaySize(StrToWstr(valcfg["Title"][3].asString())));
			MAXRR = max(MAXRR, WCH_GetWstrDisplaySize(StrToWstr(valcfg["Title"][4].asString())));
			std::wcout << StrToWstr(valcfg["Title"][0].asString());
			WCH_PrintChar(MAXKN - WCH_GetWstrDisplaySize(StrToWstr(valcfg["Title"][0].asString())), L' ');
			std::wcout << L" | " + StrToWstr(valcfg["Title"][1].asString());
			WCH_PrintChar(MAXVT - WCH_GetWstrDisplaySize(StrToWstr(valcfg["Title"][1].asString())), L' ');
			std::wcout << L" | " + StrToWstr(valcfg["Title"][2].asString());
			WCH_PrintChar(MAXDE - WCH_GetWstrDisplaySize(StrToWstr(valcfg["Title"][2].asString())), L' ');
			std::wcout << L" | " + StrToWstr(valcfg["Title"][3].asString());
			WCH_PrintChar(MAXDV - WCH_GetWstrDisplaySize(StrToWstr(valcfg["Title"][3].asString())), L' ');
			std::wcout << L" | " + StrToWstr(valcfg["Title"][4].asString()) << std::endl;
			WCH_PrintChar(MAXKN, L'-');
			std::wcout << L" | ";
			WCH_PrintChar(MAXVT, L'-');
			std::wcout << L" | ";
			WCH_PrintChar(MAXDE, L'-');
			std::wcout << L" | ";
			WCH_PrintChar(MAXDV, L'-');
			std::wcout << L" | ";
			WCH_PrintChar(MAXRR, L'-');
			std::wcout << std::endl;
			cnt = 0;
			for (auto it = WCH_support_settings.begin(); it != WCH_support_settings.end(); it++, cnt++) {
				std::wcout << get<0>(*it);
				WCH_PrintChar(MAXKN - WCH_GetWstrDisplaySize(get<0>(*it)), L' ');
				std::wcout << L" | " << get<1>(*it);
				WCH_PrintChar(MAXVT - WCH_GetWstrDisplaySize(get<1>(*it)), L' ');
				std::wcout << L" | " << StrToWstr(valcfg["Content"][cnt].asString());
				WCH_PrintChar(MAXDE - WCH_GetWstrDisplaySize(StrToWstr(valcfg["Content"][cnt].asString())), L' ');
				std::wcout << L" | " << get<2>(*it);
				WCH_PrintChar(MAXDV - WCH_GetWstrDisplaySize(get<2>(*it)), L' ');
				std::wcout << L" | " << StrToWstr(WCH_Language[get<3>(*it) ? "Yes" : "No"].asString()) << std::endl;
			}
		}
	}
}
