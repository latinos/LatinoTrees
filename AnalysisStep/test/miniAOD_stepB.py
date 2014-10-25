import FWCore.ParameterSet.Config as cms
import LatinoTrees.Misc.VarParsing as opts
import re
import sys

options = opts.VarParsing('analysis')

options.register ('summary',True,opts.VarParsing.multiplicity.singleton,opts.VarParsing.varType.bool,'Print run summary')
options.register ('eventsToProcess','',opts.VarParsing.multiplicity.list,opts.VarParsing.varType.string,'Events to process')
options.register ('skipEvents',0,opts.VarParsing.multiplicity.singleton,opts.VarParsing.varType.int,'Number of events to skip')
options.register ('label','XXX',opts.VarParsing.multiplicity.singleton,opts.VarParsing.varType.string,'Label')
options.register ('json','YYY',opts.VarParsing.multiplicity.singleton,opts.VarParsing.varType.string,'Json file for data')
options.register ('id',0,opts.VarParsing.multiplicity.singleton,opts.VarParsing.varType.string,'Dataset id')
options.register ('scale',0,opts.VarParsing.multiplicity.singleton,opts.VarParsing.varType.float,'Scale factor')
options.register ('doTauEmbed',False,opts.VarParsing.multiplicity.singleton,opts.VarParsing.varType.bool,'Turn on DY embedding mode (can be \'True\' or \'False\'')
options.register ('selection','TightTight',opts.VarParsing.multiplicity.singleton,opts.VarParsing.varType.string,'Selection level [TightTight,LooseLoose]')
options.register ('doSameSign',False,opts.VarParsing.multiplicity.singleton,opts.VarParsing.varType.bool,'Turn on Same Sign mode (can be \'True\' or \'False\'')
options.register ('doType01met',False,opts.VarParsing.multiplicity.singleton,opts.VarParsing.varType.bool,'Turn on Type01 met correction Sign mode (can be \'True\' or \'False\'')
options.register ('doSusy',False, opts.VarParsing.multiplicity.singleton,opts.VarParsing.varType.bool,'Turn on Susy MC dumper (can be \'True\' or \'False\'')
options.register ('doHiggs',False,opts.VarParsing.multiplicity.singleton,opts.VarParsing.varType.bool,'Turn on Higgs MC mass dumper (can be \'True\' or \'False\'')
options.register ('doLHE',False,opts.VarParsing.multiplicity.singleton,opts.VarParsing.varType.bool,'Turn on LHE dumper (can be \'True\' or \'False\'')
options.register ('typeLHEcomment',0,opts.VarParsing.multiplicity.singleton,opts.VarParsing.varType.int,'type of comment in LHE, 0 [default] powheg scale variation, 1 MG for anomalous couplings')
options.register ('doGen',False,opts.VarParsing.multiplicity.singleton,opts.VarParsing.varType.bool,'Turn on gen Variables dumper (can be \'True\' or \'False\'')
options.register ('doGenVV',False,opts.VarParsing.multiplicity.singleton,opts.VarParsing.varType.bool,'Turn on gen truth Variables dumper (can be \'True\' or \'False\'')
options.register ('doNoFilter',False,opts.VarParsing.multiplicity.singleton,opts.VarParsing.varType.bool,
                  'Turn on no filter requirement, not even requiring 2 leptons! Needed for unfolding at GEN (can be \'True\' or \'False\'')
options.register ('doEleIsoId',False,opts.VarParsing.multiplicity.singleton,opts.VarParsing.varType.bool,'Turn on electron id/iso sumper (can be \'True\' or \'False\'')
options.register ('acceptDuplicates',False,opts.VarParsing.multiplicity.singleton,opts.VarParsing.varType.bool,
                  'accept duplicates. Suggested true for private production (can be \'True\' or \'False\'')
options.register ('doFatJet',False,opts.VarParsing.multiplicity.singleton,opts.VarParsing.varType.bool,'Turn on Fat (can be \'True\' or \'False\'')
options.register ('puInformation','addPileupInfo',opts.VarParsing.multiplicity.singleton,opts.VarParsing.varType.string,'name of pile-up information collection: it may change for premixing samples in MC, addPileupInfo [default], mixData for premixinf')
options.register ('stopAtMiniAOD',False,opts.VarParsing.multiplicity.singleton,opts.VarParsing.varType.bool,'if true it stops at MiniAOD, otherwise it runs the full stepB')
options.parseArguments()

process = cms.Process('PAT')

