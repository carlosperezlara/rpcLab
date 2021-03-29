function IVRunnerList(ilimit, nwaitseconds)
   reset()
   beeper.beep(0.05,200)

   -- Given a list of voltages (vList), step through each voltage
   errorqueue.clear()

   smu.source.func = smu.FUNC_DC_VOLTAGE
   -- smu.source.delay = 0.1 -- it is overwritten if autodelay is ON (default)

   smu.measure.terminals=smu.TERMINALS_REAR
   smu.measure.func = smu.FUNC_DC_CURRENT
   smu.measure.autozero.once()
   smu.measure.autorange = smu.ON
   -- smu.measure.nplc = 3 -- default is 1

   defbuffer1.clear()
   ivBuffer = buffer.make(2500)
   ivBuffer.clear()
   ivBuffer.clearcache()
   ivBuffer.appendmode = 1
   smu.source.ilimit.level = ilimit -- recommendation from thomas 3-5 mA tops
   smu.source.output = smu.ON

   for element = 0, 526, 1
   do
      val = 0
      if( element>=0 )
      then
	val = 0.0 + 0.5*(element+1)
      end
      if( element>=65 )
      then
	val = 33.0 + 0.25*(element-65)
      end
      if( element>=77 )
      then
	val = 36.0 + 0.1*(element-77)
      end
      if( element>=87 )
      then
	val = 37.0 + 0.025*(element-87)
      end
      if( element>=127 )
      then
	val = 38.0 + 0.0125*(element-127)
      end
      smu.source.level = val
      --smu.source.level = mylist[element]
      --local starttime = os.time()
      --local endtime = starttime + nwaitseconds

      --i = smu.measure.read()
      --closemaxrange = math.abs(i) > 0.60*smu.measure.range
      --closeminrange = math.abs(i) < 0.05*smu.measure.range
      --if closeminrange then
      --   if smu.measure.range/10 >= 1-6 then
      --     smu.measure.range = smu.measure.range/10
      --elseif closemaxrange then
      --   smu.measure.range = smu.measure.range*10

      --repeat
      --      i = smu.measure.read()
      --until os.time() > endtime

      i = smu.measure.read( ivBuffer )
       if (smu.source.ilimit.tripped == smu.ON) then
	 if (smu.source.ilimit.level >= ilimit) then
	    break
	 end
      end
   end

   smu.source.level = 0
   ivBuffer.appendmode = 0
   smu.source.output = smu.OFF

   -- printbuffer(1, ivBuffer.n, ivBuffer.timestamps, ivBuffer.sourcevalues,  ivBuffer.sourceunits, ivBuffer.readings,  ivBuffer.units)
   beeper.beep(0.2,200)
end
