#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#
# Latino (master) production of Run2016 25ns data
#
# DAS query: dataset=/*/Run2016*-PromptReco-v*/MINIAOD
#
# For GT and more, see https://twiki.cern.ch/twiki/bin/view/CMSPublic/WorkBookMiniAOD
#
# GlobalTag choice,
#
# cat /cvmfs/cms.cern.ch/slc6_amd64_gcc530/cms/cmssw/CMSSW_8_0_17/src/Configuration/AlCa/python/autoCond.py | grep run2_data
#    'run2_data'         :   '80X_dataRun2_v17',
#    'run2_data_relval'  :   '80X_dataRun2_relval_v15',
# 
#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~


samples['DoubleEG_Run2016B-PromptReco-v2']       = ['/DoubleEG/Run2016B-PromptReco-v2/MINIAOD',       ['label=DoubleEG']]
samples['DoubleMuon_Run2016B-PromptReco-v2']     = ['/DoubleMuon/Run2016B-PromptReco-v2/MINIAOD',     ['label=DoubleMuon']]
samples['MuonEG_Run2016B-PromptReco-v2']         = ['/MuonEG/Run2016B-PromptReco-v2/MINIAOD',         ['label=MuEG']]
samples['SingleElectron_Run2016B-PromptReco-v2'] = ['/SingleElectron/Run2016B-PromptReco-v2/MINIAOD', ['label=SingleElectron']]
samples['SingleMuon_Run2016B-PromptReco-v2']     = ['/SingleMuon/Run2016B-PromptReco-v2/MINIAOD',     ['label=SingleMuon']]

#samples['DoubleEG_Run2016C-PromptReco-v2']       = ['/DoubleEG/Run2016C-PromptReco-v2/MINIAOD',       ['label=DoubleEG']]
#samples['DoubleMuon_Run2016C-PromptReco-v2']     = ['/DoubleMuon/Run2016C-PromptReco-v2/MINIAOD',     ['label=DoubleMuon']]
#samples['MuonEG_Run2016C-PromptReco-v2']         = ['/MuonEG/Run2016C-PromptReco-v2/MINIAOD',         ['label=MuEG']]
#samples['SingleElectron_Run2016C-PromptReco-v2'] = ['/SingleElectron/Run2016C-PromptReco-v2/MINIAOD', ['label=SingleElectron']]
#samples['SingleMuon_Run2016C-PromptReco-v2']     = ['/SingleMuon/Run2016C-PromptReco-v2/MINIAOD',     ['label=SingleMuon']]

#samples['DoubleEG_Run2016D-PromptReco-v2']       = ['/DoubleEG/Run2016D-PromptReco-v2/MINIAOD',       ['label=DoubleEG']]
#samples['DoubleMuon_Run2016D-PromptReco-v2']     = ['/DoubleMuon/Run2016D-PromptReco-v2/MINIAOD',     ['label=DoubleMuon']]
#samples['MuonEG_Run2016D-PromptReco-v2']         = ['/MuonEG/Run2016D-PromptReco-v2/MINIAOD',         ['label=MuEG']]
#samples['SingleElectron_Run2016D-PromptReco-v2'] = ['/SingleElectron/Run2016D-PromptReco-v2/MINIAOD', ['label=SingleElectron']]
#samples['SingleMuon_Run2016D-PromptReco-v2']     = ['/SingleMuon/Run2016D-PromptReco-v2/MINIAOD',     ['label=SingleMuon']]

#samples['DoubleEG_Run2016E-PromptReco-v2']       = ['/DoubleEG/Run2016E-PromptReco-v2/MINIAOD',       ['label=DoubleEG']]
#samples['DoubleMuon_Run2016E-PromptReco-v2']     = ['/DoubleMuon/Run2016E-PromptReco-v2/MINIAOD',     ['label=DoubleMuon']]
#samples['MuonEG_Run2016E-PromptReco-v2']         = ['/MuonEG/Run2016E-PromptReco-v2/MINIAOD',         ['label=MuEG']]
#samples['SingleElectron_Run2016E-PromptReco-v2'] = ['/SingleElectron/Run2016E-PromptReco-v2/MINIAOD', ['label=SingleElectron']]
#samples['SingleMuon_Run2016E-PromptReco-v2']     = ['/SingleMuon/Run2016E-PromptReco-v2/MINIAOD',     ['label=SingleMuon']]

#samples['DoubleEG_Run2016F-PromptReco-v1']       = ['/DoubleEG/Run2016F-PromptReco-v1/MINIAOD',       ['label=DoubleEG']]
#samples['DoubleMuon_Run2016F-PromptReco-v1']     = ['/DoubleMuon/Run2016F-PromptReco-v1/MINIAOD',     ['label=DoubleMuon']]
#samples['MuonEG_Run2016F-PromptReco-v1']         = ['/MuonEG/Run2016F-PromptReco-v1/MINIAOD',         ['label=MuEG']]
#samples['SingleElectron_Run2016F-PromptReco-v1'] = ['/SingleElectron/Run2016F-PromptReco-v1/MINIAOD', ['label=SingleElectron']]
#samples['SingleMuon_Run2016F-PromptReco-v1']     = ['/SingleMuon/Run2016F-PromptReco-v1/MINIAOD',     ['label=SingleMuon']]


