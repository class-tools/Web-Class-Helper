/*
Web Class Helper Main File 2.1.2
This source code file is under MIT License.
Copyright (c) 2022 - 2023 Class Tools Develop Team
Contributors: jsh-jsh ren-yc
*/
#include "version.hpp"
#include "shasum.hpp"
#include "./modules/basic.hpp"
#include "./modules/apis.hpp"
#include "./modules/file-process.hpp"
#include "./modules/functions.hpp"
#include "./modules/init.hpp"
#include "./modules/commands.hpp"

int32_t wmain(int32_t _argc, wchar_t* _argv[]) {
	WCH_Init();
	if (_argc > 1) {
		SPDLOG_WARN(fmt::format(L"Number of redundant command line arguments: {}.", _argc - 1));
	}
	WCH_path_exec = _argv[0];
	while (true) {
		if (WCH_cmd_line) {
			WCH_CL_Init();
			if (WCH_support_command.find(WCH_list_command[0]) != WCH_support_command.end()) {
				WCH_support_command.find(WCH_list_command[0])->second();
			} else {
				WCH_InputCommandIncorrect();
			}
			std::wcout << std::endl;
		} else {
			WCH_Sleep(1000);
		}
	}
	return 0;
}
