import os
import sys

if __name__ == '__main__':

    print sys.argv
    if len(sys.argv) <= 1 :
        print "no arguments?"
        print "Usage to submit:     python multiCmssw2latino_split.py file.py [0/1 = dryRun/exec]"
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

               print "rm -rf list.txt list_*"
               print "/afs/cern.ch/project/eos/installation/0.3.84-aquamarine/bin/eos.select ls " + inputFolder + " | grep " + pattern + " | awk '{ print \"root://eoscms.cern.ch//eos/cms/" + inputFolder + "\"$1 }' > list.txt"
               print "split --lines=20 -d list.txt list_"
               print "for x in `ls list_*` ; do hadd -f `ls $x | awk --field-separator=\"_\" '{ print \"/tmp/" + requestName + "_\"$2\".root\" }'` `cat $x` ; done"
               print "for x in `ls /tmp/" + requestName + "*` ; do python ../cmssw2latino.py $x ; done"
               print "for x in `ls latino_" + requestName + "*` ; do /afs/cern.ch/project/eos/installation/0.3.84-aquamarine/bin/eos.select cp $x " + outputDirectory + "/ ; done"          
               print "rm list.txt list_*"
               print "rm latino_" + requestName + "*.root /tmp/" + requestName + "*.root "     

           # really execute instructions
           else :

               os.system("rm -rf list.txt list_*")
               os.system("/afs/cern.ch/project/eos/installation/0.3.84-aquamarine/bin/eos.select ls " + inputFolder + " | grep " + pattern + " | awk '{ print \"root://eoscms.cern.ch//eos/cms/" + inputFolder + "\"$1 }' > list.txt")
               os.system("split --lines=20 -d list.txt list_")
               os.system("for x in `ls list_*` ; do hadd -f `ls $x | awk --field-separator=\"_\" '{ print \"/tmp/" + requestName + "_\"$2\".root\" }'` `cat $x` ; done")
               os.system("for x in `ls /tmp/" + requestName + "*` ; do python ../cmssw2latino.py $x ; done")
               os.system("for x in `ls latino_" + requestName + "*` ; do /afs/cern.ch/project/eos/installation/0.3.84-aquamarine/bin/eos.select cp $x " + outputDirectory + "/ ; done")
               os.system("rm list.txt list_*")
               os.system("rm latino_" + requestName + "*.root /tmp/" + requestName + "*.root ")

    # error
    else :
       print "Error: ", SamplesFile, " does not exist "