pyCfgParams.append('globalTag=80X_dataRun2_Prompt_v8')
pyCfgParams.append('is50ns=False')
pyCfgParams.append('isPromptRecoData=True')


### Jun21 [ Run2016B ] There is v2
#
# pyCfgParams.append('globalTag=80X_dataRun2_Prompt_v8')
# Cert_271036-274443_13TeV_PromptReco_Collisions16_JSON.txt
# config.Data.runRange = '271036-274443'
#
### Jul05 [ Run2016B ] v2 in progress
#
# pyCfgParams.append('globalTag=80X_dataRun2_Prompt_v8')
# Cert_271036-275125_13TeV_PromptReco_Collisions16_JSON.txt
# config.Data.runRange = '274444-275125'
#
### Jul08 [ Run2016B and Run2016C] Needs v2
#
# pyCfgParams.append('globalTag=80X_dataRun2_Prompt_v8')
# Cert_271036-275783_13TeV_PromptReco_Collisions16_JSON.txt
# config.Data.runRange = '275126-275783'
#
### Jul11_NoL1T
#
# pyCfgParams.append('globalTag=80X_dataRun2_Prompt_v8')
# Cert_271036-276097_13TeV_PromptReco_Collisions16_JSON_NoL1T_v2.txt
# config.Data.runRange = '275784-276097'
#
### Jul15_DCSONLY
#
# pyCfgParams.append('globalTag=80X_dataRun2_Prompt_v8')
# json_DCSONLY.txt
# config.Data.runRange = '276098-276581'
#
### Jul26
#
# pyCfgParams.append('globalTag=80X_dataRun2_Prompt_v8')
# Cert_271036-276811_13TeV_PromptReco_Collisions16_JSON.txt
# config.Data.runRange = '276582-276811'
#
### Aug31
#
# pyCfgParams.append('globalTag=80X_dataRun2_v17')
# Cert_271036-279116_13TeV_PromptReco_Collisions16_JSON_NoL1T.txt
# config.Data.runRange = '276831-278808'
#


config.Data.lumiMask       = '/afs/cern.ch/cms/CAF/CMSCOMM/COMM_DQM/certification/Collisions16/13TeV/Cert_271036-279116_13TeV_PromptReco_Collisions16_JSON_NoL1T.txt'
config.Data.splitting      = 'LumiBased'
config.Data.unitsPerJob    = 6
config.Data.outLFNDirBase  = '/store/group/phys_higgs/cmshww/amassiro/RunII/2016/Jul05_v2/data/25ns/'
config.Data.runRange       = '274444-275125'
config.JobType.maxMemoryMB = 2500


#brilcalc lumi -b "STABLE BEAMS" --normtag /afs/cern.ch/user/l/lumipro/public/normtag_file/normtag_DATACERT.json -u /pb -i /afs/cern.ch/cms/CAF/CMSCOMM/COMM_DQM/certification/Collisions16/13TeV/Cert_271036-277148_13TeV_PromptReco_Collisions16_JSON.txt --hltpath "HLT_Mu17_TrkIsoVVL_v*"

#brilcalc lumi -b "STABLE BEAMS" --normtag /afs/cern.ch/user/l/lumipro/public/normtag_file/normtag_DATACERT.json -u /fb -i /afs/cern.ch/cms/CAF/CMSCOMM/COMM_DQM/certification/Collisions16/13TeV/Cert_271036-277148_13TeV_PromptReco_Collisions16_JSON.txt --hltpath "HLT_Ele45_WPLoose_Gsf_v*"


#brilcalc lumi -b "STABLE BEAMS" --normtag /afs/cern.ch/user/l/lumipro/public/normtag_file/normtag_DATACERT.json -u /fb -i /afs/cern.ch/cms/CAF/CMSCOMM/COMM_DQM/certification/Collisions16/13TeV/Cert_271036-276811_13TeV_PromptReco_Collisions16_JSON.txt --hltpath "HLT_Ele45_WPLoose_Gsf_v*"


#brilcalc lumi -b "STABLE BEAMS" --normtag /afs/cern.ch/user/l/lumipro/public/normtag_file/normtag_DATACERT.json -u /fb -i /afs/cern.ch/cms/CAF/CMSCOMM/COMM_DQM/certification/Collisions16/13TeV/Cert_271036-276811_13TeV_PromptReco_Collisions16_JSON.txt --hltpath "HLT_Ele27_eta2p1_WPLoose_Gsf_v*"


#brilcalc lumi -b "STABLE BEAMS" --normtag /afs/cern.ch/user/l/lumipro/public/normtag_file/normtag_DATACERT.json -u /fb -i /afs/cern.ch/cms/CAF/CMSCOMM/COMM_DQM/certification/Collisions16/13TeV/Cert_271036-276811_13TeV_PromptReco_Collisions16_JSON.txt --hltpath "HLT_Mu8_TrkIsoVVL_Ele17_CaloIdL_TrackIdL_IsoVL_v*"


#   ------------------------------
#     dataset | from run | to run
#   ----------+----------+--------
#    Run2016B |   272007 | 275376
#    Run2016C |   275657 | 276283
#    Run2016D |   276315 | 276811
#    Run2016E |   276831 | 277420
#    Run2016F |   277772 | 278808
#    Run2016G |   278820 |
#   ------------------------------
#
# https://twiki.cern.ch/twiki/bin/view/CMS/PdmV2016Analysis
