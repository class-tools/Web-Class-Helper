/*
Web Class Helper Exception Handling Module File 2.0.0
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
		MessageBox(NULL, ("Oops! An error occured.\nPlease inform our developers with the error message by sending an E-mail to ldfx_ryc@163.com.\nError message: " + errorcode + " " + errormsg).c_str(), "WCH ERROR", MB_ICONERROR | MB_OK);
	}
	return 0;
}