#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#
# Latino MoneyMonster (20May2016) production of Run2015D 25ns data
#
# Reading Run2016B-PromptReco-v* processing, produced with CMSSW 8_0_5
#
# DAS query: dataset=/*/Run2016B-PromptReco-v*/MINIAOD
# https://cmsweb.cern.ch/das/request?view=list&limit=50&instance=prod%2Fglobal&input=dataset%3D%2F*%2FRun2016B-PromptReco-v*%2FMINIAOD
#
# for GT and more, see https://twiki.cern.ch/twiki/bin/view/CMSPublic/WorkBookMiniAOD
#
#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

samples['DoubleEG_Run2016B-PromptReco-v2']       = ['/DoubleEG/Run2016B-PromptReco-v2/MINIAOD',       ['label=DoubleEG']]
samples['DoubleMuon_Run2016B-PromptReco-v2']     = ['/DoubleMuon/Run2016B-PromptReco-v2/MINIAOD',     ['label=DoubleMuon']]
samples['MuonEG_Run2016B-PromptReco-v2']         = ['/MuOnia/Run2016B-PromptReco-v2/MINIAOD',         ['label=MuEG']]
samples['SingleElectron_Run2016B-PromptReco-v2'] = ['/SingleElectron/Run2016B-PromptReco-v2/MINIAOD', ['label=SingleElectron']]
samples['SingleMuon_Run2016B-PromptReco-v2']     = ['/SingleMuon/Run2016B-PromptReco-v2/MINIAOD',     ['label=SingleMuon']]
#samples['MET_Run2016B-PromptReco-v2']           = ['/MET/Run2016B-PromptReco-v2/MINIAOD',            ['label=MET']]           # For MET filter studies and for trigger studies
#samples['SinglePhoton_Run2016B-PromptReco-v2']  = ['/SinglePhoton/Run2016B-PromptReco-v2/MINIAOD',   ['label=SinglePhoton']]  # For MET filter studies

pyCfgParams.append('globalTag=80X_dataRun2_Prompt_v8')
pyCfgParams.append('is50ns=False')
pyCfgParams.append('isPromptRecoData=True')

# ~~~ First pass
#config.Data.lumiMask     = '/afs/cern.ch/cms/CAF/CMSCOMM/COMM_DQM/certification/Collisions16/13TeV/DCSOnly/json_DCSONLY.txt'
# ~~~~ Second pass, 218.042 /pb
# https://hypernews.cern.ch/HyperNews/CMS/get/physics-validation/2648.html
config.Data.lumiMask      = '/afs/cern.ch/cms/CAF/CMSCOMM/COMM_DQM/certification/Collisions16/13TeV/Cert_271036-273450_13TeV_PromptReco_Collisions16_JSON_NoL1T.txt'
config.Data.splitting     = 'LumiBased'
config.Data.unitsPerJob   = 5
config.Data.outLFNDirBase = '/store/group/phys_higgs/cmshww/amassiro/RunII/2016/May20/data/25ns/'
