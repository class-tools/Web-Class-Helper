import datetime
from PIL import ImageGrab
ts = datetime.datetime.now().timestamp()
f=open("b.txt","r")
s=f.read()
s=s.strip("\n")
s+="/"
s+=str(ts)
s+=".jpg"
image = ImageGrab.grabclipboard() 
image.save(s)
f.close()

