import time
import vxi11
instr = vxi11.Instrument("10.168.233.224")

f = open('ivcurve.lua')
commands = f.readlines()
f.close()

#f = open('inputVoltagesHDR2.txt')
#f = open('inputVoltagesS12.txt')
#volts = f.readlines()
#f.close()

#nvolts = 0
#cmd = 'mylist={'
#for volt in volts:
#    cmd += str(float(volt)) + ","
#    nvolts += 1
#cmd = cmd[0:-1]
#cmd += '}'
#print(cmd)
#print(nvolts," values")

instr.write('reset()') # making sure the buffer is cleared
instr.write('script.delete("testInfo")')
instr.write("loadscript testInfo\n")
for line in commands:
    #print(line)
    instr.write(line);
instr.write("endscript")
#instr.write(cmd)
instr.write("testInfo.run()")

instr.write("IVRunnerList(0.005,1)") #limit current to 3mA
print("waiting for ", 90, " seconds")
time.sleep(90)
print("DONE waiting")

#instr.read() #read buff
#instr.read() #read buff
    
instr.write('print(ivBuffer.n)')
whatn = instr.read()
print(whatn)
n = int(whatn)
output = []
for i in range(n):
    instr.write('printbuffer({0}, {1}, ivBuffer.timestamps, ivBuffer.sourcevalues,  ivBuffer.sourceunits, ivBuffer.readings,  ivBuffer.units)'.format(i+1,i+1))
    output.append( instr.read() )
    #print( i, output[i] )

f = open('outputdata.csv', 'w+')
for i in range(n):
    f.write(output[i])
    f.write('\n')
f.close()
