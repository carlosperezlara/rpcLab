import time
import vxi11
instr = vxi11.Instrument("10.168.233.224")

f = open('handshake.lua')
commands = f.readlines()
f.close()

instr.write('reset()') # making sure the buffer is cleared
instr.write('script.delete("testInfo")')
instr.write("loadscript testInfo\n")
for line in commands:
    print(line)
    instr.write(line);
instr.write("endscript")
instr.write("testInfo.run()")
instr.write("ShakyShaky()")
