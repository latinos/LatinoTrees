#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#
# Look for samples via:
#
#    https://cmsweb.cern.ch/das/request?view=list&limit=50&instance=prod%2Fglobal&input=dataset%3D%2F*%2FRunIISpring16MiniAODv2-PUSpring16_80X_mcRun2_asymptotic_2016_*%2FMINIAODSIM
#    https://cmsweb.cern.ch/das/request?view=plain&limit=150&instance=prod%2Fglobal&input=dataset%3D%2F*%2FRunIISpring16MiniAODv2-PUSpring16_80X_mcRun2_asymptotic_2016_*%2FMINIAODSIM
#    https://cmsweb.cern.ch/das/request?view=plain&limit=150&instance=prod%2Fglobal&input=dataset%3D%2F*%2FRunIISpring16MiniAODv2-PUSpring16_80X_mcRun2_asymptotic_2016_*%2FMINIAODSIM
#
# And for signals:
#   https://cmsweb.cern.ch/das/request?view=plain&limit=150&instance=prod%2Fglobal&input=dataset%3D%2F*%2FRunIISpring16MiniAODv2*mcRun2_asymptotic_2016_*%2FMINIAODSIM       
#
# Only 125:
#   https://cmsweb.cern.ch/das/request?view=plain&limit=150&instance=prod%2Fglobal&input=dataset%3D%2FGluGlu*125*%2FRunIISpring16MiniAODv2*mcRun2_asymptotic_2016_*%2FMINIAODSIM
#
# For GT and more, see https://twiki.cern.ch/twiki/bin/view/CMSPublic/WorkBookMiniAOD
#
#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~



#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
# Author: Davide Valsecchi
#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

## VBS semileptonic signal                    

samples["WpToLNu_WmTo2J_QCD"] = ['/WplusToLNuWminusTo2JJJ_QCD_LO_SM_MJJ100PTJ10_TuneCUETP8M1_13TeV-madgraph-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM', ['label=WpToLNu_WmTo2J_QCD', 'id=00001']]
samples["WpTo2J_WmToLNu_QCD"] = ['/WplusTo2JWminusToLNuJJ_QCD_LO_SM_MJJ100PTJ10_TuneCUETP8M1_13TeV-madgraph-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM', ['label=WpTo2J_WmToLNu_QCD', 'id=00002']]

samples["WpToLNu_WpTo2J_QCD"] = ['/WplusToLNuWplusTo2JJJ_QCD_LO_SM_MJJ100PTJ10_TuneCUETP8M1_13TeV-madgraph-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM', ['label=WpToLNu_WpTo2J_QCD', 'id=00003']]
samples["WmToLNu_WmTo2J_QCD"] = ['/WminusToLNuWminusTo2JJJ_QCD_LO_SM_MJJ100PTJ10_TuneCUETP8M1_13TeV-madgraph-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM', ['label=WmToLNu_WmTo2J_QCD', 'id=00004']]

samples["WpToLNu_ZTo2J_QCD"] = ['/WplusToLNuZTo2JJJ_QCD_LO_SM_MJJ100PTJ10_TuneCUETP8M1_13TeV-madgraph-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM', ['label=WpToLNu_ZTo2J_QCD', 'id=00005']]
samples["WpTo2J_ZTo2L_QCD"] = ['/WplusTo2JZTo2LJJ_QCD_LO_SM_MJJ100PTJ10_TuneCUETP8M1_13TeV-madgraph-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM', ['label=WpTo2J_ZTo2L_QCD', 'id=00006']] 
samples["WmToLNu_ZTo2J_QCD"] = ['/WminusToLNuZTo2JJJ_QCD_LO_SM_MJJ100PTJ10_TuneCUETP8M1_13TeV-madgraph-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM', ['label=WmToLNu_ZTo2J_QCD', 'id=00007']]
samples["WmTo2J_ZTo2L_QCD"] = ['/WminusTo2JZTo2LJJ_QCD_LO_SM_MJJ100PTJ10_TuneCUETP8M1_13TeV-madgraph-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM', ['label=WmTo2J_ZTo2L_QCD', 'id=00008']] 

samples["ZTo2L_ZTo2J_QCD"] = ['/ZTo2LZTo2JJJ_QCD_LO_SM_MJJ100PTJ10_TuneCUETP8M1_13TeV-madgraph-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM', ['label=ZTo2L_ZTo2J_QCD', 'id=00009']] 

 # Additional global configuration
########

#config.Data.outLFNDirBase = '/store/group/phys_higgs/cmshww/amassiro/RunII/2016/Jun03/MC/v2/'
config.Data.outLFNDirBase = '/store/group/OneLepton/QCD_semileptonic_summer16'
# this name is used partially in the post-processing.
# Let's try not to change it for a given production era

pyCfgParams.append('globalTag=80X_mcRun2_asymptotic_2016_TrancheIV_v6')
pyCfgParams.append('doMCweights=True')
pyCfgParams.append('doLHE=True')

#config.Site.storageSite = 'T3_IT_MIB'


