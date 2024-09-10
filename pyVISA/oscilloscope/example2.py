from oscilloscopes import lecroy

_EVENTS = 10000
_IP = "192.168.0.111"
_FILESUFFIX = "example2"

#INIT
inst = lecroy( _IP, timeout=_EVENTS*2*60 ) #timeout in seconds
inst.clearAll()

#WINDOW SETUP
inst.setTime( hscale=0.1e-6, hdel=0, debug=True )
inst.setChannel( chn="C1", vscale=0.050, voff=-0.050, debug=True )
inst.setChannel( chn="C2", vscale=0.050, voff=-0.050, debug=True )

#TRIGGER SETUP
inst.setTriggerLevel( chn="C1", slope="POS", lev=0.1, debug=True )
inst.setTriggerLevel( chn="C2", slope="POS", lev=0.1, debug=True )
inst.setTriggerMultiStage( debug=True )


#ACQUIRE
inst.acquire( cnt=_EVENTS, fileSuffix=_FILESUFFIX )

