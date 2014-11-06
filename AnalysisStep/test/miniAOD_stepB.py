import FWCore.ParameterSet.Config as cms
import LatinoTrees.Misc.VarParsing as opts
import re
import sys, os

########################
#### Option parsing ####
########################

options = opts.VarParsing('analysis')

## general info for the process
options.register ('skipEvents',0,opts.VarParsing.multiplicity.singleton,opts.VarParsing.varType.int,'Number of events to skip')
options.register ('label','XXX',opts.VarParsing.multiplicity.singleton,opts.VarParsing.varType.string,'Label')
options.register ('json','YYY',opts.VarParsing.multiplicity.singleton,opts.VarParsing.varType.string,'Json file for data')
options.register ('id',0,opts.VarParsing.multiplicity.singleton,opts.VarParsing.varType.string,'Dataset id')
options.register ('scale',0,opts.VarParsing.multiplicity.singleton,opts.VarParsing.varType.float,'Scale factor')
options.register ('acceptDuplicates',False,opts.VarParsing.multiplicity.singleton,opts.VarParsing.varType.bool,
                  'accept duplicates. Suggested true for private production (can be \'True\' or \'False\'')
options.register ('globalTag','POSTLS162_V4::All',opts.VarParsing.multiplicity.singleton,opts.VarParsing.varType.string,'give the global tag string to be used')
options.register ('cmsGeometry','Extended2019',opts.VarParsing.multiplicity.singleton,opts.VarParsing.varType.string,'give a string related to the upgrade geometry to consider')

## kind of selection
options.register ('selection','TightTight',opts.VarParsing.multiplicity.singleton,opts.VarParsing.varType.string,'Selection level [TightTight,LooseLoose]')
options.register ('doSameSign',False,opts.VarParsing.multiplicity.singleton,opts.VarParsing.varType.bool,'Turn on Same Sign mode (can be \'True\' or \'False\'')
options.register ('doNoFilter',False,opts.VarParsing.multiplicity.singleton,opts.VarParsing.varType.bool,
                  'Turn on no filter requirement, not even requiring 2 leptons! Needed for unfolding at GEN (can be \'True\' or \'False\'')

## met filter option
options.register ('doMETFilter',False,opts.VarParsing.multiplicity.singleton,opts.VarParsing.varType.bool,'Apply met filters can be true or false')

## dump LHE info
options.register ('doSusy',False, opts.VarParsing.multiplicity.singleton,opts.VarParsing.varType.bool,'Turn on Susy MC dumper (can be \'True\' or \'False\'')
options.register ('doHiggs',True,opts.VarParsing.multiplicity.singleton,opts.VarParsing.varType.bool,'Turn on Higgs MC mass dumper (can be \'True\' or \'False\'')
options.register ('doLHE',True,opts.VarParsing.multiplicity.singleton,opts.VarParsing.varType.bool,'Turn on LHE dumper (can be \'True\' or \'False\'')
options.register ('typeLHEcomment',0,opts.VarParsing.multiplicity.singleton,opts.VarParsing.varType.int,'type of comment in LHE, 0 [default] powheg scale variation, 1 MG for anomalous couplings')

## dump gen info
options.register ('doGen',True,opts.VarParsing.multiplicity.singleton,opts.VarParsing.varType.bool,'Turn on gen Variables dumper (can be \'True\' or \'False\'')
## dump iso info
options.register ('doEleIsoId',False,opts.VarParsing.multiplicity.singleton,opts.VarParsing.varType.bool,'Turn on electron id/iso sumper (can be \'True\' or \'False\'')
## dump fat jet info
options.register ('doFatJet',False,opts.VarParsing.multiplicity.singleton,opts.VarParsing.varType.bool,'Turn on Fat (can be \'True\' or \'False\'')
## pile up info
options.register ('puInformation','addPileupInfo',opts.VarParsing.multiplicity.singleton,opts.VarParsing.varType.string,'name of pile-up information collection: it may change for premixing samples in MC, addPileupInfo [default], mixData for premixinf')
## add more jets
options.register ("doAdditionalJets",True,opts.VarParsing.multiplicity.singleton,opts.VarParsing.varType.bool,' from 5 up to 8 jets in the event')
## run puppi flag
options.register ('runPUPPISequence',True,opts.VarParsing.multiplicity.singleton,opts.VarParsing.varType.bool,'Turn on PUPPI jets (can be \'True\' or \'False\'')
## stop at miniAOD level
options.register ('producePATObjects',False,opts.VarParsing.multiplicity.singleton,opts.VarParsing.varType.bool,'if true it stops at MiniAOD, otherwise it runs the full stepB')



## More Info for generating the tree
options.register ('jetIdWP',"1",opts.VarParsing.multiplicity.singleton,opts.VarParsing.varType.string,'type of jetID to be applied')
options.register ('pileupjetIdWP',"0",opts.VarParsing.multiplicity.singleton,opts.VarParsing.varType.string,'type of pileup jetID to be applied')
options.register ('CJVminPt',"30.",opts.VarParsing.multiplicity.singleton,opts.VarParsing.varType.string,'minimum pt cut in some cases --> counting')
options.register ('CJVmaxEta',"5.",opts.VarParsing.multiplicity.singleton,opts.VarParsing.varType.string,'max jet eta cut')
options.register ('DphiJetVetominPt',"15.",opts.VarParsing.multiplicity.singleton,opts.VarParsing.varType.string,'pt min cut when do dphillJet')
options.register ('DphiJetVetominEta',"5.",opts.VarParsing.multiplicity.singleton,opts.VarParsing.varType.string,'max jet eta cut')
options.register ('minPtBVeto',"10.",opts.VarParsing.multiplicity.singleton,opts.VarParsing.varType.string,'min pt for bveto')
options.register ('DzBVeto',"999.",opts.VarParsing.multiplicity.singleton,opts.VarParsing.varType.string,'dz cut when do bveto')

options.parseArguments()

###################################
#### Basic Process declaration ####
###################################

process = cms.Process('PAT')

# import of standard configurations
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff') 
process.load('Configuration.StandardSequences.MagneticField_38T_PostLS1_cff')
process.load('Configuration.StandardSequences.EndOfProcess_cff')
process.load("FWCore.MessageService.MessageLogger_cfi")

