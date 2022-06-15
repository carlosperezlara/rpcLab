import time
import vxi11
import numpy as np
import pickle as pkl

inst = vxi11.Instrument("192.168.0.176");
print(inst.ask("*IDN?"));

volts = ""
for i in np.arange(0.0,35.0, .25):
    volts = volts + ("," + str(i))

for i in np.arange(35.0, 38.0, .1):
    volts = volts + ("," + str(i))

volts = volts[1:]

volts_len = len(volts.split(","))

inst.write("*RST");
inst.write(":SOUR:FUNC:MODE VOLT");
inst.write(":SOUR:VOLT:MODE SWE"); #Sweeping mode
inst.write(":SOUR:LIST:VOLT " + volts)
inst.write(":SENS:FUNC CURR");
#inst.write(":SENS:CURR:NPLC 0.1");   # aperture time to 0.1 PLC
inst.write(":SENS:CURR:APER 0.5");   # aperture time to 0.1 PLC

inst.timeout = 360;

inst.write(":SENS:CURR:prot 0.002"); # 1mA limit
inst.write(":TRIG:SOUR aint");  # automatic trigger
inst.write(":TRIG:COUN " + str(volts_len))     # trigger counts
inst.write(":OUTP ON");
inst.write(":INIT (@1)");

inst.write(":FETC:ARR:CURR? (@1)");

msg = inst.read();
inst.write(":OUTP OFF")

arr = msg.split(",")

clean_arr = []

#output_file = open(r"output.txt", "w")
volts = volts.split(",")
for i in range(len(arr)):
    arr[i].replace("\n", "")
    clean_arr.append([float(volts[i]), float(arr[i])])

pkl.dump(clean_arr, open("output.p", "wb"))

