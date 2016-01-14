#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#
# Latino StarWars (08Jan) production of Run2015 25ns data
#
# Reading 16Dec2015-v1 processing, produced with CMSSW 7_6_3
#
# DAS query: dataset=/*/*16Dec2015*/MINIAOD status=VALID
# https://cmsweb.cern.ch/das/request?view=list&limit=50&instance=prod%2Fglobal&input=dataset+dataset%3D%2F*%2F*16Dec2015*%2FMINIAOD+status%3DVALID
#
#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

samples['DoubleEG_Run2015C_25ns-16Dec2015-v1'] = ['/DoubleEG/Run2015C_25ns-16Dec2015-v1/MINIAOD', ['label=DoubleEG']]
#samples['DoubleEG_Run2015D_25ns-16Dec2015-v1'] = ['/DoubleEG/Run2015D-16Dec2015-v1/MINIAOD',      ['label=DoubleEG']]  # PRODUCTION

#samples['DoubleMuon_Run2015C_25ns-16Dec2015-v1'] = ['/DoubleMuon/Run2015C_25ns-16Dec2015-v1/MINIAOD', ['label=DoubleMuon']]  # PRODUCTION
samples['DoubleMuon_Run2015D_25ns-16Dec2015-v1'] = ['/DoubleMuon/Run2015D-16Dec2015-v1/MINIAOD',      ['label=DoubleMuon']]

samples['MuonEG_Run2015C_25ns-16Dec2015-v1'] = ['/MuonEG/Run2015C_25ns-16Dec2015-v1/MINIAOD', ['label=MuonEG']]
#samples['MuonEG_Run2015D_25ns-16Dec2015-v1'] = ['/MuonEG/Run2015D-16Dec2015-v1/MINIAOD',      ['label=MuonEG']]  # PRODUCTION

samples['SingleElectron_Run2015C_25ns-16Dec2015-v1'] = ['/SingleElectron/Run2015C_25ns-16Dec2015-v1/MINIAOD', ['label=SingleElectron']]
#samples['SingleElectron_Run2015D_25ns-16Dec2015-v1'] = ['/SingleElectron/Run2015D-16Dec2015-v1/MINIAOD',      ['label=SingleElectron']]  # PRODUCTION

samples['SingleMuon_Run2015C_25ns-16Dec2015-v1'] = ['/SingleMuon/Run2015C_25ns-16Dec2015-v1/MINIAOD', ['label=SingleMuon']]
#samples['SingleMuon_Run2015D_25ns-16Dec2015-v1'] = ['/SingleMuon/Run2015D-16Dec2015-v1/MINIAOD',      ['label=SingleMuon']]  # PRODUCTION

pyCfgParams.append('globalTag=76X_dataRun2_v15')
pyCfgParams.append('is50ns=False')
pyCfgParams.append('isPromptRecoData=True')  # FIXME check for met filters

config.Data.lumiMask      = '/afs/cern.ch/cms/CAF/CMSCOMM/COMM_DQM/certification/Collisions15/13TeV/Cert_246908-260627_13TeV_PromptReco_Collisions15_25ns_JSON_v2.txt'
config.Data.splitting     = 'LumiBased'
config.Data.unitsPerJob   = 10
config.Data.outLFNDirBase = '/store/group/phys_higgs/cmshww/amassiro/RunII/08Jan/data/25ns/'
