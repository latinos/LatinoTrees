import os
import sys
import subprocess

def findFiles( inputDir, target ):
    "Returns a list of subdirectories that contain the target files"
    outDirs = []
    inDir = inputDir[:]

    if subprocess.check_output("/afs/cern.ch/project/eos/installation/0.3.84-aquamarine/bin/eos.select ls " + inDir + " | grep " + pattern + " | wc -l", shell=True) == '0\n':
        subDirsString = (subprocess.check_output("/afs/cern.ch/project/eos/installation/0.3.84-aquamarine/bin/eos.select ls " + inDir, shell=True)).rstrip('\n')
        subDirs = subDirsString.split('\n')
        print "Sub Directories: ", subDirs
        
        if len(subDirs) == 0:
            return
        
        # Correct for use of wildcard * (only works for one * !)
        if '*' in inDir:
            inDir = inDir.rstrip('/').rstrip('*')
            if inDir.split('/')[-1] in subDirs[0]:
                inDir = os.path.split(inDir)[0]
            inDir = inDir+'/'
        
        for d in subDirs:
            outDirs += findFiles(inDir+d+'/',target)

    else: 
        outDirs.append(inDir)
    return outDirs

if __name__ == '__main__':

    print sys.argv
    if len(sys.argv) <= 1 :
        print "No arguments?"
        print "Usage to submit:     python multiLxbatchCmssw2latino_split_autoComplete.py file.py [0/1 = dryRun/exec, default dryRun]"
        exit()

    # default is 50 files per hadd
    nfiles_hadd = '50'
    
    dryRun = True
    if len(sys.argv) > 2:
       if (sys.argv[2]) == '0' :
         dryRun = True
       if (sys.argv[2]) == '1' :
         dryRun = False 

    samples = {}
    SamplesFile = sys.argv[1]
    print " SamplesFile =", SamplesFile
    
    if os.path.exists(SamplesFile) :
        handle = open(SamplesFile,'r')
        exec(handle)
        handle.close()
                
        # samples to be analysed
       
        # folder for the scripts
        os.system("rm -rf lxbatch/")
        os.system("mkdir lxbatch/")
                       
        for key, value in samples.iteritems():
            print key, ' -> ', value
        
            requestName = key
            inputFolder = value[0]
            pattern     = value[1]

            inputFolder += '/'
            print " outputDirectory =", outputDirectory
            print " inputFolder =", inputFolder
            print " pattern =", pattern
            
            inputDirs = findFiles( inputFolder, pattern)

            os.system("rm -rf list_" + requestName + "*")
            for d in inputDirs:
                os.system("/afs/cern.ch/project/eos/installation/0.3.84-aquamarine/bin/eos.select ls " + d + " | grep " + pattern + " | awk '{ print \"root://eoscms.cern.ch//eos/cms/" + d + "\"$1 }' >> list_" + requestName + ".txt")
            os.system("awk 'NR%" + nfiles_hadd + "==1 { file = \"list_" + requestName + "\" sprintf(\"__part%d.txt\", i) ; i++ } { print > file }' list_" + requestName + ".txt")
            nfiles = int(os.popen("ls list_" + requestName + "__part*.txt | wc -l").read())
            print " nfiles =", nfiles

            for i in range(0, nfiles) :
                requestName_i = requestName + "__part" + str(i)
                filename = "lxbatch/job_" + requestName_i + ".sh"
                target = open(filename, 'w')
                latino_directory = os.getcwd()
                target.write("cd " + latino_directory + "\n")
                target.write("eval `scramv1 runtime -sh`" + "\n")
                target.write("cd -\n")
                hadd_list = os.popen("awk -v p=\"\" '{ if ($1!=\"\") p=p\" \"$1 }; END{ print \"hadd -f " + requestName_i + ".root\" p}' " + latino_directory + "/list_" + requestName_i + ".txt").read()
                target.write(hadd_list + "\n")
                target.write("python " + latino_directory + "/../cmssw2latino.py " + requestName_i + ".root -o latino_" + requestName_i + ".root\n")
                target.write("rm " + requestName_i + ".root\n")
                target.write("/afs/cern.ch/project/eos/installation/0.3.84-aquamarine/bin/eos.select cp latino_" + requestName_i + ".root " + outputDirectory + "\n")
                target.write("rm latino_" + requestName_i + ".root\n")
                target.close()
                os.system("chmod +x " + filename)
                if not dryRun:
                    os.system("bsub -q 1nd < " + filename)

            os.system("rm list_" + requestName + "*")

    else :
        print "Error: ", SamplesFile, " does not exist "
