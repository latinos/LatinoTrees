import os
import sys

if __name__ == '__main__':

    print sys.argv
    if len(sys.argv) <= 1 :
       print "no arguments?"
       print "Usage to submit:     python multiLxbatchCmssw2latino.py file.py"
       exit()
       

    samples = {}
    SamplesFile = sys.argv[1]
    print " SamplesFile = ", SamplesFile
    
    if os.path.exists(SamplesFile) :
       handle = open(SamplesFile,'r')
       exec(handle)
       handle.close()
                
       # samples to be analysed
       
       # folder for the scripts
       os.system("rm -r lxbatch/")      
       os.system("mkdir lxbatch/")      
                       
       for key, value in samples.iteritems():
           print key, ' -> ', value
        
           requestName = key
           inputFolder = value[0]
           pattern     = value[1]
           print " outputDirectory = ", outputDirectory
          
           # create scripts
           filename = "lxbatch/job_" + requestName + ".sh"
           target = open(filename, 'w')

           whereAmI = os.getcwd()
           target.write("cd " + whereAmI + "\n")
           target.write("eval `scramv1 runtime -sh`" + "\n")
   
           target.write("/afs/cern.ch/project/eos/installation/0.3.84-aquamarine/bin/eos.select ls  " + inputFolder + " | grep " + pattern + " | awk -v p="" '{if ($1!=\"\") p=p\" root://eoscms.cern.ch//eos/cms/" + inputFolder + "/\"$1}; END{print \"hadd /tmp/" + requestName + ".root\" p}' | /bin/sh" + "\n")           
           target.write("python ../cmssw2latino.py /tmp/" + requestName + ".root -o /tmp/latino_" + requestName + ".root" + "\n")      
           # /afs/cern.ch/project/eos/installation/0.3.84-aquamarine/bin/eos.select   is actually "eos"
           target.write("/afs/cern.ch/project/eos/installation/0.3.84-aquamarine/bin/eos.select cp /tmp/latino_" + requestName + ".root " + outputDirectory + "/" + "\n")           
           #target.write("cmsStage /tmp/latino_" + requestName + ".root " + outputDirectory + "/" + "\n")
           target.write("rm /tmp/latino_" + requestName + ".root " + "\n")      
           target.write("rm /tmp/" + requestName + ".root " + "\n")

           target.close()        

           os.system ("chmod +x " + filename)
           os.system ("bsub -q 1nd < " + filename)
           
           
    # error
    else :
       print "Error: ", SamplesFile, " does not exist "
       

       