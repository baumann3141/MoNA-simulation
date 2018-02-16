import subprocess
listthing = [15.325,
             16.325,
             17.325,
             18.325,
             19.325,
             20.325,
             21.325,
             22.325,
             23.325,
             24.325,
             25.325,
             26.325,
             27.325,
             28.325,
             29.325,
             30.325,
             31.325,
             32.325,
             33.325,
             34.325,
             35.325]
for item in listthing:
    inpfile = open("nn-Li11_sctl.inp","w")
    inpfile.write("11 3 "+str(item)+" 1.27 .75 0. 0. .3 6.9  1.27 .75 3000 .001 0.")
    inpfile.close()
    cmd = "./go_sctl"
    proc = subprocess.Popen(cmd,shell=True,stdout=subprocess.PIPE)
    proc.communicate()
    cmd = "mv -f scatlengthn_Be12.dat ~/analysis/swave_archive/Cross_artemis/scatlength_"+str(item)+".dat"
    proc = subprocess.Popen(cmd,shell=True,stdout=subprocess.PIPE)
    proc.communicate()

