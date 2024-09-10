from oscilloscopes import lecroy

_EVENTS = 10000
_IP = "192.168.0.111"
_FILESUFFIX = "example1"

#INIT
inst = lecroy( _IP, timeout=_EVENTS*2*60 ) #timeout in seconds
inst.clearAll()

#WINDOW SETUP
inst.setTime( hscale=10e-6, hdel=0, debug=True )
inst.setChannel( chn="C1", vscale=0.200, voff=+0.100, debug=True )
inst.setChannel( chn="C2", vscale=0.200, voff=-0.100, debug=True )

#TRIGGER SETUP
inst.setTriggerLevel( chn="C1", slope="POS", lev=0.1, debug=True )
inst.setTriggerEdge( src="C1", debug=True )

#ACQUIRE
inst.acquire( cnt=_EVENTS, fileSuffix=_FILESUFFIX )

