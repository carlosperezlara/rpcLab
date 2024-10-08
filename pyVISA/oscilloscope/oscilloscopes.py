import vxi11

class keysight:
    def __init__(self, address):
        self.instr = vxi11.Instrument( address )
    
    def clearAll(self):
        print( self.instr.ask('*IDN?') )
        self.instr.write('STOP')
        self.instr.write("*CLS")
        self.instr.write("*RST")
        self.instr.write(":AUToscale")

    def setTrigger( self, chn="CHANnel1", slope="POSitive", lev=0.1, debug=False ): ### units V
        self.instr.write( ":TRIGger:EDGE:LEVel {}".format(lev) )
        self.instr.write( ":TRIGger:EDGE:SLOPe {}".format(slope) )
        self.instr.write( ":TRIGger:MODE EDGE" )
        self.instr.write( ":TRIGger:EDGE:SOURce {}".format(chn) )
        if(debug):
            print( "Trigger mode:", self.instr.ask(":TRIGger:MODE?") )
            print( "Trigger source:", self.instr.ask(":TRIGger:EDGE:SOURce?") )
            print( "Trigger level:", self.instr.ask(":TRIGger:EDGE:LEVel?") )
            print( "Trigger slope:", self.instr.ask(":TRIGger:EDGE:SLOPe?") )
    
    def setChannel( self, chn="CHANnel1", vscale=50e-3, voff=0.0, debug=False ): ### units V
        self.instr.write( ":{}:SCALe {}".format(chn,vscale) )
        self.instr.write( ":{}:OFFSet {}".format(chn,voff) )
        if(debug):
            print( "{} scale:".format(chn), self.instr.ask( ":{}:SCALe?".format(chn) ) )
            print( "{} offset:".format(chn), self.instr.ask( ":{}:OFFSet?".format(chn) ) )

    def setTime( self, hscale=0.0002, hoff=0.0, debug=False ): ### units seconds
        self.instr.write( ":TIMebase:SCALe {}".format(hscale) )
        self.instr.write( ":TIMebase:POSition {}".format(hoff) )
        if(debug):
            print( "Timebase scale:", self.instr.ask( ":TIMebase:SCALe?" ) )
            print( "Timebase offset:", self.instr.ask( ":TIMebase:POSition?" ) )

    def acquire( self, cnt=1000, pnt=300, fileSuffix="tmp", debug=True ):
        self.instr.write( ":ACQuire:TYPE NORMal" )
        self.instr.write( ":ACQuire:MODE SEGMented" )
        self.instr.write( ":ACQuire:SEGMented:COUNt {}".format(cnt) )
        self.instr.write( ":SAVE:WAVeform:SEGMented ALL" )
        self.instr.write( ":WAVeform:SEGMented:ALL 1" )
        self.instr.write( ":WAVeform:FORMat BYTE" )
        self.instr.write( ":WAVeform:POINts:MODE RAW" )
        self.instr.write( ":WAVeform:POINts {}".format(pnt) )
        self.instr.write( ":DIGitize CHANnel1,CHANnel2,CHANnel3,CHANnel4" )
        if(debug):
            print( "Acquire type::", self.instr.ask( ":ACQuire:TYPE?" ) )
            print( "Acquire segments:", self.instr.ask( ":ACQuire:SEGMented:COUNt?" ) )
            print( "Acquire points:", self.instr.ask(":ACQuire:POINts?") )
        
        f1 = open( "C1--{}.bin".format(fileSuffix), "wb" )
        f2 = open( "C2--{}.bin".format(fileSuffix), "wb" )
        f3 = open( "C3--{}.bin".format(fileSuffix), "wb" )
        f4 = open( "C4--{}.bin".format(fileSuffix), "wb" )
        
        ### CHN1
        self.instr.write( ":WAVeform:SOURce CHANnel1" )
        self.instr.write( ":WAVeform:DATA?" )
        f1.write( self.instr.read_raw() )
        ### CHN2
        self.instr.write( ":WAVeform:SOURce CHANnel2" )
        self.instr.write( ":WAVeform:DATA?" )
        f2.write( self.instr.read_raw() )
        ### CHN3
        self.instr.write( ":WAVeform:SOURce CHANnel3" )
        self.instr.write( ":WAVeform:DATA?" )
        f3.write( self.instr.read_raw() )
        ### CHN4
        self.instr.write( ":WAVeform:SOURce CHANnel4" )
        self.instr.write( ":WAVeform:DATA?" )
        f4.write( self.instr.read_raw() )

