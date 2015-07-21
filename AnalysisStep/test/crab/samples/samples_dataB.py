########
# 25 ns
########
#        CRAB task name          DAS name                                            Latino name
samples['SingleElectron']    = ['/SingleElectron/Run2015B-PromptReco-v1/MINIAOD',      ['label=SingleElectron']]
samples['DoubleEG']          = ['/DoubleEG/Run2015B-PromptReco-v1/MINIAOD',            ['label=DoubleEG']]
samples['DoubleMu']          = ['/DoubleMuon/Run2015B-PromptReco-v1/MINIAOD',          ['label=DoubleMuon']]
samples['DoubleMuLow']       = ['/DoubleMuonLowMass/Run2015B-PromptReco-v1/MINIAOD',   ['label=DoubleMuon']]
samples['SingleMu']          = ['/SingleMu/Run2015B-PromptReco-v1/MINIAOD',            ['label=SingleMuon']]
samples['SingleMuon']        = ['/SingleMuon/Run2015B-PromptReco-v1/MINIAOD',          ['label=SingleMuon']]




########
# Alternative global configuration
########
# config.Data.outLFNDirBase = '/store/user/piedra/test'
# config.Site.storageSite   = 'T2_ES_IFCA'
config.Data.outLFNDirBase = '/store/group/phys_higgs/cmshww/amassiro/RunII/13Jul/data/'
config.Data.lumiMask = "/afs/cern.ch/cms/CAF/CMSCOMM/COMM_DQM/certification/Collisions15/13TeV/Cert_246908-251252_13TeV_PromptReco_Collisions15_JSON.txt"

pyCfgParams.append('globalTag=GR_P_V56')
pyCfgParams.append('json=testJson')

config.General.workArea     = 'crab_projects_13July'




