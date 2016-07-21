#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#
# Latino NinjaTurtles (master) production of Run2016 25ns data
#
# Reading Run2016*-PromptReco-v* processing, produced with CMSSW 8_0_5
#
# DAS query: dataset=/*/Run2016*-PromptReco-v*/MINIAOD
#
# For GT and more, see https://twiki.cern.ch/twiki/bin/view/CMSPublic/WorkBookMiniAOD
#
#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~


samples['DoubleEG_Run2016B-PromptReco-v2']       = ['/DoubleEG/Run2016B-PromptReco-v2/MINIAOD',       ['label=DoubleEG']]
samples['DoubleMuon_Run2016B-PromptReco-v2']     = ['/DoubleMuon/Run2016B-PromptReco-v2/MINIAOD',     ['label=DoubleMuon']]
samples['MuonEG_Run2016B-PromptReco-v2']         = ['/MuonEG/Run2016B-PromptReco-v2/MINIAOD',         ['label=MuEG']]
samples['SingleElectron_Run2016B-PromptReco-v2'] = ['/SingleElectron/Run2016B-PromptReco-v2/MINIAOD', ['label=SingleElectron']]
samples['SingleMuon_Run2016B-PromptReco-v2']     = ['/SingleMuon/Run2016B-PromptReco-v2/MINIAOD',     ['label=SingleMuon']]

samples['DoubleEG_Run2016C-PromptReco-v2']       = ['/DoubleEG/Run2016C-PromptReco-v2/MINIAOD',       ['label=DoubleEG']]
samples['DoubleMuon_Run2016C-PromptReco-v2']     = ['/DoubleMuon/Run2016C-PromptReco-v2/MINIAOD',     ['label=DoubleMuon']]
samples['MuonEG_Run2016C-PromptReco-v2']         = ['/MuonEG/Run2016C-PromptReco-v2/MINIAOD',         ['label=MuEG']]
samples['SingleElectron_Run2016C-PromptReco-v2'] = ['/SingleElectron/Run2016C-PromptReco-v2/MINIAOD', ['label=SingleElectron']]
samples['SingleMuon_Run2016C-PromptReco-v2']     = ['/SingleMuon/Run2016C-PromptReco-v2/MINIAOD',     ['label=SingleMuon']]

samples['DoubleEG_Run2016D-PromptReco-v2']       = ['/DoubleEG/Run2016D-PromptReco-v2/MINIAOD',       ['label=DoubleEG']]
samples['DoubleMuon_Run2016D-PromptReco-v2']     = ['/DoubleMuon/Run2016D-PromptReco-v2/MINIAOD',     ['label=DoubleMuon']]
samples['MuonEG_Run2016D-PromptReco-v2']         = ['/MuonEG/Run2016D-PromptReco-v2/MINIAOD',         ['label=MuEG']]
samples['SingleElectron_Run2016D-PromptReco-v2'] = ['/SingleElectron/Run2016D-PromptReco-v2/MINIAOD', ['label=SingleElectron']]
samples['SingleMuon_Run2016D-PromptReco-v2']     = ['/SingleMuon/Run2016D-PromptReco-v2/MINIAOD',     ['label=SingleMuon']]


pyCfgParams.append('globalTag=80X_dataRun2_Prompt_v8')
pyCfgParams.append('is50ns=False')
pyCfgParams.append('isPromptRecoData=True')


### Jun21
#
# Cert_271036-274443_13TeV_PromptReco_Collisions16_JSON.txt
# config.Data.runRange = '271036-274443'
#
### Jul05
#
# Cert_271036-275125_13TeV_PromptReco_Collisions16_JSON.txt
# config.Data.runRange = '274444-275125'
#
### Jul08
#
# Cert_271036-275783_13TeV_PromptReco_Collisions16_JSON.txt
# config.Data.runRange = '275126-275783'
#
### Jul11_NoL1T
#
# Cert_271036-276097_13TeV_PromptReco_Collisions16_JSON_NoL1T_v2.txt
# config.Data.runRange = '275784-276097'
#
### Jul15_DCSONLY
#
# json_DCSONLY.txt
# config.Data.runRange = '276098-276581'


config.Data.lumiMask       = '/afs/cern.ch/cms/CAF/CMSCOMM/COMM_DQM/certification/Collisions16/13TeV/DCSOnly/json_DCSONLY.txt'
config.Data.splitting      = 'LumiBased'
config.Data.unitsPerJob    = 6
config.Data.outLFNDirBase  = '/store/group/phys_higgs/cmshww/amassiro/RunII/2016/Jul15_DCSONLY/data/25ns/'
config.Data.runRange       = '276098-276581'
config.JobType.maxMemoryMB = 2500


#   ------------------------------
#     dataset | from run | to run
#   ----------+----------+--------
#    Run2016B |   272007 | 275376
#    Run2016C |   275657 | 276283
#    Run2016D |   276315 | 276811
#    Run2016E |   276831 |
#   ------------------------------
#
# https://twiki.cern.ch/twiki/bin/view/CMS/PdmV2016Analysis
