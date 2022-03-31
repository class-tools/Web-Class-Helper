/*
Web-Class-Helper File Process Module Header File 1.1.0
This source code file is under MIT License.
Copyright (c) 2022 Class Tools Develop Team
Contributors: jsh-jsh ren-yc
*/
#pragma once
#include <bits/stdc++.h>
#include <windows.h>
#include <conio.h>
#include <direct.h>
#include "commands.h"
#include "functions.h"
using namespace std;

ifstream fin;
ofstream fout;

inline void in_data(string fname) {
    freopen(fname.c_str(), "r", stdin);
}

inline void out_data(string fname) {
    freopen(fname.c_str(), "w", stdout);
}