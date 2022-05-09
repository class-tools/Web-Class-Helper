'''
Web Class Helper Speed Testing Module 2.0.0
This source code file is under MIT License.
Copyright (c) 2022 Class Tools Develop Team
Contributors: jsh-jsh ren-yc
'''
import psutil
import sys
from tkinter import *
from tkinter.messagebox import showerror
from time import sleep

def errno_1():
    showerror(title = "Fatal Error", message = "[Errno 1] Cannot get download and upload speed.")
    print("FATAL ERROR: [Errno 1] Cannot get download and upload speed.")

def speed_test():
    try:
        try:
            s1 = psutil.net_io_counters(pernic = True)["以太网"]
            sleep(1)
            s2 = psutil.net_io_counters(pernic = True)["以太网"]
        except:
            s1 = psutil.net_io_counters(pernic = True)["WLAN"]
            sleep(1)
            s2 = psutil.net_io_counters(pernic = True)["WLAN"]
    except:
        errno_1()
        root.quit()
        sys.exit(1)
    result = s2.bytes_recv - s1.bytes_recv
    return str('%.2f' % (result / 1024)) + ' KB / S'

def ui_updata(do):
    data = do()
    speed_label = root.children['speed_label']
    speed_label.config(text = data)
    root.after(1000, lambda: ui_updata(do))

root = Tk()
root.geometry('200x100')
root.title('Speed Test')
root.resizable(width = False, height = False)
Label(width = 100, height = 50, name = 'speed_label', text = '-.- KB / S', font = ('Hack', 20, 'bold')).pack()
root.after(1000, lambda :ui_updata(speed_test))
root.mainloop()
