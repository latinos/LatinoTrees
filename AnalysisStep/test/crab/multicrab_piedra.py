import os
from WMCore.Configuration import Configuration
config = Configuration()

pyCfgParams = ['outputFile=stepB_MC.root', 'doNoFilter=True', 'doMuonIsoId=True', 'doGen=True', 'doLHE=False', 'runPUPPISequence=False']

config.section_('General')
config.General.transferLogs = True

config.section_('JobType')
config.JobType.pluginName  = 'Analysis'
config.JobType.psetName    = '../stepB.py'
config.JobType.outputFiles = ['stepB_MC.root']

config.section_('Data')    
config.Data.inputDBS        = 'global'
config.Data.splitting       = 'FileBased'
config.Data.unitsPerJob     = 1
config.Data.publishDataName = 'NoFilter'
config.Data.ignoreLocality  = True

config.section_('Site')
config.Site.storageSite = 'T2_ES_IFCA'


if __name__ == '__main__':

    from CRABAPI.RawCommand import crabCommand

    # Make sure you set this parameter (here or above in the config, it does not matter)
    config.General.workArea = 'crab_projects_28Apr'

    def submit(config):
        res = crabCommand('submit', config = config)

    # Samples to be analysed
                   
    config.General.requestName = 'QCD_Pt-20toInf'
    config.Data.inputDataset   = '/QCD_Pt-20toInf_MuEnrichedPt15_PionKaonDecay_Tune4C_13TeV_pythia8/Phys14DR-PU20bx25_PHYS14_25_V1-v3/MINIAODSIM'
    config.JobType.pyCfgParams = list(pyCfgParams)
    config.JobType.pyCfgParams.extend(['label=QCD_Pt-20toInf', 'id=12345', 'scale=79.80534'])
    submit(config)

    config.General.requestName = 'ZJets_Madgraph'
    config.Data.inputDataset   = '/DYJetsToLL_M-50_13TeV-madgraph-pythia8/Phys14DR-PU20bx25_PHYS14_25_V1-v1/MINIAODSIM'
    config.JobType.pyCfgParams = list(pyCfgParams)
    config.JobType.pyCfgParams.extend(['label=ZJets_Madgraph', 'id=12345', 'scale=2.12968'])
    submit(config)

    config.General.requestName = 'WJetsToLNu'
    config.Data.inputDataset   = '/WJetsToLNu_13TeV-madgraph-pythia8-tauola/Phys14DR-PU20bx25_PHYS14_25_V1-v1/MINIAODSIM'
    config.JobType.pyCfgParams = list(pyCfgParams)
    config.JobType.pyCfgParams.extend(['label=WJetsToLNu', 'id=12345', 'scale=2.04731'])
    submit(config)
