#!/usr/bin/python
import sys
import optparse
import os
import subprocess

def valid_arch():
    """Makes sure that the computer architecture is correct to run Geant"""
    command = "function blobs {\nif [ $(file /bin/bash | awk "+"'{ print $3 }') == "+'"64-bit" ]; then\nreturn 1\nelse return 0\nfi\n}\nblobs'
    proc =  subprocess.Popen(command,shell=True) # This executes the shell command to check the architecture
    return proc.wait() # This returns the return value of the shell command

def valid_computer():
    """The exception to the architecture rule is when it is running on Shea's laptop, hence this function"""
    command = "uname -r"
    proc = subprocess.Popen(command,shell=True,stdout=subprocess.PIPE) # spawns the process to check if it's on my laptop
    info = proc.communicate() # waits till the end of the process, then info is {stdout,stderror}
    outstring = info[0]       # Look at the stdout
    if outstring.rfind('-ARCH') != -1: # and check if it's on arch linux
        return 1
    else:
        return 0

def main():
    argparser = optparse.OptionParser()

    # Here is where you specify what the flags and arguments are
    argparser.add_option('--infile','-i',dest='ifile',help='specify input file',default=False)
    argparser.add_option('--outfile','-o',dest='ofile',help='specify output file',default=False)
    argparser.add_option('--mode','-m',dest='mode',help='specify geant run mode. Options are "batch" and "manual"... default is batch',default="batch")
    argparser.add_option('--verbosity','-v',dest='verbosity',help='specify verbosity of output, e.g. how much information is kept around',default=0)
    argparser.add_option('--threshold','-t',dest='threshold',help='specify hardware thresholds in MeVee',default=0.)
    argparser.add_option('--nneutrons','-n',dest='nneuts',help='Specify number of neutrons getting shot out of st-mona',default=2)
    argparser.add_option('--tgtid'    ,'-g',dest='tgtid' ,help='Specify which Be target the reactions happened in',default=1)
    argparser.add_option('--visualization','-s',dest='visualization',help='Turn visualization on and off. Options are vison and visoff -- default if visoff',default="visoff")
    argparser.add_option('--uniqueID','-u',dest='uniqueID',help='Unique ID needed to run multiply geant4 sims at same time',default="z")
    argparser.add_option('--dirdata','-d',dest='dirdata',help='data directory',default="./")
    argparser.add_option('--monaConfig','-c',dest='monaConfig',help='MoNA Detector Configuration File',default="./")

    # Here is where you transfer that to variables the code will use
    options, arguments = argparser.parse_args()
    ifile = options.ifile
    ofile = options.ofile
    mode = options.mode
    verbosity = options.verbosity
    threshold = float(options.threshold)
    nneuts = int(options.nneuts)
    tgtid  = int(options.tgtid)
    visualization = options.visualization
    uniqueID = options.uniqueID
    dirdata = options.dirdata
    monaconfig = options.monaConfig

    print uniqueID


    # Do an architecture check because there are issues with Geant on 32 bit machines
    print "Checking computer architecture..."
    if 1:
        print "...ok!"
        # First, call the c++ code which makes the temporary pythia file out of the .root input file
        print "Analyzing st-mona output..."
        os.system("./root2pythia "+ifile+" "+str(nneuts)+" "+str(tgtid)+" "+uniqueID+" "+dirdata)
        print "Running root2pythia..."
        if nneuts == 2:
            print "Running geant for two neutrons...\n"
            # Next, run the geant simulation
            print "Starting up Geant for 1st neutron..."
            os.system("cd ../ && n2_vault "+mode+" "+str(threshold)+" "+visualization+" "+uniqueID+" "+dirdata+" "+monaconfig)
            #Copy files to run the second neutron through geant
            os.system("cp "+dirdata+"/temp"+uniqueID+".indat "+dirdata+"/neutron1temp"+uniqueID+".indat && cp "+dirdata+"/neutron2temp"+uniqueID+".indat "+\
		       dirdata+"/temp"+uniqueID+".indat && cp "+dirdata+"/default"+uniqueID+".root "+dirdata+"/neutron1"+uniqueID+".root")
            # Next, run 2nd neutron though the geant simulation
            print "Starting up Geant for 2nd neutron..."
            os.system("cd ../ && n2_vault "+mode+" "+str(threshold)+" "+visualization+" "+uniqueID+" "+dirdata+" "+monaconfig)
            #Copy files to run the second neutron through geant
            os.system("cp "+dirdata+"/default"+uniqueID+".root "+dirdata+"/neutron2"+uniqueID+".root")
            # Now, run the c++ code which merges the Geant output with the .root file
            print "Merging st-mona and Geant output files..."
            os.system("./merge "+ifile+' '+ofile+" "+str(nneuts)+' '+uniqueID+' '+dirdata)
            # Finally, remove the temporary input file and output file
            print "Also not deleting the temporary files"
            if not verbosity:
                print "Cleaning up..."
                subprocess.Popen("cd "+dirdata+" && rm temp"+uniqueID+".indat && rm temp"+uniqueID+".mac && rm default"+uniqueID+".root",shell=True)
                subprocess.Popen("cd "+dirdata+" && rm neutron1temp"+uniqueID+".indat && rm neutron1"+uniqueID+".root",shell=True)
                subprocess.Popen("cd "+dirdata+" && rm neutron2temp"+uniqueID+".indat && rm neutron2"+uniqueID+".root",shell=True)
            print "Done!"
        elif nneuts == 3:
            print "Running geant for two neutrons...\n"
            # Next, run the geant simulation
            print "Starting up Geant for 1st neutron..."
            os.system("cd ../ && n2_vault "+mode+" "+str(threshold)+" "+visualization+" "+uniqueID+" "+dirdata+" "+monaconfig)
            #Copy files to run the second neutron through geant
            os.system("cp "+dirdata+"/temp"+uniqueID+".indat "+dirdata+"/neutron1temp"+uniqueID+".indat && cp "+dirdata+"/neutron2temp"+uniqueID+".indat "+\
	    	       dirdata+"/temp"+uniqueID+".indat && cp "+dirdata+"/default"+uniqueID+".root "+dirdata+"/neutron1"+uniqueID+".root")
            # Next, run 2nd neutron though the geant simulation
            print "Starting up Geant for 2nd neutron..."
            os.system("cd ../ && n2_vault "+mode+" "+str(threshold)+" "+visualization+" "+uniqueID+" "+dirdata+" "+monaconfig)
            #Copy files to run the second neutron through geant
            os.system("cp "+dirdata+"/default"+uniqueID+".root "+dirdata+"/neutron2"+uniqueID+".root")
            # Now, run the c++ code which merges the Geant output with the .root file
            #print "Merging st-mona and Geant output files..."
            #os.system("./merge "+ifile+' '+ofile+" "+str(nneuts)+' '+uniqueID+' '+dirdata)
            # Finally, remove the temporary input file and output file
            #print "Also not deleting the temporary files"

	        #Copy files to run the third neutron through geant
            os.system("cp "+dirdata+"/temp"+uniqueID+".indat "+dirdata+"/neutron2temp"+uniqueID+".indat && cp "+dirdata+"/neutron3temp"+uniqueID+".indat "\
		       +dirdata+"/temp"+uniqueID+".indat && cp "+dirdata+"/default"+uniqueID+".root "+dirdata+"/neutron2"+uniqueID+".root")
            # Next, run 3rd neutron though the geant simulation
            print "Starting up Geant for 3rd neutron..."
            os.system("cd ../ && n2_vault "+mode+" "+str(threshold)+" "+visualization+" "+uniqueID+" "+dirdata+" "+monaconfig)
            #Copy files to run the second neutron through geant
            os.system("cp "+dirdata+"/default"+uniqueID+".root "+dirdata+"/neutron3"+uniqueID+".root")
            # Now, run the c++ code which merges the Geant output with the .root file
            print "Merging st-mona and Geant output files..."
            os.system("./merge "+ifile+' '+ofile+" "+str(nneuts)+' '+uniqueID+' '+dirdata)
            # Finally, remove the temporary input file and output file
            print "Also not deleting the temporary files"
            if not verbosity:
                print "Cleaning up..."
                subprocess.Popen("cd "+dirdata+" && rm temp"+uniqueID+".indat && rm temp"+uniqueID+".mac && rm default"+uniqueID+".root",shell=True)
                subprocess.Popen("cd "+dirdata+" && rm neutron1temp"+uniqueID+".indat && rm neutron1"+uniqueID+".root",shell=True)
                subprocess.Popen("cd "+dirdata+" && rm neutron2temp"+uniqueID+".indat && rm neutron2"+uniqueID+".root",shell=True)
                subprocess.Popen("cd "+dirdata+" && rm neutron3temp"+uniqueID+".indat && rm neutron3"+uniqueID+".root",shell=True)

            print "Done!"

        elif nneuts == 4:
            print "Running geant for four neutrons...\n"
            # Next, run the geant simulation
            print "Starting up Geant for 1st neutron..."
            os.system("cd ../ && n2_vault "+mode+" "+str(threshold)+" "+visualization+" "+uniqueID+" "+dirdata+" "+monaconfig)
            #Copy files to run the second neutron through geant
            os.system("cp "+dirdata+"/temp"+uniqueID+".indat "+dirdata+"/neutron1temp"+uniqueID+".indat && cp "+dirdata+"/neutron2temp"+uniqueID+".indat "+\
	    	       dirdata+"/temp"+uniqueID+".indat && cp "+dirdata+"/default"+uniqueID+".root "+dirdata+"/neutron1"+uniqueID+".root")
            # Next, run 2nd neutron though the geant simulation
            print "Starting up Geant for 2nd neutron..."
            os.system("cd ../ && n2_vault "+mode+" "+str(threshold)+" "+visualization+" "+uniqueID+" "+dirdata+" "+monaconfig)
            #Copy files to run the second neutron through geant
            os.system("cp "+dirdata+"/default"+uniqueID+".root "+dirdata+"/neutron2"+uniqueID+".root")
            # Now, run the c++ code which merges the Geant output with the .root file
            #print "Merging st-mona and Geant output files..."
            #os.system("./merge "+ifile+' '+ofile+" "+str(nneuts)+' '+uniqueID+' '+dirdata)
            # Finally, remove the temporary input file and output file
            #print "Also not deleting the temporary files"

	    #Copy files to run the third neutron through geant
            os.system("cp "+dirdata+"/temp"+uniqueID+".indat "+dirdata+"/neutron2temp"+uniqueID+".indat && cp "+dirdata+"/neutron3temp"+uniqueID+".indat "\
		       +dirdata+"/temp"+uniqueID+".indat && cp "+dirdata+"/default"+uniqueID+".root "+dirdata+"/neutron2"+uniqueID+".root")
            # Next, run 3rd neutron though the geant simulation
            print "Starting up Geant for 3rd neutron..."
            os.system("cd ../ && n2_vault "+mode+" "+str(threshold)+" "+visualization+" "+uniqueID+" "+dirdata+" "+monaconfig)
            #Copy files to run the third neutron through geant
            os.system("cp "+dirdata+"/default"+uniqueID+".root "+dirdata+"/neutron3"+uniqueID+".root")

	    #Copy files to run the fourth neutron through geant
            os.system("cp "+dirdata+"/temp"+uniqueID+".indat "+dirdata+"/neutron3temp"+uniqueID+".indat && cp "+dirdata+"/neutron4temp"+uniqueID+".indat "\
		       +dirdata+"/temp"+uniqueID+".indat && cp "+dirdata+"/default"+uniqueID+".root "+dirdata+"/neutron3"+uniqueID+".root")
            # Next, run 4th neutron though the geant simulation
            print "Starting up Geant for 4th neutron..."
            os.system("cd ../ && n2_vault "+mode+" "+str(threshold)+" "+visualization+" "+uniqueID+" "+dirdata+" "+monaconfig)
            #Copy files to run the 4th neutron through geant
            os.system("cp "+dirdata+"/default"+uniqueID+".root "+dirdata+"/neutron4"+uniqueID+".root")


            # Now, run the c++ code which merges the Geant output with the .root file
            print "Merging st-mona and Geant output files..."
            os.system("./merge "+ifile+' '+ofile+" "+str(nneuts)+' '+uniqueID+' '+dirdata)
            # Finally, remove the temporary input file and output file
            print "Also not deleting the temporary files"
            if not verbosity:
                print "Cleaning up..."
                subprocess.Popen("cd "+dirdata+" && rm temp"+uniqueID+".indat && rm temp"+uniqueID+".mac && rm default"+uniqueID+".root",shell=True)
                subprocess.Popen("cd "+dirdata+" && rm neutron1temp"+uniqueID+".indat && rm neutron1"+uniqueID+".root",shell=True)
                subprocess.Popen("cd "+dirdata+" && rm neutron2temp"+uniqueID+".indat && rm neutron2"+uniqueID+".root",shell=True)
                subprocess.Popen("cd "+dirdata+" && rm neutron3temp"+uniqueID+".indat && rm neutron3"+uniqueID+".root",shell=True)
		subprocess.Popen("cd "+dirdata+" && rm neutron4temp"+uniqueID+".indat && rm neutron3"+uniqueID+".root",shell=True)
            print "Done!"

        elif nneuts == 1:
            print "Running geant for a single neutron...\n"
            # Next, run the geant simulation
            print "Starting up Geant..."
            os.system("cd ../ && n2_vault "+mode+" "+str(threshold)+" "+visualization+" "+uniqueID+' '+dirdata+" "+monaconfig)
            # Now, run the c++ code which merges the Geant output with the .root file
            print "Merging st-mona and Geant output files..."
            os.system("./merge "+ifile+' '+ofile+" "+str(nneuts)+' '+uniqueID+' '+dirdata)
            # Finally, remove the temporary input file and output file
            if not verbosity:
                print "Cleaning up..."
                subprocess.Popen("cd "+dirdata+" && rm temp"+uniqueID+".indat && rm temp"+uniqueID+".mac && rm default"+uniqueID+".root",shell=True)
            print "Done!"
        else:
            print "Bad -n parameter (number of neutrons). Options are 0 or 1. I am exiting.\n"
    else:
        print "Should not run Geant4 on this computer, goto fishtank"


if __name__ == "__main__":
    main()
