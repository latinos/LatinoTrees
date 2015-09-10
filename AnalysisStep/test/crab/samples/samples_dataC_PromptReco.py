########
# 2015C
########
samples['PromptReco_SingleElectron']    = ['/SingleElectron/Run2015C-PromptReco-v1/MINIAOD',    ['label=SingleElectron']]
samples['PromptReco_DoubleEG']          = ['/DoubleEG/Run2015C-PromptReco-v1/MINIAOD',          ['label=DoubleEG']]
samples['PromptReco_DoubleMuon']        = ['/DoubleMuon/Run2015C-PromptReco-v1/MINIAOD',        ['label=DoubleMuon']]
samples['PromptReco_DoubleMuonLowMass'] = ['/DoubleMuonLowMass/Run2015C-PromptReco-v1/MINIAOD', ['label=DoubleMuon']]
samples['PromptReco_SingleMuon']        = ['/SingleMuon/Run2015C-PromptReco-v1/MINIAOD',        ['label=SingleMuon']]
samples['PromptReco_MuonEG']            = ['/MuonEG/Run2015C-PromptReco-v1/MINIAOD',            ['label=MuEG']]

config.Data.lumiMask = '/afs/cern.ch/cms/CAF/CMSCOMM/COMM_DQM/certification/Collisions15/13TeV/Cert_246908-255031_13TeV_PromptReco_Collisions15_25ns_JSON_v2.txt'
config.Data.runRange = '253659-999999'

pyCfgParams.append('globalTag=GR_P_V56')
pyCfgParams.append('isPromptRecoData=True')


########
# Additional global configuration
########
config.Data.outLFNDirBase = '/store/group/phys_higgs/cmshww/amassiro/RunII/1Sep/data/'
#config.Data.outLFNDirBase = '/store/user/piedra/test'
#config.Site.storageSite   = 'T2_ES_IFCA'

