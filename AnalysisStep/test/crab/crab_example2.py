from WMCore.Configuration import Configuration
config = Configuration()

config.section_('General')
config.General.transferLogs = True
config.General.requestName  = 'QCD_Pt-20toInf'
#config.General.requestName  = 'WJetsToLNu'
#config.General.requestName  = 'TTJets'
#config.General.requestName  = 'TBarToLeptons_s'
#config.General.requestName  = 'TToLeptons_s'
#config.General.requestName  = 'TBarToLeptons_t'
#config.General.requestName  = 'TToLeptons_t'

config.section_('JobType')
config.JobType.pluginName  = 'Analysis'
config.JobType.psetName    = '../stepB.py'
config.JobType.outputFiles = ['stepB_MC.root']
config.JobType.pyCfgParams = ['label=HWW', 'id=12345', 'scale=1', 'outputFile=stepB_MC.root', 'doNoFilter=True', 'doMuonIsoId=True', 'doGen=True', 'doLHE=False', 'runPUPPISequence=False']

config.section_('Data')
config.Data.inputDataset    = '/QCD_Pt-20toInf_MuEnrichedPt15_PionKaonDecay_Tune4C_13TeV_pythia8/Phys14DR-PU20bx25_PHYS14_25_V1-v3/MINIAODSIM'
#config.Data.inputDataset    = '/WJetsToLNu_13TeV-madgraph-pythia8-tauola/Phys14DR-PU20bx25_PHYS14_25_V1-v1/MINIAODSIM'
#config.Data.inputDataset    = '/TTJets_MSDecaysCKM_central_Tune4C_13TeV-madgraph-tauola/Phys14DR-PU20bx25_PHYS14_25_V1-v1/MINIAODSIM'
#config.Data.inputDataset    = '/TBarToLeptons_s-channel-CSA14_Tune4C_13TeV-aMCatNLO-tauola/Phys14DR-PU20bx25_PHYS14_25_V1-v1/MINIAODSIM'
#config.Data.inputDataset    = '/TToLeptons_s-channel-CSA14_Tune4C_13TeV-aMCatNLO-tauola/Phys14DR-PU20bx25_PHYS14_25_V1-v1/MINIAODSIM'
#config.Data.inputDataset    = '/TBarToLeptons_t-channel_Tune4C_CSA14_13TeV-aMCatNLO-tauola/Phys14DR-PU20bx25_tsg_PHYS14_25_V1-v2/MINIAODSIM'
#config.Data.inputDataset    = '/TToLeptons_t-channel-CSA14_Tune4C_13TeV-aMCatNLO-tauola/Phys14DR-PU20bx25_tsg_PHYS14_25_V1-v2/MINIAODSIM'
config.Data.splitting       = 'FileBased'
config.Data.unitsPerJob     = 1
config.Data.publishDataName = 'NoFilter'
config.Data.ignoreLocality  = True

config.section_('Site')
config.Site.storageSite = 'T2_ES_IFCA'
