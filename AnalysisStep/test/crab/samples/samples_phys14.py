
#        Latino name           DAS name                                                                                                           Options
samples['QCD']             = ['/QCD_Pt-20toInf_MuEnrichedPt15_PionKaonDecay_Tune4C_13TeV_pythia8/Phys14DR-PU20bx25_PHYS14_25_V1-v3/MINIAODSIM', ['label=QCD',             'id=0001', 'scale=79.80534']]
samples['DYJetsToLL']      = ['/DYJetsToLL_M-50_13TeV-madgraph-pythia8/Phys14DR-PU20bx25_PHYS14_25_V1-v1/MINIAODSIM',                           ['label=DYJetsToLL',      'id=0002', 'scale=2.12968']]
samples['WJetsToLNu']      = ['/WJetsToLNu_13TeV-madgraph-pythia8-tauola/Phys14DR-PU20bx25_PHYS14_25_V1-v1/MINIAODSIM',                         ['label=WJetsToLNu',      'id=0003', 'scale=2.04731']]
samples['WWTo2L2Nu']       = ['/WWTo2L2Nu_CT10_13TeV-powheg-pythia8-tauola/Spring14miniaod-PU20bx25_POSTLS170_V5-v1/MINIAODSIM',                ['label=WWTo2L2Nu',       'id=0004', 'scale=0.01385']]
samples['WZ']              = ['/WZJetsTo3LNu_Tune4C_13TeV-madgraph-tauola/Phys14DR-PU20bx25_PHYS14_25_V1-v1/MINIAODSIM',                        ['label=WZ',              'id=0005', 'scale=0.27833']]
samples['TTJets' ]         = ['/TTJets_MSDecaysCKM_central_Tune4C_13TeV-madgraph-tauola/Phys14DR-PU20bx25_PHYS14_25_V1-v1/MINIAODSIM',          ['label=TTJets',          'id=0006', 'scale=0.03269']]
samples['TBarToLeptons_s'] = ['/TBarToLeptons_s-channel-CSA14_Tune4C_13TeV-aMCatNLO-tauola/Phys14DR-PU20bx25_PHYS14_25_V1-v1/MINIAODSIM',       ['label=TBarToLeptons_s', 'id=0007', 'scale=0.01664']]
samples['TToLeptons_s']    = ['/TToLeptons_s-channel-CSA14_Tune4C_13TeV-aMCatNLO-tauola/Phys14DR-PU20bx25_PHYS14_25_V1-v1/MINIAODSIM',          ['label=TToLeptons_s',    'id=0008', 'scale=0.01440']]
samples['TBarToLeptons_t'] = ['/TBarToLeptons_t-channel_Tune4C_CSA14_13TeV-aMCatNLO-tauola/Phys14DR-PU20bx25_tsg_PHYS14_25_V1-v2/MINIAODSIM',   ['label=TBarToLeptons_t', 'id=0009', 'scale=0.32380']]
samples['TToLeptons_t']    = ['/TToLeptons_t-channel-CSA14_Tune4C_13TeV-aMCatNLO-tauola/Phys14DR-PU20bx25_tsg_PHYS14_25_V1-v2/MINIAODSIM',      ['label=TToLeptons_t',    'id=0010', 'scale=0.27204']]
samples['gg_HWW']          = ['/GluGluToHToWWTo2LAndTau2Nu_M-125_13TeV-powheg-pythia6/Phys14DR-PU20bx25_tsg_PHYS14_25_V1-v1/MINIAODSIM',        ['label=gg_HWW',          'id=0011', 'scale=0.00996']]
samples['VBF_HWW']         = ['/VBF_HToWWToLAndTauNuQQ_M-125_13TeV-powheg-pythia6/Phys14DR-PU20bx25_tsg_PHYS14_25_V1-v1/MINIAODSIM',            ['label=VBF_HWW',         'id=0012', 'scale=0.00089']]
samples['VH_HWW']          = ['/WH_ZH_HToWW_2Or3WToLNuAndTau_M-125_13TeV_pythia6/Spring14miniaod-PU20bx25_POSTLS170_V5-v1/MINIAODSIM',          ['label=VH_HWW',          'id=0013', 'scale=0.00030']]

# If some global configuration needs to be changed, it goes here
config.Data.outLFNDirBase = '/store/user/piedra/test'
config.Site.storageSite   = 'T2_ES_IFCA'
