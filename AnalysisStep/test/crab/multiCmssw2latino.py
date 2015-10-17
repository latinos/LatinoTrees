import os
import sys

if __name__ == '__main__':

    print sys.argv
    if len(sys.argv) <= 1 :
        print "no arguments?"
        print "Usage to submit:     python multiCmssw2latino.py file.py [0/1 = dryRun/exec]"
        exit()
       
    # default is "execute"   
    dryRun = False
    if len(sys.argv) == 3 :
       if (sys.argv[2]) == '0' :
         dryRun = True
       if (sys.argv[2]) == '1' :
         dryRun = False  
       

    samples = {}
    SamplesFile = sys.argv[1]
    print " SamplesFile = ", SamplesFile
    
    if os.path.exists(SamplesFile) :
       handle = open(SamplesFile,'r')
       exec(handle)
       handle.close()
                
       # samples to be analysed
                   
       for key, value in samples.iteritems():
           print key, ' -> ', value
        
           requestName = key
           inputFolder = value[0]
           pattern     = value[1]
           print " outputDirectory = ", outputDirectory
           
           # just print on screen
           if dryRun :
             print "/afs/cern.ch/project/eos/installation/0.3.84-aquamarine/bin/eos.select ls " + inputFolder + " | grep " + pattern + " | awk -v p="" '{if ($1!=\"\") p=p\" root://eoscms.cern.ch//eos/cms/" + inputFolder + "/\"$1}; END{print \"hadd /tmp/" + requestName + ".root\" p}' | /bin/sh"
             
             print "python ../cmssw2latino.py /tmp/" + requestName + ".root -o /tmp/latino_" + requestName + ".root"
             # /afs/cern.ch/project/eos/installation/0.3.84-aquamarine/bin/eos.select   is actually "eos"
             print "/afs/cern.ch/project/eos/installation/0.3.84-aquamarine/bin/eos.select cp /tmp/latino_" + requestName + ".root " + outputDirectory + "/"          
             print "rm /tmp/latino_" + requestName + ".root "    
             print "rm /tmp/" + requestName + ".root "     

           # really execute instructions
           else :

             print ("   /afs/cern.ch/project/eos/installation/0.3.84-aquamarine/bin/eos.select ls " + inputFolder + " | grep " + pattern + " | awk -v p=\"\" '{if ($1!=\"\") p=p\" root://eoscms.cern.ch//eos/cms/" + inputFolder + "/\"$1}; END{print \"hadd /tmp/" + requestName + ".root\" p}' | /bin/sh")
             os.system("/afs/cern.ch/project/eos/installation/0.3.84-aquamarine/bin/eos.select ls " + inputFolder + " | grep " + pattern + " | awk -v p=\"\" '{if ($1!=\"\") p=p\" root://eoscms.cern.ch//eos/cms/" + inputFolder + "/\"$1}; END{print \"hadd /tmp/" + requestName + ".root\" p}' | /bin/sh")
             
             os.system("python ../cmssw2latino.py /tmp/" + requestName + ".root -o /tmp/latino_" + requestName + ".root")      
             # /afs/cern.ch/project/eos/installation/0.3.84-aquamarine/bin/eos.select   is actually "eos"
             os.system("/afs/cern.ch/project/eos/installation/0.3.84-aquamarine/bin/eos.select cp /tmp/latino_" + requestName + ".root " + outputDirectory + "/")           
             os.system("rm /tmp/latino_" + requestName + ".root ")      
             os.system("rm /tmp/" + requestName + ".root ")      

    # error
    else :
       print "Error: ", SamplesFile, " does not exist "

