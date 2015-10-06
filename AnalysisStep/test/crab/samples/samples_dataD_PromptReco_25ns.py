########
# 2015D
########
samples['Run2015D_PromptReco_25ns_DoubleEG']       = ['/DoubleEG/Run2015D-PromptReco-v3/MINIAOD',       ['label=DoubleEG']]
samples['Run2015D_PromptReco_25ns_DoubleMuon']     = ['/DoubleMuon/Run2015D-PromptReco-v3/MINIAOD',     ['label=DoubleMuon']]
samples['Run2015D_PromptReco_25ns_MuonEG']         = ['/MuonEG/Run2015D-PromptReco-v3/MINIAOD',         ['label=MuEG']]
samples['Run2015D_PromptReco_25ns_SingleElectron'] = ['/SingleElectron/Run2015D-PromptReco-v3/MINIAOD', ['label=SingleElectron']]
samples['Run2015D_PromptReco_25ns_SingleMuon']     = ['/SingleMuon/Run2015D-PromptReco-v3/MINIAOD',     ['label=SingleMuon']]

pyCfgParams.append('globalTag=74X_dataRun2_v2')
pyCfgParams.append('is50ns=False')
pyCfgParams.append('isPromptRecoData=True')

config.Data.outLFNDirBase = '/store/group/phys_higgs/cmshww/amassiro/RunII/17Sep/data/'
config.Data.lumiMask      = '/afs/cern.ch/cms/CAF/CMSCOMM/COMM_DQM/certification/Collisions15/13TeV/Cert_246908-257599_13TeV_PromptReco_Collisions15_25ns_JSON.txt'
#config.Data.runRange     = '253659-999999'
config.Data.splitting     = 'LumiBased'
config.Data.unitsPerJob   = 5

# See https://hypernews.cern.ch/HyperNews/CMS/get/computing-tools/949/1.html
#config.Site.blacklist = ['T2_US_Nebraska','T2_US_UCSD','T2_IT_Legnaro']
