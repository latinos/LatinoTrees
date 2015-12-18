########
# 2015D >> Jonatan
########
samples['Run2015D_PromptReco_25ns_DoubleEG']       = ['/DoubleEG/Run2015D-PromptReco-v4/MINIAOD',       ['label=DoubleEG']]
samples['Run2015D_PromptReco_25ns_DoubleMuon']     = ['/DoubleMuon/Run2015D-PromptReco-v4/MINIAOD',     ['label=DoubleMuon']]
samples['Run2015D_PromptReco_25ns_MuonEG']         = ['/MuonEG/Run2015D-PromptReco-v4/MINIAOD',         ['label=MuEG']]
samples['Run2015D_PromptReco_25ns_SingleElectron'] = ['/SingleElectron/Run2015D-PromptReco-v4/MINIAOD', ['label=SingleElectron']]
samples['Run2015D_PromptReco_25ns_SingleMuon']     = ['/SingleMuon/Run2015D-PromptReco-v4/MINIAOD',     ['label=SingleMuon']]

pyCfgParams.append('globalTag=74X_dataRun2_v5')
pyCfgParams.append('is50ns=False')
pyCfgParams.append('isPromptRecoData=True')

# 1280.23/pb
#config.Data.lumiMask      = '/afs/cern.ch/cms/CAF/CMSCOMM/COMM_DQM/certification/Collisions15/13TeV/Cert_246908-258750_13TeV_PromptReco_Collisions15_25ns_JSON.txt'
# 2136.361/pb
config.Data.lumiMask      = '/afs/cern.ch/cms/CAF/CMSCOMM/COMM_DQM/certification/Collisions15/13TeV/Cert_246908-260627_13TeV_PromptReco_Collisions15_25ns_JSON.txt'
config.Data.runRange      = '258159-999999'
config.Data.splitting     = 'LumiBased'
config.Data.unitsPerJob   = 10
config.Data.outLFNDirBase = '/store/group/phys_higgs/cmshww/amassiro/RunII/21Oct/data/25ns/'


#new one:
 #/afs/cern.ch/cms/CAF/CMSCOMM/COMM_DQM/certification/Collisions15/13TeV/Cert_246908-260627_13TeV_PromptReco_Collisions15_25ns_JSON_v2.txt
 # for next production