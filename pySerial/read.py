import serial
import time
ser = serial.Serial('/dev/ttyACM0',9600)
time.sleep(2)
b = ser.readline()
#print(b)
str_rn = b.decode()
#print(str_rn)
str = str_rn.rstrip()
for X in range(3):
    print(str)
    time.sleep(1)
