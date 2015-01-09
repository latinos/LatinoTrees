import FWCore.ParameterSet.Config as cms

#select collections
selectedElectronsBase = cms.EDFilter("PATElectronRefSelector",
    src = cms.InputTag("slimmedElectrons"),
    filter = cms.bool(False),
    cut = cms.string("pt > 5 && abs(eta)<2.5")
    )





#cut = cms.string('''abs(eta)<2.5 && pt>20. &&
    #gsfTrack.isAvailable() &&
    #gsfTrack.hitPattern().numberOfLostHits(\'MISSING_INNER_HITS\') < 2 &&
    #(pfIsolationVariables().sumChargedHadronPt+
    #max(0.,pfIsolationVariables().sumNeutralHadronEt+
    #pfIsolationVariables().sumPhotonEt-
    #0.5*pfIsolationVariables().sumPUPt))/pt < 0.15'''))


ELE_BASE = "( pt > 8 && abs(eta)<2.5 )"
wwEleBase = selectedElectronsBase.clone( cut = ELE_BASE )










########################
# Phys14 recipe
# https://twiki.cern.ch/twiki/bin/view/CMS/CutBasedElectronIdentificationRun2


ELE_ID_TIGHT = ("  (( isEB "+ 
                           " && sigmaIetaIeta < 0.0106" +
                           " && hadronicOverEm < 0.025" +
                           " && abs(deltaPhiSuperClusterTrackAtVtx) < 0.031  " +
                           " && abs(deltaEtaSuperClusterTrackAtVtx) < 0.0091 " +
                           " && abs(dB('PV2D')) < 0.0126  " +
                           " && abs( sqrt( dB('PV3D')*dB('PV3D') - dB('PV2D')*dB('PV2D') ) ) < 0.0116  " +
                           " && abs(1./energy - 1/p) < 0.0609 " +
                           " && (pfIsolationVariables().sumChargedHadronPt+ max(0.,pfIsolationVariables().sumNeutralHadronEt+pfIsolationVariables().sumPhotonEt-0.5*pfIsolationVariables().sumPUPt))/pt < 0.1649 " +
                           " && gsfTrack.isAvailable() && gsfTrack.hitPattern().numberOfLostHits(\'MISSING_INNER_HITS\') <= 1 "+
                           " && userFloat('convValueMapProd:passVtxConvert') != 0 " +
                           " ) || " +
                   "( (!isEB) " +
                           " && sigmaIetaIeta < 0.0305" +
                           " && hadronicOverEm < 0.0835" +
                           " && abs(deltaPhiSuperClusterTrackAtVtx) < 0.0359  " +
                           " && abs(deltaEtaSuperClusterTrackAtVtx) < 0.0106  " +
                           " && abs(dB('PV2D')) < 0.0163  " +
                           " && abs( sqrt( dB('PV3D')*dB('PV3D') - dB('PV2D')*dB('PV2D') ) ) < 0.5999  " +
                           " && abs(1./energy - 1/p) < 0.1126 " +
                           " && (pfIsolationVariables().sumChargedHadronPt+ max(0.,pfIsolationVariables().sumNeutralHadronEt+pfIsolationVariables().sumPhotonEt-0.5*pfIsolationVariables().sumPUPt))/pt < 0.2075 " +
                           " && gsfTrack.isAvailable() && gsfTrack.hitPattern().numberOfLostHits(\'MISSING_INNER_HITS\') <= 1 "+
                           " && userFloat('convValueMapProd:passVtxConvert') != 0 " +
                           " ) " +
                    ")")

ELE_ID_ROBUSTTIGHT = (" electronID('eidRobustTight') ")



# && (fbrem > 0.15 || ( abs(superCluster.eta) < 1. && eSuperClusterOverP > 0.95 )) ) ||" +
#" ( (!isEB) && pt < 20 && sigmaIetaIeta < 0.03 &&" +
#" deltaPhiSuperClusterTrackAtVtx > -0.02 && deltaPhiSuperClusterTrackAtVtx < 0.02 &&" +
#" deltaEtaSuperClusterTrackAtVtx > -0.005 && deltaEtaSuperClusterTrackAtVtx < 0.005 &&" +
#" hadronicOverEm < 0.1 && fbrem > 0.15) ||" +
#" ( isEB && pt > 20 && sigmaIetaIeta < 0.01 &&" +
#" deltaPhiSuperClusterTrackAtVtx > -0.06 && deltaPhiSuperClusterTrackAtVtx < 0.06 &&" +
#" deltaEtaSuperClusterTrackAtVtx > -0.004 && deltaEtaSuperClusterTrackAtVtx < 0.004 &&" +
#" hadronicOverEm < 0.04) ||" +
#" ( (!isEB) && pt > 20 && sigmaIetaIeta < 0.03 && " +
#" deltaPhiSuperClusterTrackAtVtx > -0.03 && deltaPhiSuperClusterTrackAtVtx < 0.03 &&" +
#" deltaEtaSuperClusterTrackAtVtx > -0.007 && deltaEtaSuperClusterTrackAtVtx < 0.007 && " +
#" hadronicOverEm < 0.1 ) ) ")


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


