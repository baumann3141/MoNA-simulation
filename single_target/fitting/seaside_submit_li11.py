"""To submit a directory of PACE outputs that need to be run through
GEANT4 as jobs to the seaside cluster at the NSCL"""
import subprocess
import os
import sys

def pbs_maker(idir,path,par1,par2,par3,par4,par5):
    """Make the .pbs job submit file given a .out file

    Keyword Arguments:
    idir - input file directory
    ifile - input .out file name
    """
    #ofname = "submit_Ei"+str(par1)+"_Swave"+str(par2)+"_SV"+str(par3)+"_L"+str(par4)+"_Gin"+str(par5)+".pbs"
    ofname = "submit_Ei"+str(par1)+"_Gi"+str(par2)+"_Ev"+str(par3)+"_Gv"+str(par4)+"_path"+str(path)+".pbs"
    ofile = open(idir+"/"+ofname,"w")
    # start writing the submission script
    ofile.write("#!/bin/bash\n")
    ofile.write("#PBS -M smithj@nscl.msu.edu\n")
    #ofile.write("### name of job, something meaningful\n")
    #ofile.write("#PBS -N Li13_2nDecay\n")
    #ofile.write("### email on _a_bort, _b_egin, _e_nd\n")
    #ofile.write("#PBS -m abe\n")
    #ofile.write("### combine stdout/stderr\n")
    #ofile.write("#PBS -j oe\n")
    #ofile.write("### resource requests (time in HH:MM:SS)\n")
    ofile.write("#PBS -l nodes=1 -l walltime=3:00:00\n")
    #ofile.write("export PBS_O_WORKDIR='"+idir+"'\n")
    #ofile.write("cd ${PBS_O_WORKDIR}\n")
    ofile.write("printenv\n");
    ofile.write("printenv USER\n");
    ofile.write("printenv HOME\n");
    ofile.write("source /opt/lucid/GEANT4/current/env.sh \n");
    ofile.write("cd /projects/mona-sim/smithj/st_mona/bin/\n")
    ofile.write("/projects/mona-sim/smithj/st_mona/bin/9li_2ndecay_path"+str(path)+"_single.sh "+str(par1)+" "+str(par2)+" "+str(par3)+" "+str(par4)+"\n")
    ofile.close()
    return ofname

def frange(start, end=None, inc=None):
    
    if end == None:
        end = start + 0.0
        start = 0.0

    if inc == None:
        inc = 1.0

    L = []
    while 1:
        next = start + len(L) * inc
        if inc > 0 and next > end:
            break
        elif inc < 0 and next < end:
            break
        L.append(next)
        
    return L

import time

def jobmaker():
    """Make .pbs files for each .out file and submit job
    
    Keyword Arguments:
    rootdir - root directory to look for .out files in
    """
    # loop over parameters of interest

    #-----------------------11Li states----------------------#
    #location of 800 keV state in 11Li
    #energyb = [0.9]
    #widthb = [3.0]

    # parameter space for 800 keV state in 11Li
    energyb = [0.3, 0.5, 0.7, 0.9, 1.1, 1.3, 1.5]
    widthb = [0.1, 0.3, 0.5, 0.75, 1.0, 1.25, 1.5, 2.0, 2.5]
    #widthb = [0.1, 0.3, 0.5, 0.7, 0.9, 1.1, 1.3, 1.5, 2.0, 2.5, 3.0, 3.5, 4.5, 6.5]

    #location of 3.5 MeV state in 11Li
    #energya = [4.5]    #Ei  total energy
    #widtha = [5.0]            #Gi total decay width

    # parameter space for 3.5 MeV state in 11Li
    energya = [1.0, 2.0, 3.0, 4.0, 5.0]
    #energya = [1.0, 2.0, 3.0, 4.0, 5.0, 6.0]
    widtha = [0.25, 0.5, 0.75, 1.0, 1.5, 2.0, 3.0, 5.0]
    #widtha = [0.25, 0.5, 0.75, 1.0, 1.5, 2.0, 2.5, 3.0, 5.0, 7.0, 9.0]
   
    #-----------------------10Li states----------------------#
    # location of s-decay state in 10Li around 50 keV
    #energyf = [0.025]
    #widthf = [1.0]

    # parameter space for s-wave state in 10Li around 50 keV
    energyf = [0.025, 0.05, 0.075,0.1,0.125]
    widthf = [0.2, 0.4, 0.6, 0.8, 1.0, 1.1, 1.2]

    # location of p-decay state in 10Li around 200 keV
    #energye = [0.1]
    #widthe = [0.1]

    # parameter space of p-decay state in 10Li around 200 keV
    energye = [0.1, 0.15, 0.2, 0.25, 0.3]
    widthe = [0.05, 0.1, 0.2, 0.3, 0.4, 0.5]

    # location of p-decay state in 10Li around 525 keV
    #energyd = [0.55]
    #widthd = [1.0]

    # parameter space of p-decay state in 10Li around 525 keV
    energyd = [0.5, 0.55, 0.6, 0.65, 0.7]
    widthd = [0.25, 0.3, 0.5, 0.6, 0.8, 1.0, 1.2, 1.4]

    # location of p-decay state in 10Li around 1.2 MeV
    #energyc = [1.15]
    #widthc = [0.001]

    # parameter space of p-decay state in 10Li around 1.2 MeV
    energyc = [0.7, 0.85, 1.0, 1.15, 1.3]
    widthc = [0.1, 0.2, 0.3, 0.4, 0.5, 0.6]

    # null parameter space
    null = [0]

    #paths = [1,2,3,4,5,7,8,10,16,17,18,19]
    #paths = [1,2,3,4,5,7,8,10,11,12,13,14,16,17,18,19]
    paths = [2,4]
    Lvalue = 1
