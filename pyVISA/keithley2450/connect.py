import time
import vxi11
instr = vxi11.Instrument("169.254.131.3")

f = open('ivcurve.lua')
commands = f.readlines()
f.close()

f = open('inputVoltages.txt')
volts = f.readlines()
f.close()

cmd = 'mylist = {'
for volt in volts:
    cmd += str(float(volt)) + ","
cmd = cmd[0:-1]
cmd += '}'
#print(cmd)

instr.write('reset()') # making sure the buffer is cleared
instr.write('script.delete("testInfo")')
instr.write("loadscript testInfo\n")
for line in commands:
    #print(line)
    instr.write(line);
instr.write("endscript")
instr.write(cmd)
instr.write("testInfo.run()")
instr.write("IVRunnerList(mylist,0.004)")
print("waiting")
time.sleep(20)
print("DONE waiting")

instr.write('print(ivBuffer.n)')
n = int( instr.read() )
for i in range(n):
    instr.write('printbuffer({0}, {1}, ivBuffer.timestamps, ivBuffer.sourcevalues,  ivBuffer.sourceunits, ivBuffer.readings,  ivBuffer.units)'.format(i+1,i+1))
    print( instr.read() )
