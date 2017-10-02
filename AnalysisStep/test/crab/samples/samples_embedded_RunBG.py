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

samples['DY_EmbeddingRun2016B_emu'] = ['/EmbeddingRun2016B/ElMuFinalState-imputSep16DoubleMu_mirror_miniAOD-v2/USER', ['label=DY_EmbeddingRun2016B_emu', 'id=80600', 'doTauEmbed=True']]
samples['DY_EmbeddingRun2016C_emu'] = ['/EmbeddingRun2016C/ElMuFinalState-imputSep16DoubleMu_mirror_miniAOD-v2/USER', ['label=DY_EmbeddingRun2016C_emu', 'id=80601', 'doTauEmbed=True']]
samples['DY_EmbeddingRun2016D_emu'] = ['/EmbeddingRun2016D/ElMuFinalState-imputSep16DoubleMu_mirror_miniAOD-v2/USER', ['label=DY_EmbeddingRun2016D_emu', 'id=80602', 'doTauEmbed=True']]
samples['DY_EmbeddingRun2016E_emu'] = ['/EmbeddingRun2016E/ElMuFinalState-imputSep16DoubleMu_mirror_miniAOD-v2/USER', ['label=DY_EmbeddingRun2016E_emu', 'id=80603', 'doTauEmbed=True']]
samples['DY_EmbeddingRun2016F_emu'] = ['/EmbeddingRun2016F/ElMuFinalState-imputSep16DoubleMu_mirror_miniAOD-v2/USER', ['label=DY_EmbeddingRun2016F_emu', 'id=80604', 'doTauEmbed=True']]
samples['DY_EmbeddingRun2016G_emu'] = ['/EmbeddingRun2016G/ElMuFinalState-imputSep16DoubleMu_mirror_miniAOD-v2/USER', ['label=DY_EmbeddingRun2016G_emu', 'id=80605', 'doTauEmbed=True']]



##~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
##~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~


config.Data.outLFNDirBase = '/store/group/phys_higgs/cmshww/amassiro/RunII/2016/Apr2017/MC/'
# this name is used partially in the post-processing.
# Let's try not to change it for a given production era

pyCfgParams.append('globalTag=80X_mcRun2_asymptotic_2016_TrancheIV_v8')
     

