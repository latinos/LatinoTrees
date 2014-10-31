import FWCore.ParameterSet.Config as cms
import LatinoTrees.Misc.VarParsing as opts
import re
import sys

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
## kind of selection
options.register ('selection','TightTight',opts.VarParsing.multiplicity.singleton,opts.VarParsing.varType.string,'Selection level [TightTight,LooseLoose]')
options.register ('doSameSign',False,opts.VarParsing.multiplicity.singleton,opts.VarParsing.varType.bool,'Turn on Same Sign mode (can be \'True\' or \'False\'')
options.register ('doNoFilter',False,opts.VarParsing.multiplicity.singleton,opts.VarParsing.varType.bool,
                  'Turn on no filter requirement, not even requiring 2 leptons! Needed for unfolding at GEN (can be \'True\' or \'False\'')
## special things
options.register ('doTauEmbed',False,opts.VarParsing.multiplicity.singleton,opts.VarParsing.varType.bool,'Turn on DY embedding mode (can be \'True\' or \'False\'')
options.register ('doType01met',False,opts.VarParsing.multiplicity.singleton,opts.VarParsing.varType.bool,'Turn on Type01 met correction Sign mode (can be \'True\' or \'False\'')
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
## run puppi flag
options.register ('runPUPPISequence',True,opts.VarParsing.multiplicity.singleton,opts.VarParsing.varType.bool,'Turn on PUPPI jets (can be \'True\' or \'False\'')
## stop at miniAOD level
options.register ('produceMiniAOD',False,opts.VarParsing.multiplicity.singleton,opts.VarParsing.varType.bool,'if true it stops at MiniAOD, otherwise it runs the full stepB')
## add more jets
options.register ("doAdditionalJets",False,opts.VarParsing.multiplicity.singleton,opts.VarParsing.varType.bool,' from 5 up to 8 jets in the event')

## More Info for generating the tree
options.register ('jetIdWP',"1",opts.VarParsing.multiplicity.singleton,opts.VarParsing.varType.string,'type of jetID to be applied')
options.register ('pileupjetIdWP',"1",opts.VarParsing.multiplicity.singleton,opts.VarParsing.varType.string,'type of pileup jetID to be applied')
options.register ('CJVminPt',"30.",opts.VarParsing.multiplicity.singleton,opts.VarParsing.varType.string,'minimum pt cut in some cases')
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
process.load('Configuration.EventContent.EventContent_cff')
process.load('SimGeneral.MixingModule.mixNoPU_cfi')
process.load('Configuration.StandardSequences.Services_cff')
process.load('Configuration.StandardSequences.GeometryRecoDB_cff')
process.load('Configuration.StandardSequences.MagneticField_38T_cff')
process.load('Configuration.StandardSequences.EndOfProcess_cff')
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')

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
process.GlobalTag = GlobalTag(process.GlobalTag, 'POSTLS162_V4::All', '')

###################################################
#### MINIAOD Definition and patDefaultSequence ####
###################################################

process.load("PhysicsTools.PatAlgos.producersLayer1.patCandidates_cff")
process.load("PhysicsTools.PatAlgos.selectionLayer1.selectedPatCandidates_cff")
process.load("PhysicsTools.PatAlgos.slimming.packedPFCandidates_cfi")
process.load("PhysicsTools.PatAlgos.slimming.lostTracks_cfi")
process.load("PhysicsTools.PatAlgos.slimming.offlineSlimmedPrimaryVertices_cfi")
process.load("PhysicsTools.PatAlgos.slimming.genParticles_cff")
process.load("PhysicsTools.PatAlgos.slimming.selectedPatTrigger_cfi")
process.load("PhysicsTools.PatAlgos.slimming.slimmedJets_cfi")
process.load("PhysicsTools.PatAlgos.slimming.slimmedGenJets_cfi")
process.load("PhysicsTools.PatAlgos.slimming.slimmedElectrons_cfi")
process.load("PhysicsTools.PatAlgos.slimming.slimmedMuons_cfi")
process.load("PhysicsTools.PatAlgos.slimming.slimmedPhotons_cfi")
process.load("PhysicsTools.PatAlgos.slimming.slimmedTaus_cfi")
process.load("PhysicsTools.PatAlgos.slimming.slimmedMETs_cfi")
process.load("RecoEgamma.EgammaPhotonProducers.reducedEgamma_cfi")

