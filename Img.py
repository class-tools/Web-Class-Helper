'''
Web-Class-Helper 1.0.8
This source code File is under MIT License.
Copyright (c) 2022 Class Tools Develop Team
Contributors: jsh-jsh ren-yc
'''
import datetime
from PIL import ImageGrab

# Main
File = open("img.tmp", "r", encoding = 'utf-8')
ImageGrab.grabclipboard().save(File.read().strip("\n") + "/" + str(datetime.datetime.now().timestamp()) + ".jpg")
File.close()
