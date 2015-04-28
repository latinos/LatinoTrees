import os
from WMCore.Configuration import Configuration
config = Configuration()

pyCfgParams = ['outputFile=stepB_MC.root', 'doNoFilter=True',  'doMuonIsoId=True',  'doGen=True',   'doLHE=True',  'runPUPPISequence=True', 'doBTag=True' ]

config.section_('General')
config.General.transferOutputs = True
config.General.requestName = 'MCtest_13Feb2015'

config.section_('JobType')
config.JobType.psetName = '../stepB.py'
config.JobType.pluginName = 'Analysis'
config.JobType.outputFiles = ['stepB_MC.root']

# to fix cmssw releases
config.JobType.allowNonProductionCMSSW = True

config.section_('Data')    
config.Data.unitsPerJob = 10   # since files based, 10 files per job
config.Data.inputDBS = 'https://cmsweb.cern.ch/dbs/prod/global/DBSReader/'
config.Data.splitting = 'FileBased'    #'LumiBased'
config.Data.outLFN = '/store/group/phys_higgs/cmshww/amassiro/RunII/test/'

config.section_('Site')
config.Site.storageSite = 'T2_CH_CERN'



if __name__ == '__main__':

    from CRABAPI.RawCommand import crabCommand

    # Make sure you set this parameter (here or above in the config it does not matter)
    config.General.workArea = 'crab_projects_12Mar'

    def submit(config):
        res = crabCommand('submit', config = config)

    #########    From now on that's what users should modify: this is the a-la-CRAB2 configuration part.
   

    # samples to be analysed
                   
    config.General.requestName = 'DYll'
    config.Data.inputDataset = '/DYJetsToLL_M-50_13TeV-madgraph-pythia8-tauola_v2/Phys14DR-AVE30BX50_tsg_PHYS14_ST_V1-v1/MINIAODSIM'
    config.JobType.pyCfgParams = list(pyCfgParams)
    config.JobType.pyCfgParams.extend(['label=DYll', 'id=12345', 'scale=1.23'])
    submit(config)

    config.General.requestName = 'DYll50ns'
    config.Data.inputDataset = '/DYJetsToLL_M-50_13TeV-madgraph-pythia8/Phys14DR-PU4bx50_PHYS14_25_V1-v1/MINIAODSIM'
    config.JobType.pyCfgParams = list(pyCfgParams)
    config.JobType.pyCfgParams.extend(['label=DYll50ns', 'id=12345', 'scale=2.13'])
    submit(config)

    config.General.requestName = 'DYll25ns'
    config.Data.inputDataset = '/DYJetsToLL_M-50_13TeV-madgraph-pythia8/Phys14DR-PU20bx25_PHYS14_25_V1-v1/MINIAODSIM'
    config.JobType.pyCfgParams = list(pyCfgParams)
    config.JobType.pyCfgParams.extend(['label=DYll25ns', 'id=12345', 'scale=2.13'])
    submit(config)


    config.General.requestName = 'ggHwwlvlv'
    config.Data.inputDataset = '/GluGluToHToWWTo2LAndTau2Nu_M-125_13TeV-powheg-pythia6/Phys14DR-AVE30BX50_tsg_PHYS14_ST_V1-v1/MINIAODSIM'
    config.JobType.pyCfgParams = list(pyCfgParams)
    config.JobType.pyCfgParams.extend(['label=ggHwwlvlv', 'id=12', 'scale=0.12'])
    #print " pyCfgParams = ", pyCfgParams
    #print " config.JobType.pyCfgParams = ", config.JobType.pyCfgParams
    submit(config)
