########
# 2015D >> Rocio
########
samples['Run2015D_05Oct2015_25ns_DoubleEG']       = ['/DoubleEG/Run2015D-05Oct2015-v1/MINIAOD',       ['label=DoubleEG']]
samples['Run2015D_05Oct2015_25ns_DoubleMuon']     = ['/DoubleMuon/Run2015D-05Oct2015-v1/MINIAOD',     ['label=DoubleMuon']]
samples['Run2015D_05Oct2015_25ns_MuonEG']         = ['/MuonEG/Run2015D-05Oct2015-v2/MINIAOD',         ['label=MuEG']]
samples['Run2015D_05Oct2015_25ns_SingleElectron'] = ['/SingleElectron/Run2015D-05Oct2015-v1/MINIAOD', ['label=SingleElectron']]
samples['Run2015D_05Oct2015_25ns_SingleMuon']     = ['/SingleMuon/Run2015D-05Oct2015-v1/MINIAOD',     ['label=SingleMuon']]

pyCfgParams.append('globalTag=74X_dataRun2_v5')
pyCfgParams.append('is50ns=False')
pyCfgParams.append('isPromptRecoData=False')

# 1280.23/pb
config.Data.lumiMask      = '/afs/cern.ch/cms/CAF/CMSCOMM/COMM_DQM/certification/Collisions15/13TeV/Cert_246908-258750_13TeV_PromptReco_Collisions15_25ns_JSON.txt'
config.Data.runRange      = '256630-258158'
config.Data.splitting     = 'LumiBased'
config.Data.unitsPerJob   = 10
config.Data.outLFNDirBase = '/store/group/phys_higgs/cmshww/amassiro/RunII/21Oct/data/25ns/'
