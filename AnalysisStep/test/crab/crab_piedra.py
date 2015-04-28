from WMCore.Configuration import Configuration
config = Configuration()

config.section_('General')
config.General.transferLogs = True
config.General.requestName  = 'WH_ZH_HToWW_Spring14'
#config.General.requestName = 'ppTOzh_zTO2v_hTOwwTO2l2v'

config.section_('JobType')
config.JobType.pluginName  = 'Analysis'
config.JobType.psetName    = '../stepB.py'
config.JobType.outputFiles = ['stepB_MC.root']
config.JobType.pyCfgParams = ['label=HWW', 'id=12345', 'scale=1', 'outputFile=stepB_MC.root', 'doNoFilter=True', 'doMuonIsoId=True', 'doGen=True', 'doLHE=False', 'runPUPPISequence=False']

config.section_('Data')
config.Data.inputDataset    = '/WH_ZH_HToWW_2Or3WToLNuAndTau_M-125_13TeV_pythia6/Spring14miniaod-PU20bx25_POSTLS170_V5-v1/MINIAODSIM'
#config.Data.inputDataset   = '/CRAB_UserFiles/dburns-ppTOzh_zTO2v_hTOwwTO2l2v_13TeV_AODSIM_v1-7d492cb64f2cdaff326f939f96e45c96/USER'
config.Data.inputDBS        = 'global'
#config.Data.inputDBS       = 'phys03'  # To be used with dburns datasets
config.Data.splitting       = 'FileBased'
config.Data.unitsPerJob     = 1
config.Data.publishDataName = 'NoFilter'
config.Data.ignoreLocality  = True
#config.Data.ignoreLocality = False  # To be used with dburns datasets

config.section_('Site')
config.Site.storageSite = 'T2_ES_IFCA'
