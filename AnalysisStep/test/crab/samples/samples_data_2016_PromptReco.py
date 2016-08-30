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
samples['MuonEG_Run2016B-PromptReco-v2']         = ['/MuonEG/Run2016B-PromptReco-v2/MINIAOD',         ['label=MuEG']]
samples['SingleElectron_Run2016B-PromptReco-v2'] = ['/SingleElectron/Run2016B-PromptReco-v2/MINIAOD', ['label=SingleElectron']]
samples['SingleMuon_Run2016B-PromptReco-v2']     = ['/SingleMuon/Run2016B-PromptReco-v2/MINIAOD',     ['label=SingleMuon']]
samples['MET_Run2016B-PromptReco-v2']            = ['/MET/Run2016B-PromptReco-v2/MINIAOD',            ['label=MET']]           # For MET filter studies and for trigger studies
#samples['SinglePhoton_Run2016B-PromptReco-v2']  = ['/SinglePhoton/Run2016B-PromptReco-v2/MINIAOD',   ['label=SinglePhoton']]  # For MET filter studies

pyCfgParams.append('globalTag=80X_dataRun2_Prompt_v8')
pyCfgParams.append('is50ns=False')
pyCfgParams.append('isPromptRecoData=True')


### ~~~ First pass
# config.Data.lumiMask = '/afs/cern.ch/cms/CAF/CMSCOMM/COMM_DQM/certification/Collisions16/13TeV/DCSOnly/json_DCSONLY.txt'
#
### ~~~ Second pass, 0.218/fb
# https://hypernews.cern.ch/HyperNews/CMS/get/physics-validation/2648.html
# config.Data.lumiMask = '/afs/cern.ch/cms/CAF/CMSCOMM/COMM_DQM/certification/Collisions16/13TeV/Cert_271036-273450_13TeV_PromptReco_Collisions16_JSON_NoL1T.txt'
#
### ~~~ Third pass, 0.804/fb
# https://hypernews.cern.ch/HyperNews/CMS/get/physics-validation/2657.html
# config.Data.lumiMask = '/afs/cern.ch/cms/CAF/CMSCOMM/COMM_DQM/certification/Collisions16/13TeV/Cert_271036-274240_13TeV_PromptReco_Collisions16_JSON.txt'
#
### ~~~ Fourth pass, 2.583/fb
# brilcalc lumi -b "STABLE BEAMS" -u /fb -i /afs/cern.ch/cms/CAF/CMSCOMM/COMM_DQM/certification/Collisions16/13TeV/Cert_271036-274443_13TeV_PromptReco_Collisions16_JSON.txt
# +-------+------+-------+-------+-------------------+------------------+
# | nfill | nrun | nls   | ncms  | totdelivered(/fb) | totrecorded(/fb) |
# +-------+------+-------+-------+-------------------+------------------+
# | 23    | 65   | 29211 | 29206 | 2.693             | 2.583            |
# +-------+------+-------+-------+-------------------+------------------+
# #Check JSON:
# #(run,ls) in json but not in results: [(273158, 10), (274094, 105), (274094, 106), (274094, 107)]
#
### ~~~ Fifth pass, 4.011/fb
# brilcalc lumi -b "STABLE BEAMS" \
#               --normtag /afs/cern.ch/user/l/lumipro/public/normtag_file/normtag_DATACERT.json \
#               -u /fb \
#               -i /afs/cern.ch/cms/CAF/CMSCOMM/COMM_DQM/certification/Collisions16/13TeV/Cert_271036-275125_13TeV_PromptReco_Collisions16_JSON.txt
# +-------+------+-------+-------+-------------------+------------------+
# | nfill | nrun | nls   | ncms  | totdelivered(/fb) | totrecorded(/fb) |
# +-------+------+-------+-------+-------------------+------------------+
# | 28    | 83   | 40546 | 40541 | 4.182             | 4.011            |
# +-------+------+-------+-------+-------------------+------------------+


config.Data.lumiMask      = '/afs/cern.ch/cms/CAF/CMSCOMM/COMM_DQM/certification/Collisions16/13TeV/Cert_271036-275125_13TeV_PromptReco_Collisions16_JSON.txt'
config.Data.splitting     = 'LumiBased'
config.Data.unitsPerJob   = 4
#config.Data.outLFNDirBase = '/store/group/phys_higgs/cmshww/amassiro/RunII/2016/May20/data/25ns/'
#config.Data.outLFNDirBase = '/store/group/phys_higgs/cmshww/amassiro/RunII/2016/Jun03/data/25ns/'
#config.Data.outLFNDirBase = '/store/group/phys_higgs/cmshww/amassiro/RunII/2016/Jun07/data/25ns/'
#config.Data.outLFNDirBase = '/store/group/phys_higgs/cmshww/amassiro/RunII/2016/Jun21/data/25ns/'
config.Data.outLFNDirBase  = '/store/group/phys_higgs/cmshww/amassiro/RunII/2016/Jul04/data/25ns/'


#brilcalc lumi -b "STABLE BEAMS" --normtag /afs/cern.ch/user/l/lumipro/public/normtag_file/normtag_DATACERT.json -u /pb -i /afs/cern.ch/cms/CAF/CMSCOMM/COMM_DQM/certification/Collisions16/13TeV/Cert_271036-277148_13TeV_PromptReco_Collisions16_JSON.txt --hltpath "HLT_Mu17_TrkIsoVVL_v*"

#brilcalc lumi -b "STABLE BEAMS" --normtag /afs/cern.ch/user/l/lumipro/public/normtag_file/normtag_DATACERT.json -u /fb -i /afs/cern.ch/cms/CAF/CMSCOMM/COMM_DQM/certification/Collisions16/13TeV/Cert_271036-277148_13TeV_PromptReco_Collisions16_JSON.txt --hltpath "HLT_Ele45_WPLoose_Gsf_v*"


#brilcalc lumi -b "STABLE BEAMS" --normtag /afs/cern.ch/user/l/lumipro/public/normtag_file/normtag_DATACERT.json -u /fb -i /afs/cern.ch/cms/CAF/CMSCOMM/COMM_DQM/certification/Collisions16/13TeV/Cert_271036-276811_13TeV_PromptReco_Collisions16_JSON.txt --hltpath "HLT_Ele45_WPLoose_Gsf_v*"


#brilcalc lumi -b "STABLE BEAMS" --normtag /afs/cern.ch/user/l/lumipro/public/normtag_file/normtag_DATACERT.json -u /fb -i /afs/cern.ch/cms/CAF/CMSCOMM/COMM_DQM/certification/Collisions16/13TeV/Cert_271036-276811_13TeV_PromptReco_Collisions16_JSON.txt --hltpath "HLT_Ele27_eta2p1_WPLoose_Gsf_v*"


#brilcalc lumi -b "STABLE BEAMS" --normtag /afs/cern.ch/user/l/lumipro/public/normtag_file/normtag_DATACERT.json -u /fb -i /afs/cern.ch/cms/CAF/CMSCOMM/COMM_DQM/certification/Collisions16/13TeV/Cert_271036-276811_13TeV_PromptReco_Collisions16_JSON.txt --hltpath "HLT_Mu8_TrkIsoVVL_Ele17_CaloIdL_TrackIdL_IsoVL_v*"