process.MessageLogger.destinations = ['cout', 'cerr']
process.MessageLogger.cerr.FwkReport.reportEvery = 1

if options.acceptDuplicates :
    process.source = cms.Source('PoolSource',
          fileNames = cms.untracked.vstring( options.inputFiles ),
          skipEvents = cms.untracked.uint32( options.skipEvents ),
          duplicateCheckMode = cms.untracked.string('noDuplicateCheck'),
          )
else :
    process.source = cms.Source('PoolSource',
          fileNames = cms.untracked.vstring( options.inputFiles ),
          skipEvents = cms.untracked.uint32( options.skipEvents )
          )


process.options = cms.untracked.PSet(allowUnscheduled = cms.untracked.bool(True))
process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(options.maxEvents) )

from Configuration.AlCa.GlobalTag import GlobalTag
process.GlobalTag = GlobalTag(process.GlobalTag,options.globalTag,'')

###################
## GEOMETRY LOAD ##
###################

if options.cmsGeometry == "Extended2019" :
 process.load('Configuration.Geometry.GeometryExtended2019Reco_cff')
elif options.cmsGeometry == "Extended2023" :
 process.load('Configuration.Geometry.GeometryExtended2023_cff')
elif options.cmsGeometry == "Extended2023Pixel" :
 process.load('Configuration.Geometry.GeometryExtended2023Pixel_cff')
elif options.cmsGeometry == "Extended2023TTI" :
 process.load('Configuration.Geometry.GeometryExtended2023TTIReco_cff')
elif options.cmsGeometry == "Extended2023Muon" :
 process.load('Configuration.Geometry.GeometryExtended2023MuonReco_cff')
elif options.cmsGeometry == "Extended2023Muon4Eta" :
 process.load('Configuration.Geometry.GeometryExtended2023Muon4EtaReco_cff')
elif options.cmsGeometry == "Extended2023HGCal" :
 process.load('Configuration.Geometry.GeometryExtended2023HGCalReco_cff')
elif options.cmsGeometry == "Extended2023HGCalMuon": 
 process.load('Configuration.Geometry.GeometryExtended2023HGCalMuonReco_cff')
elif options.cmsGeometry == "Extended2023HGCalMuon4Eta": 
 process.load('Configuration.Geometry.GeometryExtended2023HGCalMuon4EtaReco_cff')
elif options.cmsGeometry == "Extended2023SHCal": 
 process.load('Configuration.Geometry.GeometryExtended2023SHCalReco_cff')
elif options.cmsGeometry == "Extended2023SHCal4Eta": 
 process.load('Configuration.Geometry.GeometryExtended2023SHCal4EtaReco_cff')
elif options.cmsGeometry == "Extended2023SHCalNoTaper": 
 process.load('Configuration.Geometry.GeometryExtended2023SHCalNoTaperReco_cff')
elif options.cmsGeometry == "Extended2023SHCalNoTaper4Eta":
 process.load('Configuration.Geometry.GeometryExtended2023SHCalNoTaper4EtaReco_cff')
elif options.cmsGeometry == "Extended2023CFCal": 
 process.load('Configuration.Geometry.GeometryExtended2023CFCal_cff')
elif options.cmsGeometry == "Extended2023CFCal4Eta": 
 process.load('Configuration.Geometry.GeometryExtended2023CFCal4Eta_cff')
else:
 sys.exit("Problem with CMSSW BASE GEOMETRY --> not known --> exit");


###########################
### MET FILTER SEQUENCE ###
###########################

if options.doMETFilter :

 from RecoMET.METFilters.metFilters_cff           import goodVertices
 from CommonTools.RecoAlgos.HBHENoiseFilter_cfi   import *
 from RecoMET.METFilters.CSCTightHaloFilter_cfi   import *
 from RecoMET.METFilters.hcalLaserEventFilter_cfi import *
 from RecoMET.METFilters.EcalDeadCellTriggerPrimitiveFilter_cfi import *
 from RecoMET.METFilters.eeBadScFilter_cfi         import *
 from RecoMET.METFilters.ecalLaserCorrFilter_cfi   import *
 from RecoMET.METFilters.trackingFailureFilter_cfi import *
 from RecoMET.METFilters.trackingPOGFilters_cff    import *
 from RecoMET.METFilters.trackingPOGFilters_cfi    import *

 #process.HBHENoiseFilter          = HBHENoiseFilter.clone() # crash on upgrade sample
 process.CSCTightHaloFilter       = CSCTightHaloFilter.clone()
 process.hcalLaserEventFilter     = hcalLaserEventFilter.clone()
 #process.EcalDeadCellTriggerPrimitiveFilter = EcalDeadCellTriggerPrimitiveFilter.clone() # crash on upgrade samples
 process.goodVertices             = goodVertices.clone()
 process.trackingFailureFilter    = trackingFailureFilter.clone()
 #process.eeBadScFilter            = eeBadScFilter.clone() # crash on upgrade samples
 process.ecalLaserCorrFilter      = ecalLaserCorrFilter.clone()
 process.manystripclus53X         = manystripclus53X.clone()
 process.toomanystripclus53X      = toomanystripclus53X.clone()
 process.logErrorTooManyClusters  = logErrorTooManyClusters.clone()

 process.metFilters = cms.Sequence( #process.HBHENoiseFilter*
                                    process.CSCTightHaloFilter*
                                    process.hcalLaserEventFilter*
                                    #process.EcalDeadCellTriggerPrimitiveFilter* 
                                    process.goodVertices*
                                    #process.eeBadScFilter*
                                    process.ecalLaserCorrFilter*
                                   ~process.manystripclus53X * ~process.toomanystripclus53X * ~process.logErrorTooManyClusters )
                                     
 
###############################
### START THE PAT SEQUENCE ####
###############################

