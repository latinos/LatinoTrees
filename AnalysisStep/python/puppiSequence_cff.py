import FWCore.ParameterSet.Config as cms

from Dummy.Puppi.Puppi_cff import puppiCentral, puppiForward, puppi 



####### make clustering in AK4 puppi jets
from RecoJets.JetProducers.ak5PFJets_cfi  import ak5PFJets

AK4PFJetsPuppi = ak5PFJets.clone(
    src      = cms.InputTag('puppi','Puppi'),
    rParam   = cms.double(0.4),
    jetPtMin = cms.double(10)
)

AK5PFJetsPuppi = ak5PFJets.clone(
    src      = cms.InputTag('puppi','Puppi'),
    rParam   = cms.double(0.5),
    jetPtMin = cms.double(10)
)

### puppi met
from RecoMET.METProducers.PFMET_cfi import pfMet
pfMetPuppi = pfMet.clone();
pfMetPuppi.src = cms.InputTag('puppi','Puppi')
pfMetPuppi.calculateSignificance = False # this can't be easily implemented on packed PF candidates at the moment                                                                  

### final sequence ###
puppiSequence = cms.Sequence(puppi*
                             AK5PFJetsPuppi*
                             pfMetPuppi)
