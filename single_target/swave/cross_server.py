import subprocess
import time
import multiprocessing
import Queue
import multiprocessing.managers
import threading
import pexpect
import os

fishtank = ["pike","steelhead","walleye"] # list of fishtank computer names
element = ["krypton"] # list of element computer names


class QueueManager(multiprocessing.managers.BaseManager):
    """Manager of the queue"""
    """
    def listinit(self):
        self.inputs = []
        self.outputs = []
    def appendlist(self,item):
        self.outputs.append(item)
    """


def serve_till_done(server,manager):
    '''
    Run the server until done with queue
    '''
    multiprocessing.managers.current_process()._manager_server = server
    try:
        try:
            while 1:


                try:
                    print "Listening."
                    c = server.listener.accept()
		    print "Done listening."
                    print c
                except (OSError, IOError):
                    continue
                print "Starting things up."
                t = threading.Thread(target=server.handle_request, args=(c,))
                t.daemon = True
                t.start()
                # added this part to default serve_forever to get it to break out
                # when the queue has filled properly
                while 1:
                    try:
                        #print "attempt to read from queue"
                        temp = manager.outqueue.get(False)
                        #print "read",temp
                        manager.outputs.append(temp)
                        #manager.appendlist(temp)
                        #print "now have",manager.outputs
                    except Queue.Empty:
                        #print "failed to read from queue.. empty"
                        break
                if len(manager.outputs) >= len(manager.inputs):
                    #print "output length = input length",len(manager.outputs),len(manager.inputs)
                    raise KeyboardInterrupt
                # END the part that I added
        except (KeyboardInterrupt, SystemExit):
            pass
    finally:
        server.stop = 999
        server.listener.close()

def process_output(output,fname):
    sout = sorted(output,key=lambda out:out[0])
    fout = open(fname,"w")
    for i in sout:
        print "energy",i[0],"cross",i[1]
        fout.write(str(i[0])+" "+str(i[1])+"\n")
    fout.close()

def create_and_run_server(arglist,fname):
    """testing the remote server running crap"""
    # make input / output queues to handle communication
    inqueue = Queue.Queue()
    outqueue = Queue.Queue()


    # make functions that access the queues so the server can actually communicate
    QueueManager.register('get_inqueue', callable=lambda:inqueue)
    QueueManager.register('get_outqueue', callable=lambda:outqueue)
    QueueManager.register('get_resultqueue', callable=lambda:resultqueue)

    # make the server manager on port 51000
    mgr = QueueManager(address=('', 51000), authkey='abracadabra')
    mgr.inqueue = inqueue
    mgr.outqueue = outqueue
    mgr.inputs = arglist
    mgr.outputs = []

    # dump the contents of the arglist into the queue
    for stuff in arglist:
        #print "putting",stuff,"in arglist"
        inqueue.put(stuff)

    s = mgr.get_server() # the actual server
    serve_till_done(s,mgr)

    process_output(mgr.outputs,fname)

    #print mgr.outputs

def log_and_run(sshcmd,bashcmd):
    # log into one remote computer and run a client
    p = pexpect.spawn(sshcmd) # log into the computer - note I assume we can log in without a password
    print "Spawned."
    p.expect('~ >') # look for characters telling me I've managed to log in
    print "ssh'd... Firing up a worker."
    #p.sendline("cd .")
    p.sendline(bashcmd)
    print "Client begun."
    p.expect("QUIT",timeout=None) # wait 3 minutes to time out
    print "Found something."
    p.sendline("exit")
    #print "exited"

if __name__=="__main__":

    dirname = "$STMONA/swave/li10/"
    for filename in os.listdir(dirname):
        # only get the ones we want...
        if filename.find(".cross") == -1 and filename.find("phase_ang_") !=-1:
            # carve up the energies to be used ... load up the file 1st
            ifile = open(dirname+filename)
            deltaE = 0.03
            args = []
            for index,line in enumerate(ifile):
                #print "index",index
                e = deltaE*float(index+1)
                shift = float(line)
                temp = [e,shift]
                args.append(temp)
                """
                if index >= 100: # throw this in to stop things for a smaller test
                    break
                """
            fname = dirname+"cs/"+filename+".cross"
	    print "here"

            # set up the master server
            master = multiprocessing.Process(target=create_and_run_server,args=(args,fname))
            master.start()

            plist = []
            # log onto all the computers to be used, fire up the workers
            for name in fishtank:
                sshcmd = "ssh "+name
                bashcmd = "python $STMONA/swave/cross_worker.py"
		print "log and run",sshcmd,bashcmd
                plist.append(multiprocessing.Process(target=log_and_run,args=(sshcmd,bashcmd)))
                plist[-1].start()

            # wrap things up
            for process in plist:
                process.join()

            # wait until everything is done processing
            master.join()


"""
if __name__=="__main__":
    # This testing program illustrates how to use queues to run a bunch of jobs
    # in parallel
    ###################
    ## MANAGER BLOCK ##
    ###################
    mgr = multiprocessing.Manager() # not used right now
    qin = multiprocessing.Queue() # make an input queue
    qin.put([0,1,2,3]) # slap a set of instructions in the queue
    qin.put([-2,-3,-4,-5,-6,-7,-8]) # slap a second set of instructions there

    qout = multiprocessing.Queue() # make an output queue

    ################
    ## POOL BLOCK ##
    ################
    pool_size = multiprocessing.cpu_count()*2 # how many processes we want to run in parallel
    pool = multiprocessing.Pool(processes=pool_size) # make a "pool" - way of easily handling simple parallel stuff
    while True: # loop over the stuff in queue
        try: # try to read from the queue
            inputs = qin.get(False) # the false tells it not to block e.g. it won't wait for more
                                    # stuff to arrive in the queue but rather will raise exceptions
            #print ('Pool    :', pool_outputs)
        except Queue.Empty: # qin.get will raise this exception if there's nothing in the queue
            print("end of input queue reached.. exiting")
            break

        pool_outputs = pool.map(getload, inputs) # if we got something, use the map funtion to 
                                                 # run the number of processes given with the
                                                 # args give. it wants an input list of args
                                                 # and a target function to run
        qout.put(pool_outputs) # put the results in the output queue
        print(len(pool_outputs)) # print length of the output to see that it's what we expect

    # parse the resulting queue to demonstrate that things did get passed through
    while True:
        try:
            outputs = qout.get(False)
        except Queue.Empty:
            print("end of output queue reached.. exiting")
            break
        print(len(outputs))


    #inputs = [[5.2,1],[4.5,2],[3.4,3],[2.9,8]]
"""
