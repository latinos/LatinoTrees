import FWCore.ParameterSet.Config as cms

dressedElectrons = cms.EDProducer("DressedLeptons",
  PdgId = cms.int32(11),
  DeltaR = cms.double(0.3),
  genParticles = cms.InputTag("genParticles")  
)

dressedMuons = cms.EDProducer("DressedLeptons",
  PdgId = cms.int32(13),
  DeltaR = cms.double(0.3),
  genParticles = cms.InputTag("genParticles")
)
