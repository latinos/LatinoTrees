########
# 25ns
########


# W+jets >> Alicia
samples['WJetsToLNu']             = ['/WJetsToLNu_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8/RunIISpring15MiniAODv2-74X_mcRun2_asymptotic_v2-v1/MINIAODSIM',              ['label=WJetsToLNu',             'id=00001', 'doMCweights=True']]
samples['WJetsToLNu_HT100_200']   = ['/WJetsToLNu_HT-100To200_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISpring15MiniAODv2-74X_mcRun2_asymptotic_v2-v1/MINIAODSIM',   ['label=WJetsToLNuHT100to200',   'id=20001', 'doMCweights=True']]
samples['WJetsToLNu_HT200_400']   = ['/WJetsToLNu_HT-200To400_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISpring15MiniAODv2-74X_mcRun2_asymptotic_v2-v1/MINIAODSIM',   ['label=WJetsToLNuHT200to400',   'id=20002', 'doMCweights=True']]
samples['WJetsToLNu_HT400_600']   = ['/WJetsToLNu_HT-400To600_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISpring15MiniAODv2-74X_mcRun2_asymptotic_v2-v1/MINIAODSIM',   ['label=WJetsToLNuHT400to600',   'id=20003', 'doMCweights=True']]
samples['WJetsToLNu_HT600_inf']   = ['/WJetsToLNu_HT-600ToInf_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISpring15MiniAODv2-74X_mcRun2_asymptotic_v2-v1/MINIAODSIM',   ['label=WJetsToLNuHT600toinf',   'id=20004', 'doMCweights=True']]
samples['WJetsToLNu_HT600_800']   = ['/WJetsToLNu_HT-600To800_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISpring15MiniAODv2-74X_mcRun2_asymptotic_v2-v1/MINIAODSIM',   ['label=WJetsToLNuHT600to800',   'id=20005', 'doMCweights=True']]
samples['WJetsToLNu_HT800_1200']  = ['/WJetsToLNu_HT-800To1200_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISpring15MiniAODv2-74X_mcRun2_asymptotic_v2-v1/MINIAODSIM',  ['label=WJetsToLNuHT800to1200',  'id=20006', 'doMCweights=True']]
samples['WJetsToLNu_HT1200_2500'] = ['/WJetsToLNu_HT-1200To2500_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISpring15MiniAODv2-74X_mcRun2_asymptotic_v2-v1/MINIAODSIM', ['label=WJetsToLNuHT1200to2500', 'id=20007', 'doMCweights=True']]
samples['WJetsToLNu_HT2500_inf']  = ['/WJetsToLNu_HT-2500ToInf_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISpring15MiniAODv2-74X_mcRun2_asymptotic_v2-v1/MINIAODSIM',  ['label=WJetsToLNuHT2500toinf',  'id=20008', 'doMCweights=True']]


# DY >> Alicia
samples['DYJetsToLL_M-10to50']   = ['/DYJetsToLL_M-10to50_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8/RunIISpring15MiniAODv2-74X_mcRun2_asymptotic_v2-v1/MINIAODSIM', ['label=DYJetsToLL_M-10to50',   'id=00002', 'doMCweights=True']]
samples['DYJetsToLL_M-50']       = ['/DYJetsToLL_M-50_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8/RunIISpring15MiniAODv2-74X_mcRun2_asymptotic_v2-v1/MINIAODSIM',     ['label=DYJetsToLL_M-50',       'id=00003', 'doMCweights=True']]
samples['DYJetsToLL_M-5to50-LO'] = ['/DYJetsToLL_M-5to50_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISpring15MiniAODv2-74X_mcRun2_asymptotic_v2-v1/MINIAODSIM',   ['label=DYJetsToLL_M-5to50-LO', 'id=30002', 'doMCweights=True']]
samples['DYJetsToLL_M-50-LO']    = ['/DYJetsToLL_M-50_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISpring15MiniAODv2-74X_mcRun2_asymptotic_v2-v1/MINIAODSIM',      ['label=DYJetsToLL_M-50-LO',    'id=30003', 'doMCweights=True']]


# VV >> Alicia
samples['WWTo2L2Nu']  = ['/WWTo2L2Nu_13TeV-powheg/RunIISpring15MiniAODv2-74X_mcRun2_asymptotic_v2-v1/MINIAODSIM',                         ['label=WWTo2L2Nu', 'id=00004', 'doMCweights=True']]
samples['WWToLNuQQ']  = ['/WWToLNuQQ_13TeV-powheg/RunIISpring15MiniAODv2-74X_mcRun2_asymptotic_v2-v1/MINIAODSIM',                         ['label=WWToLNuQQ', 'id=00031', 'doMCweights=True']]
samples['WZ']         = ['/WZ_TuneCUETP8M1_13TeV-pythia8/RunIISpring15MiniAODv2-74X_mcRun2_asymptotic_v2-v1/MINIAODSIM',                  ['label=WZ',        'id=00005', 'doLHE=False']]
samples['WZTo3LNu']   = ['/WZTo3LNu_TuneCUETP8M1_13TeV-powheg-pythia8/RunIISpring15MiniAODv2-74X_mcRun2_asymptotic_v2-v1/MINIAODSIM',     ['label=WZTo3LNu',  'id=00006', 'doMCweights=True']]
samples['WZJets']     = ['/WZJets_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8/RunIISpring15MiniAODv2-74X_mcRun2_asymptotic_v2-v1/MINIAODSIM', ['label=WZJets',    'id=00007', 'doMCweights=True']]
samples['WZTo2L2Q']   = ['/WZTo2L2Q_13TeV_amcatnloFXFX_madspin_pythia8/RunIISpring15MiniAODv2-74X_mcRun2_asymptotic_v2-v1/MINIAODSIM',    ['label=WZTo2L2Q',  'id=00008', 'doMCweights=True']]
samples['VVTo2L2Nu']  = ['/VVTo2L2Nu_13TeV_amcatnloFXFX_madspin_pythia8/RunIISpring15MiniAODv2-74X_mcRun2_asymptotic_v2-v1/MINIAODSIM',   ['label=VVTo2L2Nu', 'id=00032', 'doMCweights=True']]


# ggWW >> Ankita
samples['GluGluWWTo2L2Nu_MCFM']       = ['/GluGluWWTo2L2Nu_MCFM_13TeV/RunIISpring15DR74-Asympt25ns_MCRUN2_74_V9-v1/MINIAODSIM',   ['label=GluGluWWTo2L2Nu_MCFM', 'id=00033', 'doMCweights=True', 'metNoHF=\"\"', 'LHEweightSource=source', 'puInformation=addPileupInfo' ]]
#samples['GluGluWWTo2L2NuHiggs_MCFM'] = ['',   ['label=GluGluWWTo2L2NuHiggs_MCFM', 'id=00034', 'doMCweights=True']]


