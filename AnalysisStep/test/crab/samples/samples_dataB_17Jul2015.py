########
# 2015B
########
samples['Run2015B_17Jul2015_SingleElectron']    = ['/SingleElectron/Run2015B-17Jul2015-v1/MINIAOD',    ['label=SingleElectron']]
samples['Run2015B_17Jul2015_DoubleEG']          = ['/DoubleEG/Run2015B-17Jul2015-v1/MINIAOD',          ['label=DoubleEG']]
samples['Run2015B_17Jul2015_DoubleMuon']        = ['/DoubleMuon/Run2015B-17Jul2015-v1/MINIAOD',        ['label=DoubleMuon']]
samples['Run2015B_17Jul2015_DoubleMuonLowMass'] = ['/DoubleMuonLowMass/Run2015B-17Jul2015-v1/MINIAOD', ['label=DoubleMuon']]
samples['Run2015B_17Jul2015_SingleMuon']        = ['/SingleMuon/Run2015B-17Jul2015-v1/MINIAOD',        ['label=SingleMuon']]
samples['Run2015B_17Jul2015_MuonEG']            = ['/MuonEG/Run2015B-17Jul2015-v1/MINIAOD',            ['label=MuEG']]

pyCfgParams.append('globalTag=74X_dataRun2_v2')
pyCfgParams.append('is50ns=True')
pyCfgParams.append('isPromptRecoData=False')

config.Data.outLFNDirBase = '/store/group/phys_higgs/cmshww/amassiro/RunII/17Sep/data/'
config.Data.lumiMask = '/afs/cern.ch/cms/CAF/CMSCOMM/COMM_DQM/certification/Collisions15/13TeV/Cert_246908-255031_13TeV_PromptReco_Collisions15_50ns_JSON_v2.txt'
config.Data.runRange = '251162-251562'