from RecoMET.METFilters.metFilters_cff import metFilters, goodVertices
from CommonTools.RecoAlgos.HBHENoiseFilter_cfi import *
from RecoMET.METFilters.CSCTightHaloFilter_cfi import *
from RecoMET.METFilters.hcalLaserEventFilter_cfi import *
from RecoMET.METFilters.EcalDeadCellTriggerPrimitiveFilter_cfi import *
from RecoMET.METFilters.eeBadScFilter_cfi import *
from RecoMET.METFilters.ecalLaserCorrFilter_cfi import *
from RecoMET.METFilters.trackingFailureFilter_cfi import *
from RecoMET.METFilters.trackingPOGFilters_cff import *

# Automatic addition of the customisation function from PhysicsTools.PatAlgos.slimming.miniAOD_tools
from PhysicsTools.PatAlgos.slimming.miniAOD_tools import miniAOD_customizeAllMC 
#call to customisation function miniAOD_customizeAllMC imported from PhysicsTools.PatAlgos.slimming.miniAOD_tools
process = miniAOD_customizeAllMC(process)

### Egamma Fix
process.load('RecoEgamma.ElectronIdentification.electronIdCutBased_cfi')
process.load('RecoEgamma.EgammaIsolationAlgos.particleBasedIsoProducer_cfi')
process.load('RecoEgamma.ElectronIdentification.electronIdSequence_cff')
process.load('RecoEgamma.PhotonIdentification.photonId_cff')

process.particleBasedIsolation.photonTmpProducer   = cms.InputTag("gedPhotons")
process.particleBasedIsolation.electronTmpProducer = cms.InputTag("gedGsfElectrons")
process.reducedEgamma.singleConversions            = cms.InputTag("gedPhotonCore")

process.egammaReduction = cms.Sequence(process.eIdSequence+
                                       process.PhotonIDProdGED+
                                       process.particleBasedIsolation+
                                       process.reducedEgamma)


## pat Jet partp
process.patJets.userData.userInts.src.insert(False,cms.InputTag("pileupJetId","fullId"))
process.patJetFlavourAssociation.jets      = cms.InputTag("ak5PFJetsCHS")
process.patJetFlavourAssociation.rParam    = cms.double(0.5)
process.jetTracksAssociatorAtVertex.jets   = cms.InputTag("ak5PFJetsCHS")
process.patJetCorrFactors.src              = cms.InputTag("ak5PFJetsCHS")
process.patJetCorrFactors.payload          = cms.string('AK5PFchs')
process.patJetGenJetMatch.src              = cms.InputTag("ak5PFJetsCHS")
process.patJetPartonAssociationLegacy.jets = cms.InputTag("ak5PFJetsCHS")
process.patJetPartonMatch.src              = cms.InputTag("ak5PFJetsCHS")
process.patJets.jetSource                  = cms.InputTag("ak5PFJetsCHS")
process.softMuonTagInfos.jets              = cms.InputTag("a54PFJetsCHS")
process.softPFElectronsTagInfos.jets       = cms.InputTag("ak5PFJetsCHS")
process.softPFMuonsTagInfos.jets           = cms.InputTag("ak5PFJetsCHS")
process.slimmedGenJets.src                 = cms.InputTag("ak5GenJets")   
process.pfJetMETcorr.src                   = cms.InputTag("ak5PFJets")
process.pfJetMETcorr.offsetCorrLabel       = cms.string('ak5PFL1Fastjet')
process.pfJetMETcorr.jetCorrLabel          = cms.string('ak5PFL1FastL2L3')
process.patJetFlavourAssociationAK5PFForMetUnc.rParam = cms.double(0.5)
process.ak4PFJetsPtrs.src      = cms.InputTag("ak5PFJets")
process.patTrigger.processName = cms.string('RECO')

process.load('LatinoTrees.AnalysisStep.jetESProducers_cff')

# Output definition
if options.produceMiniAOD:
 process.MINIAODSIMoutput = cms.OutputModule("PoolOutputModule",
    compressionLevel = cms.untracked.int32(4),
    compressionAlgorithm = cms.untracked.string('LZMA'),
    eventAutoFlushCompressedSize = cms.untracked.int32(15728640),
    outputCommands = process.MINIAODSIMEventContent.outputCommands,
    fileName = cms.untracked.string('miniAOD-prod_PAT.root'),
    dataset = cms.untracked.PSet(
        filterName = cms.untracked.string(''),
        dataTier = cms.untracked.string('')
    )
 )

 process.MINIAODSIMoutput.outputCommands += cms.untracked.vstring('keep *_addPileupInfo_*_*')
 process.MINIAODSIMoutput_step = cms.EndPath(process.MINIAODSIMoutput)