# ZZ >> Ankita
samples['ZZ']         = ['/ZZ_TuneCUETP8M1_13TeV-pythia8/RunIISpring15MiniAODv2-74X_mcRun2_asymptotic_v2-v1/MINIAODSIM',                  ['label=ZZ',        'id=00009', 'doLHE=False']]
samples['ZZTo2L2Q']   = ['/ZZTo2L2Q_13TeV_amcatnloFXFX_madspin_pythia8/RunIISpring15MiniAODv2-74X_mcRun2_asymptotic_v2-v1/MINIAODSIM',    ['label=ZZTo2L2Q',  'id=00010', 'doMCweights=True']]
samples['ZZTo4L']     = ['/ZZTo4L_13TeV_powheg_pythia8/RunIISpring15MiniAODv2-74X_mcRun2_asymptotic_v2-v2/MINIAODSIM',                    ['label=ZZTo4L',    'id=00011', 'doMCweights=True']]
samples['ZZTo2L2Nu']  = ['/ZZTo2L2Nu_13TeV_powheg_pythia8/RunIISpring15MiniAODv2-74X_mcRun2_asymptotic_v2-v2/MINIAODSIM',                 ['label=ZZTo2L2Nu', 'id=00030', 'doMCweights=True']]
samples['ggZZ4e']     = ['/GluGluToZZTo4e_BackgroundOnly_13TeV_MCFM/RunIISpring15MiniAODv2-74X_mcRun2_asymptotic_v2-v1/MINIAODSIM',       ['label=ggZZ4e',    'id=00040', 'LHEweightSource=source']]
samples['ggZZ4m']     = ['/GluGluToZZTo4mu_BackgroundOnly_13TeV_MCFM/RunIISpring15MiniAODv2-74X_mcRun2_asymptotic_v2-v1/MINIAODSIM',      ['label=ggZZ4m',    'id=00041', 'LHEweightSource=source']]
samples['ggZZ4t']     = ['/GluGluToZZTo4tau_BackgroundOnly_13TeV_MCFM/RunIISpring15MiniAODv2-74X_mcRun2_asymptotic_v2-v1/MINIAODSIM',     ['label=ggZZ4t',    'id=00042', 'LHEweightSource=source']]
samples['ggZZ2e2m']   = ['/GluGluToZZTo2e2mu_BackgroundOnly_13TeV_MCFM/RunIISpring15MiniAODv2-74X_mcRun2_asymptotic_v2-v1/MINIAODSIM',    ['label=ggZZ2e2m',  'id=00043', 'LHEweightSource=source']]
samples['ggZZ2e2t']   = ['/GluGluToZZTo2e2tau_BackgroundOnly_13TeV_MCFM/RunIISpring15MiniAODv2-74X_mcRun2_asymptotic_v2-v1/MINIAODSIM',   ['label=ggZZ2e2t',  'id=00044', 'LHEweightSource=source']]
samples['ggZZ2m2t']   = ['/GluGluToZZTo2mu2tau_BackgroundOnly_13TeV_MCFM/RunIISpring15MiniAODv2-74X_mcRun2_asymptotic_v2-v1/MINIAODSIM',  ['label=ggZZ2m2t',  'id=00045', 'LHEweightSource=source']]


# Single top >> Ankita
samples['ST_t-channel_antitop'] = ['/ST_t-channel_antitop_4f_leptonDecays_13TeV-powheg-pythia8_TuneCUETP8M1/RunIISpring15MiniAODv2-74X_mcRun2_asymptotic_v2-v1/MINIAODSIM', ['label=ST_t-channel_antitop', 'id=00012', 'doMCweights=True', 'LHEweightSource=source']]
samples['ST_t-channel_top']     = ['/ST_t-channel_top_4f_leptonDecays_13TeV-powheg-pythia8_TuneCUETP8M1/RunIISpring15MiniAODv2-74X_mcRun2_asymptotic_v2-v1/MINIAODSIM',     ['label=ST_t-channel_top',     'id=00013', 'doMCweights=True', 'LHEweightSource=source']]
samples['ST_t-channel']         = ['/ST_t-channel_4f_leptonDecays_13TeV-amcatnlo-pythia8_TuneCUETP8M1/RunIISpring15MiniAODv2-74X_mcRun2_asymptotic_v2-v1/MINIAODSIM',       ['label=ST_t-channel',         'id=00014', 'doMCweights=True']]
samples['ST_tW_antitop']        = ['/ST_tW_antitop_5f_inclusiveDecays_13TeV-powheg-pythia8_TuneCUETP8M1/RunIISpring15MiniAODv2-74X_mcRun2_asymptotic_v2-v1/MINIAODSIM',     ['label=ST_tW_antitop',        'id=00015', 'doMCweights=True']]
samples['ST_tW_top']            = ['/ST_tW_top_5f_inclusiveDecays_13TeV-powheg-pythia8_TuneCUETP8M1/RunIISpring15MiniAODv2-74X_mcRun2_asymptotic_v2-v2/MINIAODSIM',         ['label=ST_tW_top',            'id=00016', 'doMCweights=True']]
samples['ST_s-channel']         = ['/ST_s-channel_4f_leptonDecays_13TeV-amcatnlo-pythia8_TuneCUETP8M1/RunIISpring15MiniAODv2-74X_mcRun2_asymptotic_v2-v1/MINIAODSIM',       ['label=ST_s-channel',         'id=00022', 'doMCweights=True']]


# Top >> Ankita
samples['TT']                = ['/TT_TuneCUETP8M1_13TeV-powheg-pythia8/RunIISpring15MiniAODv2-74X_mcRun2_asymptotic_v2-v1/MINIAODSIM',                         ['label=TT',               'id=00017', 'doMCweights=True']]
samples['TTJets']            = ['/TTJets_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8/RunIISpring15MiniAODv2-74X_mcRun2_asymptotic_v2-v3/MINIAODSIM',               ['label=TTJets',           'id=00018', 'doMCweights=True']]
samples['TTTo2L2Nu']         = ['/TTTo2L2Nu_13TeV-powheg/RunIISpring15MiniAODv2-74X_mcRun2_asymptotic_v2-v1/MINIAODSIM',                                       ['label=TTTo2L2Nu',        'id=00019', 'doMCweights=True']]
samples['TTWJetsToLNu']      = ['/TTWJetsToLNu_TuneCUETP8M1_13TeV-amcatnloFXFX-madspin-pythia8/RunIISpring15MiniAODv2-74X_mcRun2_asymptotic_v2-v1/MINIAODSIM', ['label=TTWJetsToLNu',     'id=00020', 'doMCweights=True']]
samples['TTZToLLNuNu_M-10']  = ['/TTZToLLNuNu_M-10_TuneCUETP8M1_13TeV-amcatnlo-pythia8/RunIISpring15MiniAODv2-74X_mcRun2_asymptotic_v2-v2/MINIAODSIM',         ['label=TTZToLLNuNu_M-10', 'id=00021', 'doMCweights=True']]


# GluGluH >> Ankita
samples['GluGluHToWWTo2L2Nu_M120']       = ['/GluGluHToWWTo2L2Nu_M120_13TeV_powheg_JHUgen_pythia8/RunIISpring15MiniAODv2-74X_mcRun2_asymptotic_v2-v2/MINIAODSIM',  ['label=GluGluHToWWTo2L2Nu_M120',       'id=01120', 'doMCweights=True']]
samples['GluGluHToWWTo2L2Nu_M125']       = ['/GluGluHToWWTo2L2Nu_M125_13TeV_powheg_JHUgen_pythia8/RunIISpring15MiniAODv2-74X_mcRun2_asymptotic_v2-v1/MINIAODSIM',  ['label=GluGluHToWWTo2L2Nu_M125',       'id=01125', 'doMCweights=True']]
samples['GluGluHToWWTo2L2Nu_M130']       = ['/GluGluHToWWTo2L2Nu_M130_13TeV_powheg_JHUgen_pythia8/RunIISpring15MiniAODv2-74X_mcRun2_asymptotic_v2-v1/MINIAODSIM',  ['label=GluGluHToWWTo2L2Nu_M130',       'id=01130', 'doMCweights=True']]
samples['GluGluHToWWTo2L2NuAMCNLO_M125'] = ['/GluGluHToWWTo2L2Nu_M125_13TeV_amcatnloFXFX_pythia8/RunIISpring15MiniAODv2-74X_mcRun2_asymptotic_v2-v2/MINIAODSIM',   ['label=GluGluHToWWTo2L2NuAMCNLO_M125', 'id=07125', 'doMCweights=True']]
samples['GluGluHToWWToLNuQQ_M450']       = ['/GluGluHToWWToLNuQQ_M450_13TeV_powheg_JHUgen_pythia8/RunIISpring15MiniAODv2-74X_mcRun2_asymptotic_v2-v1/MINIAODSIM',  ['label=GluGluHToWWToLNuQQ_M450',       'id=60450', 'doMCweights=True']]
samples['GluGluHToWWToLNuQQ_M650']       = ['/GluGluHToWWToLNuQQ_M650_13TeV_powheg_JHUgen_pythia8/RunIISpring15MiniAODv2-74X_mcRun2_asymptotic_v2-v1/MINIAODSIM',  ['label=GluGluHToWWToLNuQQ_M650',       'id=60650', 'doMCweights=True']]
samples['GluGluHToWWToLNuQQ_M1000']      = ['/GluGluHToWWToLNuQQ_M1000_13TeV_powheg_JHUgen_pythia8/RunIISpring15MiniAODv2-74X_mcRun2_asymptotic_v2-v1/MINIAODSIM', ['label=GluGluHToWWToLNuQQ_M1000',      'id=61000', 'doMCweights=True']]
samples['GluGluHToZZTo4L_M125']          = ['/GluGluHToZZTo4L_M125_13TeV_powheg_JHUgen_pythia8/RunIISpring15MiniAODv2-74X_mcRun2_asymptotic_v2-v1/MINIAODSIM',     ['label=GluGluHToZZTo4L_M125',          'id=11125', 'doMCweights=True']]
samples['GluGluHToTauTau_M120']          = ['/GluGluHToTauTau_M120_13TeV_powheg_pythia8/RunIISpring15MiniAODv2-74X_mcRun2_asymptotic_v2-v1/MINIAODSIM',            ['label=GluGluHToTauTau_M120',          'id=21120', 'doMCweights=True']]
samples['GluGluHToTauTau_M125']          = ['/GluGluHToTauTau_M125_13TeV_powheg_pythia8/RunIISpring15MiniAODv2-74X_mcRun2_asymptotic_v2-v1/MINIAODSIM',            ['label=GluGluHToTauTau_M125',          'id=21125', 'doMCweights=True']]
samples['GluGluHToTauTau_M130']          = ['/GluGluHToTauTau_M130_13TeV_powheg_pythia8/RunIISpring15MiniAODv2-74X_mcRun2_asymptotic_v2-v1/MINIAODSIM',            ['label=GluGluHToTauTau_M130',          'id=21130', 'doMCweights=True']]


