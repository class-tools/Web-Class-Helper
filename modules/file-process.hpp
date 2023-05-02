/*
Web Class Helper File Processing Module Header File 2.1.2
This source code file is under MIT License.
Copyright (c) 2022 - 2023 Class Tools Develop Team
Contributors: jsh-jsh ren-yc
*/
void WCH_read_clock() {
	// Read clock data.
	WCH_Time q = WCH_GetTime();
	std::wstring NowWeekDay = WCH_list_weekday[(q.Day + 2 * q.Month + 3 * (q.Month + 1) / 5 + q.Year + q.Year / 4 - q.Year / 100 + q.Year / 400 + 1) % 7];
	std::wstring FilePath = WCH_path_data + L"\\data\\clock.json";
	Json::Value val;
	fin.open(FilePath);
	if (!fin.is_open()) {
		return;
	}
	if (JSON_Reader.parse(fin, val)) {
		try {
			SPDLOG_INFO(fmt::format(L"Reading file \"{}\"", FilePath));
			WCH_num_clock = val[WstrToStr(NowWeekDay)].size();
			for (int32_t i = 0; i < WCH_num_clock; i++) {
				WCH_list_clock.insert(make_tuple(val[WstrToStr(NowWeekDay)][i][0].asInt(), val[WstrToStr(NowWeekDay)][i][1].asInt(), StrToWstr(val[WstrToStr(NowWeekDay)][i][2].asString())));
			}
		} catch (...) {
			goto ERR;
		}
	} else {
	ERR:
		SPDLOG_WARN(fmt::format(L"Data in file \"{}\" corrupted", FilePath));
	}
	fin.close();
}

void WCH_read_task() {
	// Read task data.
	std::wstring FilePath = WCH_path_data + L"\\data\\task.json";
	Json::Value val;
	fin.open(FilePath);
	if (!fin.is_open()) {
		return;
	}
	if (JSON_Reader.parse(fin, val)) {
		try {
			SPDLOG_INFO(fmt::format(L"Reading file \"{}\"", FilePath));
			WCH_num_task = val.size();
			for (int32_t i = 0; i < WCH_num_task; i++) {
				WCH_list_task.insert(StrToWstr(val[i].asString()));
			}
		} catch (...) {
			goto ERR;
		}
	} else {
	ERR:
		SPDLOG_WARN(fmt::format(L"Data in file \"{}\" corrupted", FilePath));
	}
	fin.close();
}

void WCH_read_work() {
	// Read work data.
	std::wstring FilePath = WCH_path_data + L"\\data\\work.json";
	Json::Value val;
	fin.open(FilePath);
	if (!fin.is_open()) {
		return;
	}
	if (JSON_Reader.parse(fin, val)) {
		try {
			SPDLOG_INFO(fmt::format(L"Reading file \"{}\"", FilePath));
			WCH_num_work = val.size();
			for (int32_t i = 0; i < WCH_num_work; i++) {
				WCH_list_work.insert(make_pair(StrToWstr(val[i][0].asString()), StrToWstr(val[i][1].asString())));
			}
		} catch (...) {
			goto ERR;
		}
	} else {
	ERR:
		SPDLOG_WARN(fmt::format(L"Data in file \"{}\" corrupted", FilePath));
	}
	fin.close();
}

void WCH_read_settings() {
	// Read settings data.
	std::wstring FilePath = WCH_path_data + L"\\settings.json";
	fin.open(FilePath);
	if (!fin.is_open()) {
		goto ERR;
	}
	if (JSON_Reader.parse(fin, WCH_Settings)) {
		try {
			if (!WCH_pre_start) {
				SPDLOG_INFO(fmt::format(L"Reading file \"{}\"", FilePath));
			}
			if (!WCH_Settings.isMember("StartTime")) {
				throw std::runtime_error("");
			}
			for (auto it = WCH_support_settings.begin(); it != WCH_support_settings.end(); it++) {
				if (!WCH_Settings.isMember(WstrToStr(get<0>(*it)))) {
					throw std::runtime_error("");
				} else if (!WCH_CheckConfigValid(get<0>(*it), StrToWstr(WCH_Settings[WstrToStr(get<0>(*it))].asString())).first) {
					throw std::runtime_error("");
				}
			}
			if (WCH_Settings.size() != WCH_support_settings.size() + 1) {
				throw std::runtime_error("");
			}
		} catch (...) {
			goto ERR;
		}
	} else {
	ERR:
		WCH_Settings.clear();
		WCH_Settings["StartTime"] = "00000000000000";
		for (auto it = WCH_support_settings.begin(); it != WCH_support_settings.end(); it++) {
			WCH_Settings[WstrToStr(get<0>(*it))] = WstrToStr(get<2>(*it));
		}
		if (!WCH_pre_start) {
			SPDLOG_WARN(fmt::format(L"Data in file \"{}\" corrupted", FilePath));
		}
	}
	fin.close();
}

