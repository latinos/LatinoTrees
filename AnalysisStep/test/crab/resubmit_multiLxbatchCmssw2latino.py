#!/usr/bin/env python
"""
Usage: python resubmit_multiLxbatchCmssw2latino.py samples_file.py

Resubmits jobs from lxbatch/ for which the output is missing in the outputDirectory
"""

import os
import sys
import subprocess

if __name__ == '__main__':

    print sys.argv
    if len(sys.argv) <= 1 :
        print "No arguments?"
        print "Usage to submit:     python resubmit_multiLxbatchCmssw2latino.py file.py"
        exit()

    samples = {}
    SamplesFile = sys.argv[1]
    print " SamplesFile =", SamplesFile
    
    if os.path.exists(SamplesFile) :
        handle = open(SamplesFile,'r')
        exec(handle)
        handle.close()
                       
        print " outputDirectory =", outputDirectory
        
        for fn in os.listdir('lxbatch/'):
            if os.path.isfile('lxbatch/'+fn):
                fileToSearch = subprocess.check_output("tail -1 lxbatch/" + fn, shell=True).lstrip('rm ').rstrip('\n')
                if subprocess.check_output("/afs/cern.ch/project/eos/installation/0.3.84-aquamarine/bin/eos.select ls " + outputDirectory + " | grep " + fileToSearch + " | wc -l", shell=True) == '0\n': 
		    print "resubmit ", fn
                    os.system("bsub -q 1nd < lxbatch/" + fn)
        
    else :
        print "Error: ", SamplesFile, " does not exist "