# HW >> Daniel
samples['HWminusJ_HToWW_M120']     = ['/HWminusJ_HToWW_M120_13TeV_powheg_pythia8/RunIISpring15MiniAODv2-74X_mcRun2_asymptotic_v2-v1/MINIAODSIM',  ['label=HWminusJ_HToWW_M120',     'id=02120', 'doMCweights=True']]
samples['HWminusJ_HToWW_M125']     = ['/HWminusJ_HToWW_M125_13TeV_powheg_pythia8/RunIISpring15MiniAODv2-74X_mcRun2_asymptotic_v2-v1/MINIAODSIM',  ['label=HWminusJ_HToWW_M125',     'id=02125', 'doMCweights=True']]
samples['HWminusJ_HToWW_M130']     = ['/HWminusJ_HToWW_M130_13TeV_powheg_pythia8/RunIISpring15MiniAODv2-74X_mcRun2_asymptotic_v2-v1/MINIAODSIM',  ['label=HWminusJ_HToWW_M130',     'id=02130', 'doMCweights=True']]
samples['HWplusJ_HToWW_M120']      = ['/HWplusJ_HToWW_M120_13TeV_powheg_pythia8/RunIISpring15MiniAODv2-74X_mcRun2_asymptotic_v2-v1/MINIAODSIM',   ['label=HWplusJ_HToWW_M120',      'id=03120', 'doMCweights=True']]
samples['HWplusJ_HToWW_M125']      = ['/HWplusJ_HToWW_M125_13TeV_powheg_pythia8/RunIISpring15MiniAODv2-74X_mcRun2_asymptotic_v2-v1/MINIAODSIM',   ['label=HWplusJ_HToWW_M125',      'id=03125', 'doMCweights=True']]
samples['HWplusJ_HToWW_M130']      = ['/HWplusJ_HToWW_M130_13TeV_powheg_pythia8/RunIISpring15MiniAODv2-74X_mcRun2_asymptotic_v2-v1/MINIAODSIM',   ['label=HWplusJ_HToWW_M130',      'id=03130', 'doMCweights=True']]
samples['HWminusJ_HToTauTau_M120'] = ['/WminusHToTauTau_M120_13TeV_powheg_pythia8/RunIISpring15MiniAODv2-74X_mcRun2_asymptotic_v2-v1/MINIAODSIM', ['label=HWminusJ_HToTauTau_M120', 'id=52120', 'doMCweights=True']]
samples['HWminusJ_HToTauTau_M125'] = ['/WminusHToTauTau_M125_13TeV_powheg_pythia8/RunIISpring15MiniAODv2-74X_mcRun2_asymptotic_v2-v1/MINIAODSIM', ['label=HWminusJ_HToTauTau_M125', 'id=52125', 'doMCweights=True']]
samples['HWminusJ_HToTauTau_M130'] = ['/WminusHToTauTau_M130_13TeV_powheg_pythia8/RunIISpring15MiniAODv2-74X_mcRun2_asymptotic_v2-v1/MINIAODSIM', ['label=HWminusJ_HToTauTau_M130', 'id=52130', 'doMCweights=True']]
samples['HWplusJ_HToTauTau_M120']  = ['/WplusHToTauTau_M120_13TeV_powheg_pythia8/RunIISpring15MiniAODv2-74X_mcRun2_asymptotic_v2-v1/MINIAODSIM',  ['label=HWplusJ_HToTauTau_M120',  'id=53120', 'doMCweights=True']]
samples['HWplusJ_HToTauTau_M125']  = ['/WplusHToTauTau_M125_13TeV_powheg_pythia8/RunIISpring15MiniAODv2-74X_mcRun2_asymptotic_v2-v1/MINIAODSIM',  ['label=HWplusJ_HToTauTau_M125',  'id=53125', 'doMCweights=True']]
samples['HWplusJ_HToTauTau_M130']  = ['/WplusHToTauTau_M130_13TeV_powheg_pythia8/RunIISpring15MiniAODv2-74X_mcRun2_asymptotic_v2-v1/MINIAODSIM',  ['label=HWplusJ_HToTauTau_M130',  'id=53130', 'doMCweights=True']]


# HZ >> Daniel
samples['HZJ_HToWW_M120']     = ['/HZJ_HToWW_M120_13TeV_powheg_pythia8/RunIISpring15MiniAODv2-74X_mcRun2_asymptotic_v2-v1/MINIAODSIM',  ['label=HZJ_HToWW_M120',     'id=06120', 'doMCweights=True']]
samples['HZJ_HToWW_M125']     = ['/HZJ_HToWW_M125_13TeV_powheg_pythia8/RunIISpring15MiniAODv2-74X_mcRun2_asymptotic_v2-v1/MINIAODSIM',  ['label=HZJ_HToWW_M125',     'id=06125', 'doMCweights=True']]
samples['HZJ_HToWW_M130']     = ['/HZJ_HToWW_M130_13TeV_powheg_pythia8/RunIISpring15MiniAODv2-74X_mcRun2_asymptotic_v2-v1/MINIAODSIM',  ['label=HZJ_HToWW_M130',     'id=06130', 'doMCweights=True']]
samples['HZJ_HToTauTau_M120'] = ['/ZHToTauTau_M120_13TeV_powheg_pythia8/RunIISpring15MiniAODv2-74X_mcRun2_asymptotic_v2-v1/MINIAODSIM', ['label=HZJ_HToTauTau_M120', 'id=53120', 'doMCweights=True']]
samples['HZJ_HToTauTau_M125'] = ['/ZHToTauTau_M125_13TeV_powheg_pythia8/RunIISpring15MiniAODv2-74X_mcRun2_asymptotic_v2-v1/MINIAODSIM', ['label=HZJ_HToTauTau_M125', 'id=53125', 'doMCweights=True']]
samples['HZJ_HToTauTau_M130'] = ['/ZHToTauTau_M130_13TeV_powheg_pythia8/RunIISpring15MiniAODv2-74X_mcRun2_asymptotic_v2-v1/MINIAODSIM', ['label=HZJ_HToTauTau_M130', 'id=53130', 'doMCweights=True']]


