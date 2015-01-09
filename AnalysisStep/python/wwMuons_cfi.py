import FWCore.ParameterSet.Config as cms

#select collections
selectedMuonsBase = cms.EDFilter("PATMuonRefSelector",
    src = cms.InputTag("slimmedMuons"),
    filter = cms.bool(False),
    cut = cms.string("pt > 5 && abs(eta)<2.4")
    )


MUO_BASE = "( pt > 8 && abs(eta)<2.5 )"
wwMuoBase = selectedMuonsBase.clone( cut = MUO_BASE )



########################
# Phys14 recipe
# https://twiki.cern.ch/twiki/bin/view/CMS/CutBasedElectronIdentificationRun2


#MUO_ID_TIGHT = ("isTightMuon") -> it does NOT work
MUO_ID_TIGHT = ("("+
                  "(pfIsolationR04().sumChargedHadronPt+max(0.,pfIsolationR04().sumNeutralHadronEt+pfIsolationR04().sumPhotonEt-0.50*pfIsolationR04().sumPUPt))/pt < 0.20 " +
                  " && (isPFMuon && (isGlobalMuon || isTrackerMuon) ) " +
                  " )")

wwMuoTight       = selectedMuonsBase.clone( cut = MUO_BASE + " && " + MUO_ID_TIGHT )


#
#
# |  ____|         | |
# | |__   _ __   __| |
# |  __| | '_ \ / _` |
# | |____| | | | (_| |
# |______|_| |_|\__,_|
#

wwMuonSequence = cms.Sequence(
    wwMuoBase
    * wwMuoTight
)


