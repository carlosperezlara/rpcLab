import oscilloscopes

_EVENTS = 1
_IP = "192.168.0.169"
_FILESUFFIX = "AutoCosmics_340V_1"

#inst = keysight( "192.168.0.55" )
inst = oscilloscopes.lecroy( _IP, timeout=_EVENTS*2*60 ) #timeout in seconds

inst.clearAll()
inst.setTime( hscale=10e-9, hdel=0, debug=True )
inst.setTriggerLevel( chn="C1", slope="NEG", lev=-0.200, debug=True )
inst.setTriggerLevel( chn="C2", slope="NEG", lev=-0.040, debug=True )
#inst.setTriggerPattern( src="C1,H,C2,H", con="OR", debug=True )
inst.setTriggerMultiStage( debug=True )
inst.setChannel( chn="C1", vscale=0.200, voff=+0.600, debug=True )
inst.setChannel( chn="C2", vscale=0.050, voff=+0.150, debug=True )
inst.setChannel( chn="C3", vscale=0.020, voff=+0.040, debug=True )
inst.setChannel( chn="C4", vscale=0.020, voff=+0.040, debug=True )
inst.setChannel( chn="C5", vscale=0.020, voff=+0.040, debug=True )
inst.setChannel( chn="C6", vscale=0.020, voff=+0.040, debug=True )
inst.acquire( cnt=_EVENTS, fileSuffix=_FILESUFFIX )
