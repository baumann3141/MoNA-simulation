"""To reproduce the functionality of nicleisma.for in calculating s-matrices"""
import math
class swavecalc():
    """Class to do s-wave calculations. For the moment it's a pos of pseudo-fortran. That'll get fixed eventually"""
    def __init__(self):
        self.optout = 0.
        self.param = []
        self.parin = []
        self.parmin = []
        self.parmax = []
        for i in range(14):
            self.param.append(0.)
            self.parin.append(0.)
            self.parmin.append(0.)
            self.parmax.append(0.)
        self.rmax = 0.
        self.dr = 0.
        self.sr = []
        self.si = []
        for i in range(251):
            temp = []
            for j in range(5):
                temp.append(0.0)
            self.sr.append(temp)
            self.si.append(temp)
        self.nint = 0.
        self.mwvs = 0.
        self.mxl = 0.
        self.lmx = 0.
        self.lmn = 0.
        self.lset = 0.
        self.ldum = 0.
        self.optin = 0.
        self.ap3 = 0.
        self.athird = 0.
        self.cmu = 0.
        self.gamma = 0.
        self.kwn = 0.
        self.ksq = 0.
        self.drin = 0.
        self.rmaxin = 0.
        self.amp = 0.
        self.amt = 0.
        self.alpar = 0.
        self.sdfc=0.
        self.sdfa=0.
        self.sdfb=0.
        self.ecm = 0.
        self.constr = [0,1,2,3,4,5,6,7,8,9,10,11,12,13]
            
    #RMX(R,A,V,RMX0)=R+A*DLOG(DMAX1(V,1.0D-3)/(1.0D-3*ECM)*DMAX1(A,1.0D-3)*(KWN+(GAMMA+KWN*R)/RMX0))
    def RMX(R,A,V,RMX0):
        """Function to figure out rmax.... whatever that is"""
        return R+A*math.log(max(V,1.0e-3)/(1.0e-3*self.ecm)*max(A,1.0e-3)*(self.kwn+(self.gamma+self.kwn*R)/RMX0))

    def optmod(self,newint,error):
        """Optical model calculation... i dunno what it does yet"""
        USX  =self.param[ 1]
        RSX  =self.param[ 2]
        AS   =self.param[ 3]
        WSX  =self.param[ 4]
        WDX  =self.param[ 5]
        RIX  =self.param[ 6]
        AI   =self.param[ 7]
        USPX =self.param[ 8]
        WSPX =self.param[ 9]
        RSPX =self.param[10]
        ASP  =self.param[11]
        RCX  =self.param[12]
        NORM =self.param[13]

        # variables I don't know what to do with yet
        MICI = 0.
        MICR = 0.
        MICS = 0.
        NEWINT = 0
        WSTWO = 0.


        ispin=2
        LSTYPE=1
        # correction 18 july 89,ISPIN=2 ON 4 SEP 89
        error=False
        ETA1=1.0e-8
        R12=1.0/12.0
        COR=1.0/30.0

        for index in range(1,13):
            # this is going to be a logical disaster area for the moment while I sort out what the hell is going on
            # .. basically there are a lot of goto statements in the fortran that I'm effectively reproducing here.
            if  not ((MICI!=0.and index>4. and index<=7) or (MICR!=0 and index>1 and index<=3) or (MICS !=0 and index>=9 and index<=11)):
                II=self.constr[index]
                self.param[index]=self.parin[II]
                if not (self.param[index]>=self.parmin[index] and self.param[index] <= self.parmax[index]):
                    error=True
            if not (MICI==1):
                RIMI=abs(RIX)*self.athird
                if (RIX < 0.0):
                    RIMI=RIMI+ABS(RIX)*ap3
            if not (WSX*RIMI /3.0+2.0*WDX*AI>=0.0):
                print "CALCULATION TERMINATED--VOLUME INTEGRAL OF IMAGINARY POTENTIAL POSITIVE"
                error = True
            if error:
                return

            if not NEWINT:
                SPNORB=False
                MICS1=MICS+1
            #GO TO (13,14),MICS1
            sflag = False
            if MICS1 == 1:
                if not (abs(RSPX)<=0.001 or ASP<=0.001): #GO TO 16
                    if ISPIN > 1:
                        SPNORB=True
                    parmin[11]=.001
                    sflag = True
                    #GO TO 15
            elif MICS1 == 2:
                if not (USPX==0.0 and WSPX==0.0): #GO TO 16
                    if ISPIN>1:
                        SPNORB=True
                    sflag = True
            if not ((sflag and SPNORB) or WSTWO>=5): #GO TO 18
                USPX=0.0
                WSPX=0.0
                RSPX=0.0
                ASP=0.0

    def s_matrix(self,iname="nn-Li11_pha.inp",oname="smali_py.dat"):
        """Calculate s-matrices given an input file name and an output file name
        
        Keyword Arguments:
        iname - name of input file
        oname - name of output file

        Special Note: this is to begin with going to be formatted very much
        like the original fortran. Once I really understand what it's doing
        and verify it, I'll move toward making it relatively readable.
        """
        # First, open up the input file and extract the inputs
        ifile = open(iname)
        ofile = open(oname,"w")
        iline = ifile.readline()
        inputs = iline.split()
        # Check for valid number of inputs as a test of validity
        if len(inputs) != 14:
            print "malformed input file! Exiting..."
            ifile.close()
            return

        # random-ass variables from the fortran that appear to be needed.
        error = False

        # now turn the inputs into variables
        amt    = float(inputs[0])      # A of core
        zt     = float(inputs[1])      # Charge of core
        Ur     = float(inputs[2])      # Depth of real potential
        Rr     = float(inputs[3])      # \rho in \rho A^{1/3}
        Ar     = float(inputs[4])      # diffuseness of real potential
        Wi     = float(inputs[5])      # depth of imaginary potential
        Ri     = float(inputs[6])      # radius of imaginary potential
        Ai     = float(inputs[7])      # diffuseness of imaginary potential
        Usp    = float(inputs[8])      # depth of spin-orbit
        Rsp    = float(inputs[9])      # radius of spin-orbit
        Asp    = float(inputs[10])     # diffuseness of spin-orbit
        nestep = int(inputs[11])     # number of steps in energy
        dein   = float(inputs[12])     # step size in energy (MeV)
        alpar  = float(inputs[13])     # surface correction to potential

        amp=1.                   # A of valence particle
        zp=0.                    # Z of valence particle
        athird=amt**(1.0/3.0)
        ap3=amp**(1.0/3.0)
        # TODO: why the hard coded coef?
        cmu=0.04783258*amp*amt/(amp+amt) # some sort of reduced mass?

        parin = []
        for i in range(14):
            parin.append(0.0)

        parin[1]=Ur
        parin[3]=Ar
        parin[4]=Wi
        parin[7]=Ai
        parin[8]=Usp
        parin[9]=0.    #wsp
        parin[10]=Rsp
        parin[11]=Asp
        parin[12]=1.
        parin[13]=1.
        
        #AEF=EEF     # what does this do? These two characters aren't declared or ever used again

        # now loop on the energy steps
        for i in range(nestep):
            # set relative energy
            einc=float(i+1)*dein # added +1 to reproduce behavior of fortran

            ij=einc-20.
            parin[1]=Ur   #-.145*einc#-24./9.#24*(N-Z)/A Perey
                        # why is this done twice?

            # if(ij)20,30,30 # this is an arithmetic if statement in fortran. Its sensical equivalent is below
            # apprently we're overriding some default settings for the optical potentials.
            # that seems weird and I wonder why it's done.
            if ij < 0.:
                parin[2]=Rr
                parin[6]=Ri
                parin[5]=Wi #!+.365*EINC #WS=13.5
            elif ij >= 0.:
                parin[2]=Rr #-0.005*ij
                parin[6]=Ri #-0.0025*ij
                parin[4]=0. #einc*0.375-7.5!WV
                parin[5]=Wi #!+.365*EINC #WS=13.5

            if einc > 40.:
                parin[4]=0. #7.5-.02*(einc-40)
                parin[5]=Wi #1.666!16.266-0.1*(einc-40)

            if einc > 120:
                parin[4]=0 #5.9
                parin[5]=Wi #1.666!8.226-0.07*(einc-120)

            # loops like we're storing some variables in 3 different locations at this point... what gives?
            a1=parin[1]
            a2=parin[2]
            a5=parin[5]
            a4=parin[4]

            # energy in center of mass
            self.ecm=einc*amt/(amp+amt)
            # TODO: why the hard coded coef?
            self.gamma=.157454*zp*zt*math.sqrt(amp/einc)
            ksq=cmu*self.ecm
            self.kwn=math.sqrt(ksq)
            
            self.optmod(True,error)

            # look for evidence of resonances i guess
            if i != 1:
                if self.si[1][1]*self.sdfc < 0:
                    print 'resonance s',einc

                if self.si[2][1]*self.sdfa < 0:
                    print 'resonance p1/2',einc
                
                if self.si[2][2]*self.sdfb < 0:
                    print'resonance p3/2',einc

            ofile.write(str(einc)) # write the energy to file

            # loop over the relevant matrix elements and write them to file.
            for ij in range(int(self.lmn),int(self.lmx)+1):
                #DO 888 IJ=lmn,lmx
                ab=(ij-1+.5)/self.kwn
       
                #DO 889 J=1,2
                for j in range(1,3):
                    ofile.write(" " + str(self.sr[ij][j]) + " " +str(self.si[ij][j]))

            # set these parameters used to find resonances now. Duplication of data is funky isnt it
            self.sdfc=self.si[1][1]
            self.sdfa=self.si[2][1]
            self.sdfb=self.si[2][2]

            # finally add an end of line
            ofile.write('\n')

if __name__ == "__main__":
    sthing = swavecalc()
    sthing.s_matrix()
