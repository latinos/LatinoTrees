import FWCore.ParameterSet.Config as cms

#select collections
selectedElectronsBase = cms.EDFilter("PATElectronRefSelector",
    src = cms.InputTag("slimmedElectrons"),
    filter = cms.bool(False),
    cut = cms.string("pt > 5 && abs(eta)<2.5")
    )



ELE_BASE = "( pt > 8 && abs(eta)<2.5 )"
wwEleBase = selectedElectronsBase.clone( cut = ELE_BASE )




########################
# Phys14 recipe
# https://twiki.cern.ch/twiki/bin/view/CMS/CutBasedElectronIdentificationRun2
# https://twiki.cern.ch/twiki/bin/view/CMS/CutBasedElectronIdentificationRun2#PHYS14_selection_all_conditions



# LOOSE without isolation

ELE_ID_LOOSE_NO_ISO = ("  (( isEB "+ 
                           " && sigmaIetaIeta < 0.010331" +
                           " && hadronicOverEm < 0.093068" +
                           " && abs(deltaPhiSuperClusterTrackAtVtx) < 0.094739  " +
                           " && abs(deltaEtaSuperClusterTrackAtVtx) < 0.009277 " +
                           #" && abs(dB('PV2D')) < 0.035904  " +
                           #" && abs( sqrt( dB('PV3D')*dB('PV3D') - dB('PV2D')*dB('PV2D') ) ) < 0.075496 " +
                           " && abs(1./energy - 1/p) < 0.189968 " +
                           " && gsfTrack.isAvailable() && gsfTrack.hitPattern().numberOfLostHits(\'MISSING_INNER_HITS\') <= 1 "+
                           " && passConversionVeto() " +                           
                           #" && (abs(userFloat('convValueMapProd:dist')) > 0.02 || abs(userFloat('convValueMapProd:dcot')) > 0.02 )" +
                           " ) || " +
                   "( (!isEB) " +
                           " && sigmaIetaIeta < 0.031838" +
                           " && hadronicOverEm < 0.115754" +
                           " && abs(deltaPhiSuperClusterTrackAtVtx) < 0.149934  " +
                           " && abs(deltaEtaSuperClusterTrackAtVtx) < 0.009833  " +
                           #" && abs(dB('PV2D')) < 0.099266  " +
                           #" && abs( sqrt( dB('PV3D')*dB('PV3D') - dB('PV2D')*dB('PV2D') ) ) < 0.197897  " +
                           " && abs(1./energy - 1/p) < 0.140662 " +
                           " && gsfTrack.isAvailable() && gsfTrack.hitPattern().numberOfLostHits(\'MISSING_INNER_HITS\') <= 1 "+
                           " && passConversionVeto() " +                           
                           #" && (abs(userFloat('convValueMapProd:dist')) > 0.02 || abs(userFloat('convValueMapProd:dcot')) > 0.02 )" +
                           " ) " +
                    ")")

wwEleLooseNoIso       = selectedElectronsBase.clone( cut = ELE_BASE + " && " + ELE_ID_LOOSE_NO_ISO )



# LOOSE