# ttH >> Daniel
samples['ttHJetToNonbb_M120']   = ['/ttHJetToNonbb_M120_13TeV_amcatnloFXFX_madspin_pythia8/RunIISpring15MiniAODv2-74X_mcRun2_asymptotic_v2-v1/MINIAODSIM',      ['label=ttHJetToNonbb_M120',   'id=04120', 'doMCweights=True']]
samples['ttHJetToNonbb_M125']   = ['/ttHJetToNonbb_M125_13TeV_amcatnloFXFX_madspin_pythia8/RunIISpring15MiniAODv2-74X_mcRun2_asymptotic_v2-v1/MINIAODSIM',      ['label=ttHJetToNonbb_M125',   'id=04125', 'doMCweights=True']]
samples['ttHJetToNonbb_M130']   = ['/ttHJetToNonbb_M130_13TeV_amcatnloFXFX_madspin_pythia8/RunIISpring15MiniAODv2-74X_mcRun2_asymptotic_v2-v1/MINIAODSIM',      ['label=ttHJetToNonbb_M130',   'id=04130', 'doMCweights=True']]
samples['ttHJetToNonbb_M125_A'] = ['/ttHJetToNonbb_M125_13TeV_amcatnloFXFX_madspin_pythia8/RunIISpring15MiniAODv2-74X_mcRun2_asymptotic_v2_ext1-v1/MINIAODSIM', ['label=ttHJetToNonbb_M125_A', 'id=04125', 'doMCweights=True']]
samples['ttHJetTobb_M120']      = ['/ttHJetTobb_M120_13TeV_amcatnloFXFX_madspin_pythia8/RunIISpring15MiniAODv2-74X_mcRun2_asymptotic_v2-v1/MINIAODSIM',         ['label=ttHJetTobb_M120',      'id=34120', 'doMCweights=True']]
samples['ttHJetTobb_M125']      = ['/ttHJetTobb_M125_13TeV_amcatnloFXFX_madspin_pythia8/RunIISpring15MiniAODv2-74X_mcRun2_asymptotic_v2-v1/MINIAODSIM',         ['label=ttHJetTobb_M125',      'id=34125', 'doMCweights=True']]
samples['ttHJetTobb_M130']      = ['/ttHJetTobb_M130_13TeV_amcatnloFXFX_madspin_pythia8/RunIISpring15MiniAODv2-74X_mcRun2_asymptotic_v2-v1/MINIAODSIM',         ['label=ttHJetTobb_M130',      'id=34130', 'doMCweights=True']] 


# VBFH >> Khakim
samples['VBFHToWWTo2L2Nu_M120']       = ['/VBFHToWWTo2L2Nu_M120_13TeV_powheg_JHUgen_pythia8/RunIISpring15MiniAODv2-74X_mcRun2_asymptotic_v2-v1/MINIAODSIM', ['label=VBFHToWWTo2L2Nu_M120',       'id=05120', 'doMCweights=True']]
samples['VBFHToWWTo2L2Nu_M125']       = ['/VBFHToWWTo2L2Nu_M125_13TeV_powheg_JHUgen_pythia8/RunIISpring15MiniAODv2-74X_mcRun2_asymptotic_v2-v1/MINIAODSIM', ['label=VBFHToWWTo2L2Nu_M125',       'id=05125', 'doMCweights=True']]
samples['VBFHToWWTo2L2Nu_M130']       = ['/VBFHToWWTo2L2Nu_M130_13TeV_powheg_JHUgen_pythia8/RunIISpring15MiniAODv2-74X_mcRun2_asymptotic_v2-v1/MINIAODSIM', ['label=VBFHToWWTo2L2Nu_M130',       'id=05130', 'doMCweights=True']]
samples['VBFHToWWTo2L2NuAMCNLO_M125'] = ['/VBFHToWWTo2L2Nu_M125_13TeV_amcatnlo_pythia8/RunIISpring15MiniAODv2-74X_mcRun2_asymptotic_v2-v1/MINIAODSIM',      ['label=VBFHToWWTo2L2NuAMCNLO_M125', 'id=08125', 'doMCweights=True']]
samples['VBFHToTauTau_M120']          = ['/VBFHToTauTau_M120_13TeV_powheg_pythia8/RunIISpring15MiniAODv2-74X_mcRun2_asymptotic_v2-v1/MINIAODSIM',           ['label=VBFHToTauTau_M120',          'id=51120', 'doMCweights=True']]
samples['VBFHToTauTau_M125']          = ['/VBFHToTauTau_M125_13TeV_powheg_pythia8/RunIISpring15MiniAODv2-74X_mcRun2_asymptotic_v2-v1/MINIAODSIM',           ['label=VBFHToTauTau_M125',          'id=51125', 'doMCweights=True']]
samples['VBFHToTauTau_M130']          = ['/VBFHToTauTau_M130_13TeV_powheg_pythia8/RunIISpring15MiniAODv2-74X_mcRun2_asymptotic_v2-v1/MINIAODSIM',           ['label=VBFHToTauTau_M130',          'id=51130', 'doMCweights=True']]


# VVV >> Khakim
#samples['WWW'] = ['',                                                                                                       ['label=WWW', 'id=00000', 'doMCweights=True']]
samples['WWZ']  = ['/WWZ_TuneCUETP8M1_13TeV-amcatnlo-pythia8/RunIISpring15MiniAODv2-74X_mcRun2_asymptotic_v2-v1/MINIAODSIM', ['label=WWZ', 'id=00000', 'doMCweights=True']]
samples['WZZ']  = ['/WZZ_TuneCUETP8M1_13TeV-amcatnlo-pythia8/RunIISpring15MiniAODv2-74X_mcRun2_asymptotic_v2-v1/MINIAODSIM', ['label=WZZ', 'id=00000', 'doMCweights=True']]
samples['ZZZ']  = ['/ZZZ_TuneCUETP8M1_13TeV-amcatnlo-pythia8/RunIISpring15MiniAODv2-74X_mcRun2_asymptotic_v2-v1/MINIAODSIM', ['label=ZZZ', 'id=00000', 'doMCweights=True']]


# Vg >> Khakim
samples['Wg']      = ['/WGToLNuG_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8/RunIISpring15MiniAODv2-74X_mcRun2_asymptotic_v2-v1/MINIAODSIM',        ['label=Wg',     'id=00000', 'doMCweights=True']]
samples['Wg500']   = ['/WGToLNuG_PtG-500_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISpring15MiniAODv2-74X_mcRun2_asymptotic_v2-v1/MINIAODSIM', ['label=Wg500',  'id=00000', 'doMCweights=True']]
#samples['WgStar'] = ['',                                                                                                                       ['label=WgStar', 'id=00000', 'doMCweights=True']]
samples['Zg']      = ['/ZGTo2LG_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8/RunIISpring15MiniAODv2-74X_mcRun2_asymptotic_v2-v1/MINIAODSIM',         ['label=Zg',     'id=00000', 'doMCweights=True']]
#samples['ZgStar'] = ['',                                                                                                                       ['label=ZgStar', 'id=00000', 'doMCweights=True']]


# QCD >> Khakim
samples['QCD_Pt-15to20_MuEnrichedPt5']     = ['/QCD_Pt-15to20_MuEnrichedPt5_TuneCUETP8M1_13TeV_pythia8/RunIISpring15MiniAODv2-74X_mcRun2_asymptotic_v2-v1/MINIAODSIM',                ['label=QCD_Pt-15to20_MuEnrichedPt5',     'id=10001', 'doLHE=False' ]]
samples['QCD_Pt-20toInf_MuEnrichedPt15']   = ['/QCD_Pt-20toInf_MuEnrichedPt15_TuneCUETP8M1_13TeV_pythia8/RunIISpring15MiniAODv2-74X_mcRun2_asymptotic_v2-v1/MINIAODSIM',              ['label=QCD_Pt-20toInf_MuEnrichedPt15',   'id=10002', 'doLHE=False' ]]
samples['QCD_Pt-15to20_EMEnriched']        = ['/QCD_Pt-15to20_EMEnriched_TuneCUETP8M1_13TeV_pythia8/RunIISpring15MiniAODv2-74X_mcRun2_asymptotic_v2-v1/MINIAODSIM',                   ['label=QCD_Pt-15to20_EMEnriched',        'id=10003', 'doLHE=False' ]]
samples['QCD_Pt-20to30_EMEnriched']        = ['/QCD_Pt-20to30_EMEnriched_TuneCUETP8M1_13TeV_pythia8/RunIISpring15MiniAODv2-74X_mcRun2_asymptotic_v2-v1/MINIAODSIM',                   ['label=QCD_Pt-20to30_EMEnriched',        'id=10004', 'doLHE=False' ]]
samples['QCD_Pt-30to50_EMEnriched']        = ['/QCD_Pt-30to50_EMEnriched_TuneCUETP8M1_13TeV_pythia8/RunIISpring15MiniAODv2-74X_mcRun2_asymptotic_v2-v1/MINIAODSIM',                   ['label=QCD_Pt-30to50_EMEnriched',        'id=10005', 'doLHE=False' ]]
samples['QCD_Pt-50to80_EMEnriched']        = ['/QCD_Pt-50to80_EMEnriched_TuneCUETP8M1_13TeV_pythia8/RunIISpring15MiniAODv2-74X_mcRun2_asymptotic_v2-v1/MINIAODSIM',                   ['label=QCD_Pt-50to80_EMEnriched',        'id=10006', 'doLHE=False' ]]
samples['QCD_Pt-30toInf_DoubleEMEnriched'] = ['/QCD_Pt-30toInf_DoubleEMEnriched_MGG-40to80_TuneCUETP8M1_13TeV_Pythia8/RunIISpring15MiniAODv2-74X_mcRun2_asymptotic_v2-v1/MINIAODSIM', ['label=QCD_Pt-30toInf_DoubleEMEnriched', 'id=10007', 'doLHE=False' ]]


