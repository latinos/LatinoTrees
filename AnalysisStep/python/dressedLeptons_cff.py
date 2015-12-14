import FWCore.ParameterSet.Config as cms

from LatinoTrees.AnalysisStep.dressedLeptons_cfi import *


dressedElectrons03 = dressedElectrons.clone(DeltaR = 0.3)
dressedMuons03     = dressedMuons.clone(DeltaR = 0.3)

dressedElectrons01 = dressedElectrons.clone(DeltaR = 0.1)
dressedMuons01     = dressedMuons.clone(DeltaR = 0.1)

dressedLeptonsSequence = cms.Sequence(dressedElectrons03 + dressedMuons03 + dressedElectrons01 + dressedMuons01)
