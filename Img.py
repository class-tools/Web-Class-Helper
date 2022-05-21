'''
Web Class Helper Image Grabbing Module 2.0.1-alpha.2
This source code file is under MIT License.
Copyright (c) 2022 Class Tools Develop Team
Contributors: jsh-jsh ren-yc hjl2011
'''
import datetime
from PIL import ImageGrab
import os.path as p
import os as o
import datetime as dt

f = open('data/pi-save.dat','r')

pi_savepath = str(f.read())
pispLen = int(len(pi_savepath))
if pi_savepath[pispLen - 1] == '/' or pi_savepath[pispLen - 1] == '/' :
    pi_savepath = pi_savepath[:-1]

if pi_savepath == "" or not(p.exists(pi_savepath)) :
    print("Path does not exist.\nPlease modify the value in data/pi-save.dat .")
    f.close()
    with open('logs/latest.log','w',encoding = "utf-8") as file :
        w = dt.datetime.now()
        file.write("[")
        file.write(w.strftime("%H:%M:%S"))
        file.write("] [ERR]: Path does not exist.")
        file.close()
    o.system("notepad.exe \"data/pi-save.dat\"")
    
else :
    ImageGrab.grabclipboard().save(pi_savepath + "/" + str(datetime.datetime.now().timestamp()) + ".jpg")
    f.close()