# VBS >> Jasper
samples['WpWpJJ_EWK_QCD']                = ['/WpWpJJ_EWK-QCD_TuneCUETP8M1_13TeV-madgraph-pythia8/RunIISpring15MiniAODv2-74X_mcRun2_asymptotic_v2-v1/MINIAODSIM',             ['label=WpWpJJ_EWK_QCD',               'id=40001', 'doMCweights=True']]
samples['WpWpJJ_EWK']                    = ['/WpWpJJ_EWK_TuneCUETP8M1_13TeV-madgraph-pythia8/RunIISpring15MiniAODv2-74X_mcRun2_asymptotic_v2-v1/MINIAODSIM',                 ['label=WpWpJJ_EWK',                   'id=40002', 'doMCweights=True']]
samples['WpWpJJ_QCD']                    = ['/WpWpJJ_QCD_TuneCUETP8M1_13TeV-madgraph-pythia8/RunIISpring15MiniAODv2-74X_mcRun2_asymptotic_v2-v1/MINIAODSIM',                 ['label=WpWpJJ_QCD',                   'id=40003', 'doMCweights=True']]
samples['WpWmJJ_EWK_QCD_noTop']          = ['/WWJJToLNuLNu_EWK_QCD_noTop_13TeV-madgraph-pythia8/RunIISpring15MiniAODv2-74X_mcRun2_asymptotic_v2-v1/MINIAODSIM',              ['label=WpWmJJ_EWK_QCD_noTop',         'id=40004', 'doMCweights=True']]
#samples['WpWmJJ_EWK_noTop']             = ['/WWJJToLNuLNu_EWK_noTop_13TeV-madgraph-pythia8/RunIISpring15MiniAODv2-74X_mcRun2_asymptotic_v2-v1/MINIAODSIM',                  ['label=WpWmJJ_EWK_noTop',             'id=40005', 'doMCweights=True']]
samples['WpWmJJ_QCD_noTop']              = ['/WWJJToLNuLNu_QCD_noTop_13TeV-madgraph-pythia8/RunIISpring15MiniAODv2-74X_mcRun2_asymptotic_v2-v1/MINIAODSIM',                  ['label=WpWmJJ_QCD_noTop',             'id=40006', 'doMCweights=True']]
#samples['WpWmJJ_EWK_QCD_noTop_noHiggs'] = ['/WWJJToLNuLNu_EWK_QCD_noTop-noHiggs_13TeV-madgraph-pythia8/RunIISpring15MiniAODv2-74X_mcRun2_asymptotic_v2-v1/MINIAODSIM',      ['label=WpWmJJ_EWK_QCD_noTop_noHiggs', 'id=40007', 'doMCweights=True']]
#samples['WpWmJJ_EWK']                   = ['/WWJJToLNuLNu_EWK_13TeV-madgraph-pythia8/RunIISpring15MiniAODv2-74X_mcRun2_asymptotic_v2-v1/MINIAODSIM',                        ['label=WpWmJJ_EWK',                   'id=40008', 'doMCweights=True']]
#samples['TWJ']                          = ['/TWJToLNuLNu_EWK_13TeV-madgraph-pythia8/RunIISpring15MiniAODv2-74X_mcRun2_asymptotic_v2-v1/MINIAODSIM',                         ['label=TWJ',                          'id=40009', 'doMCweights=True']]
samples['WLLJJToLNu_M-60_EWK_QCD']       = ['/WLLJJToLNu_M-60_EWK_QCD_TuneCUETP8M1_13TeV-madgraph-pythia8/RunIISpring15MiniAODv2-74X_mcRun2_asymptotic_v2-v1/MINIAODSIM',    ['label=WLLJJToLNu_M-60_EWK_QCD',      'id=40010', 'doMCweights=True']]
samples['WLLJJToLNu_M-60_EWK']           = ['/WLLJJToLNu_M-60_EWK_13TeV-madgraph-pythia8/RunIISpring15MiniAODv2-74X_mcRun2_asymptotic_v2-v1/MINIAODSIM',                     ['label=WLLJJToLNu_M-60_EWK',          'id=40011', 'doMCweights=True']]
samples['WLLJJToLNu_M-60_QCD']           = ['/WLLJJToLNu_M-60_QCD_13TeV-madgraph-pythia8/RunIISpring15MiniAODv2-74X_mcRun2_asymptotic_v2-v1/MINIAODSIM',                     ['label=WLLJJToLNu_M-60_QCD',          'id=40012', 'doMCweights=True']]
samples['WLLJJToLNu_M-4to60_EWK_QCD']    = ['/WLLJJToLNu_M-4to60_EWK_QCD_TuneCUETP8M1_13TeV-madgraph-pythia8/RunIISpring15MiniAODv2-74X_mcRun2_asymptotic_v2-v1/MINIAODSIM', ['label=WLLJJToLNu_M-4to60_EWK_QCD',   'id=40013', 'doMCweights=True']]
samples['WZJJ_EWK_QCD']                  = ['/WZJJ_EWK_QCD_13TeV-madgraph-pythia8/RunIISpring15MiniAODv2-74X_mcRun2_asymptotic_v2-v1/MINIAODSIM',                            ['label=WZJJ_EWK_QCD',                 'id=40014', 'doMCweights=True']]
#samples['WZJJ_EWK']                     = ['/WZJJ_EWK_13TeV-madgraph-pythia8/RunIISpring15MiniAODv2-74X_mcRun2_asymptotic_v2-v1/MINIAODSIM',                                ['label=WZJJ_EWK',                     'id=40015', 'doMCweights=True']]
samples['WZJJ_QCD']                      = ['/WZJJ_QCD_13TeV-madgraph-pythia8/RunIISpring15MiniAODv2-74X_mcRun2_asymptotic_v2-v1/MINIAODSIM',                                ['label=WZJJ_QCD',                     'id=40016', 'doMCweights=True']]
samples['WW_DoubleScattering']           = ['/WW_DoubleScattering_13TeV-pythia8/RunIISpring15MiniAODv2-74X_mcRun2_asymptotic_v2-v1/MINIAODSIM',                              ['label=WW_DoubleScattering',          'id=40017', 'doMCweights=True']]


