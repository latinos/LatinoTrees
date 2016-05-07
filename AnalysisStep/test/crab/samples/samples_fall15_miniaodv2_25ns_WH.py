#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
# Arun
#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
## HW
samples['HWplusJ_HToWW_M125']      = ['/HWplusJ_HToWWTo2L2Nu_WToLNu_M125_13TeV_powheg_pythia8/RunIIFall15MiniAODv2-PU25nsData2015v1_76X_mcRun2_asymptotic_v12-v1/MINIAODSIM',  ['label=HWplusJ_HToWW_M125',  'id=03125', 'doMCweights=True']]

samples['HWminusJ_HToWW_M125']     = ['/HWminusJ_HToWWTo2L2Nu_WToLNu_M125_13TeV_powheg_pythia8/RunIIFall15MiniAODv2-PU25nsData2015v1_76X_mcRun2_asymptotic_v12-v1/MINIAODSIM', ['label=HWminusJ_HToWW_M125', 'id=02125', 'doMCweights=True']]

## HZ
samples['HZJ_HToWW_M125']     = ['/HZJ_HToWWTo2L2Nu_ZTo2L_M125_13TeV_powheg_pythia8/RunIIFall15MiniAODv2-PU25nsData2015v1_76X_mcRun2_asymptotic_v12-v1/MINIAODSIM', ['label=HZJ_HToWW_M125', 'id=06125', 'doMCweights=True']]

########
# Additional global configuration
########
config.Data.outLFNDirBase = '/store/group/phys_higgs/cmshww/arun/latino_VH_Fall15/'
pyCfgParams.append('globalTag=76X_mcRun2_asymptotic_v12')

