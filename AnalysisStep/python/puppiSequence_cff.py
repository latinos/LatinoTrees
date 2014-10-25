import FWCore.ParameterSet.Config as cms

from RecoJets.JetProducers.ak5PFJets_cfi  import ak5PFJets

from RecoMET.METProducers.PFMET_cfi import pfMet

################################
################################       
################################       

def makePuppiAlgo(process):

 ### puppi met
 process.load('Dummy.Puppi.Puppi_cff')
 process.puppi.candName = cms.untracked.string('particleFlow')
 process.puppi.vertexName = cms.untracked.string('offlinePrimaryVertices')

 process.particleFlowPuppiPtrs = process.particleFlowPtrs.clone(src = cms.InputTag("puppi","Puppi"))

 process.pfPuppiPileUp = process.pfPileUp.clone( PFCandidates = cms.InputTag("particleFlowPuppiPtrs"))

 process.pfPuppiPileUpJME = process.pfPileUpJME.clone( PFCandidates = cms.InputTag("particleFlowPuppiPtrs"))

 process.pfPuppiNoPileUp = process.pfNoPileUp.clone( bottomCollection = cms.InputTag("particleFlowPuppiPtrs"),
                                                     topCollection = cms.InputTag("pfPuppiPileUp"))

 process.pfPuppiNoPileUpJME = process.pfNoPileUpJME.clone( bottomCollection = cms.InputTag("particleFlowPuppiPtrs"),
                                                           topCollection = cms.InputTag("pfPuppiPileUp"))


 ## packed puppi candidates 
 process.packedPFCandidatesPuppi = process.packedPFCandidates.clone(inputCollection = cms.InputTag("puppi","Puppi"),
                                                                    inputCollectionFromPVTight = cms.InputTag("pfPuppiNoPileUp"),
                                                                    inputCollectionFromPVLoose = cms.InputTag("pfPuppiNoPileUpJME"))

 process.pfMetPuppi = pfMet.clone( src = cms.InputTag('puppi','Puppi'),
                                   calculateSignificance = False # this can't be easily implemented on packed PF candidates at the moment                          
                                   )          

 process.puppiSequence = cms.Sequence(process.puppi*
                                      process.particleFlowPuppiPtrs*
                                      process.pfPuppiPileUp*
                                      process.pfPuppiNoPileUp*
                                      process.pfPuppiPileUpJME*
                                      process.pfPuppiNoPileUpJME*
                                      process.packedPFCandidatesPuppi*
                                      process.pfMetPuppi)                                                                 

