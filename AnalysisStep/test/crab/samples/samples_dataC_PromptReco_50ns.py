########
# 2015C
########
samples['Run2015C_PromptReco_50ns_DoubleEG']       = ['/DoubleEG/Run2015C-PromptReco-v1/MINIAOD',       ['label=DoubleEG']]
samples['Run2015C_PromptReco_50ns_DoubleMuon']     = ['/DoubleMuon/Run2015C-PromptReco-v1/MINIAOD',     ['label=DoubleMuon']]
samples['Run2015C_PromptReco_50ns_MuonEG']         = ['/MuonEG/Run2015C-PromptReco-v1/MINIAOD',         ['label=MuEG']]
samples['Run2015C_PromptReco_50ns_SingleElectron'] = ['/SingleElectron/Run2015C-PromptReco-v1/MINIAOD', ['label=SingleElectron']]
samples['Run2015C_PromptReco_50ns_SingleMuon']     = ['/SingleMuon/Run2015C-PromptReco-v1/MINIAOD',     ['label=SingleMuon']]

pyCfgParams.append('globalTag=74X_dataRun2_v2')
pyCfgParams.append('is50ns=True')
pyCfgParams.append('isPromptRecoData=True')

config.Data.outLFNDirBase = '/store/group/phys_higgs/cmshww/amassiro/RunII/17Sep/data/'
config.Data.lumiMask      = '/afs/cern.ch/cms/CAF/CMSCOMM/COMM_DQM/certification/Collisions15/13TeV/Cert_246908-255031_13TeV_PromptReco_Collisions15_50ns_JSON_v2.txt'
config.Data.runRange      = '254833'
config.Data.splitting     = 'LumiBased'
config.Data.unitsPerJob   = 1

# See https://hypernews.cern.ch/HyperNews/CMS/get/computing-tools/949/1.html
config.Site.blacklist = ['T2_US_Nebraska','T2_US_UCSD','T2_IT_Legnaro']
