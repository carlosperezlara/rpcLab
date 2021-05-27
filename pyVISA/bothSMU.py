import vxi11 
import time
import sys
import signal 
from datetime import datetime as dt

finish = False
def sigintHandler(signal, frame):
    print ('Ctrl-C Raised...cleaning up')
    frame.f_globals['finish'] = True
    finish = True

def sigtermHandler(signal, frame):
    print('Sig Term received, cleaning up')
    frame.f_globals['finish'] = True
    
    
runnumber = sys.argv[1]

sampleSeconds = sys.argv[2]

path = sys.argv[3] #"/data/testbeam_2021_05_data/sensors_data/"

filenametemp = "currentB1B2_"
ext = ".csv"
filename = path + filenametemp + str(runnumber) + ext
savefile = open(filename,"w")

def readKeithleyCurrent(inst):


    inst.write("print(smu.measure.read())")
    return inst.read()


def readAgilentCurrent(inst):

    inst.write(':MEAS:CURR?')
    return inst.read()


def setKeithleyVoltage(inst, v):

    inst.write('smu.source.level = {0}'.format(v))




def setAgilentVolage(inst, v):

    inst.write(':SOUR:VOLT {0}'.format(v))


def readAgilentVoltage(inst):
    inst.write(':MEAS:VOLT?')
    return inst.read()

def readKeithleyVoltage(inst):
    inst.write('print(smu.source.level)')
    return inst.read()
    
agi = vxi11.Instrument('192.168.133.176')
k2450 = vxi11.Instrument('192.168.133.60')    
try:


    line = agi.ask('*IDN?')
    timeout = 0
    while line.find('2911') == -1:
        line = agi.read()
        timeout = timeout + 1
        if timeout > 5:
            break

    print (line)
    k2450.timeout = 2.0

        

    line = k2450.ask('*IDN?')
    timeout = 0
    while line.find('2450') == -1:
        line = k2450.read()
        timeout = timeout + 1
        if timeout > 5:
            break
    
    print (line)
    agi.timeout = 2.0
    #clear buffer 

    k2450.write("print('Done')")
    msg = ''
    while msg.find('Done') == -1:
        print(msg)
        msg = k2450.read()
    
    vBiasKei = 38.8
    vBiasAgi = 38.8
    vLow = 41.4

#    while True:
     #   print('Waiting...Press enter to start running, will go for 12s')
      #  line = sys.__stdin__.readline()
       # if line.find('STOP') != -1:
        #    break

    agiCurrent = readAgilentCurrent(agi)
    keithCurrent = readKeithleyCurrent(k2450)
    print ('Setting Keithley V Bias: {0}, Agilent: {1}'.format(vBiasKei, vBiasAgi))
        #setAgilentVolage(agi, vBiasAgi)
        #setKeithleyVoltage(k2450, vBiasKei)
    agiCurrent = readAgilentCurrent(agi)
    keithCurrent = readKeithleyCurrent(k2450)
    print ('Agi Current: {0}\nKeith Current: {1}'.format(agiCurrent, keithCurrent))
    print ('Agi Vb: {0}\nKeith Vb: {1}'.format(vBiasAgi, vBiasKei))
    signal.signal(signal.SIGINT, sigintHandler)
    signal.signal(signal.SIGTERM, sigtermHandler)
    for i in range(0, int(sampleSeconds)):
        #print('IN SPILL!')
        if finish:
            break
        time.sleep(0.7)
        now = dt.now()
        current_time = now.strftime("%D %H:%M:%S")
        agiCurrent = readAgilentCurrent(agi)
        keithCurrent = readKeithleyCurrent(k2450)
        print ('Agi Current: {0}\nKeith Current: {1}'.format(agiCurrent, keithCurrent))
        savefile.write(current_time + ", B2:" + agiCurrent + ", B1:" + keithCurrent + '\n')
        savefile.flush()
        #print ('Lowering vBias to {0}'.format(vLow))
        #setAgilentVolage(agi, vLow)
        #setKeithleyVoltage(k2450, vLow)
        agiCurrent = readAgilentCurrent(agi)
        keithCurrent = readKeithleyCurrent(k2450)
        print ('Agi Current: {0}\nKeith Current: {1}'.format(agiCurrent, keithCurrent))
        
    print('Final Voltage Keithley:{0}'.format(readKeithleyVoltage(k2450)))
    print('Final Current Keithley:{0}'.format(readKeithleyCurrent(k2450)))
    print('Final Voltage Agilent:{0}'.format(readAgilentVoltage(agi)))
    print('Final Current Agilent:{0}'.format(readAgilentCurrent(agi)))

        

        
except Exception as e:
    print (e)
finally:
    k2450.close()
    agi.close()
    savefile.close()