# import of standard configurations
process.load('Configuration.StandardSequences.Services_cff')
process.load('SimGeneral.HepPDTESSource.pythiapdt_cfi')
process.load('FWCore.MessageService.MessageLogger_cfi')
process.load('Configuration.EventContent.EventContent_cff')
process.load('SimGeneral.MixingModule.mixNoPU_cfi')
process.load('Configuration.StandardSequences.GeometryRecoDB_cff')
process.load('Configuration.StandardSequences.MagneticField_38T_cff')
process.load('Configuration.StandardSequences.PATMC_cff')
process.load('Configuration.StandardSequences.EndOfProcess_cff')
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')

process.load("FWCore.MessageService.MessageLogger_cfi")
process.MessageLogger.destinations = ['cout', 'cerr']
process.MessageLogger.cerr.FwkReport.reportEvery = 1000

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

# Output definition
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

from Configuration.AlCa.GlobalTag import GlobalTag
process.GlobalTag = GlobalTag(process.GlobalTag, 'POSTLS162_V4::All', '')

process.endjob_step = cms.EndPath(process.endOfProcess)
process.MINIAODSIMoutput_step = cms.EndPath(process.MINIAODSIMoutput)

# Automatic addition of the customisation function from PhysicsTools.PatAlgos.slimming.miniAOD_tools
from PhysicsTools.PatAlgos.slimming.miniAOD_tools import miniAOD_customizeAllMC 
#call to customisation function miniAOD_customizeAllMC imported from PhysicsTools.PatAlgos.slimming.miniAOD_tools
process = miniAOD_customizeAllMC(process)


### MET Filters 
process.Flag_HBHENoiseFilter.remove(process.HBHENoiseFilter)
process.Flag_METFilters.remove(process.HBHENoiseFilter)
process.trackingFailureFilter.JetSource=cms.InputTag("ak5PFJets")

### Egamma Fix
process.load('RecoEgamma.ElectronIdentification.electronIdCutBased_cfi')
process.load('RecoEgamma.EgammaIsolationAlgos.particleBasedIsoProducer_cfi')
process.load('RecoEgamma.ElectronIdentification.electronIdSequence_cff')
process.load('RecoEgamma.PhotonIdentification.photonId_cff')

process.particleBasedIsolation.photonTmpProducer   = cms.InputTag("gedPhotons")
process.particleBasedIsolation.electronTmpProducer = cms.InputTag("gedGsfElectrons")
process.reducedEgamma.singleConversions = cms.InputTag("gedPhotonCore")

process.miniAOD.replace(process.reducedEgamma,process.eIdSequence+
                                              process.PhotonIDProdGED+
                                              process.particleBasedIsolation+
                                              process.reducedEgamma)

## Jet part
process.patJetFlavourAssociation.jets    = cms.InputTag("ak5PFJetsCHS")
process.patJetFlavourAssociation.rParam  = cms.double(0.5)
process.jetTracksAssociatorAtVertex.jets = cms.InputTag("ak5PFJetsCHS")
process.patJetCorrFactors.src     = cms.InputTag("ak5PFJetsCHS")
process.patJetCorrFactors.payload = cms.string('AK5PFchs')
process.patJetGenJetMatch.src     = cms.InputTag("ak5PFJetsCHS")
process.patJetPartonAssociationLegacy.jets = cms.InputTag("ak5PFJetsCHS")
process.patJetPartonMatch.src = cms.InputTag("ak5PFJetsCHS")
process.patJets.jetSource     = cms.InputTag("ak5PFJetsCHS")
process.softMuonTagInfos.jets = cms.InputTag("a54PFJetsCHS")
process.softPFElectronsTagInfos.jets = cms.InputTag("ak5PFJetsCHS")
process.softPFMuonsTagInfos.jets     = cms.InputTag("ak5PFJetsCHS")
process.slimmedGenJets.src = cms.InputTag("ak5GenJets")   
process.pfJetMETcorr.src   = cms.InputTag("ak5PFJets")
process.pfJetMETcorr.offsetCorrLabel = cms.string('ak5PFL1Fastjet')
process.pfJetMETcorr.jetCorrLabel    = cms.string('ak5PFL1FastL2L3')
process.patJetFlavourAssociationAK5PFForMetUnc.rParam = cms.double(0.5)
process.ak4PFJetsPtrs.src      = cms.InputTag("ak5PFJets")
process.patTrigger.processName = cms.string('RECO')

process.load('LatinoTrees.AnalysisStep.jetESProducers_cff')