# ttDM >> Juan
samples['ttDM0001scalar00010'] = ['/TTbarDMJets_scalar_Mchi-1_Mphi-10_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIIFall15MiniAODv2-PU25nsData2015v1_76X_mcRun2_asymptotic_v12_ext1-v1/MINIAODSIM', ['label=ttDM0001scalar00010', 'id=90025', 'doLHE=False']]
samples['ttDM0001scalar00020'] = ['/TTbarDMJets_scalar_Mchi-1_Mphi-20_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIIFall15MiniAODv2-PU25nsData2015v1_76X_mcRun2_asymptotic_v12_ext1-v1/MINIAODSIM', ['label=ttDM0001scalar00020', 'id=90026', 'doLHE=False']]
samples['ttDM0001scalar00050'] = ['/TTbarDMJets_scalar_Mchi-1_Mphi-50_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIIFall15MiniAODv2-PU25nsData2015v1_76X_mcRun2_asymptotic_v12_ext1-v1/MINIAODSIM', ['label=ttDM0001scalar00050', 'id=90027', 'doLHE=False']]
samples['ttDM0001scalar00100'] = ['/TTbarDMJets_scalar_Mchi-1_Mphi-100_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIIFall15MiniAODv2-PU25nsData2015v1_76X_mcRun2_asymptotic_v12_ext1-v1/MINIAODSIM', ['label=ttDM0001scalar00100', 'id=90028', 'doLHE=False']]
samples['ttDM0001scalar00200'] = ['/TTbarDMJets_scalar_Mchi-1_Mphi-200_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIIFall15MiniAODv2-PU25nsData2015v1_76X_mcRun2_asymptotic_v12_ext1-v1/MINIAODSIM', ['label=ttDM0001scalar00200', 'id=90029', 'doLHE=False']]
samples['ttDM0001scalar00300'] = ['/TTbarDMJets_scalar_Mchi-1_Mphi-300_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIIFall15MiniAODv2-PU25nsData2015v1_76X_mcRun2_asymptotic_v12_ext1-v1/MINIAODSIM', ['label=ttDM0001scalar00300', 'id=90030', 'doLHE=False']]
samples['ttDM0001scalar00500'] = ['/TTbarDMJets_scalar_Mchi-1_Mphi-500_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIIFall15MiniAODv2-PU25nsData2015v1_76X_mcRun2_asymptotic_v12_ext1-v1/MINIAODSIM', ['label=ttDM0001scalar00500', 'id=90031', 'doLHE=False']]
samples['ttDM0001scalar01000'] = ['/TTbarDMJets_scalar_Mchi-1_Mphi-1000_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIIFall15MiniAODv2-PU25nsData2015v1_76X_mcRun2_asymptotic_v12_ext1-v1/MINIAODSIM', ['label=ttDM0001scalar01000', 'id=90032', 'doLHE=False']]
samples['ttDM0001scalar10000'] = ['/TTbarDMJets_scalar_Mchi-1_Mphi-10000_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIIFall15MiniAODv2-PU25nsData2015v1_76X_mcRun2_asymptotic_v12_ext1-v1/MINIAODSIM', ['label=ttDM0001scalar10000', 'id=90034', 'doLHE=False']]
samples['ttDM0010scalar00010'] = ['/TTbarDMJets_scalar_Mchi-10_Mphi-10_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIIFall15MiniAODv2-PU25nsData2015v1_76X_mcRun2_asymptotic_v12_ext1-v1/MINIAODSIM', ['label=ttDM0010scalar00010', 'id=90035', 'doLHE=False']]
samples['ttDM0010scalar00015'] = ['/TTbarDMJets_scalar_Mchi-10_Mphi-15_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIIFall15MiniAODv2-PU25nsData2015v1_76X_mcRun2_asymptotic_v12_ext1-v1/MINIAODSIM', ['label=ttDM0010scalar00015', 'id=90036', 'doLHE=False']]
samples['ttDM0010scalar00050'] = ['/TTbarDMJets_scalar_Mchi-10_Mphi-50_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIIFall15MiniAODv2-PU25nsData2015v1_76X_mcRun2_asymptotic_v12_ext1-v1/MINIAODSIM', ['label=ttDM0010scalar00050', 'id=90037', 'doLHE=False']]
samples['ttDM0010scalar00100'] = ['/TTbarDMJets_scalar_Mchi-10_Mphi-100_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIIFall15MiniAODv2-PU25nsData2015v1_76X_mcRun2_asymptotic_v12_ext1-v1/MINIAODSIM', ['label=ttDM0010scalar00100', 'id=90038', 'doLHE=False']]
samples['ttDM0010scalar10000'] = ['/TTbarDMJets_scalar_Mchi-10_Mphi-10000_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIIFall15MiniAODv2-PU25nsData2015v1_76X_mcRun2_asymptotic_v12_ext1-v1/MINIAODSIM', ['label=ttDM0010scalar10000', 'id=90039', 'doLHE=False']]
samples['ttDM0050scalar00010'] = ['/TTbarDMJets_scalar_Mchi-50_Mphi-10_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIIFall15MiniAODv2-PU25nsData2015v1_76X_mcRun2_asymptotic_v12_ext1-v1/MINIAODSIM', ['label=ttDM0050scalar00010', 'id=90040', 'doLHE=False']]
samples['ttDM0050scalar00050'] = ['/TTbarDMJets_scalar_Mchi-50_Mphi-50_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIIFall15MiniAODv2-PU25nsData2015v1_76X_mcRun2_asymptotic_v12_ext1-v1/MINIAODSIM', ['label=ttDM0050scalar00050', 'id=90041', 'doLHE=False']]
samples['ttDM0050scalar00095'] = ['/TTbarDMJets_scalar_Mchi-50_Mphi-95_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIIFall15MiniAODv2-PU25nsData2015v1_76X_mcRun2_asymptotic_v12_ext1-v1/MINIAODSIM', ['label=ttDM0050scalar00095', 'id=90042', 'doLHE=False']]
samples['ttDM0050scalar00200'] = ['/TTbarDMJets_scalar_Mchi-50_Mphi-200_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIIFall15MiniAODv2-PU25nsData2015v1_76X_mcRun2_asymptotic_v12_ext1-v1/MINIAODSIM', ['label=ttDM0050scalar00200', 'id=90043', 'doLHE=False']]
samples['ttDM0050scalar00300'] = ['/TTbarDMJets_scalar_Mchi-50_Mphi-300_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIIFall15MiniAODv2-PU25nsData2015v1_76X_mcRun2_asymptotic_v12_ext1-v1/MINIAODSIM', ['label=ttDM0050scalar00300', 'id=90044', 'doLHE=False']]
samples['ttDM0050scalar10000'] = ['/TTbarDMJets_scalar_Mchi-50_Mphi-10000_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIIFall15MiniAODv2-PU25nsData2015v1_76X_mcRun2_asymptotic_v12_ext1-v1/MINIAODSIM', ['label=ttDM0050scalar10000', 'id=90045', 'doLHE=False']]
samples['ttDM0150scalar00010'] = ['/TTbarDMJets_scalar_Mchi-150_Mphi-10_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIIFall15MiniAODv2-PU25nsData2015v1_76X_mcRun2_asymptotic_v12_ext1-v1/MINIAODSIM', ['label=ttDM0150scalar00010', 'id=90046', 'doLHE=False']]
samples['ttDM0150scalar00200'] = ['/TTbarDMJets_scalar_Mchi-150_Mphi-200_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIIFall15MiniAODv2-PU25nsData2015v1_76X_mcRun2_asymptotic_v12_ext1-v1/MINIAODSIM', ['label=ttDM0150scalar00200', 'id=90047', 'doLHE=False']]
samples['ttDM0150scalar00295'] = ['/TTbarDMJets_scalar_Mchi-150_Mphi-295_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIIFall15MiniAODv2-PU25nsData2015v1_76X_mcRun2_asymptotic_v12_ext1-v1/MINIAODSIM', ['label=ttDM0150scalar00295', 'id=90048', 'doLHE=False']]
samples['ttDM0150scalar00500'] = ['/TTbarDMJets_scalar_Mchi-150_Mphi-500_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIIFall15MiniAODv2-PU25nsData2015v1_76X_mcRun2_asymptotic_v12_ext1-v1/MINIAODSIM', ['label=ttDM0150scalar00500', 'id=90049', 'doLHE=False']]
samples['ttDM0150scalar01000'] = ['/TTbarDMJets_scalar_Mchi-150_Mphi-1000_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIIFall15MiniAODv2-PU25nsData2015v1_76X_mcRun2_asymptotic_v12_ext1-v1/MINIAODSIM', ['label=ttDM0150scalar01000', 'id=90050', 'doLHE=False']]
samples['ttDM0150scalar10000'] = ['/TTbarDMJets_scalar_Mchi-150_Mphi-10000_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIIFall15MiniAODv2-PU25nsData2015v1_76X_mcRun2_asymptotic_v12_ext1-v1/MINIAODSIM', ['label=ttDM0150scalar10000', 'id=90051', 'doLHE=False']]
samples['ttDM0500scalar00010'] = ['/TTbarDMJets_scalar_Mchi-500_Mphi-10_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIIFall15MiniAODv2-PU25nsData2015v1_76X_mcRun2_asymptotic_v12_ext1-v1/MINIAODSIM', ['label=ttDM0500scalar00010', 'id=90052', 'doLHE=False']]
samples['ttDM0500scalar00500'] = ['/TTbarDMJets_scalar_Mchi-500_Mphi-500_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIIFall15MiniAODv2-PU25nsData2015v1_76X_mcRun2_asymptotic_v12_ext1-v1/MINIAODSIM', ['label=ttDM0500scalar00500', 'id=90053', 'doLHE=False']]
samples['ttDM0500scalar00995'] = ['/TTbarDMJets_scalar_Mchi-500_Mphi-995_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIIFall15MiniAODv2-PU25nsData2015v1_76X_mcRun2_asymptotic_v12_ext1-v1/MINIAODSIM', ['label=ttDM0500scalar00995', 'id=90054', 'doLHE=False']]
samples['ttDM0500scalar10000'] = ['/TTbarDMJets_scalar_Mchi-500_Mphi-10000_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIIFall15MiniAODv2-PU25nsData2015v1_76X_mcRun2_asymptotic_v12_ext1-v1/MINIAODSIM', ['label=ttDM0500scalar10000', 'id=90056', 'doLHE=False']]
samples['ttDM1000scalar00010'] = ['/TTbarDMJets_scalar_Mchi-1000_Mphi-10_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIIFall15MiniAODv2-PU25nsData2015v1_76X_mcRun2_asymptotic_v12_ext1-v1/MINIAODSIM', ['label=ttDM1000scalar00010', 'id=90057', 'doLHE=False']]
samples['ttDM1000scalar01000'] = ['/TTbarDMJets_scalar_Mchi-1000_Mphi-1000_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIIFall15MiniAODv2-PU25nsData2015v1_76X_mcRun2_asymptotic_v12_ext1-v1/MINIAODSIM', ['label=ttDM1000scalar01000', 'id=90058', 'doLHE=False']]
samples['ttDM1000scalar10000'] = ['/TTbarDMJets_scalar_Mchi-1000_Mphi-10000_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIIFall15MiniAODv2-PU25nsData2015v1_76X_mcRun2_asymptotic_v12_ext1-v1/MINIAODSIM', ['label=ttDM1000scalar10000', 'id=90060', 'doLHE=False']]


