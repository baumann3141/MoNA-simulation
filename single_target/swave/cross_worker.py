"""To calculate cross sections"""
import subprocess
import time
import multiprocessing
import Queue
import multiprocessing.managers
import threading

import cmath
import math
import ROOT as r

r.gSystem.Load("$STMONA/swave/crosscalc_C.so")

def cross_calc(inputs):
    print "running calculation"
    ccalc = r.crosscalc()
    imax=300
    ef = inputs.ef
    phase = inputs.phase

    #ANC - Asymptotic normalization constant
    ccalc.ca=1.31#for 14B
    #  ca=1.987 #for 14Be

    #constantes
    ccalc.pi=3.14159
    ccalc.v=11.1
    #ccalc.v=10.0996# correspond aux 22n 1/2 target velocity
    #ccalc.v=9.374# correspond aux 49.5 MeV/A
    # v=9.348 # correspond a 49.5 MeV/A
    ccalc.a=0.75
    ccalc.m=1.0432
    ccalc.h=6.5821
    #ccalc.ei=-1.847#pour be14
    #ccalc.ei=-0.969#pour b14
    #ccalc.ei=-1.28#pour n22
    #ccalc.ei=-8.00#pour testing
    #ccalc.ei=-2.00#pour testing
    #ccalc.ei=-4.418 #for 28Ne - JKS, 21 Sept 2011 - binding energy of the scatter particle, in this case the neutron separation energy, for beam (in this case 29
    #ccalc.ei=-4.878 #for 13B - JKS, 9 April 2013 - binding energy of the scattered particle, in this case the neutron separation energy, for beam (in this case 13B)
    ccalc.ei=-3.37 #for 12B - JKS, 12 July 2013 - binding energy of the scattered particle, in this case the neutron separation energy, for beam (in this case 12B)
    #ei=-1.847 #pour be14
    ccalc.gamma=math.sqrt(-2.*ccalc.m*ccalc.ei)/ccalc.h
    ccalc.Vun=7628.93 # integrale volume pour 9Be
    ccalc.rs=5.81 #9Li+9Be - JKS, 3 May 2013 (11Be is a halo nucleus)
    #ccalc.rs=6.67 #11Be+9Be - JKS, 9 April 2013 (11Be is a halo nucleus)
    #ccalc.rs=7.1 #27Ne+9Be - JKS, 23 Nov 2011
    #ccalc.rs=6.71230# pour 20+9
    #ccalc.rs=6.28632# pour 12+9

    ccalc.de=0.03
    ccalc.dz=0.001
    ccalc.db=0.1

    ccalc.li=0.
    ccalc.mi=0.
    ccalc.lf=0.
    ccalc.mf=0.

    ccalc.constant=0.5*((ccalc.Vun*ccalc.ca*ccalc.gamma/(ccalc.h*ccalc.h*ccalc.v))**2)*ccalc.m/(ccalc.pi*(2.*ccalc.li+1.))

    ccalc.ef=0.
    ef0 = ccalc.de 

    ccalc.ef = ef
    crossfin = ccalc.calculate_cross(phase)
    cross_done = 10.*2.*math.pi*crossfin

    rvalue = [ef,cross_done]
    return rvalue


    """
    # loop over the file
    i = 0
    ds = 1. # "downscale" factor
    for i,line in enumerate(ifile):
        if i > 150:
            # stop after 1.5 MeV because we don't need that right now
            break
        if i%ds != 0:
            continue

        print "line",i
        phase = float(line)
        ccalc.ef=ef0+ccalc.de*float(i);

        crossfin = ccalc.calculate_cross(phase)

        ofile.write(str(ccalc.ef) + " " +  str(10.*2.*math.pi*crossfin) + "\n")
    """
class argclass():
    ef = 0.
    phase = 0.
    def __init__(self,ef,phase):
        self.ef = ef
        self.phase = phase


if __name__=="__main__":
    class QueueManager(multiprocessing.managers.BaseManager):
        pass

    print "here 2"
    QueueManager.register('get_inqueue')
    QueueManager.register('get_outqueue')
    m = QueueManager(address=('helium', 51000), authkey='abracadabra')
    m.connect()
    iq = m.get_inqueue()
    oq = m.get_outqueue()

    #pool_size = multiprocessing.cpu_count()*2 # how many processes we want to run in parallel
    pool_size = 6 # how many processes we want to run in parallel
    pool = multiprocessing.Pool(processes=pool_size) # make a "pool" - way of easily handling simple parallel stuff
    run = True
    while run: # loop over the stuff in queue
        arguments = []
        for i in range(pool_size):
            try: # try to read from the queue
                inputs = iq.get(False) # the false tells it not to block e.g. it won't wait for more
                                        # stuff to arrive in the queue but rather will raise exceptions
                arguments.append(argclass(inputs[0],inputs[1]))
                print "getting input",inputs
                #print ('Pool    :', pool_outputs)
            except Queue.Empty: # qin.get will raise this exception if there's nothing in the queue
                print("end of input queue reached.. exiting",i)
                run = False
                break

        #print "inque",inputs
        #print (inputs[0],inputs[1])
        #args = argclass(inputs[0],inputs[1]) # have to do this class thing to get around rules with the map function
        #print "calling functionszzzzz"
        pool_outputs = pool.map(cross_calc, arguments) # if we got something, use the map funtion to 
                                                 # run the number of processes given with the
                                                 # args give. it wants an input list of args
                                                 # and a target function to run
        for i in pool_outputs:
            #print "puttin",i,"in out queue"
            oq.put(i) # put the results in the output queue
        #print(len(pool_outputs)) # print length of the output to see that it's what we expect

    #m.connect() # ping the server at the end to make sure he reads one last time

    # and we're done
    print "QUIT"
