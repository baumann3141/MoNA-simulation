"""To run a number of phase shift calculations varying the depth of the woods saxon potential"""
import numpy as np
import pylab as pl
import subprocess
def go_phase():
    """Run the calculation of the phase shift for particular settings"""
    pass

def write_phase():
    """Write the calculated phase shift information to file"""
    pass
def format_inp(args,fname):
    """Format the input file that the fortran needs"""
    ofile = open(fname,"w")
    ostring = ""
    for arg in args:
        ostring += str(arg)+" "
    ostring = ostring[0:-1] # chop off the last space because its stupid
    ofile.write(ostring)
    ofile.close()

if __name__=="__main__":
    # set up the range of potentials
    pots = [i for i in np.arange(44.35,46.55,0.01)]
    A = 9              # A of core nucleus
    Z = 3               # Z of core nucleus
    Rws = 1.27         # Woods-Saxon radius for Rws*(A**(1.3))
    Aws = 0.75         # Woods-Saxon diffuseness
    Vim = 0.0          # Imaginary WS depth
    Rim = 0.0          # Imaginary WS radius
    Aim = 0.3          # imaginary WS diffuseness
    Vso = 6.9          # Spin-Orbit strength
    Rso = 1.27         # SO radius
    Aso = 0.75         # SO diffuseness
    Nsteps = 300       # number of steps to consider - 300 is the max
    deltastep = 0.03    # change in energy between each step
    alphapar = 0.0     # potential correction term... see the paper for details

    # now begin the actual loop over the parameters and run some crap
    for Vws in pots:
        args = [A,Z,Vws,Rws,Aws,Vim,Rim,Aim,Vso,Rso,Aso,Nsteps,deltastep,alphapar]
        format_inp(args,"scan_phase.inp") # make the input file the fortran wants

        # make a list of all the shell commands we wanna run in sequence
        #cmds = ["rm smali.dat","f2c niclisma.f","gcc -lf2c -lm -o nic niclisma.c",'echo "scan_phase">gui',"./nic < gui","gfortran phase.for","./a.out","awk '{if(FNR==1){print \""+str(Vws)+" \"$2}}' Qscatlengthn_Be12.dat >> pot_scat.dat","rm Q*","gfortran simp.for","./a.out","rm phase_s.dat","rm nic","rm gui","rm a.out","mv phase_s2.dat phase_ang.dat"]
        cmds = ["rm smali.dat","f2c niclisma.f","gcc -lf2c -lm -o nic niclisma.c",'echo "scan_phase">gui',"./nic < gui","gfortran phase.for","./a.out","awk '{if(FNR==1){print \""+str(Vws)+" \"$2}}' Qscatlengthn_Be12.dat >> pot_scat.dat","rm Q*","gfortran simp.for","./a.out","rm phase_s.dat","rm nic","rm gui","rm a.out","mv phase_s2.dat phase_ang.dat","cp phase_ang.dat ../li10/phase_ang_"+str(Vws)+".dat"]
        # go ahead and run all the shell commands in sequence
        for command in cmds:
            proc = subprocess.Popen(command,shell=True) # convert the fortran to c
            proc.wait()