if str(os.getenv('CMSSW_VERSION')).find('SLHC') :

 ### Egamma Fix before PAT sequence -> for photon and electron ID info
 from RecoEgamma.ElectronIdentification.electronIdCutBased_cfi import eidCutBased
 from RecoEgamma.ElectronIdentification.electronIdSequence_cff import eidRobustLoose, eidRobustTight, eidRobustHighEnergy, eidLoose, eidTight

 process.eleIdRobustLoose      = eidRobustLoose.clone( src = cms.InputTag("gedGsfElectrons"))
 process.eleIdRobustTight      = eidRobustTight.clone( src = cms.InputTag("gedGsfElectrons"))
 process.eleIdRobustHighEnergy = eidRobustHighEnergy.clone(src = cms.InputTag("gedGsfElectrons"))
 process.eleIdLoose            = eidLoose.clone( src = cms.InputTag("gedGsfElectrons"))
 process.eleIdTight            = eidTight.clone( src = cms.InputTag("gedGsfElectrons"))


 process.eleIdSequence = cms.Sequence(process.eleIdRobustLoose*process.eleIdRobustTight*process.eleIdRobustHighEnergy*process.eleIdLoose*process.eleIdTight)

 from RecoEgamma.PhotonIdentification.photonId_cff import photonIDSequence
 from RecoEgamma.PhotonIdentification.photonId_cfi import PhotonIDProd
 process.PhotonIDProdGED = PhotonIDProd.clone(photonProducer = cms.string('gedPhotons'))

 process.egammaIdentification = cms.Sequence(process.eleIdSequence+
                                             process.PhotonIDProdGED)


 ####################################################
 ## load the patSequence and selected pat sequence ##
 ####################################################

 ## PAT electron  
 process.load('PhysicsTools.PatAlgos.producersLayer1.electronProducer_cff') 

 ## build particle flow fwd pointers
 process.load('RecoParticleFlow.PFProducer.particleFlowTmpPtrs_cfi')
 process.particleFlowTmpPtrs.src = cms.InputTag("particleFlow") 
 process.makePatElectrons.replace(process.electronMatch,process.particleFlowTmpPtrs*process.electronMatch)

 ## build CHS subtraction
 process.load('CommonTools.ParticleFlow.pfNoPileUpIso_cff')
 process.pfNoPileUpIsoSequence = cms.Sequence(process.pfPileUpIso*process.pfNoPileUpIso)
 process.makePatElectrons.replace(process.electronMatch,process.pfNoPileUpIsoSequence*process.electronMatch)

 ## build particles division for isolation
 process.load('CommonTools.ParticleFlow.ParticleSelectors.pfSortByType_cff')
 process.makePatElectrons.replace(process.electronMatch,process.pfSortByTypeSequence*process.electronMatch)

 ## build isoDeposits
 process.load('RecoParticleFlow.PFProducer.electronPFIsolationDeposits_cff')
 process.elPFIsoDepositCharged.src    = cms.InputTag("gedGsfElectrons")
 process.elPFIsoDepositChargedAll.src = cms.InputTag("gedGsfElectrons")
 process.elPFIsoDepositGamma.src      = cms.InputTag("gedGsfElectrons")
 process.elPFIsoDepositNeutral.src    = cms.InputTag("gedGsfElectrons")                            
 process.elPFIsoDepositPU.src         = cms.InputTag("gedGsfElectrons")                                            
 process.makePatElectrons.replace(process.electronMatch,process.electronPFIsolationDepositsSequence*process.electronMatch)

 ## build isolation values
 from RecoParticleFlow.PFProducer.electronPFIsolationValues_cff import *
 process.elPFIsoValueCharged03PFId    = elPFIsoValueCharged03PFId.clone()
 process.elPFIsoValueChargedAll03PFId = elPFIsoValueChargedAll03PFId.clone()
 process.elPFIsoValueGamma03PFId      = elPFIsoValueGamma03PFId.clone()
 process.elPFIsoValueNeutral03PFId    = elPFIsoValueNeutral03PFId.clone()
 process.elPFIsoValuePU03PFId         = elPFIsoValuePU03PFId.clone()
 process.elPFIsoValueCharged04PFId    = elPFIsoValueCharged04PFId.clone()
 process.elPFIsoValueChargedAll04PFId = elPFIsoValueChargedAll04PFId.clone()
 process.elPFIsoValueGamma04PFId      = elPFIsoValueGamma04PFId.clone()
 process.elPFIsoValueNeutral04PFId    = elPFIsoValueNeutral04PFId.clone()
 process.elPFIsoValuePU04PFId         = elPFIsoValuePU04PFId.clone()

 process.electronPFIsolationValuesSequence = cms.Sequence( process.elPFIsoValueCharged03PFId*process.elPFIsoValueChargedAll03PFId*
                                                           process.elPFIsoValueGamma03PFId* process.elPFIsoValueNeutral03PFId*
                                                           process.elPFIsoValuePU03PFId* process.elPFIsoValueCharged04PFId*
                                                           process.elPFIsoValueChargedAll04PFId* process.elPFIsoValueGamma04PFId*
                                                           process.elPFIsoValueNeutral04PFId* process.elPFIsoValuePU04PFId)
                  
 process.makePatElectrons.replace(process.electronMatch,process.electronPFIsolationValuesSequence*process.electronMatch)

 ## build electron match with gen particles
 process.electronMatch.src = cms.InputTag("gedGsfElectrons")

 ## build pat electrons
 process.patElectrons.electronSource = cms.InputTag("gedGsfElectrons")

 process.patElectrons.electronIDSources = cms.PSet(
        eidTight = cms.InputTag("eleIdTight"),
        eidLoose = cms.InputTag("eleIdLoose"),
        eidRobustTight = cms.InputTag("eleIdRobustTight"),
        eidRobustHighEnergy = cms.InputTag("eleIdRobustHighEnergy"),
        eidRobustLoose = cms.InputTag("eleIdRobustLoose")
 )

 process.patElectrons.isolationValues = cms.PSet(
        pfNeutralHadrons = cms.InputTag("elPFIsoValueNeutral04PFId"),
        pfChargedAll = cms.InputTag("elPFIsoValueChargedAll04PFId"),
        pfPUChargedHadrons = cms.InputTag("elPFIsoValuePU04PFId"),
        pfPhotons = cms.InputTag("elPFIsoValueGamma04PFId"),
        pfChargedHadrons = cms.InputTag("elPFIsoValueCharged04PFId")
 )
 process.patElectrons.isolationValuesNoPFId = cms.PSet(
        pfNeutralHadrons = cms.InputTag("elPFIsoValueNeutral03PFId"),
        pfChargedAll = cms.InputTag("elPFIsoValueChargedAll03PFId"),
        pfPUChargedHadrons = cms.InputTag("elPFIsoValuePU03PFId"),
        pfPhotons = cms.InputTag("elPFIsoValueGamma03PFId"),
        pfChargedHadrons = cms.InputTag("elPFIsoValueCharged03PFId")
 )
 
 
 ### PAT muon fix
 process.load('PhysicsTools.PatAlgos.producersLayer1.muonProducer_cff') 

 ## build iso deposits 
 process.load('RecoMuon.MuonIsolation.muonPFIsolationDeposits_cff')
 process.muPFIsoDepositCharged.src    = cms.InputTag("muons")
 process.muPFIsoDepositChargedAll.src = cms.InputTag("muons")
 process.muPFIsoDepositGamma.src      = cms.InputTag("muons")
 process.muPFIsoDepositNeutral.src    = cms.InputTag("muons")                            
 process.muPFIsoDepositPU.src         = cms.InputTag("muons")                            

 process.makePatMuons.replace(process.muonMatch,process.muonPFIsolationDepositsSequence*process.muonMatch)

 ## build iso values 
 from RecoMuon.MuonIsolation.muonPFIsolationValues_cff import*
 process.muPFIsoValueCharged03    = muPFIsoValueCharged03.clone()
 process.muPFIsoValueChargedAll03 = muPFIsoValueChargedAll03.clone()
 process.muPFIsoValueGamma03      = muPFIsoValueGamma03.clone()
 process.muPFIsoValueNeutral03    = muPFIsoValueNeutral03.clone()
 process.muPFIsoValuePU03         = muPFIsoValuePU03.clone()

 process.muPFIsoValueCharged04    = muPFIsoValueCharged04.clone()
 process.muPFIsoValueChargedAll04 = muPFIsoValueChargedAll04.clone()
 process.muPFIsoValueGamma04      = muPFIsoValueGamma04.clone()
 process.muPFIsoValueNeutral04    = muPFIsoValueNeutral04.clone()
 process.muPFIsoValuePU04         = muPFIsoValuePU04.clone()

 process.muonPFIsolationValuesSequence = cms.Sequence(process.muPFIsoValueCharged03*process.muPFIsoValueChargedAll03*
                                                      process.muPFIsoValueGamma03*process.muPFIsoValueNeutral03*process.muPFIsoValuePU03*
                                                      process.muPFIsoValueCharged04*process.muPFIsoValueChargedAll04*
                                                      process.muPFIsoValueGamma04*process.muPFIsoValueNeutral04*process.muPFIsoValuePU04)

 process.makePatMuons.replace(process.muonMatch,process.muonPFIsolationValuesSequence*process.muonMatch)
  
 ## build pat muons
 process.patMuons.muonSource       = cms.InputTag("muons")
 process.patMuons.embedPFCandidate = cms.bool(True)
 process.patMuons.embedPFCandidate = cms.bool(True)
 process.patMuons.embedTrack       = cms.bool(True) 
 process.patMuons.embedTunePMuonBestTrack = cms.bool(True)
 process.patMuons.embedMuonBestTrack      = cms.bool(True)
 process.patMuons.embedCombinedMuon       = cms.bool(True)
 process.patMuons.isolationValues = cms.PSet(
        pfNeutralHadrons    = cms.InputTag("muPFIsoValueNeutral04"),
        pfChargedAll        = cms.InputTag("muPFIsoValueChargedAll04"),
        pfPUChargedHadrons  = cms.InputTag("muPFIsoValuePU04"),
        pfPhotons           = cms.InputTag("muPFIsoValueGamma04"),
        pfChargedHadrons    = cms.InputTag("muPFIsoValueCharged04")
 )

 #### PAT photon fix
 process.load('PhysicsTools.PatAlgos.producersLayer1.photonProducer_cff')
 
 ### build iso deposits
 process.load('RecoParticleFlow.PFProducer.photonPFIsolationDeposits_cff')
 process.phPFIsoDepositCharged.src    = cms.InputTag("gedPhotons")
 process.phPFIsoDepositChargedAll.src = cms.InputTag("gedPhotons")
 process.phPFIsoDepositGamma.src      = cms.InputTag("gedPhotons")
 process.phPFIsoDepositNeutral.src    = cms.InputTag("gedPhotons")                            
 process.phPFIsoDepositPU.src         = cms.InputTag("gedPhotons")                            

 process.makePatPhotons.replace(process.photonMatch,process.photonPFIsolationDepositsSequence*process.photonMatch)

 ### build iso values --> not necessary since in SLHC relase no isolation values can be filled directly
 process.photonMatch.src = cms.InputTag("gedPhotons")
 
 ## pat photons
 process.patPhotons.photonIDSources = cms.PSet(
        PhotonCutBasedIDTight = cms.InputTag("PhotonIDProdGED","PhotonCutBasedIDTight"),
        PhotonCutBasedIDLoose = cms.InputTag("PhotonIDProdGED","PhotonCutBasedIDLoose")
 )
 process.patPhotons.photonSource = cms.InputTag("gedPhotons")
 process.patPhotons.addPhotonID = cms.bool(True)

 
 ##### fix for patJets
 process.load('RecoBTag.Configuration.RecoBTag_cff')
 
 ## build jet energy corrections
 process.makePatJets = cms.Sequence()
 process.load('PhysicsTools.PatAlgos.recoLayer0.jetCorrections_cff')
 process.patJetCorrFactors.payload          = cms.string('AK5PFchs')
 process.patJetCorrFactors.src              = cms.InputTag("ak5PFJetsCHS")
 process.patJetCorrFactors.rho              = cms.InputTag("fixedGridRhoFastjetAll")
 process.makePatJets += process.patJetCorrFactors

 ## jet track association 
 from RecoJets.JetAssociationProducers.ak5JTA_cff import ak5JetTracksAssociatorAtVertexPF
 process.jetTracksAssociatorAtVertex = ak5JetTracksAssociatorAtVertexPF.clone()
 process.makePatJets += process.jetTracksAssociatorAtVertex
 
 ## jet charge 
 process.load('PhysicsTools.PatAlgos.recoLayer0.jetTracksCharge_cff')
 process.patJetCharge.src      = cms.InputTag("jetTracksAssociatorAtVertex")
 process.makePatJets += process.patJetCharge
 
 ## parton and gen jet match
 process.load('PhysicsTools.PatAlgos.mcMatchLayer0.jetMatch_cfi')
 process.patJetPartonMatch.src = cms.InputTag("ak5PFJetsCHS")
 process.patJetGenJetMatch.src = cms.InputTag("ak5PFJetsCHS")
 process.makePatJets += process.patJetPartonMatch
 process.makePatJets += process.patJetGenJetMatch


 ## jet flavour
 process.load('PhysicsTools.PatAlgos.mcMatchLayer0.jetFlavourId_cff')
 process.patJetPartonAssociationLegacy.src  = cms.InputTag("ak5PFJetsCHS")
 process.patJetFlavourAssociation.jets      = cms.InputTag("ak5PFJetsCHS")
 process.makePatJets += process.patJetFlavourId 
 process.makePatJets += process.patJetFlavourAssociation
 
  
 ## load btagging dictionary
 process.load('PhysicsTools.PatAlgos.recoLayer0.bTagging_cff')

 ## build pile-up jet ID
 from RecoJets.JetProducers.PileupJetIDParams_cfi import full_5x_chs
 full_5x_chs.tmvaWeights =  cms.string("LatinoTrees/AnalysisStep/data/TMVAClassification_5x_BDT_chsFullPlusRMS.weights.xml")

 from RecoJets.JetProducers.PileupJetID_cfi import pileupJetIdProducerChs
 process.pileupJetId = pileupJetIdProducerChs.clone( rho = cms.InputTag("fixedGridRhoFastjetAll"),
                                                     jets = cms.InputTag("ak5PFJetsCHS"),
                                                     residualsTxt = cms.FileInPath('LatinoTrees/AnalysisStep/data/download.url'))
               
 process.makePatJets += process.pileupJetId

 ## build pat jets
 process.load('PhysicsTools.PatAlgos.producersLayer1.jetProducer_cfi')
 process.patJets.addGenJetMatch      = cms.bool(True)
 process.patJets.addGenJetMatch      = cms.bool(True)
 process.patJets.embedGenJetMatch    = cms.bool(True)
 process.patJets.addGenPartonMatch   = cms.bool(True)
 process.patJets.embedGenPartonMatch = cms.bool(True)
 process.patJets.addPartonJetMatch   = cms.bool(True)
 process.patJets.addJetCharge        = cms.bool(True)
 process.patJets.addAssociatedTracks = cms.bool(True)
 process.patJets.addEfficiencies     = cms.bool(False)
 process.patJets.addResolutions      = cms.bool(False)
 process.patJets.addJetID            = cms.bool(True)
 process.patJets.addDiscriminators   = cms.bool(True)
 process.patJets.embedPFCandidates   = cms.bool(False)
 process.patJets.addJetFlavourInfo   = cms.bool(True)
 process.patJets.getJetMCFlavour     = cms.bool(True)
 process.patJets.useLegacyJetMCFlavour = cms.bool(False)
 process.patJets.addBTagInfo         = cms.bool(True)
 process.patJets.addTagInfos         = cms.bool(True)
 process.patJets.userData = cms.PSet(
        userCands = cms.PSet(
            src = cms.VInputTag("")),
        userInts = cms.PSet(
            src = cms.VInputTag(cms.InputTag("pileupJetId","fullId"), "")),
        userFloats = cms.PSet(
            src = cms.VInputTag(cms.InputTag("pileupJetId","fullDiscriminant"))),
        userClasses = cms.PSet(
            src = cms.VInputTag("")),
        userFunctionLabels = cms.vstring(),
        userFunctions = cms.vstring()
 )

 process.patJets.jetSource = cms.InputTag("ak5PFJetsCHS")
 process.patJets.discriminatorSources = cms.VInputTag(cms.InputTag("jetBProbabilityBJetTags"), 
                                                      cms.InputTag("jetProbabilityBJetTags"), 
                                                      cms.InputTag("trackCountingHighPurBJetTags"), 
                                                      cms.InputTag("trackCountingHighEffBJetTags"), 
                                                      cms.InputTag("simpleSecondaryVertexHighEffBJetTags"),
                                                      cms.InputTag("simpleSecondaryVertexHighPurBJetTags"), 
                                                      cms.InputTag("combinedSecondaryVertexBJetTags"),
                                                      cms.InputTag("combinedSecondaryVertexMVABJetTags"))
 process.patJets.trackAssociationSource = cms.InputTag("jetTracksAssociatorAtVertex")
 process.makePatJets += process.patJets

 
 ## build pat MET   
 process.makePatMETs = cms.Sequence()

 ## met corrections for ak5 PF jets  with CHS
 process.load('JetMETCorrections.Type1MET.pfMETCorrections_cff')
 process.ak5PFJetsPtrs.src = cms.InputTag("ak5PFJetsCHS")
 process.pfJetMETcorr.src  = cms.InputTag('ak5PFJetsCHS')
 process.makePatMETs += process.producePFMETCorrections
 
 
 process.load('PhysicsTools.PatAlgos.producersLayer1.metProducer_cfi')
 process.patMETs.metSource = cms.InputTag("pfType1CorrectedMet")
 process.makePatMETs += process.patMETs

 ## summary
 from PhysicsTools.PatAlgos.producersLayer1.patCandidates_cff import patCandidateSummary
 process.patCandidateSummary = patCandidateSummary.clone()

 ## load the selected pat sequence
 process.load('PhysicsTools.PatAlgos.selectionLayer1.electronSelector_cfi')
 process.load('PhysicsTools.PatAlgos.selectionLayer1.muonSelector_cfi')
 process.load('PhysicsTools.PatAlgos.selectionLayer1.photonSelector_cfi')
 process.load('PhysicsTools.PatAlgos.selectionLayer1.jetSelector_cfi')
 from PhysicsTools.PatAlgos.selectionLayer1.selectedPatCandidates_cff import selectedPatCandidateSummary
 process.selectedPatCandidateSummary = selectedPatCandidateSummary.clone()

 process.selectedPatCandidates = cms.Sequence( process.selectedPatElectrons*
                                               process.selectedPatMuons*
                                               process.selectedPatPhotons*
                                               process.selectedPatJets*
                                               process.selectedPatCandidateSummary)
                                             