ELE_ID_LOOSE = ("  (( isEB "+ 
                           " && sigmaIetaIeta < 0.010331" +
                           " && hadronicOverEm < 0.093068" +
                           " && abs(deltaPhiSuperClusterTrackAtVtx) < 0.094739  " +
                           " && abs(deltaEtaSuperClusterTrackAtVtx) < 0.009277 " +
                           #" && abs(dB('PV2D')) < 0.035904  " +
                           #" && abs( sqrt( dB('PV3D')*dB('PV3D') - dB('PV2D')*dB('PV2D') ) ) < 0.075496 " +
                           " && abs(1./energy - 1/p) < 0.189968 " +
                           #" && ( " +
                               #"(abs(superCluster.eta) >= 0.000 && abs(superCluster.eta) < 0.800 && (pfIsolationVariables().sumChargedHadronPt+ max(0.,pfIsolationVariables().sumNeutralHadronEt+pfIsolationVariables().sumPhotonEt - 0.1013 * rho))/pt < 0.130136 )|| "+
                               #"(abs(superCluster.eta) >= 0.8 && abs(superCluster.eta) < 1.3 && (pfIsolationVariables().sumChargedHadronPt+ max(0.,pfIsolationVariables().sumNeutralHadronEt+pfIsolationVariables().sumPhotonEt - 0.0988 * rho))/pt < 0.130136 )|| " +
                               #"(abs(superCluster.eta) >= 1.3 && abs(superCluster.eta) < 2.0 && (pfIsolationVariables().sumChargedHadronPt+ max(0.,pfIsolationVariables().sumNeutralHadronEt+pfIsolationVariables().sumPhotonEt - 0.0572 * rho))/pt < 0.130136 )|| " +
                               #"(abs(superCluster.eta) >= 2.0 && abs(superCluster.eta) < 2.2 && (pfIsolationVariables().sumChargedHadronPt+ max(0.,pfIsolationVariables().sumNeutralHadronEt+pfIsolationVariables().sumPhotonEt - 0.0842 * rho))/pt < 0.130136 )|| " +
                               #"(abs(superCluster.eta) >= 2.2 && abs(superCluster.eta) < 2.5 && (pfIsolationVariables().sumChargedHadronPt+ max(0.,pfIsolationVariables().sumNeutralHadronEt+pfIsolationVariables().sumPhotonEt - 0.1530 * rho))/pt < 0.130136 )   "+ 
                               #") " +
                           " && gsfTrack.isAvailable() && gsfTrack.hitPattern().numberOfLostHits(\'MISSING_INNER_HITS\') <= 1 "+
                           " && (abs(userFloat('convValueMapProd:dist')) > 0.02 || abs(userFloat('convValueMapProd:dcot')) > 0.02 )" +
                           " ) || " +
                   "( (!isEB) " +
                           " && sigmaIetaIeta < 0.031838" +
                           " && hadronicOverEm < 0.115754" +
                           " && abs(deltaPhiSuperClusterTrackAtVtx) < 0.149934  " +
                           " && abs(deltaEtaSuperClusterTrackAtVtx) < 0.009833  " +
                           #" && abs(dB('PV2D')) < 0.099266  " +
                           #" && abs( sqrt( dB('PV3D')*dB('PV3D') - dB('PV2D')*dB('PV2D') ) ) < 0.197897  " +
                           " && abs(1./energy - 1/p) < 0.140662 " +
                           #" && ( " +
                               #"(abs(superCluster.eta) >= 0.000 && abs(superCluster.eta) < 0.800 && (pfIsolationVariables().sumChargedHadronPt+ max(0.,pfIsolationVariables().sumNeutralHadronEt+pfIsolationVariables().sumPhotonEt - 0.1013 * rho))/pt < 0.130136 )|| "+
                               #"(abs(superCluster.eta) >= 0.8 && abs(superCluster.eta) < 1.3 && (pfIsolationVariables().sumChargedHadronPt+ max(0.,pfIsolationVariables().sumNeutralHadronEt+pfIsolationVariables().sumPhotonEt - 0.0988 * rho))/pt < 0.130136 )|| " +
                               #"(abs(superCluster.eta) >= 1.3 && abs(superCluster.eta) < 2.0 && (pfIsolationVariables().sumChargedHadronPt+ max(0.,pfIsolationVariables().sumNeutralHadronEt+pfIsolationVariables().sumPhotonEt - 0.0572 * rho))/pt < 0.130136 )|| " +
                               #"(abs(superCluster.eta) >= 2.0 && abs(superCluster.eta) < 2.2 && (pfIsolationVariables().sumChargedHadronPt+ max(0.,pfIsolationVariables().sumNeutralHadronEt+pfIsolationVariables().sumPhotonEt - 0.0842 * rho))/pt < 0.130136 )|| " +
                               #"(abs(superCluster.eta) >= 2.2 && abs(superCluster.eta) < 2.5 && (pfIsolationVariables().sumChargedHadronPt+ max(0.,pfIsolationVariables().sumNeutralHadronEt+pfIsolationVariables().sumPhotonEt - 0.1530 * rho))/pt < 0.130136 )   "+ 
                               #") " +
                           " && gsfTrack.isAvailable() && gsfTrack.hitPattern().numberOfLostHits(\'MISSING_INNER_HITS\') <= 1 "+
                           " && (abs(userFloat('convValueMapProd:dist')) > 0.02 || abs(userFloat('convValueMapProd:dcot')) > 0.02 )" +
                           " ) " +
                    ")")


