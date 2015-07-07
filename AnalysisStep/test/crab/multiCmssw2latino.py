import os
import sys

if __name__ == '__main__':

    print sys.argv
    if len(sys.argv) <= 1 :
       print "no arguments?"
       print "Usage to submit:     python multiCmssw2latino.py file.py"
       exit()
       

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
           
           os.system("cmsLs " + inputFolder + " | grep " + pattern + " | awk -v p="" '{if ($5!=\"\") p=p\" root://eoscms//eos/cms\"$5}; END{print \"hadd /tmp/" + requestName + ".root\" p}' | /bin/sh")
           
           os.system("python ../cmssw2latino.py /tmp/" + requestName + ".root -o /tmp/latino_" + requestName + ".root")      
           # /afs/cern.ch/project/eos/installation/0.3.84-aquamarine/bin/eos.select   is actually "eos"
           os.system("/afs/cern.ch/project/eos/installation/0.3.84-aquamarine/bin/eos.select cp /tmp/latino_" + requestName + ".root " + outputDirectory + "/")           
            
            
    # error
    else :
       print "Error: ", SamplesFile, " does not exist "
       
       