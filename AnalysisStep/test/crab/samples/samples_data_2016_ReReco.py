#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#
# Latino ReReco production of Run2016 25ns data
#
# DAS query: dataset=/*/Run2016*-23Sep2016-v*/MINIAOD
#
# For GT and more, see https://twiki.cern.ch/twiki/bin/view/CMSPublic/WorkBookMiniAOD
#
# GlobalTag :
# miniaod recommendation: 80X_dataRun2_2016SeptRepro_v3 
#    PdmV recommendation: 80X_dataRun2_2016SeptRepro_v4  for Run2016Bv2
# 
#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~


samples['DoubleEG_Run2016B-23Sep2016-v2']       = ['/DoubleEG/Run2016B-23Sep2016-v2/MINIAOD',       ['label=DoubleEG']]
samples['DoubleMuon_Run2016B-23Sep2016-v2']     = ['/DoubleMuon/Run2016B-23Sep2016-v2/MINIAOD',     ['label=DoubleMuon']]
samples['MuonEG_Run2016B-23Sep2016-v2']         = ['/MuonEG/Run2016B-23Sep2016-v2/MINIAOD',         ['label=MuEG']]
samples['SingleElectron_Run2016B-23Sep2016-v2'] = ['/SingleElectron/Run2016B-23Sep2016-v2/MINIAOD', ['label=SingleElectron']]
samples['SingleMuon_Run2016B-23Sep2016-v2']     = ['/SingleMuon/Run2016B-23Sep2016-v2/MINIAOD',     ['label=SingleMuon']]

samples['DoubleEG_Run2016C-23Sep2016-v1']       = ['/DoubleEG/Run2016C-23Sep2016-v1/MINIAOD',       ['label=DoubleEG']]
samples['DoubleMuon_Run2016C-23Sep2016-v1']     = ['/DoubleMuon/Run2016C-23Sep2016-v1/MINIAOD',     ['label=DoubleMuon']]
samples['MuonEG_Run2016C-23Sep2016-v1']         = ['/MuonEG/Run2016C-23Sep2016-v1/MINIAOD',         ['label=MuEG']]
samples['SingleElectron_Run2016C-23Sep2016-v1'] = ['/SingleElectron/Run2016C-23Sep2016-v1/MINIAOD', ['label=SingleElectron']]
samples['SingleMuon_Run2016C-23Sep2016-v1']     = ['/SingleMuon/Run2016C-23Sep2016-v1/MINIAOD',     ['label=SingleMuon']]

samples['DoubleEG_Run2016D-23Sep2016-v1']       = ['/DoubleEG/Run2016D-23Sep2016-v1/MINIAOD',       ['label=DoubleEG']]
samples['DoubleMuon_Run2016D-23Sep2016-v1']     = ['/DoubleMuon/Run2016D-23Sep2016-v1/MINIAOD',     ['label=DoubleMuon']]
samples['MuonEG_Run2016D-23Sep2016-v1']         = ['/MuonEG/Run2016D-23Sep2016-v1/MINIAOD',         ['label=MuEG']]
samples['SingleElectron_Run2016D-23Sep2016-v1'] = ['/SingleElectron/Run2016D-23Sep2016-v1/MINIAOD', ['label=SingleElectron']]
samples['SingleMuon_Run2016D-23Sep2016-v1']     = ['/SingleMuon/Run2016D-23Sep2016-v1/MINIAOD',     ['label=SingleMuon']]

samples['DoubleEG_Run2016E-23Sep2016-v1']       = ['/DoubleEG/Run2016E-23Sep2016-v1/MINIAOD',       ['label=DoubleEG']]
samples['DoubleMuon_Run2016E-23Sep2016-v1']     = ['/DoubleMuon/Run2016E-23Sep2016-v1/MINIAOD',     ['label=DoubleMuon']]
samples['MuonEG_Run2016E-23Sep2016-v1']         = ['/MuonEG/Run2016E-23Sep2016-v1/MINIAOD',         ['label=MuEG']]
samples['SingleElectron_Run2016E-23Sep2016-v1'] = ['/SingleElectron/Run2016E-23Sep2016-v1/MINIAOD', ['label=SingleElectron']]
samples['SingleMuon_Run2016E-23Sep2016-v1']     = ['/SingleMuon/Run2016E-23Sep2016-v1/MINIAOD',     ['label=SingleMuon']]

samples['DoubleEG_Run2016F-23Sep2016-v1']       = ['/DoubleEG/Run2016F-23Sep2016-v1/MINIAOD',       ['label=DoubleEG']]
samples['DoubleMuon_Run2016F-23Sep2016-v1']     = ['/DoubleMuon/Run2016F-23Sep2016-v1/MINIAOD',     ['label=DoubleMuon']]
samples['MuonEG_Run2016F-23Sep2016-v1']         = ['/MuonEG/Run2016F-23Sep2016-v1/MINIAOD',         ['label=MuEG']]
samples['SingleElectron_Run2016F-23Sep2016-v1'] = ['/SingleElectron/Run2016F-23Sep2016-v1/MINIAOD', ['label=SingleElectron']]
samples['SingleMuon_Run2016F-23Sep2016-v1']     = ['/SingleMuon/Run2016F-23Sep2016-v1/MINIAOD',     ['label=SingleMuon']]

samples['DoubleEG_Run2016G-23Sep2016-v1']       = ['/DoubleEG/Run2016G-23Sep2016-v1/MINIAOD',       ['label=DoubleEG']]
samples['DoubleMuon_Run2016G-23Sep2016-v1']     = ['/DoubleMuon/Run2016G-23Sep2016-v1/MINIAOD',     ['label=DoubleMuon']]
samples['MuonEG_Run2016G-23Sep2016-v1']         = ['/MuonEG/Run2016G-23Sep2016-v1/MINIAOD',         ['label=MuEG']]
samples['SingleElectron_Run2016G-23Sep2016-v1'] = ['/SingleElectron/Run2016G-23Sep2016-v1/MINIAOD', ['label=SingleElectron']]
samples['SingleMuon_Run2016G-23Sep2016-v1']     = ['/SingleMuon/Run2016G-23Sep2016-v1/MINIAOD',     ['label=SingleMuon']]
samples['MET_Run2016G-23Sep2016-v1']            = ['/MET/Run2016G-23Sep2016-v1/MINIAOD',            ['label=MET']]  # For orthogonal triggers


pyCfgParams.append('globalTag=80X_dataRun2_2016SeptRepro_v4')
pyCfgParams.append('is50ns=False')
pyCfgParams.append('isPromptRecoData=True') #RECO TriggerResults

config.Data.lumiMask       = '/afs/cern.ch/cms/CAF/CMSCOMM/COMM_DQM/certification/Collisions16/13TeV/Cert_271036-280385_13TeV_PromptReco_Collisions16_JSON.txt'
config.Data.splitting      = 'LumiBased'
config.Data.unitsPerJob    = 6
config.Data.outLFNDirBase  = '/store/group/phys_higgs/cmshww/amassiro/RunII/2016/23Sep2016/data/25ns/'
config.Data.runRange       = '278820-279931'
config.JobType.maxMemoryMB = 2500


#   ------------------------------
#     dataset | from run | to run
#   ----------+----------+--------
#    Run2016B |   272007 | 275376
#    Run2016C |   275657 | 276283
#    Run2016D |   276315 | 276811
#    Run2016E |   276831 | 277420
#    Run2016F |   277772 | 278808
#    Run2016G |   278820 | 280385
#    Run2016H |   280919 |
#   ------------------------------
#
# https://twiki.cern.ch/twiki/bin/view/CMS/PdmV2016Analysis
