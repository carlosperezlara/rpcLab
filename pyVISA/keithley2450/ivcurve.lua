function IVRunnerList(vList, ilimit)
   reset()
   beeper.beep(0.05,200)

   -- Given a list of voltages (vList), step through each voltage
   errorqueue.clear()

   smu.source.func = smu.FUNC_DC_VOLTAGE
   -- smu.source.delay = 0.1 -- it is overwritten if autodelay is ON (default)

   smu.measure.func = smu.FUNC_DC_CURRENT
   smu.measure.autozero.once()
   smu.measure.autorange = smu.ON
   -- smu.measure.nplc = 3 -- default is 1

   ivBuffer = buffer.make(2500)
   ivBuffer.clear()
   ivBuffer.appendmode = 1
   element = 1
   smu.source.output = smu.ON
   while vList[element] do
      smu.source.level = vList[element]
      i = smu.measure.read( ivBuffer )
      element = element + 1
      if (smu.source.ilimit.tripped == smu.ON) then
	 if (smu.source.ilimit.level >= ilimit) then
	    break
	 end
      end
   end

   smu.source.level = 0
   ivBuffer.appendmode = 0
   smu.source.output = smu.OFF

   --printbuffer(1, ivBuffer.n, ivBuffer.timestamps, ivBuffer.sourcevalues,  ivBuffer.sourceunits, ivBuffer.readings,  ivBuffer.units)
   beeper.beep(0.2,200)
end
