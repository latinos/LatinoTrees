import FWCore.ParameterSet.Config as cms

process = cms.Process("particleDrawer")

process.load("FWCore.MessageService.MessageLogger_cfi")
process.load("SimGeneral.HepPDTESSource.pythiapdt_cfi")


####### Skim filter
process.selectedMuons = cms.EDFilter("MuonSelector",
                                     src = cms.InputTag("slimmedMuons"),
                                     cut = cms.string("isPFMuon && pt>20. && abs(eta)<2.4"))

process.allDiLep = cms.EDProducer("CandViewShallowCloneCombiner",
                                  decay = cms.string("selectedMuons@+ selectedMuons@-"),
                                  cut   = cms.string("mass > 20."),
                                  checkCharge = cms.bool(True))

process.countDiLeps = cms.EDFilter("CandViewCountFilter",
                                   src = cms.InputTag("allDiLep"),
                                   minNumber = cms.uint32(1))

process.preYieldFilter = cms.Sequence(process.selectedMuons+process.allDiLep+process.countDiLeps)


####### Number of events
process.maxEvents = cms.untracked.PSet(input = cms.untracked.int32(100))


####### MessageLogger
process.load("FWCore.MessageService.MessageLogger_cfi")
process.MessageLogger.destinations = ['cout', 'cerr']
process.MessageLogger.cerr.FwkReport.reportEvery = 1000


####### Input file
process.source = cms.Source("PoolSource",
                            fileNames = cms.untracked.vstring('root://cms-xrd-global.cern.ch//store/mc/RunIISpring16MiniAODv2/WZ_TuneCUETP8M1_13TeV-pythia8/MINIAODSIM/PUSpring16_80X_mcRun2_asymptotic_2016_miniAODv2_v0-v1/60000/16A554A2-861B-E611-80BC-90B11C0BCC58.root'),
                            skipEvents = cms.untracked.uint32(4)
                            )


####### ParticleListDrawer and ParticleTreeDrawer
process.printTree1 = cms.EDAnalyzer("ParticleListDrawer",
                                    src = cms.InputTag("prunedGenParticles"),
                                    maxEventsToPrint = cms.untracked.int32(5)
                                    )

process.printTree2 = cms.EDAnalyzer("ParticleTreeDrawer",
                                    src           = cms.InputTag("prunedGenParticles"),
                                    printP4       = cms.untracked.bool(False),
                                    printPtEtaPhi = cms.untracked.bool(False),
                                    printVertex   = cms.untracked.bool(False),
                                    printStatus   = cms.untracked.bool(False),
                                    printIndex    = cms.untracked.bool(False)
                                    )

process.printEventNumber = cms.OutputModule("AsciiOutputModule")


####### Set the path
process.p = cms.Path(process.preYieldFilter*
                     process.printTree1)
#                    process.printTree1*
#                    process.printTree2)

process.outpath = cms.EndPath(process.printEventNumber)
