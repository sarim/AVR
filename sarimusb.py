import usb
from usb.util import *
import time
# find our device
dev = usb.core.find(idVendor=0x16C0, idProduct=0x05DC)

# was it found?
if dev is None:
    raise ValueError('Device not found')


#dev.set_configuration()
st = time.time() + 1
cc = 0
while(time.time() < st):
    responsedata = dev.ctrl_transfer(CTRL_TYPE_VENDOR | CTRL_RECIPIENT_DEVICE | ENDPOINT_IN, 2, 0, 0,16)
#    print type(responsedata)
    k = int(responsedata[0]) + int(responsedata[1]) * 256
    print (k/1023.0) * 5.03
    cc += 1;
print cc
#print responsedata.tostring()
#dev.ctrl_transfer(CTRL_TYPE_VENDOR | CTRL_RECIPIENT_DEVICE | ENDPOINT_IN, 1, 0, 0,16)
