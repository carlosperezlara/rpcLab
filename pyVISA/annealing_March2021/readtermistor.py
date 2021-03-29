import telnetlib
import time
import datetime
#import vxi11
#instr = vxi11.Instrument("10.168.233.226")
tl = telnetlib.Telnet('10.168.233.226', 5025)

#print( instr.ask('*IDN?') )

#instr.write('OUTput1:STATe OFF')
#instr.write('OUTput2:STATe OFF')

#print( instr.ask('SOURce:PULSe:DCYcle?') ) 

#print( 'IS OUTPUT ON?' );
#print( '  CHANNEL 1:' );
#print( '    STATE =', instr.ask('OUTPut1:STATe?') ) 
#print( '    IMPEDANCE =', instr.ask('OUTPut1:IMpedance?') ) 
#print( '    POLARITY =', instr.ask('OUTPut1:POLarity?') ) 
#print( '      AMPLITUDE =', instr.ask('SOURce1:AM:INTernal:FUNCtion?') ) 
#print( '      AMPLITUDE =', instr.ask('SOURce1:VOLTage:LIMit:HIGH?') ) 
#print( '      AMPLITUDE =', instr.ask('SOURce1:VOLTage:LIMit:LOW?') ) 

output = [] 
i = 50
while i > 0:
    tstmp = datetime.datetime.now()
    tl.write(':MEAS:RES?\r\n')
    #time.sleep(0.1)
    msg = tl.read_some().strip()
    output.append((time.time(), msg))
    print(msg)
    time.sleep(0.9)
    i -= 1
    #print( '      AMPLITUDE =', instr.ask(':READ:ARRay? @1') ) 

    
f = open('tempdata.csv', 'w+')
for i in output:
    f.write('{0}, {1}\n'.format(tstmp.strftime('%Y-%m-%d %H:%M:%S'), i[1]))

f.close()