else:
 sys.exit("Problem with the CMSSW BASE VERSION --> not recongnized");


#############################
####### PUPPI JETS ##########
#############################
process.miniAODPAT = cms.Path() 

if options.runPUPPISequence:

  from LatinoTrees.AnalysisStep.puppiSequence_cff import makePuppiAlgo, makePatPuppiJetSequence 
  jetPuppiR = 0.5
  makePuppiAlgo(process) ## call puppi producer and puppi met
  makePatPuppiJetSequence(process,jetPuppiR)  ## call pat puppi jets

##############################
#### Output definition #######
##############################

if options.producePATObjects:
 
 if str(os.getenv('CMSSW_VERSION')).find('SLHC') :

  process.MINIAODSIMoutput = cms.OutputModule("PoolOutputModule",
    compressionLevel = cms.untracked.int32(4),
    compressionAlgorithm = cms.untracked.string('LZMA'),
    eventAutoFlushCompressedSize = cms.untracked.int32(15728640),
    outputCommands = cms.untracked.vstring('drop *',
                                           'keep *_pat*_*_PAT',
    ),
    fileName = cms.untracked.string('miniAOD-prod_PAT.root'),
    dataset = cms.untracked.PSet(
        filterName = cms.untracked.string(''),
        dataTier = cms.untracked.string('')
    )
  )

  process.MINIAODSIMoutput.outputCommands += cms.untracked.vstring('keep *_addPileupInfo_*_*')
  process.MINIAODSIMoutput_step = cms.EndPath(process.MINIAODSIMoutput)

  if options.doMETFilter :
   process.miniAODPAT += process.metFilters   

  process.miniAODPAT +=  process.egammaIdentification ## egamma reduction sequences                                                                                                
  process.miniAODPAT +=  process.makePatElectrons ## make PAT electrons
  process.miniAODPAT +=  process.makePatMuons ## make PAT muons
  process.miniAODPAT +=  process.makePatPhotons ## make PAT muons
  process.miniAODPAT +=  process.makePatJets ## make PAT jets
  process.miniAODPAT +=  process.makePatMETs ## make PAT met
  process.miniAODPAT +=  process.patCandidateSummary
  process.miniAODPAT +=  process.selectedPatCandidates

  if options.runPUPPISequence:
   process.miniAODPAT += process.puppiSequence ## add puppi particle sequence to the path
   process.miniAODPAT += process.AK5makePatJetsPuppi
  

 else:
  sys.exit("Problem with the CMSSW BASE VERSION --> not recongnized");
  
