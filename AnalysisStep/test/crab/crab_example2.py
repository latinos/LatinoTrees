from WMCore.Configuration import Configuration
config = Configuration()

config.section_('General')
config.General.transferLogs    = True
config.General.transferOutputs = True
config.General.requestName     = 'GluGluToHToWW'

config.section_('JobType')
config.JobType.pluginName  = 'Analysis'
config.JobType.psetName    = '../stepB.py'
config.JobType.outputFiles = ['stepB_MC_ggHww.root']
config.JobType.pyCfgParams = ['label=HWW', 'id=12345', 'scale=1', 'outputFile=stepB_MC_ggHww.root', 'doNoFilter=True', 'doMuonIsoId=True', 'doGen=True', 'doLHE=True', 'runPUPPISequence=True']

config.section_('Data')
config.Data.inputDataset    = '/GluGluToHToWWTo2LAndTau2Nu_M-125_13TeV-powheg-pythia6/Phys14DR-AVE30BX50_tsg_PHYS14_ST_V1-v1/MINIAODSIM'
config.Data.splitting       = 'FileBased'
config.Data.unitsPerJob     = 10
config.Data.publishDataName = 'NoFilter'
config.Data.ignoreLocality  = True

config.section_('Site')
config.Site.storageSite = 'T2_ES_IFCA'