class lecroy:
    def __init__(self, address, timeout=5):
        self.instr = vxi11.Instrument( address )
        self.instr.timeout = timeout
   
    def __del__(self):
        self.instr.write("vbs 'app.settodefaultsetup'")
        self.instr.close()
 
    def clearAll(self):
        self.instr.clear()
        self.instr.write("*CLS")
        self.instr.write("*RST")
        self.instr.write("ALST?")
        self.instr.write("vbs 'app.settodefaultsetup'")
        self.instr.write( "TRIG_MODE STOP" )
        print( self.instr.ask('*IDN?') )
        self.instr.write("GRID SINGLE")
        self.instr.write("DISPLAY ON")

    def setTriggerLevel( self, chn="C1", slope="POS", lev=0.1, debug=False ): ### units V
        self.instr.write( "{}:TRIG_LEVEL {}".format(chn,lev) )
        self.instr.write( "{}:TRIG_SLOPE {}".format(chn,slope) )
        if(debug):
            print( "Trigger level:", self.instr.ask("{}:TRIG_LEVEL?".format(chn)) )
            print( "Trigger slope:", self.instr.ask("{}:TRIG_SLOPE?".format(chn)) )
    
    def setTriggerEdge( self, src="C1", debug=False ): ### units V
        self.instr.write( "TRIG_SELECT Edge,SR,{}".format(src) )
        self.instr.write( "TRIG_MODE SINGLE" )
        if(debug):
            print( "Trigger mode:", self.instr.ask("TRIG_MODE?") )
    
    def setTriggerPattern( self, src="C1,H,C2,H", con="OR", debug=False ): ### units V
        self.instr.write( "TRIG_SELECT PA" )
        self.instr.write( "TRIG_PATTERN {},STATE,{}".format(src,con) )
        if(debug):
            print( "Trigger mode:", self.instr.ask("TRIG_MODE?") )
    
    def setTriggerMultiStage( self,  debug=False ): ### units V
        self.instr.write( "TRIG_SELECT TEQ,SR,C1,QL,C2,HT,TL,HV,10e-9S" )
        if(debug):
            print( "Trigger mode:", self.instr.ask("TRIG_MODE?") )
    
    def setChannel( self, chn="C1", vscale=50e-3, voff=0.0, debug=False ): ### units V
        self.instr.write( "{}:COUPLING D50".format(chn) )
        self.instr.write( "{}:VOLT_DIV {}".format(chn,vscale) )
        self.instr.write( "{}:OFFSET {}".format(chn,voff) )
        self.instr.write( "{}:TRACE ON".format(chn) )
        if(debug):
            print( "{} scale:".format(chn), self.instr.ask( "{}:VOLT_DIV?".format(chn) ) )
            print( "{} offset:".format(chn), self.instr.ask( "{}:OFFSET?".format(chn) ) )

    def setTime( self, hscale=0.0002, hdel=0.0, debug=False ): ### units seconds
        self.instr.write( "TIME_DIV {}".format(hscale) )
        self.instr.write( "TRIG_DELAY {}".format(hdel) )
        if(debug):
            print( "Timebase scale:", self.instr.ask( "TIME_DIV?" ) )
            print( "Timebase offset:", self.instr.ask( "TRIG_DELAY?" ) )

    def acquire( self, cnt=1000, fileSuffix="tmp" ):
        print("START ACQUISITION FOR RUN {} => {} events".format(fileSuffix,cnt))
        self.instr.write( "TRIG_MODE STOP" )
        self.instr.write( "DISPLAY OFF" )
        self.instr.write( "STORE_SETUP ALL_DISPLAYED,HDD,AUTO,OFF,FORMAT,BINARY" )
        self.instr.write( "SEQ ON, {}".format(cnt) )
        self.instr.write( "WAIT" )
        self.instr.ask( "*OPC?" )
        self.instr.write( "TRIG_MODE SINGLE" )
        self.instr.write( "ARM" )
        self.instr.write( "WAIT" )
        self.instr.ask( "*OPC?" )
        self.instr.write( "TRIG_MODE STOP" )
        self.instr.write( "vbs 'app.SaveRecall.Waveform.TraceTitle=\"{}\"'".format(fileSuffix) )
        self.instr.write( "vbs 'app.SaveRecall.Waveform.SaveFile'" )
        self.instr.write( "WAIT" )
        self.instr.ask( "*OPC?" )
        print("FINISHED SUCCESSFULLY")