samples['ttDM0001pseudo00010'] = ['/TTbarDMJets_pseudoscalar_Mchi-1_Mphi-10_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIIFall15MiniAODv2-PU25nsData2015v1_76X_mcRun2_asymptotic_v12_ext1-v1/MINIAODSIM', ['label=ttDM0001pseudo00010', 'id=90061', 'doLHE=False']]
samples['ttDM0001pseudo00050'] = ['/TTbarDMJets_pseudoscalar_Mchi-1_Mphi-50_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIIFall15MiniAODv2-PU25nsData2015v1_76X_mcRun2_asymptotic_v12_ext1-v1/MINIAODSIM', ['label=ttDM0001pseudo00050', 'id=90063', 'doLHE=False']]
samples['ttDM0001pseudo00100'] = ['/TTbarDMJets_pseudoscalar_Mchi-1_Mphi-100_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIIFall15MiniAODv2-PU25nsData2015v1_76X_mcRun2_asymptotic_v12_ext1-v1/MINIAODSIM', ['label=ttDM0001pseudo00100', 'id=90064', 'doLHE=False']]
samples['ttDM0001pseudo00200'] = ['/TTbarDMJets_pseudoscalar_Mchi-1_Mphi-200_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIIFall15MiniAODv2-PU25nsData2015v1_76X_mcRun2_asymptotic_v12_ext1-v1/MINIAODSIM', ['label=ttDM0001pseudo00200', 'id=90065', 'doLHE=False']]
samples['ttDM0001pseudo00300'] = ['/TTbarDMJets_pseudoscalar_Mchi-1_Mphi-300_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIIFall15MiniAODv2-PU25nsData2015v1_76X_mcRun2_asymptotic_v12_ext1-v1/MINIAODSIM', ['label=ttDM0001pseudo00300', 'id=90066', 'doLHE=False']]
samples['ttDM0001pseudo01000'] = ['/TTbarDMJets_pseudoscalar_Mchi-1_Mphi-1000_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIIFall15MiniAODv2-PU25nsData2015v1_76X_mcRun2_asymptotic_v12_ext1-v1/MINIAODSIM', ['label=ttDM0001pseudo01000', 'id=90068', 'doLHE=False']]
samples['ttDM0001pseudo10000'] = ['/TTbarDMJets_pseudoscalar_Mchi-1_Mphi-10000_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIIFall15MiniAODv2-PU25nsData2015v1_76X_mcRun2_asymptotic_v12_ext1-v1/MINIAODSIM', ['label=ttDM0001pseudo10000', 'id=90070', 'doLHE=False']]
samples['ttDM0010pseudo00010'] = ['/TTbarDMJets_pseudoscalar_Mchi-10_Mphi-10_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIIFall15MiniAODv2-PU25nsData2015v1_76X_mcRun2_asymptotic_v12_ext1-v1/MINIAODSIM', ['label=ttDM0010pseudo00010', 'id=90071', 'doLHE=False']]
samples['ttDM0010pseudo00015'] = ['/TTbarDMJets_pseudoscalar_Mchi-10_Mphi-15_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIIFall15MiniAODv2-PU25nsData2015v1_76X_mcRun2_asymptotic_v12_ext1-v1/MINIAODSIM', ['label=ttDM0010pseudo00015', 'id=90072', 'doLHE=False']]
samples['ttDM0010pseudo00050'] = ['/TTbarDMJets_pseudoscalar_Mchi-10_Mphi-50_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIIFall15MiniAODv2-PU25nsData2015v1_76X_mcRun2_asymptotic_v12_ext1-v1/MINIAODSIM', ['label=ttDM0010pseudo00050', 'id=90073', 'doLHE=False']]
samples['ttDM0010pseudo00100'] = ['/TTbarDMJets_pseudoscalar_Mchi-10_Mphi-100_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIIFall15MiniAODv2-PU25nsData2015v1_76X_mcRun2_asymptotic_v12_ext1-v1/MINIAODSIM', ['label=ttDM0010pseudo00100', 'id=90074', 'doLHE=False']]
samples['ttDM0010pseudo10000'] = ['/TTbarDMJets_pseudoscalar_Mchi-10_Mphi-10000_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIIFall15MiniAODv2-PU25nsData2015v1_76X_mcRun2_asymptotic_v12_ext1-v1/MINIAODSIM', ['label=ttDM0010pseudo10000', 'id=90075', 'doLHE=False']]
samples['ttDM0050pseudo00010'] = ['/TTbarDMJets_pseudoscalar_Mchi-50_Mphi-10_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIIFall15MiniAODv2-PU25nsData2015v1_76X_mcRun2_asymptotic_v12_ext1-v1/MINIAODSIM', ['label=ttDM0050pseudo00010', 'id=90076', 'doLHE=False']]
samples['ttDM0050pseudo00050'] = ['/TTbarDMJets_pseudoscalar_Mchi-50_Mphi-50_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIIFall15MiniAODv2-PU25nsData2015v1_76X_mcRun2_asymptotic_v12_ext1-v1/MINIAODSIM', ['label=ttDM0050pseudo00050', 'id=90077', 'doLHE=False']]
samples['ttDM0050pseudo00095'] = ['/TTbarDMJets_pseudoscalar_Mchi-50_Mphi-95_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIIFall15MiniAODv2-PU25nsData2015v1_76X_mcRun2_asymptotic_v12_ext1-v1/MINIAODSIM', ['label=ttDM0050pseudo00095', 'id=90078', 'doLHE=False']]
samples['ttDM0050pseudo00200'] = ['/TTbarDMJets_pseudoscalar_Mchi-50_Mphi-200_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIIFall15MiniAODv2-PU25nsData2015v1_76X_mcRun2_asymptotic_v12_ext1-v1/MINIAODSIM', ['label=ttDM0050pseudo00200', 'id=90079', 'doLHE=False']]
samples['ttDM0050pseudo00300'] = ['/TTbarDMJets_pseudoscalar_Mchi-50_Mphi-300_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIIFall15MiniAODv2-PU25nsData2015v1_76X_mcRun2_asymptotic_v12_ext1-v1/MINIAODSIM', ['label=ttDM0050pseudo00300', 'id=90080', 'doLHE=False']]
samples['ttDM0050pseudo10000'] = ['/TTbarDMJets_pseudoscalar_Mchi-50_Mphi-10000_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIIFall15MiniAODv2-PU25nsData2015v1_76X_mcRun2_asymptotic_v12_ext1-v1/MINIAODSIM', ['label=ttDM0050pseudo10000', 'id=90081', 'doLHE=False']]
samples['ttDM0150pseudo00010'] = ['/TTbarDMJets_pseudoscalar_Mchi-150_Mphi-10_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIIFall15MiniAODv2-PU25nsData2015v1_76X_mcRun2_asymptotic_v12_ext1-v1/MINIAODSIM', ['label=ttDM0150pseudo00010', 'id=90082', 'doLHE=False']]
samples['ttDM0150pseudo00200'] = ['/TTbarDMJets_pseudoscalar_Mchi-150_Mphi-200_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIIFall15MiniAODv2-PU25nsData2015v1_76X_mcRun2_asymptotic_v12_ext1-v1/MINIAODSIM', ['label=ttDM0150pseudo00200', 'id=90083', 'doLHE=False']]
samples['ttDM0150pseudo00295'] = ['/TTbarDMJets_pseudoscalar_Mchi-150_Mphi-295_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIIFall15MiniAODv2-PU25nsData2015v1_76X_mcRun2_asymptotic_v12_ext1-v1/MINIAODSIM', ['label=ttDM0150pseudo00295', 'id=90084', 'doLHE=False']]
samples['ttDM0150pseudo00500'] = ['/TTbarDMJets_pseudoscalar_Mchi-150_Mphi-500_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIIFall15MiniAODv2-PU25nsData2015v1_76X_mcRun2_asymptotic_v12_ext1-v1/MINIAODSIM', ['label=ttDM0150pseudo00500', 'id=90085', 'doLHE=False']]
samples['ttDM0150pseudo01000'] = ['/TTbarDMJets_pseudoscalar_Mchi-150_Mphi-1000_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIIFall15MiniAODv2-PU25nsData2015v1_76X_mcRun2_asymptotic_v12_ext1-v1/MINIAODSIM', ['label=ttDM0150pseudo01000', 'id=90086', 'doLHE=False']]
samples['ttDM0150pseudo10000'] = ['/TTbarDMJets_pseudoscalar_Mchi-150_Mphi-10000_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIIFall15MiniAODv2-PU25nsData2015v1_76X_mcRun2_asymptotic_v12_ext1-v1/MINIAODSIM', ['label=ttDM0150pseudo10000', 'id=90087', 'doLHE=False']]
samples['ttDM0500pseudo00010'] = ['/TTbarDMJets_pseudoscalar_Mchi-500_Mphi-10_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIIFall15MiniAODv2-PU25nsData2015v1_76X_mcRun2_asymptotic_v12_ext1-v1/MINIAODSIM', ['label=ttDM0500pseudo00010', 'id=90088', 'doLHE=False']]
samples['ttDM0500pseudo00500'] = ['/TTbarDMJets_pseudoscalar_Mchi-500_Mphi-500_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIIFall15MiniAODv2-PU25nsData2015v1_76X_mcRun2_asymptotic_v12_ext1-v1/MINIAODSIM', ['label=ttDM0500pseudo00500', 'id=90089', 'doLHE=False']]
samples['ttDM0500pseudo00995'] = ['/TTbarDMJets_pseudoscalar_Mchi-500_Mphi-995_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIIFall15MiniAODv2-PU25nsData2015v1_76X_mcRun2_asymptotic_v12_ext1-v1/MINIAODSIM', ['label=ttDM0500pseudo00995', 'id=90090', 'doLHE=False']]
samples['ttDM0500pseudo10000'] = ['/TTbarDMJets_pseudoscalar_Mchi-500_Mphi-10000_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIIFall15MiniAODv2-PU25nsData2015v1_76X_mcRun2_asymptotic_v12_ext1-v1/MINIAODSIM', ['label=ttDM0500pseudo10000', 'id=90092', 'doLHE=False']]
samples['ttDM1000pseudo00010'] = ['/TTbarDMJets_pseudoscalar_Mchi-1000_Mphi-10_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIIFall15MiniAODv2-PU25nsData2015v1_76X_mcRun2_asymptotic_v12_ext1-v1/MINIAODSIM', ['label=ttDM1000pseudo00010', 'id=90093', 'doLHE=False']]
samples['ttDM1000pseudo01000'] = ['/TTbarDMJets_pseudoscalar_Mchi-1000_Mphi-1000_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIIFall15MiniAODv2-PU25nsData2015v1_76X_mcRun2_asymptotic_v12_ext1-v1/MINIAODSIM', ['label=ttDM1000pseudo01000', 'id=90094', 'doLHE=False']]
samples['ttDM1000pseudo10000'] = ['/TTbarDMJets_pseudoscalar_Mchi-1000_Mphi-10000_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIIFall15MiniAODv2-PU25nsData2015v1_76X_mcRun2_asymptotic_v12_ext1-v1/MINIAODSIM', ['label=ttDM1000pseudo10000', 'id=90096', 'doLHE=False']]



