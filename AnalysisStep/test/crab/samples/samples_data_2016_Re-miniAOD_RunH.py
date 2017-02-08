#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#
# Latino 03Feb2017 production of Run2016H 25ns data
#
# DAS query: dataset=/*/*Run2016H-03Feb2017-v*/MINIAOD (starting from v2, v1 has no stable collisions, v3 = last 10 runs of 2016 )
#
# For GT and more, see https://twiki.cern.ch/twiki/bin/view/CMSPublic/WorkBookMiniAOD
#
# GlobalTag :
# latest JEC: 80X_dataRun2_2016SeptRepro_v7  (eras B-G)
#             80X_dataRun2_Prompt_v16        (era H) 
# Production release: 8_0_26_patch1
#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

# 
samples['DoubleEG_Run2016H-03Feb2017_ver2-v1']       = ['/DoubleEG/Run2016H-03Feb2017_ver2-v1/MINIAOD',       ['label=DoubleEG']]
samples['DoubleMuon_Run2016H-03Feb2017_ver2-v1']     = ['/DoubleMuon/Run2016H-03Feb2017_ver2-v1/MINIAOD',     ['label=DoubleMuon']]
samples['MuonEG_Run2016H-03Feb2017_ver2-v1']         = ['/MuonEG/Run2016H-03Feb2017_ver2-v1/MINIAOD',         ['label=MuEG']]
samples['SingleElectron_Run2016H-03Feb2017_ver2-v1'] = ['/SingleElectron/Run2016H-03Feb2017_ver2-v1/MINIAOD', ['label=SingleElectron']]
samples['SingleMuon_Run2016H-03Feb2017_ver2-v1']     = ['/SingleMuon/Run2016H-03Feb2017_ver2-v1/MINIAOD',     ['label=SingleMuon']]
samples['MET_Run2016H-03Feb2017_ver2-v1']     = ['/MET/Run2016H-03Feb2017_ver2-v1/MINIAOD',     ['label=MET']]

samples['DoubleEG_Run2016H-03Feb2017_ver3-v1']       = ['/DoubleEG/Run2016H-03Feb2017_ver3-v1/MINIAOD',       ['label=DoubleEG']]
samples['DoubleMuon_Run2016H-03Feb2017_ver3-v1']     = ['/DoubleMuon/Run2016H-03Feb2017_ver3-v1/MINIAOD',     ['label=DoubleMuon']]
samples['MuonEG_Run2016H-03Feb2017_ver3-v1']         = ['/MuonEG/Run2016H-03Feb2017_ver3-v1/MINIAOD',         ['label=MuEG']]
samples['SingleElectron_Run2016H-03Feb2017_ver3-v1'] = ['/SingleElectron/Run2016H-03Feb2017_ver3-v1/MINIAOD', ['label=SingleElectron']]
samples['SingleMuon_Run2016H-03Feb2017_ver3-v1']     = ['/SingleMuon/Run2016H-03Feb2017_ver3-v1/MINIAOD',     ['label=SingleMuon']]
samples['MET_Run2016H-03Feb2017_ver3-v1']     = ['/MET/Run2016H-03Feb2017_ver3-v1/MINIAOD',     ['label=MET']]



pyCfgParams.append('globalTag=80X_dataRun2_Prompt_v16')
pyCfgParams.append('is50ns=False')
pyCfgParams.append('isPromptRecoData=True') #RECO TriggerResults

#ICHEP data
#config.Data.lumiMask       = '/afs/cern.ch/cms/CAF/CMSCOMM/COMM_DQM/certification/Collisions16/13TeV/Cert_271036-276811_13TeV_03Feb2017_Collisions16_JSON.txt'
# 27.66/fb
#config.Data.lumiMask       = '/afs/cern.ch/cms/CAF/CMSCOMM/COMM_DQM/certification/Collisions16/13TeV/Cert_271036-280385_13TeV_03Feb2017_Collisions16_JSON.txt'
# 36.42/fb
config.Data.lumiMask       = '/afs/cern.ch/cms/CAF/CMSCOMM/COMM_DQM/certification/Collisions16/13TeV/Final/Cert_271036-284044_13TeV_03Feb2017_Collisions16_JSON.txt'
config.Data.splitting      = 'LumiBased'
config.Data.unitsPerJob    = 15
config.Data.outLFNDirBase  = '/store/group/phys_higgs/cmshww/amassiro/RunII/2016/Feb2017/data/25ns/'
#config.Data.runRange       = '278820-279931'
#config.JobType.maxMemoryMB = 2500


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
