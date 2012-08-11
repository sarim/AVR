import usb
from usb.util import *
# find our device
dev = usb.core.find(idVendor=0x16C0, idProduct=0x05DC)

# was it found?
if dev is None:
    raise ValueError('Device not found')


#dev.set_configuration()
#while(1):
#    responsedata = dev.ctrl_transfer(CTRL_TYPE_VENDOR | CTRL_RECIPIENT_DEVICE | ENDPOINT_IN, 2, 0, 0,16)
#    print responsedata.tostring()
dev.ctrl_transfer(CTRL_TYPE_VENDOR | CTRL_RECIPIENT_DEVICE | ENDPOINT_IN, 1, 0, 0,16)