######## start latino analysis
if not options.stopAtMiniAOD:
 process.load("LatinoTrees.AnalysisStep.stepB_cff")
 from LatinoTrees.AnalysisStep.stepB_cff import *

 doEleIsoId = options.doEleIsoId 

 doLHE = options.doLHE
 doGen = options.doGen
 doGenVV = options.doGenVV
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
 puStudy = False ## set to true to add 16, yes 16 different PU possibilities                                                                                                              
 IsoStudy = False ## Set to True to get isolation variables (and a tree build only after ID+CONV+IP, without isolation)                                                                 
 Summer11 = False # set to true if you need to run the Summer11 (changes the PU distro)                                                                                                   
 Fall11 = False # set to true if you need to run the Fall11 (changes the PU distro)                                                                                                   
 label = options.label

 if '2011' in label: label = label[:label.find('2011')]
 if '2012' in label: label = label[:label.find('2012')]
 if label in [ 'SingleElectron', 'DoubleElectron', 'SingleMuon', 'DoubleMuon', 'MuEG']:
    dataset = [label]
    id = options.id
    json = options.json
    scalef = 1
    doPDFvar = False
    doGen = false
    doGenVV = false
    doLHE = false
 elif doTauEmbed == True:
    dataset = ["AllEmbed"]
    id = options.id
    json = options.json
    scalef = 1
    doPDFvar = False
    doGen = false
    doGenVV = false
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
    process.stepBTree.variables.baseW = "%.12f" % scalef
    if mhiggs != 0:
        process.stepBTree.variables.fourW = "%.12f" % fourthGenSF
        process.stepBTree.variables.fermiW = "%.12f" % fermiSF
    else:
        process.stepBTree.variables.fourW = "1"
        process.stepBTree.variables.fermiW = "1"
    if mhiggs <=0:
        process.stepBTree.variables.kfW = cms.string("1")
    else:
        process.higgsPt.inputFilename = "HiggsAnalysis/HiggsToWW2Leptons/data/kfactors_Std/kfactors_mh%(mass)d_ren%(mass)d_fac%(mass)d.dat" % {"mass":abs(mhiggs)}
 else:
    from FWCore.PythonUtilities.LumiList import LumiList
    import os
    lumis = LumiList(filename = os.getenv('CMSSW_BASE')+'/src/LatinoTrees/Misc/Jsons/%s.json'%json)
    process.source.lumisToProcess = cms.untracked.VLuminosityBlockRange()
    process.source.lumisToProcess = lumis.getCMSSWString().split(',')
    process.stepBTree.variables.baseW = "1"
    process.stepBTree.variables.fourW = "1"
    process.stepBTree.variables.fermiW = "1"
    process.stepBTree.variables.kfW = cms.string("1")
    process.stepBTree.variables.trpu = cms.string("1")
    process.stepBTree.variables.itpu = cms.string("1")
    process.stepBTree.variables.ootpup1 = cms.string("1")
    process.stepBTree.variables.ootpum1 = cms.string("1")
    process.stepBTree.variables.puW = cms.string("1")
    process.stepBTree.variables.puAW = cms.string("1")
    process.stepBTree.variables.puBW = cms.string("1")

 process.load("LatinoTrees.AnalysisStep.skimEventProducer_cfi")

 if options.selection == 'TightTight':
    label = "Scenario6"; muon = "slimmedMuons"; ele = "slimmedElectrons"; softmu = "slimmedMuons"; preSeq = cms.Sequence(); 
 elif options.selection == 'LooseLoose':
    label = "Scenario7"; muon = "wwMuScenario7"; ele = "wwEleScenario5"; softmu = "wwMu4VetoScenario6"; preSeq = cms.Sequence();
 else:
    raise ValueError('selection must be either TightTight or LooseLoose')

 ### adding puppi
 from LatinoTrees.AnalysisStep.puppiSequence_cff import makePuppiAlgo, makePatPuppiJetSequence 
 makePuppiAlgo(process) ## call puppi producer and puppi met
 makePatPuppiJetSequence(process,0.5) ## call pat puppi jets

 process.MINIAODSIMoutput.outputCommands += cms.untracked.vstring('keep *_*puppi*_*_*',
                                                                  'keep *_*Puppi*_*_*',
                                                                  'keep *_AK5PFJetsPuppi_*_*',
                                                                  'keep *_*slimmedJetsPuppi_*_*',
                                                                  'keep *_pfMetPuppi_*_*')


 from LatinoTrees.AnalysisStep.skimEventProducer_cfi import addEventHypothesis
 process.skimEventProducer.triggerTag = cms.InputTag("TriggerResults","","PAT")
 if doTauEmbed == True:
  process.skimEventProducer.triggerTag = cms.InputTag("TriggerResults","","EmbeddedRECO")
  process.skimEventProducer.mcGenWeightTag = cms.InputTag("generator:minVisPtFilter")

 addEventHypothesis(process,label,muon,ele,softmu,preSeq,True)

 if (wztth == True) or (doPDFvar == True):
    getattr(process,"ww%s"% (label)).mcGenEventInfoTag = "generator"
    getattr(process,"ww%s"% (label)).genParticlesTag = "prunedGen"
 if doSusy == True :
    getattr(process,"ww%s"% (label)).genParticlesTag = "prunedGen"
 if doHiggs == True :
    getattr(process,"ww%s"% (label)).genParticlesTag = "prunedGen"
 if doLHE == True :
    getattr(process,"ww%s"% (label)).mcLHEEventInfoTag = "source"
    getattr(process,"ww%s"% (label)).whichLHE = cms.untracked.int32(typeLHEcomment)
 if doGen == True :
    getattr(process,"ww%s"% (label)).genParticlesTag = "prunedGen"
    getattr(process,"ww%s"% (label)).genMetTag = "genMetTrue"
    getattr(process,"ww%s"% (label)).genJetTag = cms.InputTag("ak5GenJetsNoElNoMuNoNu","","Yield")
 if doGenVV == True :
    getattr(process,"ww%s"% (label)).mcLHEEventInfoTag = "source"
    getattr(process,"ww%s"% (label)).genParticlesTag = "prunedGen"
    getattr(process,"ww%s"% (label)).genMetTag = "genMetTrue"
    getattr(process,"ww%s"% (label)).genJetTag = cms.InputTag("ak5GenJetsNoElNoMuNoNu","","Yield")
 if id in ["036", "037", "037c0", "037c1", "037c2", "037c3", "037c4", "037c5", "037c6", "037c7", "037c8", "037c9", "042", "043", "045", "046" ]: # DY-Madgraph sample    
    getattr(process,"ww%s"% (label)).genParticlesTag = "prunedGen"

 process.stepBTree.src = cms.InputTag("ww%s"% (label));

 seq = cms.Sequence()
 setattr(process, 'stepBAnalysis', seq)
 process.nverticesModule.probes = cms.InputTag("ww%s"% (label))
 seq += process.nverticesModule
 process.stepBTree.variables.nvtx = cms.InputTag("nverticesModule")
 if IsoStudy: addIsoStudyVariables(process,process.stepBTree)

 process.nPU.puLabel = cms.InputTag(options.puInformation)

 # electron id and iso variables                                                                                                                                                         
 if doEleIsoId: addEleIdIsoVariables(process,process.stepBTree)
 # LHE information dumper                                                                                                                                                                 
 if doLHE:
  addLHEVariables(process,process.stepBTree)

  process.stepBTree.variables.numbLHE = cms.string("numberOfbQuarks()")
  process.stepBTree.variables.numtLHE = cms.string("numberOftQuarks()")

  process.stepBTree.variables.HEPMCweightScale0 = cms.string("HEPMCweightScale(0)")
  process.stepBTree.variables.HEPMCweightScale1 = cms.string("HEPMCweightScale(1)")
  process.stepBTree.variables.HEPMCweightScale2 = cms.string("HEPMCweightScale(2)")
  process.stepBTree.variables.HEPMCweightScale3 = cms.string("HEPMCweightScale(3)")
  process.stepBTree.variables.HEPMCweightScale4 = cms.string("HEPMCweightScale(4)")
  process.stepBTree.variables.HEPMCweightScale5 = cms.string("HEPMCweightScale(5)")
  process.stepBTree.variables.HEPMCweightScale6 = cms.string("HEPMCweightScale(6)")

  if typeLHEcomment == 1 :
     for i in range (70) :
         exec("process.stepBTree.variables.HEPMCweightScale" + str(i+7) + " = cms.string(\"HEPMCweightScale(" + str(i+7) + ")\")")

  process.stepBTree.variables.HEPMCweightRen0 = cms.string("HEPMCweightRen(0)")
  process.stepBTree.variables.HEPMCweightRen1 = cms.string("HEPMCweightRen(1)")
  process.stepBTree.variables.HEPMCweightRen2 = cms.string("HEPMCweightRen(2)")
  process.stepBTree.variables.HEPMCweightRen3 = cms.string("HEPMCweightRen(3)")
  process.stepBTree.variables.HEPMCweightRen4 = cms.string("HEPMCweightRen(4)")
  process.stepBTree.variables.HEPMCweightRen5 = cms.string("HEPMCweightRen(5)")
  process.stepBTree.variables.HEPMCweightRen6 = cms.string("HEPMCweightRen(6)")

  process.stepBTree.variables.HEPMCweightFac0 = cms.string("HEPMCweightFac(0)")
  process.stepBTree.variables.HEPMCweightFac1 = cms.string("HEPMCweightFac(1)")
  process.stepBTree.variables.HEPMCweightFac2 = cms.string("HEPMCweightFac(2)")
  process.stepBTree.variables.HEPMCweightFac3 = cms.string("HEPMCweightFac(3)")
  process.stepBTree.variables.HEPMCweightFac4 = cms.string("HEPMCweightFac(4)")
  process.stepBTree.variables.HEPMCweightFac5 = cms.string("HEPMCweightFac(5)")
  process.stepBTree.variables.HEPMCweightFac6 = cms.string("HEPMCweightFac(6)")

 if doGen: addGenVariables(process,process.stepBTree)

 if doGenVV: addGenVVVariables(process,process.stepBTree)

 addAdditionalJets(process,process.stepBTree)

 if options.doFatJet :
    addFatJets(process,process.stepBTree)

 if dataset[0] == 'MC':
    process.nPU.src = cms.InputTag("ww%s"% (label))
    seq += process.nPU
    process.stepBTree.variables.trpu = cms.InputTag("nPU:tr")
    process.stepBTree.variables.itpu = cms.InputTag("nPU:it")
    process.stepBTree.variables.ootpup1 = cms.InputTag("nPU:p1")
    process.stepBTree.variables.ootpum1 = cms.InputTag("nPU:m1")
    if puStudy: addExtraPUWeights(process,process.stepBTree,label,seq)
    if dy:
        process.dyWeight.src = cms.InputTag("ww%s"% (label))
        process.stepBTree.variables.kfW = cms.InputTag("dyWeight")
        seq += process.dyWeight
    elif mhiggs > 0:
        process.ptWeight.src = cms.InputTag("ww%s"% (label))
        process.stepBTree.variables.kfW = cms.InputTag("ptWeight")
        seq += process.higgsPt
        seq += process.ptWeight

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
    process.stepBTree.variables.HEPMCweight = cms.string("HEPMCweight()")

 if doPDFvar == True :
    process.stepBTree.variables.pdfscalePDF = cms.string("getPDFscalePDF()")
    process.stepBTree.variables.pdfx1 = cms.string("getPDFx1()")
    process.stepBTree.variables.pdfx2 = cms.string("getPDFx2()")
    process.stepBTree.variables.pdfid1 = cms.string("getPDFid1()")
    process.stepBTree.variables.pdfid2 = cms.string("getPDFid2()")
    process.stepBTree.variables.pdfx1PDF = cms.string("getPDFx1PDF()")
    process.stepBTree.variables.pdfx2PDF = cms.string("getPDFx2PDF()")

 # path already set up                                                                                                                                                                   
 p = getattr(process,'sel'+label)
 p += seq
 setattr(process,'sel'+label,p)
 # define output                                                                                                                                                                           
 process.TFileService = cms.Service("TFileService",fileName = cms.string(options.outputFile))

 if IsoStudy:
  getattr(process,"stepBTree").cut = cms.string("!isSTA(0) && !isSTA(1) && leptEtaCut(2.4,2.5) && ptMax > 20 && ptMin > 10 && passesIP && nExtraLep(10) == 0")
 
 if SameSign:
  getattr(process,"stepBTree").cut = cms.string("q(0)*q(1) > 0 && !isSTA(0) && !isSTA(1) && leptEtaCut(2.4,2.5) && ptMax > 20 && ptMin > 10")

 # save all events                                                                                                                                                                        
 if doNoFilter:
  print ">> Dump all events"
  getattr(process,"stepBTree").cut = cms.string("1")
  getattr(process,"skim%s"% (label)).cut = cms.string("nLep >= 0")

 process.MINIAODSIMoutput_step += process.stepBTree

# p += process.stepBTree

############
processDumpFile = open('processDump.py', 'w')
print >> processDumpFile, process.dumpPython()