# Medium
ELE_ID_MEDIUM = ("  (( isEB "+ 
                 " && sigmaIetaIeta < 0.009996" +
                 " && hadronicOverEm < 0.050537" +
                 " && abs(deltaPhiSuperClusterTrackAtVtx) < 0.035973  " +
                 " && abs(deltaEtaSuperClusterTrackAtVtx) < 0.008925 " +
                 #" && abs(dB('PV2D')) < 0.012235  " +
                 #" && abs( sqrt( dB('PV3D')*dB('PV3D') - dB('PV2D')*dB('PV2D') ) ) < 0.042020  " +
                 " && abs(1./energy - 1/p) < 0.091942 " +
                 #" && ( "+
                  #"(abs(superCluster.eta) >= 0.000 && abs(superCluster.eta) < 0.800 && (pfIsolationVariables().sumChargedHadronPt+ max(0.,pfIsolationVariables().sumNeutralHadronEt+pfIsolationVariables().sumPhotonEt - 0.1013 * rho))/pt < 0.107587 )|| "+
                  #"(abs(superCluster.eta) >= 0.8 && abs(superCluster.eta) < 1.3 && (pfIsolationVariables().sumChargedHadronPt+ max(0.,pfIsolationVariables().sumNeutralHadronEt+pfIsolationVariables().sumPhotonEt - 0.0988 * rho))/pt < 0.107587 )|| " +
                  #"(abs(superCluster.eta) >= 1.3 && abs(superCluster.eta) < 2.0 && (pfIsolationVariables().sumChargedHadronPt+ max(0.,pfIsolationVariables().sumNeutralHadronEt+pfIsolationVariables().sumPhotonEt - 0.0572 * rho))/pt < 0.107587 )|| " +
                  #"(abs(superCluster.eta) >= 2.0 && abs(superCluster.eta) < 2.2 && (pfIsolationVariables().sumChargedHadronPt+ max(0.,pfIsolationVariables().sumNeutralHadronEt+pfIsolationVariables().sumPhotonEt - 0.0842 * rho))/pt < 0.107587 )|| " +
                  #"(abs(superCluster.eta) >= 2.2 && abs(superCluster.eta) < 2.5 && (pfIsolationVariables().sumChargedHadronPt+ max(0.,pfIsolationVariables().sumNeutralHadronEt+pfIsolationVariables().sumPhotonEt - 0.1530 * rho))/pt < 0.107587 )" +
                  #")"+ 
                 " && gsfTrack.isAvailable() && gsfTrack.hitPattern().numberOfLostHits(\'MISSING_INNER_HITS\') <= 1 "+
                 " && (abs(userFloat('convValueMapProd:dist')) > 0.02 || abs(userFloat('convValueMapProd:dcot')) > 0.02 )" +
                 " ) ||" +
                 
                 "( (!isEB) " +
                 " && sigmaIetaIeta < 0.030135" +
                 " && hadronicOverEm < 0.086782" +
                 " && abs(deltaPhiSuperClusterTrackAtVtx) < 0.067879  " +
                 " && abs(deltaEtaSuperClusterTrackAtVtx) < 0.007429  " +
                 #" && abs(dB('PV2D')) < 0.036719  " +
                 #" && abs( sqrt( dB('PV3D')*dB('PV3D') - dB('PV2D')*dB('PV2D') ) ) < 0.138142  " +
                 " && abs(1./energy - 1/p) < 0.100683 " +
                 #" && ( " +
                  #"(abs(superCluster.eta) >= 0.000 && abs(superCluster.eta) < 0.800 && (pfIsolationVariables().sumChargedHadronPt+ max(0.,pfIsolationVariables().sumNeutralHadronEt+pfIsolationVariables().sumPhotonEt - 0.1013 * rho))/pt < 0.113254 )|| "+
                  #"(abs(superCluster.eta) >= 0.8 && abs(superCluster.eta) < 1.3 && (pfIsolationVariables().sumChargedHadronPt+ max(0.,pfIsolationVariables().sumNeutralHadronEt+pfIsolationVariables().sumPhotonEt - 0.0988 * rho))/pt < 0.113254 )|| " +
                  #"(abs(superCluster.eta) >= 1.3 && abs(superCluster.eta) < 2.0 && (pfIsolationVariables().sumChargedHadronPt+ max(0.,pfIsolationVariables().sumNeutralHadronEt+pfIsolationVariables().sumPhotonEt - 0.0572 * rho))/pt < 0.113254 )|| " +
                  #"(abs(superCluster.eta) >= 2.0 && abs(superCluster.eta) < 2.2 && (pfIsolationVariables().sumChargedHadronPt+ max(0.,pfIsolationVariables().sumNeutralHadronEt+pfIsolationVariables().sumPhotonEt - 0.0842 * rho))/pt < 0.113254 )|| " +
                  #"(abs(superCluster.eta) >= 2.2 && abs(superCluster.eta) < 2.5 && (pfIsolationVariables().sumChargedHadronPt+ max(0.,pfIsolationVariables().sumNeutralHadronEt+pfIsolationVariables().sumPhotonEt - 0.1530 * rho))/pt < 0.113254 ) " +
                  #") "+ 
                 " && gsfTrack.isAvailable() && gsfTrack.hitPattern().numberOfLostHits(\'MISSING_INNER_HITS\') <= 1 "+
                 " && (abs(userFloat('convValueMapProd:dist')) > 0.02 || abs(userFloat('convValueMapProd:dcot')) > 0.02 )" +
                 " ) " +
                 ")")