#
    #count up the total number of jobs to submit
    totalruns = 0
    for path in paths:
        if path==1:
	    p1 = energyb
	    p2 = widthb
	    p3 = energyd
	    p4 = widthd
	elif path == 2:
	    p1 = energyb
	    p2 = widthb
	    p3 = energyf
	    p4 = widthf
	elif path == 3:
	    p1 = energya
	    p2 = widtha
	    p3 = energyd
	    p4 = widthd
	elif path == 4:
	    p1 = energya
	    p2 = widtha
	    p3 = energyf
	    p4 = widthf
	elif path == 5:
	    p1 = energyb
	    p2 = widthb
	    p3 = null
	    p4 = null
	elif path == 7:
	    p1 = energyb
	    p2 = widthb
	    p3 = null
	    p4 = null
	elif path == 8:
	    p1 = energya
	    p2 = widtha
	    p3 = null
	    p4 = null
	elif path == 10:
	    p1 = energya
	    p2 = widtha
	    p3 = null
	    p4 = null
	elif path == 11:
	    p1 = energyd
	    p2 = widthd
	    p3 = null
	    p4 = null
	elif path == 12:
	    p1 = energyf
	    p2 = null
	    p3 = null
	    p4 = null
	elif path == 13:
	    p1 = energye
	    p2 = widthe
	    p3 = null
	    p4 = null
	elif path == 14:
	    p1 = energyc
	    p2 = widthc
	    p3 = null
	    p4 = null
	elif path == 16:
	    p1 = energyb
	    p2 = widthb
	    p3 = energyc
	    p4 = widthc
	elif path == 17:
	    p1 = energyb
	    p2 = widthb
	    p3 = energye
	    p4 = widthe
	elif path == 18:
	    p1 = energya
	    p2 = widtha
	    p3 = energyc
	    p4 = widthc
	elif path == 19:
	    p1 = energya
	    p2 = widtha
	    p3 = energye
	    p4 = widthe
        for test1 in p1:
            for test2 in p2:
                for test3 in p3:                
                    for test4 in p4:
	#		    if test1*2>=test2 and test3*2>=test4:
			    if test1*2>=test2:
	                        totalruns = totalruns+1

    print "Total JOBS = ", totalruns
    
    count = 0
    tcount = 0

    datadir = "/mnt/simulations/MoNA_11Li/"

    for path in paths:
        if path==1:
	    p1 = energyb
	    p2 = widthb
	    p3 = energyd
	    p4 = widthd
	elif path == 2:
	    p1 = energyb
	    p2 = widthb
	    p3 = energyf
	    p4 = widthf
	elif path == 3:
	    p1 = energya
	    p2 = widtha
	    p3 = energyd
	    p4 = widthd
	elif path == 4:
	    p1 = energya
	    p2 = widtha
	    p3 = energyf
	    p4 = widthf
	elif path == 5:
	    p1 = energyb
	    p2 = widthb
	    p3 = null
	    p4 = null
	elif path == 7:
	    p1 = energyb
	    p2 = widthb
	    p3 = null
	    p4 = null
	elif path == 8:
	    p1 = energya
	    p2 = widtha
	    p3 = null
	    p4 = null
	elif path == 10:
	    p1 = energya
	    p2 = widtha
	    p3 = null
	    p4 = null
	elif path == 11:
	    p1 = energyd
	    p2 = widthd
	    p3 = null
	    p4 = null
	elif path == 12:
	    p1 = energyf
	    p2 = null
	    p3 = null
	    p4 = null
	elif path == 13:
	    p1 = energye
	    p2 = widthe
	    p3 = null
	    p4 = null
	elif path == 14:
	    p1 = energyc
	    p2 = widthc
	    p3 = null
	    p4 = null
	elif path == 16:
	    p1 = energyb
	    p2 = widthb
	    p3 = energyc
	    p4 = widthc
	elif path == 17:
	    p1 = energyb
	    p2 = widthb
	    p3 = energye
	    p4 = widthe
	elif path == 18:
	    p1 = energya
	    p2 = widtha
	    p3 = energyc
	    p4 = widthc
	elif path == 19:
	    p1 = energya
	    p2 = widtha
	    p3 = energye
	    p4 = widthe

        print path
        for par1 in p1:
            for par2 in p2:
                for par3 in p3:
                    for par4 in p4:
#		      if par1*2>=par2 and par3*2>=par4:
		      if par1*2>=par2:
                        if tcount>=50:
                            print "Sleeping, Total Jobs Submitted: ",count," of ",totalruns
                            print "          Jobs Left to Run = ",totalruns-count
                            while True:                                   
                                time.sleep(1)
                                #tcount=0
                                ss=0
                                pbs_status = subprocess.Popen("qselect -u smithj -s RQ",shell=True,stdout=subprocess.PIPE)
                                ss = pbs_status.stdout.read()
                                pbs_status.stdout.close()
                                pbs_status.wait()
				runningjobs = len(ss.strip().split('\n'))
                                if runningjobs<50:
				    tcount=runningjobs
                                    print "Finished current round of jobs...continue on"
				    #subprocess.Popen("rm -f /mnt/simulations/MoNA_11Li/submit*",shell=True,stdout=subprocess.PIPE)
                                    break     

                        print par1, par2, par3, par4
                        pbsname = pbs_maker(datadir,path,par1,par2,par3,par4,Lvalue);
                        proc = subprocess.Popen("qsub "+datadir+"/"+pbsname,shell=True);
                        proc.wait()
                    
                        tcount = tcount + 1
                        count = count + 1
                        print "---"
    print "END JOB SUBMISSION"

    print "Total submitted jobs"
    print count

                                                            
if __name__=="__main__":
    #jobmaker("/evtdata/mg_frag/all_output/Ne30_scan")
    jobmaker()