else :

 process.TFileService = cms.Service("TFileService",fileName = cms.string(options.outputFile))

 ## some fixes
 process.patMETCorrections.remove(process.produceCaloMETCorrections)

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
 
 ### in this case run Latino analysis and miniAOD objects by hand 
 process.miniAODPAT = cms.Path( #metFilters
                                process.egammaReduction* ## egamma reduction sequences
                                process.prunedGenParticlesWithStatusOne* ## pruned status 1 
                                process.prunedGenParticles* 
                                process.slimmedGenJets*
                                process.offlineSlimmedPrimaryVertices*
                                process.patCandidates*
                                process.selectedPatCandidates*
                                process.packedPFCandidates*
                                process.lostTracks*
                                process.selectedPatTrigger*
                                process.packedGenParticles*
                                process.slimmedJets*
                                process.slimmedJetsAK8*
                                process.slimmedElectrons*
                                process.slimmedMuons*
                                process.slimmedPhotons*
                                process.slimmedTaus*
                                process.slimmedMETs)
 


 #############################
 ####### PUPPI JETS ##########
 #############################

 if options.runPUPPISequence:

  from LatinoTrees.AnalysisStep.puppiSequence_cff import makePuppiAlgo, makePatPuppiJetSequence 
  jetPuppiR = 0.5
  makePuppiAlgo(process) ## call puppi producer and puppi met
  process.miniAODPAT += process.puppiSequence ## add puppi particle sequence to the path
  makePatPuppiJetSequence(process,jetPuppiR)  ## call pat puppi jets
  process.miniAODPAT += process.AK5makePatJetsPuppi


 ##############################
 #### RUN LATINO Analysis #####
 ##############################

 #### Run Skim Event producer
 from LatinoTrees.AnalysisStep.skimEventProducer_cfi import skimEventProducer ## take the skim event producer

 label = options.label ;

 if options.selection == 'TightTight':
    label = "Scenario6"; muon = "slimmedMuons"; ele = "slimmedElectrons"; softmu = "slimmedMuons"; preSeq = cms.Sequence(); 
 elif options.selection == 'LooseLoose':
    label = "Scenario7"; muon = "wwMuScenario7"; ele = "wwEleScenario5"; softmu = "wwMu4VetoScenario6"; preSeq = cms.Sequence();
 else:
    raise ValueError('selection must be either TightTight or LooseLoose')


 from LatinoTrees.AnalysisStep.skimEventProducer_cfi import addEventHypothesis

 skimEventProducer.triggerTag = cms.InputTag("TriggerResults","","RECO")
 if options.doTauEmbed == True:
  skimEventProducer.triggerTag = cms.InputTag("TriggerResults","","EmbeddedRECO")
  skimEventProducer.mcGenWeightTag = cms.InputTag("generator:minVisPtFilter")

 addEventHypothesis(process,skimEventProducer,label,muon,ele,softmu,"AK"+str(int(jetPuppiR*10))+"slimmedJetsPuppi","patMetPuppi",preSeq,True)
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
 doLHE = options.doLHE
 doGen = options.doGen
 doHiggs = options.doHiggs
 doSusy = options.doSusy
 doTauEmbed = options.doTauEmbed
 SameSign = options.doSameSign
 doNoFilter = options.doNoFilter
 typeLHEcomment = options.typeLHEcomment
 
 id = 0
 json = None
 mhiggs = 0
 wztth = False
 dy = False

 fourthGenSF = 1
 fermiSF = 1
 puStudy  = False ## set to true to add 16, yes 16 different PU possibilities                                                                                                              
 IsoStudy = False ## Set to True to get isolation variables (and a tree build only after ID+CONV+IP, without isolation)                                                                 
 Summer11 = False # set to true if you need to run the Summer11 (changes the PU distro)                                                                                                   
 Fall11   = False # set to true if you need to run the Fall11 (changes the PU distro)                                                                                                   

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
 elif doTauEmbed == True:
    dataset = ["AllEmbed"]
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
 #idn = re.sub('[^0-9]','',id)
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
 
 if options.doAdditionalJets: addAdditionalJets(process,process.stepBTree)

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

 if doTauEmbed == True:
    process.stepBTree.variables.mctruth = cms.string("mcGenWeight()")

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


