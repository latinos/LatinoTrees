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

# TIGHT
ELE_ID_TIGHT = ("  (( isEB "+ 
                           " && sigmaIetaIeta < 0.010181" +
                           " && hadronicOverEm < 0.037553" +
                           " && abs(deltaPhiSuperClusterTrackAtVtx) < 0.022868  " +
                           " && abs(deltaEtaSuperClusterTrackAtVtx) < 0.006574 " +
                           " && abs(dB('PV2D')) < 0.009924  " +
                           " && abs( sqrt( dB('PV3D')*dB('PV3D') - dB('PV2D')*dB('PV2D') ) ) < 0.015310  " +
                           " && abs(1./energy - 1/p) < 0.131191 " +
                           " && (pfIsolationVariables().sumChargedHadronPt+ max(0.,pfIsolationVariables().sumNeutralHadronEt+pfIsolationVariables().sumPhotonEt-0.5*pfIsolationVariables().sumPUPt))/pt < 0.074355 " +
                           " && gsfTrack.isAvailable() && gsfTrack.hitPattern().numberOfLostHits(\'MISSING_INNER_HITS\') <= 1 "+
                           " && userFloat('convValueMapProd:passVtxConvert') != 0 " +
                           " ) || " +
                   "( (!isEB) " +
                           " && sigmaIetaIeta < 0.028766" +
                           " && hadronicOverEm < 0.081902" +
                           " && abs(deltaPhiSuperClusterTrackAtVtx) < 0.032046  " +
                           " && abs(deltaEtaSuperClusterTrackAtVtx) < 0.005681  " +
                           " && abs(dB('PV2D')) < 0.027261  " +
                           " && abs( sqrt( dB('PV3D')*dB('PV3D') - dB('PV2D')*dB('PV2D') ) ) < 0.147154  " +
                           " && abs(1./energy - 1/p) < 0.106055 " +
                           " && (pfIsolationVariables().sumChargedHadronPt+ max(0.,pfIsolationVariables().sumNeutralHadronEt+pfIsolationVariables().sumPhotonEt-0.5*pfIsolationVariables().sumPUPt))/pt < 0.090185 " +
                           " && gsfTrack.isAvailable() && gsfTrack.hitPattern().numberOfLostHits(\'MISSING_INNER_HITS\') <= 1 "+
                           " && userFloat('convValueMapProd:passVtxConvert') != 0 " +
                           " ) " +
                    ")")

ELE_ID_ROBUSTTIGHT = (" electronID('eidRobustTight') ")



wwEleTight       = selectedElectronsBase.clone( cut = ELE_BASE + " && " + ELE_ID_TIGHT )
wwEleRobustTight = selectedElectronsBase.clone( cut = ELE_BASE + " && " + ELE_ID_ROBUSTTIGHT )


# LOOSE
ELE_ID_LOOSE = ("  (( isEB "+ 
                           " && sigmaIetaIeta < 0.010557" +
                           " && hadronicOverEm < 0.121476" +
                           " && abs(deltaPhiSuperClusterTrackAtVtx) < 0.072624  " +
                           " && abs(deltaEtaSuperClusterTrackAtVtx) < 0.012442 " +
                           " && abs(dB('PV2D')) < 0.022664  " +
                           " && abs( sqrt( dB('PV3D')*dB('PV3D') - dB('PV2D')*dB('PV2D') ) ) < 0.173670  " +
                           " && abs(1./energy - 1/p) < 0.221803 " +
                           " && (pfIsolationVariables().sumChargedHadronPt+ max(0.,pfIsolationVariables().sumNeutralHadronEt+pfIsolationVariables().sumPhotonEt-0.5*pfIsolationVariables().sumPUPt))/pt < 0.120026 " +
                           " && gsfTrack.isAvailable() && gsfTrack.hitPattern().numberOfLostHits(\'MISSING_INNER_HITS\') <= 1 "+
                           " && userFloat('convValueMapProd:passVtxConvert') != 0 " +
                           " ) || " +
                   "( (!isEB) " +
                           " && sigmaIetaIeta < 0.032602" +
                           " && hadronicOverEm < 0.131862" +
                           " && abs(deltaPhiSuperClusterTrackAtVtx) < 0.145129  " +
                           " && abs(deltaEtaSuperClusterTrackAtVtx) < 0.010654  " +
                           " && abs(dB('PV2D')) < 0.097358  " +
                           " && abs( sqrt( dB('PV3D')*dB('PV3D') - dB('PV2D')*dB('PV2D') ) ) < 0.198444  " +
                           " && abs(1./energy - 1/p) < 0.142283 " +
                           " && (pfIsolationVariables().sumChargedHadronPt+ max(0.,pfIsolationVariables().sumNeutralHadronEt+pfIsolationVariables().sumPhotonEt-0.5*pfIsolationVariables().sumPUPt))/pt < 0.162914 " +
                           " && gsfTrack.isAvailable() && gsfTrack.hitPattern().numberOfLostHits(\'MISSING_INNER_HITS\') <= 1 "+
                           " && userFloat('convValueMapProd:passVtxConvert') != 0 " +
                           " ) " +
                    ")")

wwEleLoose       = selectedElectronsBase.clone( cut = ELE_BASE + " && " + ELE_ID_LOOSE )




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


