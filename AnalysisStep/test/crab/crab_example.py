from WMCore.Configuration import Configuration

config = Configuration()
config.section_('General')
config.General.transferOutputs = True
config.General.requestName = 'MCtest_11Feb2015'

config.section_('JobType')
config.JobType.psetName = '../stepB.py'
config.JobType.pluginName = 'Analysis'
config.JobType.outputFiles = ['stepB_MC_ggHww.root']
#config.JobType.pyCfgParams = ['label=HWW', 'id=12345', 'outputFile=stepB_MC_ggHww.root', 'doNoFilter=True']
config.JobType.pyCfgParams = ['label=HWW', 'id=12345', 'scale=1', 'outputFile=stepB_MC_ggHww.root', 'doNoFilter=True',  'doMuonIsoId=True',  'doGen=True',   'doLHE=True',  'runPUPPISequence=True' ]

# to fix cmssw releases
config.JobType.allowNonProductionCMSSW = True


config.section_('Data')
config.Data.inputDataset = '/GluGluToHToWWTo2LAndTau2Nu_M-125_13TeV-powheg-pythia6/Phys14DR-AVE30BX50_tsg_PHYS14_ST_V1-v1/MINIAODSIM'
config.Data.unitsPerJob = 10   # since files based, 10 files per job
config.Data.inputDBS = 'https://cmsweb.cern.ch/dbs/prod/global/DBSReader/'
config.Data.splitting = 'FileBased'    #'LumiBased'
config.Data.outLFN = '/store/group/phys_higgs/cmshww/amassiro/RunII/test/'


config.section_('Site')
config.Site.storageSite = 'T2_CH_CERN'
