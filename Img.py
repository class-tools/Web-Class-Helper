'''
Web Class Helper Image Grabbing Module 2.0.0
This source code file is under MIT License.
Copyright (c) 2022 Class Tools Develop Team
Contributors: jsh-jsh ren-yc
'''
import datetime
import winreg
from PIL import ImageGrab

def get_pictures():
    key = winreg.OpenKey(winreg.HKEY_CURRENT_USER, r'Software\Microsoft\Windows\CurrentVersion\Explorer\Shell Folders')
    return winreg.QueryValueEx(key, "My Pictures")[0]

ImageGrab.grabclipboard().save(get_pictures() + "/" + str(datetime.datetime.now().timestamp()) + ".jpg")
