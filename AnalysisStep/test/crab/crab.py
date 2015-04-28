from WMCore.Configuration import Configuration
config = Configuration()

config.section_('General')
config.General.transferOutputs = True
config.General.requestName = 'MCtest_13Feb2015'

config.section_('JobType')
config.JobType.pluginName  = 'Analysis'
config.JobType.psetName    = '../stepB.py'
config.JobType.outputFiles = ['stepB_MC_ggHww.root']
config.JobType.pyCfgParams = ['label=HWW', 'id=12345', 'scale=1', 'outputFile=stepB_MC_ggHww.root', 'doNoFilter=True', 'doMuonIsoId=True', 'doGen=True', 'doLHE=True', 'runPUPPISequence=True']
config.JobType.allowNonProductionCMSSW = True  # To fix cmssw releases

config.section_('Data')
config.Data.inputDataset = '/GluGluToHToWWTo2LAndTau2Nu_M-125_13TeV-powheg-pythia6/Phys14DR-AVE30BX50_tsg_PHYS14_ST_V1-v1/MINIAODSIM'
config.Data.splitting    = 'FileBased'  #'LumiBased'
config.Data.unitsPerJob  = 10  # Since files based, 10 files per job
config.Data.inputDBS     = 'https://cmsweb.cern.ch/dbs/prod/global/DBSReader/'
config.Data.outLFN       = '/store/group/phys_higgs/cmshww/amassiro/RunII/test/'

config.section_('Site')
config.Site.storageSite = 'T2_CH_CERN'
