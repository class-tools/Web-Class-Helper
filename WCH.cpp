/*
    Web-Class-Helper v1.0.0
    JSH
*/
//#include <winsock2.h>
#include <stdio.h>
#include <windows.h>
#include <conio.h>
#include <sstream>
#include <bits/stdc++.h>
#include "WCH.h"
using namespace std;
struct TalkDate{
    int Year;
    int Month;
    int Day;
    int Hour;
    int Minute;
    int Second;
    string Name;
};
multimap<int,pair<int,string> > mm;
TalkDate GetTime();
void PrintTime(TalkDate);
void PrintError();
void PrintOK();
void Bye();
inline void in_data(string);
inline void out_data(string);
void HideWindow(bool);
bool JudgeKey();
void Game();
string op;
string Weekdayname[7] = {
    "Sunday.txt",
    "Monday.txt",
    "Tuesday.txt",
    "Wednesday.txt"
    "Thursday.txt",
    "Friday.txt",
    "Saturday.txt"};
int n;
bool fgh=1;
ifstream fin;
ofstream fout;
int main(){
    system("mode con cols=50 lines=5");
    SetConsoleTitle("Web-Class-Helper");
    TalkDate q=GetTime();
    if (q.Month == 1 || q.Month == 2)
    {
        q.Month += 12;
        q.Year--;
    }
    int Week = (q.Day + 2 * q.Month + 3 * (q.Month + 1) / 5 + q.Year + q.Year / 4 - q.Year / 100 + q.Year / 400+1) % 7;
    HideWindow(1);
    fin.open(Weekdayname[Week].c_str());
    fin>>n;
    for(int i=1;i<=n;i++){
        int h=0;
        int m=0;
        string tname="NULL";
        fin>>h>>m>>tname;
        mm.emplace(make_pair(h,make_pair(m,tname)));
        //cout<<mm.count(h)<<endl;
    }
    printf("Please input your name:");
    cin >> op;
    printf("Welcome\n");
    while(op!="bye"){
        int h=GetTime().Hour;
        for (auto it = mm.equal_range(h).first; it != mm.equal_range(h).second; it++)
        {
            if ((it->second).first == GetTime().Minute && ((it->second).second).size() > 0)
            {
                printf("\a");
                MessageBox(NULL, ((it->second).second).c_str(), "Web-Class-Helper", MB_OK);
            }
        }
        if(op=="add"){
            int h=0;
            int m=0;
            string tname="NULL";
            scanf("%d%d",&h,&m);
            cin>>tname;
            if(h>24||m>=60||h<1||m<0){
                PrintError();
            }
            else{
                n++;
                mm.emplace(make_pair(h, make_pair(m, tname)));
                PrintOK();
            }
        }
        if(op=="delete"){
            int h=0;
            int m=0;
            int flag=0;
            string tname="NULL";
            scanf("%d%d",&h,&m);
            cin>>tname;
            for (auto it = mm.equal_range(h).first; it != mm.equal_range(h).second; it++)
            {
                if ((it->second).first == m && ((it->second).second) == tname)
                {
                    mm.erase(it);
                    PrintOK();
                    flag=1;
                    n--;
                }
            }
            if(!flag){
                PrintError();
            }
        }
        if(op=="change"){
            int h = 0;
            int m = 0;
            int flag = 0;
            string tname = "NULL";
            scanf("%d%d", &h, &m);
            cin >> tname;
            for (auto it = mm.equal_range(h).first; it != mm.equal_range(h).second; it++)
            {
                if ((it->second).first == m)
                {
                    ((it->second).second) = tname;
                    PrintOK();
                    flag = 1;
                }
            }
            if (!flag)
            {
                PrintError();
            }
        }
        if(op=="ow"){
            GetGet();
        }
        if(JudgeKey()){
            HideWindow(1);
            fgh=1;
        }
        if (fgh == 1)
        {
            cin >> op;
        }
        if(op=="hide"){
            HideWindow(0);
            fgh=0;
        }
        if(op=="game"){
            Game();
        }
        if(op=="time"){
            PrintTime(GetTime());
        }
    }
    fout.open(Weekdayname[Week].c_str());
    fout<<n<<endl;
    for(auto it=mm.begin();it!=mm.end();it++){
        fout<<(it->first)<<" "<<(it->second).first<<" "<<(it->second).second<<endl;
    }
    Bye();
    return 0;
}
TalkDate GetTime()
{
    TalkDate NowTime;
    time_t rawtime;
    struct tm *ptminfo;
    time(&rawtime);
    ptminfo = localtime(&rawtime);
    NowTime.Year = ptminfo->tm_year + 1900;
    NowTime.Month = ptminfo->tm_mon + 1;
    NowTime.Day = ptminfo->tm_mday;
    NowTime.Hour = ptminfo->tm_hour;
    NowTime.Minute = ptminfo->tm_min;
    NowTime.Second = ptminfo->tm_sec;
    NowTime.Name="NULL";
    return NowTime;
}
void Bye()
{
    printf("Have a good time. Good bye");
}
void Lazy()
{
    printf("This code is very lazy,So it didn't work\n");
}
void PrintTime(TalkDate a)
{
    printf("%d %d %d %d %d %d\n", a.Year, a.Month, a.Day, a.Hour, a.Minute, a.Second);
}
void PrintError(){
    printf("This input or code is wrong.Please see your code.\n");
}
inline void in_data(string fname)
{
    freopen(fname.c_str(), "r", stdin);
}
inline void out_data(string fname)
{
    freopen(fname.c_str(), "w", stdout);
}
void HideWindow(bool ju)
{
    HWND hwnd = FindWindow("ConsoleWindowClass",NULL);
    if (hwnd)
    {
        ShowWindow(hwnd, ju);
    }
}
bool JudgeKey(){
    int a=GetKeyState(VK_CONTROL);
    int b=GetKeyState(VK_DOWN);
    if(a<0&&b<0){
        return 1;
    }
    else{
        return 0;
    }
}
void Game(){
    srand(time(0));
    int n=rand()%10000+1;
    int z=0;
    while(z!=n){
        printf("Please input your number:");
        cin>>z;
        if(z>n){
            printf("Please smaller\n");
        }
        if(z<n){
            printf("Please bigger\n");
        }
        if(z==-100000){
            return ;
        }
    }
    printf("Yeah!!You WIN!!\n");
    return ;
}
void PrintOK(){
    printf("OK!\n");
}