def makePatPuppiJetSequence( process, rParameter = 0.5):

 ####### make clustering in AK5 puppi jets
 jetRPrefix = str(int(rParameter*10));
 setattr(process,"AK"+jetRPrefix+"PFJetsPuppi", ak5PFJets.clone( src      = cms.InputTag('puppi','Puppi'),
                                                                 rParam   = cms.double(rParameter),
                                                                 jetPtMin = cms.double(10))
 )

 ####### btagging sequence for puppi jets
 ####### jet track association
 setattr(process, "AK"+jetRPrefix+"jetTracksAssociatorAtVertexPuppi",
         process.jetTracksAssociatorAtVertex.clone( jets = cms.InputTag("AK"+jetRPrefix+"PFJetsPuppi")))
 
 ## impact parameter and track counting
 setattr(process,"AK"+jetRPrefix+"impactParameterTagInfosPuppi",
         process.impactParameterTagInfos.clone( jetTracks = cms.InputTag("AK"+jetRPrefix+"jetTracksAssociatorAtVertexPuppi")))  
 setattr(process,"AK"+jetRPrefix+"trackCountingHighEffbJetTagsPuppi",
         process.trackCountingHighEffBJetTags.clone( tagInfos = cms.VInputTag(cms.InputTag("AK"+jetRPrefix+"impactParameterTagInfosPuppi")))) 
 setattr(process,"AK"+jetRPrefix+"trackCountingHighPurBjetTagsPuppi", 
         process.trackCountingHighPurBJetTags.clone( tagInfos = cms.VInputTag(cms.InputTag("AK"+jetRPrefix+"impactParameterTagInfosPuppi"))))
 setattr(process,"AK"+jetRPrefix+"trackCountingHighPurBJetTagsPuppi", 
         process.jetProbabilityBJetTags.clone( tagInfos = cms.VInputTag(cms.InputTag("AK"+jetRPrefix+"impactParameterTagInfosPuppi"))))
 setattr(process,"AK"+jetRPrefix+"jetBProbabilityBJetTagsPuppi",
         process.jetBProbabilityBJetTags.clone( tagInfos = cms.VInputTag(cms.InputTag("AK"+jetRPrefix+"impactParameterTagInfosPuppi"))))
 setattr(process,"AK"+jetRPrefix+"jetProbabilityBJetTagsPuppi",
         process.jetProbabilityBJetTags.clone( tagInfos = cms.VInputTag(cms.InputTag("AK"+jetRPrefix+"impactParameterTagInfosPuppi"))))
 ## secondary vertex tags   
 setattr(process,"AK"+jetRPrefix+"secondaryVertexTagInfoPuppi", 
         process.secondaryVertexTagInfos.clone( trackIPTagInfos = cms.InputTag("AK"+jetRPrefix+"impactParameterTagInfosPuppi")))
 setattr(process,"AK"+jetRPrefix+"simpleSecondaryVertexHighEffBJetTagsPuppi",
         process.simpleSecondaryVertexHighEffBJetTags.clone(tagInfos = cms.VInputTag(cms.InputTag("AK"+jetRPrefix+"secondaryVertexTagInfoPuppi"))))
 setattr(process,"AK"+jetRPrefix+"simpleSecondaryVertexHighPurBJetTagsPuppi",
         process.simpleSecondaryVertexHighPurBJetTags.clone(tagInfos = cms.VInputTag(cms.InputTag("AK"+jetRPrefix+"secondaryVertexTagInfoPuppi"))))
 setattr(process,"AK"+jetRPrefix+"combinedSecondaryVertexBJetTagsPuppi",
         process.combinedSecondaryVertexBJetTags.clone(tagInfos = cms.VInputTag(cms.InputTag("AK"+jetRPrefix+"impactParameterTagInfosPuppi"), cms.InputTag("AK"+jetRPrefix+"secondaryVertexTagInfoPuppi"))))
 setattr(process,"AK"+jetRPrefix+"combinedSecondaryVertexMVABJetTagsPuppi",
         process.combinedSecondaryVertexMVABJetTags.clone( tagInfos = cms.VInputTag(cms.InputTag("AK"+jetRPrefix+"impactParameterTagInfosPuppi"), cms.InputTag("AK"+jetRPrefix+"secondaryVertexTagInfoPuppi"))))
 
 ## soft leptons
 
 setattr(process,"AK"+jetRPrefix+"ghostTrackVertexTagInfosPuppi",
         process.ghostTrackVertexTagInfos.clone( trackIPTagInfos = cms.InputTag("AK"+jetRPrefix+"impactParameterTagInfosPuppi"))) 
 setattr(process,"AK"+jetRPrefix+"ghostTrackBJetTagsPuppi",
         process.ghostTrackBJetTags.clone( tagInfos = cms.VInputTag(cms.InputTag("AK"+jetRPrefix+"impactParameterTagInfosPuppi"), cms.InputTag("AK"+jetRPrefix+"ghostTrackVertexTagInfosPuppi"))))
 
 setattr(process,"AK"+jetRPrefix+"softPFMuonsTagInfosPuppi", 
         process.softPFMuonsTagInfos.clone( jets = cms.InputTag("AK"+jetRPrefix+"PFJetsPuppi")))
 
 setattr(process,"AK"+jetRPrefix+"softPFMuonBJetTagsPuppi",
         process.softPFMuonBJetTags.clone(tagInfos = cms.VInputTag(cms.InputTag("AK"+jetRPrefix+"softPFMuonsTagInfosPuppi"))))

 setattr(process,"AK"+jetRPrefix+"softPFElectronsTagInfosPuppi",
         process.softPFElectronsTagInfos.clone(jets = cms.InputTag("AK"+jetRPrefix+"PFJetsPuppi")))

 setattr(process,"AK"+jetRPrefix+"softPFElectronBJetTagsPuppi",
         process.softPFElectronBJetTags.clone(tagInfos= cms.VInputTag(cms.InputTag("AK"+jetRPrefix+"softPFElectronsTagInfosPuppi"))))
 
 ## final sequence
 setattr(process,"AK"+jetRPrefix+"btaggingPuppi", cms.Sequence( getattr(process,"AK"+jetRPrefix+"jetTracksAssociatorAtVertexPuppi")*
                                                                getattr(process,"AK"+jetRPrefix+"impactParameterTagInfosPuppi")*
                                                                getattr(process,"AK"+jetRPrefix+"trackCountingHighEffbJetTagsPuppi")*
                                                                getattr(process,"AK"+jetRPrefix+"trackCountingHighPurBjetTagsPuppi")*                                   
                                                                getattr(process,"AK"+jetRPrefix+"jetProbabilityBJetTagsPuppi")*
                                                                getattr(process,"AK"+jetRPrefix+"jetBProbabilityBJetTagsPuppi")*
                                                                getattr(process,"AK"+jetRPrefix+"secondaryVertexTagInfoPuppi")*
                                                                getattr(process,"AK"+jetRPrefix+"simpleSecondaryVertexHighEffBJetTagsPuppi")*
                                                                getattr(process,"AK"+jetRPrefix+"simpleSecondaryVertexHighPurBJetTagsPuppi")*
                                                                getattr(process,"AK"+jetRPrefix+"combinedSecondaryVertexBJetTagsPuppi")*
                                                                getattr(process,"AK"+jetRPrefix+"combinedSecondaryVertexMVABJetTagsPuppi")*
                                                                getattr(process,"AK"+jetRPrefix+"ghostTrackVertexTagInfosPuppi")*
                                                                getattr(process,"AK"+jetRPrefix+"ghostTrackBJetTagsPuppi")*
                                                                getattr(process,"AK"+jetRPrefix+"softPFMuonsTagInfosPuppi")*
                                                                getattr(process,"AK"+jetRPrefix+"softPFMuonBJetTagsPuppi")*
                                                                getattr(process,"AK"+jetRPrefix+"softPFElectronsTagInfosPuppi")*
                                                                getattr(process,"AK"+jetRPrefix+"softPFElectronBJetTagsPuppi")
                                                                )
 )

 ## jet flavout association

 setattr(process,"AK"+jetRPrefix+"patJetPuppiPartonMatch",
         process.patJetPartonMatch.clone( src = cms.InputTag("AK"+jetRPrefix+"PFJetsPuppi")))
 setattr(process,"AK"+jetRPrefix+"patJetPuppiGenJetMatch",
         process.patJetGenJetMatch.clone( src = cms.InputTag("AK"+jetRPrefix+"PFJetsPuppi")))
 setattr(process,"AK"+jetRPrefix+"patJetPuppiPartonAssociationLegacy",
         process.patJetPartonAssociationLegacy.clone( jets = cms.InputTag("AK"+jetRPrefix+"PFJetsPuppi")))
 setattr(process,"AK"+jetRPrefix+"patJetPuppiFlavourAssociation",
         process.patJetFlavourAssociation.clone(  jets = cms.InputTag("AK"+jetRPrefix+"PFJetsPuppi")))
 setattr(process,"AK"+jetRPrefix+"patPuppiJetCharge",
         process.patJetCharge.clone(src = cms.InputTag("AK"+jetRPrefix+"jetTracksAssociatorAtVertexPuppi")))

 setattr(process,"AK"+jetRPrefix+"patPuppiFlavour", cms.Sequence(getattr(process,"AK"+jetRPrefix+"patJetPuppiPartonMatch")*
                               getattr(process,"AK"+jetRPrefix+"patJetPuppiGenJetMatch")*
                               getattr(process,"AK"+jetRPrefix+"patJetPuppiPartonAssociationLegacy")*
                               getattr(process,"AK"+jetRPrefix+"patJetPuppiFlavourAssociation")*
                               getattr(process,"AK"+jetRPrefix+"patPuppiJetCharge"))
  )

 
 ### make puppi pat jets
 setattr(process,"AK"+jetRPrefix+"patJetsPuppi", process.patJets.clone( 
                                  jetSource  = cms.InputTag("AK"+jetRPrefix+"PFJetsPuppi"),
                                  genJetMatch        =  cms.InputTag("AK"+jetRPrefix+"patJetPuppiGenJetMatch"),
                                  genPartonMatch     =  cms.InputTag("AK"+jetRPrefix+"patJetPuppiPartonMatch"),
                                  JetPartonMapSource =  cms.InputTag("AK"+jetRPrefix+"patJetPuppiPartonAssociationLegacy"),
                                  jetCorrFactorsSource   = cms.VInputTag(""),
                                  jetChargeSource        = cms.InputTag("AK"+jetRPrefix+"patPuppiJetCharge"),
                                  JetFlavourInfoSource   = cms.InputTag("AK"+jetRPrefix+"patJetPuppiFlavourAssociation"),
                                  trackAssociationSource = cms.InputTag("AK"+jetRPrefix+"jetTracksAssociatorAtVertexPuppi"),       
                                  tagInfoSources         = cms.VInputTag(cms.InputTag("AK"+jetRPrefix+"secondaryVertexTagInfoPuppi")),
                                  discriminatorSources = cms.VInputTag(
                                     cms.InputTag("AK"+jetRPrefix+"jetBProbabilityBJetTagsPuppi"), cms.InputTag("AK"+jetRPrefix+"jetProbabilityBJetTagsPuppi"), 
                                     cms.InputTag("AK"+jetRPrefix+"trackCountingHighEffbJetTagsPuppi"), cms.InputTag("AK"+jetRPrefix+"trackCountingHighPurBjetTagsPuppi"), 
                                     cms.InputTag("AK"+jetRPrefix+"simpleSecondaryVertexHighEffBJetTagsPuppi"),
                                     cms.InputTag("AK"+jetRPrefix+"simpleSecondaryVertexHighPurBJetTagsPuppi"), 
                                     cms.InputTag("AK"+jetRPrefix+"combinedSecondaryVertexBJetTagsPuppi"), 
                                     cms.InputTag("AK"+jetRPrefix+"combinedSecondaryVertexMVABJetTagsPuppi")),
                                  addGenJetMatch      = cms.bool(True),
                                  embedGenJetMatch    = cms.bool(True),
                                  addGenPartonMatch   = cms.bool(True),
                                  embedGenPartonMatch = cms.bool(True),
                                  addPartonJetMatch   = cms.bool(True),
                                  addJetCharge        = cms.bool(True),
                                  addAssociatedTracks = cms.bool(True),
                                  addEfficiencies     = cms.bool(False),
                                  addResolutions      = cms.bool(False),
                                  addJetID            = cms.bool(False),
                                  addJetCorrFactors   = cms.bool(False),
                                  addDiscriminators   = cms.bool(True),
                                  embedPFCandidates   = cms.bool(False),
                                  addJetFlavourInfo   = cms.bool(True),
                                  getJetMCFlavour     = cms.bool(True),
                                  useLegacyJetMCFlavour = cms.bool(False),
                                  addBTagInfo = cms.bool(True),
                                  addTagInfos = cms.bool(True),
  )
 )


 ## make selected puppi jets
 setattr(process,"AK"+jetRPrefix+"selectedPatJetsPuppi", process.selectedPatJets.clone(src = cms.InputTag("AK"+jetRPrefix+"patJetsPuppi")))

 ## make slimmed puppi jets
 setattr(process, "AK"+jetRPrefix+"slimmedJetsPuppi", process.slimmedJets.clone( src = cms.InputTag("AK"+jetRPrefix+"selectedPatJetsPuppi"),
                                                                                 packedPFCandidates = cms.InputTag("packedPFCandidatesPuppi")))


 setattr(process,"AK"+jetRPrefix+"makePatJetsPuppi", cms.Sequence( getattr(process,"AK"+jetRPrefix+"PFJetsPuppi")*
                                                                   getattr(process,"AK"+jetRPrefix+"btaggingPuppi")*
                                                                   getattr(process,"AK"+jetRPrefix+"patPuppiFlavour")*
                                                                   getattr(process,"AK"+jetRPrefix+"patJetsPuppi")*
                                                                   getattr(process,"AK"+jetRPrefix+"selectedPatJetsPuppi")*
                                                                   getattr(process,"AK"+jetRPrefix+"slimmedJetsPuppi"))
 )
 
                               
