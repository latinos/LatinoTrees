import FWCore.ParameterSet.Config as cms

rawJets = cms.EDProducer('JetUncorrector',
   src = cms.InputTag("slimmedJets")
)