void WCH_read_language() {
	// Read language data.
	std::wstring FilePath = WCH_GetExecDir() + L"\\resources\\" + StrToWstr(WCH_Settings["Language"].asString()) + L"\\interactive.json";
	if (!WCH_CheckFileHash(FilePath, StrToWstr(WCH_Settings["Language"].asString()) == L"en-US" ? SHASUM_enUS_interactive : SHASUM_zhCN_interactive)) {
		SPDLOG_CRITICAL(fmt::format(L"Data in file \"{}\" corrupted", FilePath));
		WCH_FileProcessingFailed();
		raise(SIGINT);
	}
	fin.open(FilePath);
	std::ignore = JSON_Reader.parse(fin, WCH_Language);
	fin.close();
}

void WCH_read() {
	// Read data.
	WCH_cmd_line = false;
	WCH_read_clock();
	WCH_read_task();
	WCH_read_work();
	WCH_read_settings();
	WCH_read_language();
	WCH_cmd_line = true;
}

void WCH_save_clock() {
	// Save clock data.
	WCH_Time q = WCH_GetTime();
	std::wstring NowWeekDay = WCH_list_weekday[(q.Day + 2 * q.Month + 3 * (q.Month + 1) / 5 + q.Year + q.Year / 4 - q.Year / 100 + q.Year / 400 + 1) % 7];
	std::wstring FilePath = WCH_path_data + L"\\data\\clock.json";
	Json::Value val;
	fin.open(FilePath);
	if (fin.is_open()) {
		JSON_Reader.parse(fin, val);
	}
	fin.close();
	val[WstrToStr(NowWeekDay)].clear();
	for (auto it = WCH_list_clock.begin(); it != WCH_list_clock.end(); it++) {
		Json::Value sval;
		sval.append(get<0>(*it));
		sval.append(get<1>(*it));
		sval.append(WstrToStr(get<2>(*it)));
		val[WstrToStr(NowWeekDay)].append(sval);
	}
	if (val[WstrToStr(NowWeekDay)].size() == 0) {
		val.removeMember(WstrToStr(NowWeekDay));
	}
	if (val.size() != 0) {
		SPDLOG_INFO(fmt::format(L"Writing file \"{}\"", FilePath));
		std::ofstream fout;
		fout.open(FilePath);
		JSON_SW->write(val, &fout);
		fout.close();
	} else {
		if (std::filesystem::is_regular_file(FilePath)) {
			SPDLOG_INFO(fmt::format(L"Deleting file \"{}\"", FilePath));
			DeleteFileW(FilePath.c_str());
		}
	}
}

void WCH_save_task() {
	// Save task list data.
	std::wstring FilePath = WCH_path_data + L"\\data\\task.json";
	Json::Value val;
	if (WCH_num_task == 0) {
		if (std::filesystem::is_regular_file(FilePath)) {
			SPDLOG_INFO(fmt::format(L"Deleting file \"{}\"", FilePath));
			std::filesystem::remove(FilePath);
		}
		return;
	}
	SPDLOG_INFO(fmt::format(L"Writing file \"{}\"", FilePath));
	for (auto it = WCH_list_task.begin(); it != WCH_list_task.end(); it++) {
		val.append(WstrToStr(*it));
	}
	std::ofstream fout;
	fout.open(FilePath);
	JSON_SW->write(val, &fout);
	fout.close();
}

void WCH_save_work() {
	// Save task list data.
	std::wstring FilePath = WCH_path_data + L"\\data\\work.json";
	Json::Value val;
	if (WCH_num_work == 0) {
		if (std::filesystem::is_regular_file(FilePath)) {
			SPDLOG_INFO(fmt::format(L"Deleting file \"{}\"", FilePath));
			std::filesystem::remove(FilePath);
		}
		return;
	}
	SPDLOG_INFO(fmt::format(L"Writing file \"{}\"", FilePath));
	for (auto it = WCH_list_work.begin(); it != WCH_list_work.end(); it++) {
		Json::Value sval;
		sval.append(WstrToStr(it->first));
		sval.append(WstrToStr(it->second));
		val.append(sval);
	}
	std::ofstream fout;
	fout.open(FilePath);
	JSON_SW->write(val, &fout);
	fout.close();
}

void WCH_save_settings() {
	// Save settings data.
	std::wstring FilePath = WCH_path_data + L"\\settings.json";
	if (!WCH_pre_start) {
		SPDLOG_INFO(fmt::format(L"Writing file \"{}\"", FilePath));
	}
	std::ofstream fout;
	fout.open(FilePath);
	JSON_SW->write(WCH_Settings, &fout);
	fout.close();
}

void WCH_save() {
	// Save data.
	WCH_save_clock();
	WCH_save_task();
	WCH_save_work();
	WCH_save_settings();
}
