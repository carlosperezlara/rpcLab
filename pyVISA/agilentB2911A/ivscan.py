import time
import vxi11
import numpy as np
import pickle as pkl

inst = vxi11.Instrument("192.168.0.176");
print(inst.ask("*IDN?"));




inst.write("*RST");
inst.write(":SOUR:FUNC:MODE VOLT");
inst.write(":SOUR:VOLT:MODE SWE"); #Sweeping mode
inst.write(":SOUR:VOLT:star 0");  # min Volt
inst.write(":SOUR:VOLT:stop 40");  # max Volt
inst.write(":SOUR:VOLT:poin 200");  # steps
inst.write(":SENS:FUNC CURR");
#inst.write(":SENS:CURR:NPLC 0.1");   # aperture time to 0.1 PLC
inst.write(":SENS:CURR:APER 0.5");   # aperture time to 0.1 PLC

inst.timeout = 120;

inst.write(":SENS:CURR:prot 0.002"); # 2mA limit
inst.write(":TRIG:SOUR aint");  # automatic trigger
inst.write(":TRIG:COUN 200");     # trigger counts
inst.write(":OUTP ON");
inst.write(":INIT (@1)");

inst.write(":FETC:ARR:CURR? (@1)");

msg = inst.read();
inst.write(":OUTP OFF")

arr = msg.split(",")

clean_arr = []

#IF CHANGE VOLTAGE LIMIT OR NUMBER OF STEPS/TRIGGERS, MUST ADJUST HERE TOO
indices = np.arange(0.0, 39.0, 39.0/200.0)

output_file = open(r"output.txt", "w")
for i in range(len(arr)):
    #arr[i].replace("\n", "")
    #clean_arr.append([indices[i], float(arr[i])])
    output_file.write(str(indices[i]))
    output_file.write(" ")
    output_file.write(arr[i])
    output_file.write("\n")
output_file.close()

#pkl.dump(clean_arr, open("output.p", "wb"))

