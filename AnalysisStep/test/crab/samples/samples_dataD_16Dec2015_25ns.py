########
# 2015D >> Rocio
########
#samples['Run2015D_05Oct2015_25ns_DoubleEG']       = ['/DoubleEG/Run2015D-16Dec2015-v1/MINIAOD',       ['label=DoubleEG']]
samples['Run2015D_05Oct2015_25ns_DoubleEG']       = ['/DoubleEG/Run2015D-16Dec2015-v2/MINIAOD',       ['label=DoubleEG']]
samples['Run2015D_05Oct2015_25ns_DoubleMuon']     = ['/DoubleMuon/Run2015D-16Dec2015-v1/MINIAOD',     ['label=DoubleMuon']]
samples['Run2015D_05Oct2015_25ns_MuonEG']         = ['/MuonEG/Run2015D-16Dec2015-v1/MINIAOD',         ['label=MuEG']]
samples['Run2015D_05Oct2015_25ns_SingleElectron'] = ['/SingleElectron/Run2015D-16Dec2015-v1/MINIAOD', ['label=SingleElectron']]
samples['Run2015D_05Oct2015_25ns_SingleMuon']     = ['/SingleMuon/Run2015D-16Dec2015-v1/MINIAOD',     ['label=SingleMuon']]

pyCfgParams.append('globalTag=76X_dataRun2_v15')
pyCfgParams.append('is50ns=False')
pyCfgParams.append('isPromptRecoData=True')   # FIXME check for met filters

config.Data.lumiMask      = '/afs/cern.ch/cms/CAF/CMSCOMM/COMM_DQM/certification/Collisions15/13TeV/Cert_246908-255031_13TeV_PromptReco_Collisions15_25ns_JSON_v2.txt'
config.Data.runRange      = '254000-258158'
config.Data.splitting     = 'LumiBased'
config.Data.unitsPerJob   = 10
config.Data.outLFNDirBase = '/store/group/phys_higgs/cmshww/amassiro/RunII/08Jan/data/25ns/'

