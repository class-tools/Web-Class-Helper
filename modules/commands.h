/*
Web-Class-Helper Commands Module Header File 1.1.0
This source code file is under MIT License.
Copyright (c) 2022 Class Tools Develop Team
Contributors: jsh-jsh ren-yc
*/
#pragma once
#include <bits/stdc++.h>
#include <windows.h>
#include <conio.h>
#include <direct.h>
#include "file-process.h"
#include "functions.h"
using namespace std;

string Weekdayname[7] = {"Sunday.txt", "Monday.txt", "Tuesday.txt", "Wednesday.txt", "Thursday.txt", "Friday.txt", "Saturday.txt"};

void WCH_check_clock(int h, multimap <int, pair <int, string>> *mm) {
    for (auto it = (*mm).equal_range(h).first; it != (*mm).equal_range(h).second; it++) {
        if ((it -> second).first == WCH_GetTime().Minute && ((it -> second).second).size() > 0) {
            cout << "\a";
            MessageBox(NULL, ((it -> second).second).c_str(), "Web Class Helper", MB_OK);
        }
    }
}

void WCH_ReadData(int *n, multimap <int, pair <int, string>> *mm) {
    TalkDate q = WCH_GetTime();
    fin.open(Weekdayname[(q.Day + 2 * q.Month + 3 * (q.Month + 1) / 5 + q.Year + q.Year / 4 - q.Year / 100 + q.Year / 400 + 1) % 7].c_str());
    fin >> (*n);
    for (int i = 1; i <= *n; i++) {
        int h = 0;
        int m = 0;
        string tname = "NULL";
        fin >> h >> m >> tname;
        (*mm).emplace(make_pair(h, make_pair(m, tname)));
    }
}

void WCH_add(int *n, multimap <int, pair <int, string>> *mm) {
    int h = 0;
    int m = 0;
    string tname = "NULL";
    cin >> h >> m >> tname;
    if (h > 24 || m >= 60 || h < 1 || m < 0) {
        WCH_Error();
    } else {
        n++;
        (*mm).emplace(make_pair(h, make_pair(m, tname)));
    }
}

void WCH_delete(int *n, multimap <int, pair <int, string>> *mm) {
    int h = 0;
    int m = 0;
    int flag = 0;
    string tname = "NULL";
    cin >> h >> m >> tname;
    for (auto it = (*mm).equal_range(h).first; it != (*mm).equal_range(h).second; it++) {
        if ((it -> second).first == m && ((it -> second).second) == tname) {
            (*mm).erase(it);
            flag = 1;
            n--;
        }
    }
    if (!flag) {
        WCH_Error();
    }
}

void WCH_change(multimap <int, pair <int, string>> *mm) {
    int h = 0;
    int m = 0;
    int flag = 0;
    string tname = "NULL";
    cin >> h >> m >> tname;
    for (auto it = (*mm).equal_range(h).first; it != (*mm).equal_range(h).second; it++) {
        if ((it -> second).first == m) {
            ((it -> second).second) = tname;
            flag = 1;
        }
    }
    if (!flag) {
        WCH_Error();
    }
}

void WCH_save(int *n, multimap <int, pair <int, string>> *mm) {
    if (n != 0) {
        TalkDate q = WCH_GetTime();
        fout.open(Weekdayname[(q.Day + 2 * q.Month + 3 * (q.Month + 1) / 5 + q.Year + q.Year / 4 - q.Year / 100 + q.Year / 400 + 1) % 7].c_str());
        fout << n << endl;
        for (auto it = (*mm).begin(); it != (*mm).end(); it++) {
            fout << (it -> first) << " " << (it -> second).first << " " << (it -> second).second << endl;
        }
    }
}

void WCH_game() {
    srand(time(NULL));
    int n = rand() % 10000 + 1;
    string z = "0";
    try {
        while (stoi(z) != n) {
            cout << "Please input your number (1 ~ 10000): ";
            cin >> z;
            if (z[0] == '-' || z[0] == '0' || (z.size() > 5 && z != "10000")) {
                cout << "Number out of range." << endl;
            } else if (stoi(z) > n) {
                cout << "The answer is smaller." << endl;
            } else if (stoi(z) < n) {
                cout << "The answer is bigger." << endl;
            }
        }
    }
    catch (...) {
        cout << "Your input does not meet the requirements, game ending..." << endl << endl;
        return;
    }
    cout << "The number is " << n << ". You WIN!" << endl << endl;
    return;
}

void WCH_pi() {
    WCH_SetWindowStatus(false);
    WCH_PutPicture();
    WCH_SetWindowStatus(true);
    WCH_SaveImg();
}

void WCH_mode(bool *mode) {
    string tmp;
    cin >> tmp;
    if (tmp == "cmd") {
        *mode = true;
        printf("OK!\n\n");
    } else if (tmp == "bash") {
        *mode = false;
        printf("OK!\n\n");
    } else {
        printf("Unknown mode.\n\n");
    }
}

void WCH_unknown(string *op) {
    printf("Is it a system command? (Y/N): ");
    char tmp;
    cin >> tmp;
    if (tmp == 'Y') {
        system((*op).c_str());
    }
    cout << endl;
}

void WCH_help() {
    cout << "Commands:" << endl;
    cout << "1. quit (Quit this program)" << endl;
    cout << "2. add hour minute name (Add clock at hour:minute)" << endl;
    cout << "3. delete hour minute name (Delete clock at hour:minute)" << endl;
    cout << "4. change hour minute name (Change clock at hour:minute)" << endl;
    cout << "5. ow (Get a sentence) **From web**" << endl;
    cout << "6. hide (Hide the command line window)" << endl;
    cout << "7. game (Guessing game)" << endl;
    cout << "8. time (Get time at once)" << endl;
    cout << "9. pi (Make a screenshot)" << endl;
    cout << "10. mode cmd-mode (Switch command line mode cmd / bash)" << endl;
    cout << "11. speedtest (Start a speed test with a GUI window)" << endl;
    cout << endl;
}