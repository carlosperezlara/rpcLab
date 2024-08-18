import time
import oscilloscopes

#inst = keysight( "192.168.0.55" )
inst = oscilloscopes.lecroy( "192.168.0.171" )

inst.clearAll()
inst.setTrigger( chn="EX", slope="POS", lev=0.2, debug=True )
inst.setTime( hscale=10e-9, hdel=0, debug=True )
inst.setChannel( chn="C1", vscale=0.25, voff=+0.06, debug=True )
inst.setChannel( chn="C2", vscale=0.25, voff=+0.04, debug=True )
inst.setChannel( chn="C3", vscale=0.25, voff=+0.02, debug=True )
inst.setChannel( chn="C4", vscale=0.25, voff=+0.00, debug=True )
inst.setChannel( chn="C5", vscale=0.25, voff=-0.02, debug=True )
inst.setChannel( chn="C6", vscale=0.25, voff=-0.04, debug=True )
inst.setChannel( chn="C7", vscale=0.25, voff=-0.06, debug=True )
inst.setChannel( chn="C8", vscale=0.25, voff=-0.08, debug=True )
inst.acquire( cnt=1000, debug=True )

