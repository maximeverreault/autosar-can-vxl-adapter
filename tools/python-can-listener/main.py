import can

can.rc['interface'] = 'vector'
can.rc['channel'] = 1
can.rc['app_name'] = 'CANoe'
can.rc['bitrate'] = 500000
from can.interface import Bus

with can.Bus() as bus:
    for msg in bus:
        print(msg.data)