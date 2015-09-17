########
# 2015C
########
samples['PromptReco_25ns_SingleElectron']    = ['/SingleElectron/Run2015C-PromptReco-v1/MINIAOD',    ['label=SingleElectron']]
samples['PromptReco_25ns_DoubleEG']          = ['/DoubleEG/Run2015C-PromptReco-v1/MINIAOD',          ['label=DoubleEG']]
samples['PromptReco_25ns_DoubleMuon']        = ['/DoubleMuon/Run2015C-PromptReco-v1/MINIAOD',        ['label=DoubleMuon']]
samples['PromptReco_25ns_DoubleMuonLowMass'] = ['/DoubleMuonLowMass/Run2015C-PromptReco-v1/MINIAOD', ['label=DoubleMuon']]
samples['PromptReco_25ns_SingleMuon']        = ['/SingleMuon/Run2015C-PromptReco-v1/MINIAOD',        ['label=SingleMuon']]
samples['PromptReco_25ns_MuonEG']            = ['/MuonEG/Run2015C-PromptReco-v1/MINIAOD',            ['label=MuEG']]

pyCfgParams.append('globalTag=GR_P_V56')
pyCfgParams.append('is50ns=False')
pyCfgParams.append('isPromptRecoData=True')

config.Data.outLFNDirBase = '/store/group/phys_higgs/cmshww/amassiro/RunII/17Sep/data/'
config.Data.lumiMask = '/afs/cern.ch/cms/CAF/CMSCOMM/COMM_DQM/certification/Collisions15/13TeV/Cert_246908-255031_13TeV_PromptReco_Collisions15_25ns_JSON_v2.txt'
config.Data.runRange = '253659-999999'
