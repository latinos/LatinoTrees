#!/usr/bin/env python
"""
Usage: python translate_samples_config.py samples_file.py list_file.py

Creates an inputfile for the latino tree creation script from the multicrab input file
"""

import os
import sys
import subprocess
import json
import collections

from WMCore.Configuration import Configuration
config = Configuration()
config.section_('General')
config.section_('Data')  
config.section_('JobType')

if __name__ == '__main__':
    print sys.argv
    if len(sys.argv) <= 2 :
        print "Not  enough arguments"
        print "Usage to submit:     python translate_samples_config.py samples_file.py list_file.py"
        exit()
        
    inputFile = sys.argv[1]
    outputFile = sys.argv[2]
    samples = collections.OrderedDict()
    pyCfgParams = []
    
    if os.path.exists(inputFile) and not os.path.isdir(inputFile) :
        handle = open(inputFile,'r')
        exec(handle)
        handle.close()
   
    f = open(outputFile, 'w')
    f.write('# Automatically created input for latino tree creation script starting from ' + inputFile + "\n\n")
    
    outDir = config.Data.outLFNDirBase.rstrip('/') + "/LatinoTrees/"
    f.write('outputDirectory = "'+ outDir + '"\n\n')
    
    for key, value in samples.iteritems():
        value[0] = config.Data.outLFNDirBase.rstrip('/') + '/' + value[0].split('/')[1] + '/crab_' + key +'/'
        value[1] = 'stepB'
        f.write('samples[\''+key+'\']  = ' + json.dumps(value) + '\n' )
        
    f.close()
        