ELE_ID_ROBUSTMEDIUM = (" electronID('eidRobustMedium') ")

wwEleMedium       = selectedElectronsBase.clone( cut = ELE_BASE + " && " + ELE_ID_MEDIUM )
wwEleRobustMedium = selectedElectronsBase.clone( cut = ELE_BASE + " && " + ELE_ID_ROBUSTMEDIUM )




# TIGHT
ELE_ID_TIGHT = ("  (( isEB "+ 
                           " && sigmaIetaIeta < 0.010181" +
                           " && hadronicOverEm < 0.037553" +
                           " && abs(deltaPhiSuperClusterTrackAtVtx) < 0.022868  " +
                           " && abs(deltaEtaSuperClusterTrackAtVtx) < 0.006574 " +
                           #" && abs(dB('PV2D')) < 0.009924  " +
                           #" && abs( sqrt( dB('PV3D')*dB('PV3D') - dB('PV2D')*dB('PV2D') ) ) < 0.015310  " +
                           " && abs(1./energy - 1/p) < 0.131191 " +
                           " && (pfIsolationVariables().sumChargedHadronPt+ max(0.,pfIsolationVariables().sumNeutralHadronEt+pfIsolationVariables().sumPhotonEt-0.5*pfIsolationVariables().sumPUPt))/pt < 0.074355 " +
                           " && gsfTrack.isAvailable() && gsfTrack.hitPattern().numberOfLostHits(\'MISSING_INNER_HITS\') <= 1 "+
                           " && (abs(userFloat('convValueMapProd:dist')) > 0.02 || abs(userFloat('convValueMapProd:dcot')) > 0.02 )" +
                           " ) || " +
                   "( (!isEB) " +
                           " && sigmaIetaIeta < 0.028766" +
                           " && hadronicOverEm < 0.081902" +
                           " && abs(deltaPhiSuperClusterTrackAtVtx) < 0.032046  " +
                           " && abs(deltaEtaSuperClusterTrackAtVtx) < 0.005681  " +
                           #" && abs(dB('PV2D')) < 0.027261  " +
                           #" && abs( sqrt( dB('PV3D')*dB('PV3D') - dB('PV2D')*dB('PV2D') ) ) < 0.147154  " +
                           " && abs(1./energy - 1/p) < 0.106055 " +
                           " && (pfIsolationVariables().sumChargedHadronPt+ max(0.,pfIsolationVariables().sumNeutralHadronEt+pfIsolationVariables().sumPhotonEt-0.5*pfIsolationVariables().sumPUPt))/pt < 0.090185 " +
                           " && gsfTrack.isAvailable() && gsfTrack.hitPattern().numberOfLostHits(\'MISSING_INNER_HITS\') <= 1 "+
                           " && (abs(userFloat('convValueMapProd:dist')) > 0.02 || abs(userFloat('convValueMapProd:dcot')) > 0.02 )" +
                           " ) " +
                    ")")

ELE_ID_ROBUSTTIGHT = (" electronID('eidRobustTight') ")



wwEleTight       = selectedElectronsBase.clone( cut = ELE_BASE + " && " + ELE_ID_TIGHT )
wwEleRobustTight = selectedElectronsBase.clone( cut = ELE_BASE + " && " + ELE_ID_ROBUSTTIGHT )




#
#
# |  ____|         | |
# | |__   _ __   __| |
# |  __| | '_ \ / _` |
# | |____| | | | (_| |
# |______|_| |_|\__,_|
#

wwElectronSequence = cms.Sequence(
    wwEleBase
    * wwEleTight
    * wwEleRobustTight
)


