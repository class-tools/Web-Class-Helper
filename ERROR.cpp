/*
Web Class Helper Exception Handling Module File 2.0.1
This source code file is under MIT License.
Copyright (c) 2022 Class Tools Develop Team
Contributors: jsh-jsh ren-yc
*/
#include <bits/stdc++.h>
#include <windows.h>
#include <wininet.h>
#include <tlhelp32.h>
#include <conio.h>
#include <direct.h>
using namespace std;

int main(int argc, char* argv[]) {
	if (argc == 3) {
		string errorcode = argv[1];
		string errormsg = argv[2];
		ShowWindow(GetConsoleWindow(), SW_HIDE);
		cout << "\a";
		if (MessageBoxA(NULL, ("Oops! An error occurred.\nPlease inform our developers with the error message by open a new Issue in our GitHub Repository.\nError message: " + errorcode + " " + errormsg + "\nWould you like to visit the Issues page now?").c_str(), "WCH ERROR", MB_ICONERROR | MB_YESNO) == 6) {
			system("start resources/website/issues.url");
		}
	}
	return 0;
}