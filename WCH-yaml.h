/*
Web-Class-Helper 1.0.8
This source code file is under MIT License.
Copyright (c) 2022 Class Tools Develop Team
Contributors: jsh-jsh ren-yc
*/
#include <bits/stdc++.h>
#include "yaml-cpp/yaml.h"
using namespace std;

struct NODE1
{
    string a, b, c;
};

NODE1 ReadSettingsFile()
{
    YAML::Node config = YAML::LoadFile("../settings.yaml");
    string mode, lang, version;
    if (config["CommandMode"])
    {
        mode = config["CommandMode"].as<string>();
    }
    else
    {
        fprintf(stderr, "FATAL ERROR: Cannot read settings file.\n");
    }
    if (config["Language"])
    {
        lang = config["Language"].as<string>();
    }
    else
    {
        fprintf(stderr, "FATAL ERROR: Cannot read settings file.\n");
    }
    if (config["Version"])
    {
        version = config["Version"].as<string>();
    }
    else
    {
        fprintf(stderr, "FATAL ERROR: Cannot read settings file.\n");
    }
    return (NODE1){mode, lang, version};
}

void ReadLangFile(string )
{
    ofstream fout("../lang/" + ".yml");
    config["score"] = 99;
    fout << config;
    fout.close();
}