# monoHiggs >> Nicolo'
samples['monoH_2HDM_MZp-600_MA0-300']  = ['/ZprimeToA0hToA0chichihWWTollnunu_2HDM_MZp-600_MA0-300_13TeV-madgraph/RunIISpring15MiniAODv2-74X_mcRun2_asymptotic_v2-v1/MINIAODSIM',  ['label=monoH_2HDM_MZp-600_MA0-300',  'id=80001', 'doLHE=False']]
samples['monoH_2HDM_MZp-800_MA0-300']  = ['/ZprimeToA0hToA0chichihWWTollnunu_2HDM_MZp-800_MA0-300_13TeV-madgraph/RunIISpring15MiniAODv2-74X_mcRun2_asymptotic_v2-v1/MINIAODSIM',  ['label=monoH_2HDM_MZp-800_MA0-300',  'id=80002', 'doLHE=False']]
samples['monoH_2HDM_MZp-1000_MA0-300'] = ['/ZprimeToA0hToA0chichihWWTollnunu_2HDM_MZp-1000_MA0-300_13TeV-madgraph/RunIISpring15MiniAODv2-74X_mcRun2_asymptotic_v2-v1/MINIAODSIM', ['label=monoH_2HDM_MZp-1000_MA0-300', 'id=80003', 'doLHE=False']]
samples['monoH_2HDM_MZp-1200_MA0-300'] = ['/ZprimeToA0hToA0chichihWWTollnunu_2HDM_MZp-1200_MA0-300_13TeV-madgraph/RunIISpring15MiniAODv2-74X_mcRun2_asymptotic_v2-v1/MINIAODSIM', ['label=monoH_2HDM_MZp-1200_MA0-300', 'id=80004', 'doLHE=False']]
samples['monoH_2HDM_MZp-1400_MA0-300'] = ['/ZprimeToA0hToA0chichihWWTollnunu_2HDM_MZp-1400_MA0-300_13TeV-madgraph/RunIISpring15MiniAODv2-74X_mcRun2_asymptotic_v2-v1/MINIAODSIM', ['label=monoH_2HDM_MZp-1400_MA0-300', 'id=80005', 'doLHE=False']]
samples['monoH_2HDM_MZp-1700_MA0-300'] = ['/ZprimeToA0hToA0chichihWWTollnunu_2HDM_MZp-1700_MA0-300_13TeV-madgraph/RunIISpring15MiniAODv2-74X_mcRun2_asymptotic_v2-v1/MINIAODSIM', ['label=monoH_2HDM_MZp-1700_MA0-300', 'id=80006', 'doLHE=False']]
samples['monoH_2HDM_MZp-2000_MA0-300'] = ['/ZprimeToA0hToA0chichihWWTollnunu_2HDM_MZp-2000_MA0-300_13TeV-madgraph/RunIISpring15MiniAODv2-74X_mcRun2_asymptotic_v2-v1/MINIAODSIM', ['label=monoH_2HDM_MZp-2000_MA0-300', 'id=80007', 'doLHE=False']]
samples['monoH_2HDM_MZp-2500_MA0-300'] = ['/ZprimeToA0hToA0chichihWWTollnunu_2HDM_MZp-2500_MA0-300_13TeV-madgraph/RunIISpring15MiniAODv2-74X_mcRun2_asymptotic_v2-v1/MINIAODSIM', ['label=monoH_2HDM_MZp-2500_MA0-300', 'id=80008', 'doLHE=False']]


########
# Additional global configuration
########
config.Data.outLFNDirBase = '/store/group/phys_higgs/cmshww/amassiro/RunII/21Oct/MC/25ns/'
pyCfgParams.append('globalTag=74X_mcRun2_asymptotic_v4')
