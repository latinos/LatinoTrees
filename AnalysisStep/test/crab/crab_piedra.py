from WMCore.Configuration import Configuration
config = Configuration()

config.section_('General')
config.General.transferLogs = True
config.General.requestName  = 'VH_HWW'

config.section_('JobType')
config.JobType.pluginName  = 'Analysis'
config.JobType.psetName    = '../stepB.py'
config.JobType.outputFiles = ['stepB_MC.root']
config.JobType.pyCfgParams = ['label=HWW', 'id=12345', 'scale=1', 'outputFile=stepB_MC.root', 'doNoFilter=True', 'doMuonIsoId=True', 'doGen=True', 'doBTag=True', 'doLHE=False', 'runPUPPISequence=False']

config.section_('Data')
config.Data.inputDataset    = '/WH_ZH_HToWW_2Or3WToLNuAndTau_M-125_13TeV_pythia6/Spring14miniaod-PU20bx25_POSTLS170_V5-v1/MINIAODSIM'
config.Data.inputDBS        = 'global'
config.Data.splitting       = 'FileBased'
config.Data.unitsPerJob     = 1
config.Data.publishDataName = 'NoFilter'
config.Data.ignoreLocality  = False

config.section_('Site')
config.Site.storageSite = 'T2_ES_IFCA'
