import time
import vxi11
import numpy as np
import sys

def setV(voltage):
	print("setting voltage to " + str(voltage))
	inst = vxi11.Instrument("192.168.0.176")
	print(inst.ask("*IDN?"));
	
	inst.write(':SOUR:VOLT {0}'.format(voltage))
	inst.write("TRIG:SOUR aint")
	


args = sys.argv
setV(args[1])
