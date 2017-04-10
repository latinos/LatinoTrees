import os
import sys
import subprocess

from LatinoAnalysis.Tools.batchTools import *

def findFiles( inputDir, target ):
    "Returns a list of subdirectories that contain the target files"
    outDirs = []
    inDir = inputDir[:]

    if subprocess.check_output(lsListCommand(inDir) + " | grep " + pattern + " | wc -l", shell=True) == '0\n':
        subDirsString = (subprocess.check_output(lsListCommand(inDir), shell=True)).rstrip('\n')
        subDirs = subDirsString.split('\n')
        print "Sub Directories: ", subDirs
        
        if len(subDirs) == 0:
            return
        
        # Correct for use of wildcard * on lxplus (only works for one * !)
        if '*' in inDir and 'lxplus' in os.uname()[1]:
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

    # default is 1GB per hadd
    sizeHadd = 2e9
    
    # Don't submit jobs by default
    dryRun = True
    if len(sys.argv) > 2:
       if (sys.argv[2]) == '0' :
         dryRun = True
       if (sys.argv[2]) == '1' :
         dryRun = False 
    
    if 'iihe'  in os.uname()[1]:
        os.system('voms-proxy-init --voms cms --valid 168:0')
    if 'knu'  in os.uname()[1]:
        os.system('voms-proxy-init --voms cms --valid 168:0')

    samples = {}
    SamplesFile = sys.argv[1]
    print " SamplesFile =", SamplesFile
    
    if os.path.exists(SamplesFile) :
        handle = open(SamplesFile,'r')
        exec(handle)
        handle.close()
        
    else :
        print "Error: ", SamplesFile, " does not exist "
                
    # samples to be analysed                      
    for key, value in samples.iteritems():
        print key, ' -> ', value
    
        requestName = key
        inputFolder = value[0]
        pattern     = value[1]

        inputFolder += '/'
        print " outputDirectory =", outputDirectory
        print " inputFolder =", inputFolder
        print " pattern =", pattern
        
        # Create list with all files
        inputDirs = findFiles( inputFolder, pattern)
        os.system("rm -rf list_" + requestName + "*")
        for d in inputDirs:
            os.system(lsListCommand(d) + " | grep " + pattern + " | awk '{ print \"" + d + "\"$1 }' >> list_" + requestName + ".txt")
        
        # split in files per job
        totSize = 1e20
        iPart = -1
        with open("list_"+requestName+".txt") as f:
            for line in f:
                fileSize = float(remoteFileSize(line.rstrip('\n')))
                totSize += fileSize
                if totSize > sizeHadd:
                    iPart += 1
                    totSize = fileSize
                    with open("list_{0}__part{1}.txt".format(requestName, iPart), "w") as outputFile:
                        outputFile.write(rootReadPath(line.rstrip('\n'))+'\n')
                else:    
                    with open("list_{0}__part{1}.txt".format(requestName, iPart), "a") as outputFile:
                        outputFile.write(rootReadPath(line.rstrip('\n'))+'\n')
    
        nfiles = int(os.popen("ls list_" + requestName + "__part*.txt | wc -l").read())
        print " nfiles =", nfiles
        
        # Create the job scripts and submit
        if not dryRun:     
            TargeList=[]
            for i in range(0, nfiles) :
                requestName_i = requestName + "__part" + str(i)
                TargeList.append(requestName_i) 
            jobs = batchJobs('probeTreeToLatino',requestName,["All"],TargeList,"Step,Target")
            for i in range(0, nfiles) :
                requestName_i = requestName + "__part" + str(i)
                #jobs = batchJobs('probeTreeToLatino',requestName_i,["All"],["All"],"Step,Target")
                
                command = ''
                latino_directory = os.getcwd()
                hadd_list = os.popen("awk -v p=\"\" '{ if ($1!=\"\") p=p\" \"$1 }; END{ print \"hadd -f " + requestName_i + ".root\" p}' " + latino_directory + "/list_" + requestName_i + ".txt").read()
                command += hadd_list + "\n"
                
                outputFileName = "latino_" + requestName_i + ".root"
                command += "python " + latino_directory + "/../cmssw2latino.py " + requestName_i + ".root -o " + outputFileName + "\n"
                command += "rm " + requestName_i + ".root\n"
                jobs.Add("All",requestName_i,command)
                
                jobs.AddCopy("All",requestName_i,outputFileName, outputDirectory+'/'+outputFileName)
                jobs.Add("All",requestName_i,"rm -f latino_" + requestName_i + ".root\n")

            #jobs.Sub('8nh','4:00:00')
            if 'knu'  in os.uname()[1]:
              jobs.Sub("cms")
	    else:
              jobs.Sub()

        os.system("rm list_" + requestName + "*")