else :

 process.TFileService = cms.Service("TFileService",fileName = cms.string(options.outputFile))

 if str(os.getenv('CMSSW_VERSION')).find('SLHC') :

  ### in this case run Latino analysis and miniAOD objects by hand 
  process.miniAODPAT = cms.Path()

  if options.doMETFilter :
   process.miniAODPAT += process.metFilters   

  process.miniAODPAT += process.egammaIdentification ## egamma reduction sequences
  process.miniAODPAT += process.makePatElectrons     ## make PAT electrons
  process.miniAODPAT += process.makePatMuons ## make PAT muons
  process.miniAODPAT += process.makePatPhotons ## make PAT photons
  process.miniAODPAT += process.makePatJets ## make PAT jets
  process.miniAODPAT += process.makePatMETs  ## make PAT met
  process.miniAODPAT += process.patCandidateSummary
  process.miniAODPAT += process.selectedPatCandidates

  if options.runPUPPISequence:
   process.miniAODPAT += process.puppiSequence ## add puppi particle sequence to the path
   process.miniAODPAT += process.AK5makePatJetsPuppi
 
 else:
  sys.exit("Problem with the CMSSW BASE VERSION --> not recongnized");
 
 
 ##############################
 #### RUN LATINO Analysis #####
 ##############################

 #### Run Skim Event producer
 from LatinoTrees.AnalysisStep.skimEventProducer_cfi import skimEventProducer ## take the skim event producer

 label = options.label ;

 if options.selection == 'TightTight':
    label = "Scenario6"; muon = "selectedPatMuons"; ele = "selectedPatElectrons"; softmu = "selectedPatMuons"; preSeq = cms.Sequence(); 
 elif options.selection == 'LooseLoose':
    label = "Scenario7"; muon = "wwMuScenario7"; ele = "wwEleScenario5"; softmu = "wwMu4VetoScenario6"; preSeq = cms.Sequence();
 else:
    raise ValueError('selection must be either TightTight or LooseLoose')


 from LatinoTrees.AnalysisStep.skimEventProducer_cfi import addEventHypothesis

 skimEventProducer.triggerTag = cms.InputTag("TriggerResults","","RECO")

 addEventHypothesis(process,skimEventProducer,label,muon,ele,softmu,"AK"+str(int(jetPuppiR*10))+"selectedPatJetsPuppi","patMetPuppi",preSeq,True)
 process.miniAODPAT += getattr(process,'sel'+label)

 ######## start latino analysis
 process.load("LatinoTrees.AnalysisStep.stepB_cff")
 ### fix nPU collection
 process.nPU.puLabel = cms.InputTag(options.puInformation)
 process.nPU.src = cms.InputTag("ww%s"% (label))

 process.miniAODPAT += process.nPU

 process.nverticesModule.probes = cms.InputTag("ww%s"% (label))
 process.miniAODPAT += process.nverticesModule


 from LatinoTrees.AnalysisStep.stepB_cff import * 
 ## create the basic tree structure with some parameters as input( basic branches)
 basicStepBTreeDefinition(process,options.jetIdWP,options.pileupjetIdWP,options.CJVminPt,options.CJVmaxEta,options.DphiJetVetominPt,options.DphiJetVetominEta,options.DzBVeto,options.minPtBVeto)
 ## fix the input collection
 process.stepBTree.src = cms.InputTag("ww%s"% (label));

 process.stepBTree.variables.nvtx = cms.InputTag("nverticesModule")

 process.miniAODPAT += process.stepBTree 

 if options.runPUPPISequence: ## clone jet branches for puppi
   addPuppiVariables(process,options.jetIdWP,options.pileupjetIdWP,options.CJVminPt,options.CJVmaxEta,options.DphiJetVetominPt,options.DphiJetVetominEta,options.DzBVeto,options.minPtBVeto)
 
 doEleIsoId = options.doEleIsoId
 doLHE      = options.doLHE
 doGen      = options.doGen
 doHiggs    = options.doHiggs
 doSusy     = options.doSusy
 SameSign   = options.doSameSign
 doNoFilter = options.doNoFilter
 typeLHEcomment = options.typeLHEcomment
 
 id       = 0
 json     = None
 mhiggs   = 0
 wztth    = False
 dy       = False

 fourthGenSF = 1
 fermiSF     = 1
 puStudy     = False ## set to true to add 16, yes 16 different PU possibilities                                                                                                     
 IsoStudy    = False ## Set to True to get isolation variables (and a tree build only after ID+CONV+IP, without isolation)                                                                 
 Summer11    = False # set to true if you need to run the Summer11 (changes the PU distro)                                                                                       
 Fall11      = False # set to true if you need to run the Fall11 (changes the PU distro)                                                                                                   

 if '2011' in label: label = label[:label.find('2011')]

 if '2012' in label: label = label[:label.find('2012')]

 if label in [ 'SingleElectron', 'DoubleElectron', 'SingleMuon', 'DoubleMuon', 'MuEG']:
    dataset = [label]
    id = options.id
    json = options.json
    scalef = 1
    doPDFvar = False
    doGen = false
    doLHE = false
 else:
    dataset = ['MC', label];
    id = options.id;
    scalef = options.scale
    dowztth = re.match("wzttH*", label)
    m = re.match("ggToH(\\d+)to.*", label)
    n = re.match("vbfToH(\\d+)to.*", label)
    r = re.match("Graviton2PM*", label)
    s = re.match("Higgs0M*", label)
    t = re.match("SMH125*", label)
    doPDFvar = True
    if m:
        mhiggs = int(m.group(1))
        fourthGenSF = fourthGenScales[int(m.group(1))]
        fermiSF = 0
    elif n:
        mhiggs = -1*int(n.group(1))
        fermiSF = fermiPhobicScales[int(n.group(1))]
    elif 'DY' in label and ('ElEl' in label or 'MuMu' in label):
        dy = True
    elif dowztth:
        wztth = True
    if m or n or dowztth or r or s or t:
        doHiggs = True

 process.stepBTree.cut = process.stepBTree.cut.value().replace("DATASET", dataset[0])
 process.stepBTree.variables.trigger = process.stepBTree.variables.trigger.value().replace("DATASET",dataset[0])
 process.stepBTree.variables.dataset = str(id)

 if dataset[0] == "MC":
    process.stepBTree.variables.baseW = cms.string("%.12f" % scalef)
    if mhiggs != 0:
        process.stepBTree.variables.fourW = cms.string("%.12f" % fourthGenSF)
        process.stepBTree.variables.fermiW = cms.string("%.12f" % fermiSF)
    else:
        process.stepBTree.variables.fourW = cms.string("1")
        process.stepBTree.variables.fermiW = cms.string("1")
    if mhiggs <=0:
        process.stepBTree.variables.kfW = cms.string("1")
    else:
        process.higgsPt.inputFilename = cms.string("HiggsAnalysis/HiggsToWW2Leptons/data/kfactors_Std/kfactors_mh%(mass)d_ren%(mass)d_fac%(mass)d.dat" % {"mass":abs(mhiggs)})
 else:
    from FWCore.PythonUtilities.LumiList import LumiList
    import os
    lumis = LumiList(filename = os.getenv('CMSSW_BASE')+'/src/LatinoTrees/Misc/Jsons/%s.json'%json)
    process.source.lumisToProcess = cms.untracked.VLuminosityBlockRange()
    process.source.lumisToProcess = lumis.getCMSSWString().split(',')
    process.stepBTree.variables.baseW = cms.string("1")
    process.stepBTree.variables.fourW = cms.string("1")
    process.stepBTree.variables.fermiW = cms.string("1")
    process.stepBTree.variables.kfW = cms.string("1")
    process.stepBTree.variables.trpu = cms.string("1")
    process.stepBTree.variables.itpu = cms.string("1")
    process.stepBTree.variables.ootpup1 = cms.string("1")
    process.stepBTree.variables.ootpum1 = cms.string("1")
    process.stepBTree.variables.puW = cms.string("1")
    process.stepBTree.variables.puAW = cms.string("1")
    process.stepBTree.variables.puBW = cms.string("1")
 
 if (wztth == True) or (doPDFvar == True):
    getattr(process,"ww%s"% (label)).mcGenEventInfoTag = "generator"
    getattr(process,"ww%s"% (label)).genParticlesTag   = "packedGenParticles"
 if doSusy == True :
    getattr(process,"ww%s"% (label)).genParticlesTag   = "packedGenParticles"
 if doHiggs == True :
    getattr(process,"ww%s"% (label)).genParticlesTag   = "packedGenParticles"
 if doLHE == True :
    getattr(process,"ww%s"% (label)).mcLHEEventInfoTag = "source"
    getattr(process,"ww%s"% (label)).whichLHE = cms.untracked.int32(typeLHEcomment)
 if doGen == True :
    getattr(process,"ww%s"% (label)).genParticlesTag = "packedGenParticles"
    getattr(process,"ww%s"% (label)).genMetTag = "genMetTrue"
    getattr(process,"ww%s"% (label)).genJetTag = cms.InputTag("slimmedGenJets")
 if id in ["036", "037", "037c0", "037c1", "037c2", "037c3", "037c4", "037c5", "037c6", "037c7", "037c8", "037c9", "042", "043", "045", "046" ]: # DY-Madgraph sample    
    getattr(process,"ww%s"% (label)).genParticlesTag = "packedGenParticles"


 ## add more isolation variables
 if IsoStudy: addIsoStudyVariables(process,process.stepBTree)
 
 # electron id and iso variables                                                                                                                                                         
 if doEleIsoId: addEleIdIsoVariables(process,process.stepBTree)

 # LHE information dumper                                                                                                                                                                 
 if doLHE:
  addLHEVariables(process,process.stepBTree)

  process.stepBTree.variables.numbLHE = cms.string("numberOfbQuarks()")
  process.stepBTree.variables.numtLHE = cms.string("numberOftQuarks()")

  process.stepBTree.variables.HEPMCweightScale0 = cms.string("getHEPMCweightScale(0)")
  process.stepBTree.variables.HEPMCweightScale1 = cms.string("getHEPMCweightScale(1)")
  process.stepBTree.variables.HEPMCweightScale2 = cms.string("getHEPMCweightScale(2)")
  process.stepBTree.variables.HEPMCweightScale3 = cms.string("getHEPMCweightScale(3)")
  process.stepBTree.variables.HEPMCweightScale4 = cms.string("getHEPMCweightScale(4)")
  process.stepBTree.variables.HEPMCweightScale5 = cms.string("getHEPMCweightScale(5)")
  process.stepBTree.variables.HEPMCweightScale6 = cms.string("getHEPMCweightScale(6)")

  if typeLHEcomment == 1 :
     for i in range (70) :
         exec("process.stepBTree.variables.HEPMCweightScale" + str(i+7) + " = cms.string(\"getHEPMCweightScale(" + str(i+7) + ")\")")

  process.stepBTree.variables.HEPMCweightRen0 = cms.string("getHEPMCweightRen(0)")
  process.stepBTree.variables.HEPMCweightRen1 = cms.string("getHEPMCweightRen(1)")
  process.stepBTree.variables.HEPMCweightRen2 = cms.string("getHEPMCweightRen(2)")
  process.stepBTree.variables.HEPMCweightRen3 = cms.string("getHEPMCweightRen(3)")
  process.stepBTree.variables.HEPMCweightRen4 = cms.string("getHEPMCweightRen(4)")
  process.stepBTree.variables.HEPMCweightRen5 = cms.string("getHEPMCweightRen(5)")
  process.stepBTree.variables.HEPMCweightRen6 = cms.string("getHEPMCweightRen(6)")

  process.stepBTree.variables.HEPMCweightFac0 = cms.string("getHEPMCweightFac(0)")
  process.stepBTree.variables.HEPMCweightFac1 = cms.string("getHEPMCweightFac(1)")
  process.stepBTree.variables.HEPMCweightFac2 = cms.string("getHEPMCweightFac(2)")
  process.stepBTree.variables.HEPMCweightFac3 = cms.string("getHEPMCweightFac(3)")
  process.stepBTree.variables.HEPMCweightFac4 = cms.string("getHEPMCweightFac(4)")
  process.stepBTree.variables.HEPMCweightFac5 = cms.string("getHEPMCweightFac(5)")
  process.stepBTree.variables.HEPMCweightFac6 = cms.string("getHEPMCweightFac(6)")

 
 ## gen info isolation variables
 if doGen: addGenVariables(process,process.stepBTree)
 
 if options.doAdditionalJets: addAdditionalJets(process,process.stepBTree,options.jetIdWP,options.pileupjetIdWP,options.CJVminPt,options.CJVmaxEta,options.DphiJetVetominPt,options.DphiJetVetominEta,options.DzBVeto,options.minPtBVeto)

 if options.doFatJet :
    addFatJets(process,process.stepBTree)

 if dataset[0] == 'MC':
    process.stepBTree.variables.trpu = cms.InputTag("nPU:tr")
    process.stepBTree.variables.itpu = cms.InputTag("nPU:it")
    process.stepBTree.variables.ootpup1 = cms.InputTag("nPU:p1")
    process.stepBTree.variables.ootpum1 = cms.InputTag("nPU:m1")
    if puStudy: addExtraPUWeights(process,process.stepBTree,label,process.miniAODPAT)
    if dy:
        process.dyWeight.src = cms.InputTag("ww%s"% (label))
        process.stepBTree.variables.kfW = cms.InputTag("dyWeight")
        process.miniAODPAT += process.dyWeight
    elif mhiggs > 0:
        process.ptWeight.src = cms.InputTag("ww%s"% (label))
        process.stepBTree.variables.kfW = cms.InputTag("ptWeight")
        process.miniAODPAT += process.higgsPt
        process.miniAODPAT += process.ptWeight

    if id in ["036", "037", "037c0", "037c1", "037c2", "037c3", "037c4", "037c5", "037c6", "037c7", "037c8", "037c9", "042", "043", "045", "046" ]: # DY-Madgraph sample                   
        process.stepBTree.variables.mctruth = cms.string("getFinalStateMC()")

 if id in ["077", "078", "074" ]:
    process.StepBTree.variables.PtZ = cms.string("getZPt()")
    process.stepBTree.variables.MZ = cms.string("getZMass()")
    process.stepBTree.variables.WZchan = cms.string("getWZdecayMC()")

 if wztth == True:
    process.stepBTree.variables.mctruth = cms.string("mcHiggsProd()")
    process.stepBTree.variables.mcHWWdecay = cms.string("getWWdecayMC()")

 if doSusy == True :
    process.stepBTree.variables.susyMstop = cms.string("getSusyStopMass()")
    process.stepBTree.variables.susyMLSP = cms.string("getSusyLSPMass()")

 if doHiggs == True :
    process.stepBTree.variables.MHiggs = cms.string("getHiggsMass()")
    process.stepBTree.variables.PtHiggs = cms.string("getHiggsPt()")
    process.stepBTree.variables.HEPMCweight = cms.string("getHEPMCweight()")

 if doPDFvar == True :
    process.stepBTree.variables.pdfscalePDF = cms.string("getPDFscalePDF()")
    process.stepBTree.variables.pdfx1 = cms.string("getPDFx1()")
    process.stepBTree.variables.pdfx2 = cms.string("getPDFx2()")
    process.stepBTree.variables.pdfid1 = cms.string("getPDFid1()")
    process.stepBTree.variables.pdfid2 = cms.string("getPDFid2()")
    process.stepBTree.variables.pdfx1PDF = cms.string("getPDFx1PDF()")
    process.stepBTree.variables.pdfx2PDF = cms.string("getPDFx2PDF()")


 if IsoStudy:
  getattr(process,"stepBTree").cut = cms.string("!isSTA(0) && !isSTA(1) && leptEtaCut(2.4,2.5) && ptMax > 20 && ptMin > 10 && passesIP && nExtraLep(10) == 0")
 
 if SameSign:
  getattr(process,"stepBTree").cut = cms.string("q(0)*q(1) > 0 && !isSTA(0) && !isSTA(1) && leptEtaCut(2.4,2.5) && ptMax > 20 && ptMin > 10")

 # save all events                                                                                                                                                                        
 if doNoFilter:
  print ">> Dump all events"
  getattr(process,"stepBTree").cut = cms.string("1")
  getattr(process,"skim%s"% (label)).cut = cms.string("nLep >= 0")

############
processDumpFile = open('processDump.py', 'w')
print >> processDumpFile, process.dumpPython()


