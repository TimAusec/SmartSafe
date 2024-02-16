import datetime
import serial
import time

ser=serial.Serial(
    port='COM6',
    baudrate=9600,
    parity=serial.PARITY_NONE,
    stopbits=serial.STOPBITS_ONE,
    bytesize=serial.EIGHTBITS
)

while 1:
    input = input()
    if input=='exit':
        #ser.close()
        exit()
    if input=='Send Current Time':
        #ser.write('c')
        time.sleep(0.001)
        #ser.write(str(datetime.datetime.now()))
        print(str(datetime.datetime.now()))
