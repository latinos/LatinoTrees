import os
from WMCore.Configuration import Configuration
config = Configuration()

pyCfgParams = ['outputFile=stepB_MC.root', 'doNoFilter=True',  'doMuonIsoId=True',  'doGen=True',   'doLHE=True',  'runPUPPISequence=True', 'doBTag=True' ]

config.section_('General')
config.General.transferOutputs = True
config.General.requestName = 'MCtest_13May2015'

config.section_('JobType')
config.JobType.psetName = '../stepB.py'
config.JobType.pluginName = 'Analysis'
config.JobType.outputFiles = ['stepB_MC.root']

# to fix cmssw releases
config.JobType.allowUndistributedCMSSW = True

config.section_('Data')    
config.Data.unitsPerJob = 10   # since files based, 10 files per job
config.Data.inputDBS = 'https://cmsweb.cern.ch/dbs/prod/global/DBSReader/'
config.Data.splitting = 'FileBased'    #'LumiBased'
config.Data.outLFNDirBase = '/store/group/phys_higgs/cmshww/amassiro/RunII/test/'

config.section_('Site')
config.Site.storageSite = 'T2_CH_CERN'

import sys


if __name__ == '__main__':

    from CRABAPI.RawCommand import crabCommand

    # Make sure you set this parameter (here or above in the config it does not matter)
    config.General.workArea = 'crab_projects_13May'

    def submit(config):
        print " to do: ",config
        res = crabCommand('submit', config = config)

    #########    From now on that's what users should modify: this is the a-la-CRAB2 configuration part.
   
    print sys.argv
    if len(sys.argv) <= 1 :
       print "no arguments?"
       print "Usage: python multicrab_template.py  test.py"
       exit()
       

    samples = {}
    SamplesFile = sys.argv[1]
    print " SamplesFile = ", SamplesFile
    
    if os.path.exists(SamplesFile):
       handle = open(SamplesFile,'r')
       exec(handle)
       handle.close()
                
    # samples to be analysed
                   
    for key, value in samples.iteritems():
        print key, ' -> ', value
        
        config.General.requestName = key
        config.Data.inputDataset = value[0]
        config.JobType.pyCfgParams = list(pyCfgParams)
        config.JobType.pyCfgParams.extend(value[1])
        submit(config)

    
    
   
   
