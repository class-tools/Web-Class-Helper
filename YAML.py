'''
Web-Class-Helper 1.0.8
This source code file is under MIT License.
Copyright (c) 2022 Class Tools Develop Team
Contributors: jsh-jsh ren-yc
'''
import os
import yaml

# Get language
SettingsFile = open(os.path.join("", "settings.yml"), 'r', encoding = 'utf-8')
lang = yaml.load(SettingsFile.read(), Loader = yaml.FullLoader)["Language"]
SettingsFile.close()

# Output
File = open('yaml.tmp', 'r', encoding = 'utf-8')
command = File.read()
File.close()
if command.split()[1] == "None":
    SettingsFile = open(os.path.join("", "settings.yml"), 'r', encoding = 'utf-8')
    res = yaml.load(SettingsFile.read(), Loader = yaml.FullLoader)[command.split()[0]]
    SettingsFile.close()
else:
    LanguageFile = open(os.path.join("", "lang/" + command.split()[1] + ".yml"), 'r', encoding = 'utf-8')
    res = yaml.load(LanguageFile.read(), Loader = yaml.FullLoader)[command.split()[0]]
    LanguageFile.close()
File = open('yaml.tmp', 'w', encoding = 'utf-8')
File.write(res)
File.close()