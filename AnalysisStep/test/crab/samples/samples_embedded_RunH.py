#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#
# Look for samples via:
#
# https://cmsweb.cern.ch/das/request?view=list&limit=50&instance=prod%2Fphys03&input=dataset%3D%2FEmbeddingRun2016*%2FElMuFinalState-imputSep16DoubleMu_mirror_miniAOD-v2%2FUSER
#
#
# For GT and more, see https://twiki.cern.ch/twiki/bin/viewauth/CMS/TauTauEmbeddingSamples2016
#
#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~


# DY -> tau tau -> e mu embedded samples

samples['DY_EmbeddingRun2016H_emu'] = ['/EmbeddingRun2016H/ElMuFinalState-imputSep16DoubleMu_mirror_miniAOD-v2/USER', ['label=DY_EmbeddingRun2016H_emu', 'id=80606', 'doTauEmbed=True']]



##~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
##~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~


config.Data.outLFNDirBase = '/store/group/phys_higgs/cmshww/amassiro/RunII/2016/Apr2017/MC/'
# this name is used partially in the post-processing.
# Let's try not to change it for a given production era

pyCfgParams.append('globalTag=80X_dataRun2_Prompt_v16')
     

