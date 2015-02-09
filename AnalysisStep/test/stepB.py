import FWCore.ParameterSet.Config as cms
import LatinoTrees.Misc.VarParsing as opts
import re
import sys

options = opts.VarParsing('analysis')

#-------------------------------------------------------------------------------
# some basic cleanup
del options._register['filePrepend']
del options._register['totalSections']
del options._register['section']
del options._register['secondaryOutputFile']
del options._singletons['filePrepend']
del options._singletons['totalSections']
del options._singletons['section']
del options._singletons['secondaryOutputFile']
del options._register['secondaryInputFiles']

del options._lists['secondaryInputFiles']
#-------------------------------------------------------------------------------
options.register ( 'summary',
                  True,
                  opts.VarParsing.multiplicity.singleton,
                  opts.VarParsing.varType.bool,
                  'Print run summary')

options.register ('skipEvents',
                  0, # default value
                  opts.VarParsing.multiplicity.singleton, # singleton or list
                  opts.VarParsing.varType.int, # string, int, or float
                  'Number of events to skip')

options.register ('label',
                  'XXX',
                  opts.VarParsing.multiplicity.singleton,
                  opts.VarParsing.varType.string,
                  'Label')

options.register ('json',
                  'YYY',
                  opts.VarParsing.multiplicity.singleton,
                  opts.VarParsing.varType.string,
                  'Json file for data')

options.register ('id',
                  0, # default value
                  opts.VarParsing.multiplicity.singleton, # singleton or list
                  opts.VarParsing.varType.string, # string, int, or float
                  'Dataset id')

options.register ('scale',
                  0, # default value
                  opts.VarParsing.multiplicity.singleton, # singleton or list
                  opts.VarParsing.varType.float, # string, int, or float
                  'Scale factor')

options.register ('doTauEmbed',
                  False, # default value
                  opts.VarParsing.multiplicity.singleton, # singleton or list
                  opts.VarParsing.varType.bool,
                  'Turn on DY embedding mode (can be \'True\' or \'False\'')

options.register ('selection',
                  'TightTight',
                  opts.VarParsing.multiplicity.singleton, # singleton or list
                  opts.VarParsing.varType.string, # string, int, or float
                  'Selection level [TightTight,LooseLoose]')

options.register ('doSameSign',
                  False, # default value
                  opts.VarParsing.multiplicity.singleton, # singleton or list
                  opts.VarParsing.varType.bool,
                  'Turn on Same Sign mode (can be \'True\' or \'False\'')

options.register ('doType01met',
                  False, # default value
                  opts.VarParsing.multiplicity.singleton, # singleton or list
                  opts.VarParsing.varType.bool,
                  'Turn on Type01 met correction Sign mode (can be \'True\' or \'False\'')

options.register ('doSusy',
                  False, # default value
                  opts.VarParsing.multiplicity.singleton, # singleton or list
                  opts.VarParsing.varType.bool,
                  'Turn on Susy MC dumper (can be \'True\' or \'False\'')

options.register ('doHiggs',
                  False, # default value
                  opts.VarParsing.multiplicity.singleton, # singleton or list
                  opts.VarParsing.varType.bool,
                  'Turn on Higgs MC mass dumper (can be \'True\' or \'False\'')

options.register ('doLHE',
                  False, # default value
                  opts.VarParsing.multiplicity.singleton, # singleton or list
                  opts.VarParsing.varType.bool,
                  'Turn on LHE dumper (can be \'True\' or \'False\'')

options.register ('typeLHEcomment',
                  0, # default value
                  opts.VarParsing.multiplicity.singleton, # singleton or list
                  opts.VarParsing.varType.int, # string, int, or float
                  'type of comment in LHE, 0 [default] powheg scale variation, 1 MG for anomalous couplings')

options.register ('doGen',
                  False, # default value
                  opts.VarParsing.multiplicity.singleton, # singleton or list
                  opts.VarParsing.varType.bool,
                  'Turn on gen Variables dumper (can be \'True\' or \'False\'')

options.register ('doGenVV',
                  False, # default value
                  opts.VarParsing.multiplicity.singleton, # singleton or list
                  opts.VarParsing.varType.bool,
                  'Turn on gen truth Variables dumper, specific for VV final state (can be \'True\' or \'False\'')

options.register ('doNoFilter',
                  False, # default value
                  opts.VarParsing.multiplicity.singleton, # singleton or list
                  opts.VarParsing.varType.bool,
                  'Turn on no filter requirement, not even requiring 2 leptons! Needed for unfolding at GEN (can be \'True\' or \'False\'')

options.register ('doMuonIsoId',
                  True, # default value
                  opts.VarParsing.multiplicity.singleton, # singleton or list
                  opts.VarParsing.varType.bool,
                  'Turn on muon id/iso dumper (can be \'True\' or \'False\'')

options.register ('doEleIsoId',
                  False, # default value
                  opts.VarParsing.multiplicity.singleton, # singleton or list
                  opts.VarParsing.varType.bool,
                  'Turn on electron id/iso dumper (can be \'True\' or \'False\'')

options.register ('acceptDuplicates',
                  False, # default value
                  opts.VarParsing.multiplicity.singleton, # singleton or list
                  opts.VarParsing.varType.bool,
                  'accept duplicates. Suggested true for private production (can be \'True\' or \'False\'')

options.register ('doFatJet',
                  False, # default value
                  opts.VarParsing.multiplicity.singleton, # singleton or list
                  opts.VarParsing.varType.bool,
                  'Turn on Fat jet production and dumper (can be \'True\' or \'False\'')

options.register ('puInformation',
                  'addPileupInfo', # default value
                  opts.VarParsing.multiplicity.singleton, # singleton or list
                  opts.VarParsing.varType.string, # string, int, or float
                  'name of pile-up information collection: it may change for premixing samples in MC, addPileupInfo [default], mixData for premixinf')

options.register ('doPhotonID',
                  False, # default value
                  opts.VarParsing.multiplicity.singleton, # singleton or list
                  opts.VarParsing.varType.bool,
                  'Turn on photon ID variables (can be \'True\' or \'False\'')

options.register ('doIsoStudy',
                  False, # default value
                  opts.VarParsing.multiplicity.singleton, # singleton or list
                  opts.VarParsing.varType.bool,
                  'Turn on isolation studies, additional variables, ... (can be \'True\' or \'False\'')

options.register ('runPUPPISequence',
                  False,
                  opts.VarParsing.multiplicity.singleton,
                  opts.VarParsing.varType.bool,
                  'Turn on PUPPI jets (can be \'True\' or \'False\'')



#-------------------------------------------------------------------------------
# defaults
options.outputFile = 'stepB.root'
options.maxEvents = -1 #all events ---> already the default
#print " options.maxEvents = ",options.maxEvents
#-------------------------------------------------------------------------------

options.parseArguments()

process = cms.Process("stepB")

# logger
process.load("FWCore.MessageService.MessageLogger_cfi")
process.MessageLogger.destinations = ['cout', 'cerr']
process.MessageLogger.cerr.FwkReport.reportEvery = 1000

# accepting te duplicate is needed in case we have generated
# privately some samples and the event number always starts from 0
if options.acceptDuplicates :
    process.source = cms.Source('PoolSource',
          fileNames = cms.untracked.vstring( options.inputFiles ),
          skipEvents = cms.untracked.uint32( options.skipEvents ),
          duplicateCheckMode = cms.untracked.string('noDuplicateCheck'),
          )
else :
    process.source = cms.Source('PoolSource',
          fileNames = cms.untracked.vstring( options.inputFiles ),
          skipEvents = cms.untracked.uint32( options.skipEvents ),
          )
###

process.source.inputCommands = cms.untracked.vstring( "keep *", "drop *_conditionsInEdm_*_*", "drop *_MEtoEDMConverter_*_*")


process.options = cms.untracked.PSet(wantSummary = cms.untracked.bool(options.summary))
process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(options.maxEvents) )

# load configuration file with the variables list
process.load("LatinoTrees.AnalysisStep.stepB_cff")
from LatinoTrees.AnalysisStep.stepB_cff import * # get also functions


# load configurations
doMuonIsoId = options.doMuonIsoId
doEleIsoId = options.doEleIsoId
doLHE = options.doLHE
doGen = options.doGen
doGenVV = options.doGenVV
doHiggs = options.doHiggs
doSusy = options.doSusy
doTauEmbed = options.doTauEmbed
doSameSign = options.doSameSign
doNoFilter = options.doNoFilter
doIsoStudy = options.doIsoStudy
typeLHEcomment = options.typeLHEcomment
label = options.label
###

id = 0
json = None

wztth = False
dy = False

#puStudy = False ## set to true to add 16, yes 16 different PU possibilities
#IsoStudy = False ## Set to True to get isolation variables (and a tree build only after ID+CONV+IP, without isolation)
                 ## Note: works only if running also the step2


if '2011' in label: label = label[:label.find('2011')]
if '2012' in label: label = label[:label.find('2012')]
if '2015' in label: label = label[:label.find('2015')]

# data
if label in [ 'SingleElectron', 'DoubleElectron', 'SingleMuon', 'DoubleMuon', 'MuEG']:
    dataset = [label]
    id = options.id
    json = options.json
    scalef = 1
    doPDFvar = False
    doGen = False
    doGenVV = False
    doLHE = False

# dytt embedded sample
elif doTauEmbed == True:
    dataset = ["AllEmbed"]
    id = options.id
    json = options.json
    scalef = 1
    doPDFvar = False
    doGen = False
    doGenVV = False
    doLHE = False

# mc
else:
    dataset = ['MC', label];
    id = options.id;
    scalef = options.scale
    doPDFvar = True


process.stepBTree.variables.trigger = process.stepBTree.variables.trigger.value().replace("DATASET",dataset[0])
idn = re.sub('[^0-9]','',id)
process.stepBTree.variables.dataset = str(idn)


# mc
if dataset[0] == "MC":
    process.stepBTree.variables.baseW = "%.12f" % scalef
# data
else:
    from FWCore.PythonUtilities.LumiList import LumiList
    import os
    lumis = LumiList(filename = os.getenv('CMSSW_BASE')+'/src/LatinoTrees/Misc/Jsons/%s.json'%json)
    process.source.lumisToProcess = cms.untracked.VLuminosityBlockRange()
    process.source.lumisToProcess = lumis.getCMSSWString().split(',')
    process.stepBTree.variables.baseW = "1"
    process.stepBTree.variables.trpu = cms.string("1")
    process.stepBTree.variables.itpu = cms.string("1")
    process.stepBTree.variables.ootpup1 = cms.string("1")
    process.stepBTree.variables.ootpum1 = cms.string("1")
    process.stepBTree.variables.puW = cms.string("1")
    process.stepBTree.variables.puAW = cms.string("1")
    process.stepBTree.variables.puBW = cms.string("1")


# SkimEventProducer is where the objects are defined
process.load("LatinoTrees.AnalysisStep.skimEventProducer_cfi")


if options.selection == 'TightTight':
    labelSetup = "Scenario1"; muon = "slimmedMuons"; ele = "slimmedElectrons"; softmu = "slimmedMuons"; pho = "slimmedPhotons"; preSeq = cms.Sequence();
    #labelSetup = "Scenario1"; muon = "wwMuoTight"; ele = "wwEleTight"; softmu = "slimmedMuons"; pho = "slimmedPhotons"; preSeq = cms.Sequence();  # --> fix ele/mu tag and un-comment this line and comment the previous one
elif options.selection == 'LooseLoose':
    labelSetup = "Scenario7"; muon = "wwMuScenario7"; ele = "wwEleScenario5"; softmu = "wwMu4VetoScenario6"; pho = "wwPhoScenario1"; preSeq = cms.Sequence();
else:
    raise ValueError('selection must be either TightTight or LooseLoose')



# add puppi calculated from miniAOD
#   since puppi must be run as first
#   we include puppi in the "preSequence"
#   it's not possible to add it afterwards, 
#   unless we transform everything into a sequence
#   and let cmssw do the rest ...

if options.runPUPPISequence:
    #process.load("CommonTools.PileupAlgos.Puppi_cff")
    #process.load("RecoJets.JetProducers.ak4PFJetsPuppi_cfi")
    #process.puppi.candName = cms.InputTag('packedPFCandidates')
    #process.puppi.vertexName = cms.InputTag('offlineSlimmedPrimaryVertices')
    #puppi_onMiniAOD = cms.Sequence(process.puppi + process.ak4PFJetsPuppi)

    #from CommonTools.PileupAlgos.Puppi_cff import puppi
    #from RecoJets.JetProducers.ak4PFJetsPuppi_cfi import ak4PFJetsPuppi    
    #process.myak4PFJetsPuppi = ak4PFJetsPuppi.clone( rParam = 0.4 )
    #process.myak4PFJetsPuppi.src = cms.InputTag('mypuppi')
    #process.mypuppi = puppi.clone()
    #process.mypuppi.candName = cms.InputTag('packedPFCandidates')
    #process.mypuppi.vertexName = cms.InputTag('offlineSlimmedPrimaryVertices')
    #puppi_onMiniAOD = cms.Sequence(process.mypuppi + process.myak4PFJetsPuppi) #ak4PFJetsPuppi)

    #puppi_onMiniAOD = cms.Path(puppi + ak4PFJetsPuppi)
    #preSeq += puppi_onMiniAOD


    from LatinoTrees.AnalysisStep.puppiSequence_cff import makePuppiAlgo, makePatPuppiJetSequence, makePatPuppiMetSequence

    jetPuppiR = 0.4
    makePuppiAlgo(process) ## call puppi producer and puppi met
    makePatPuppiJetSequence(process,jetPuppiR) ## call pat puppi jets
    makePatPuppiMetSequence(process) ## call pat puppi met

    # now add to the preSequence
    preSeq += process.puppi_onMiniAOD
    preSeq += process.makePatPuppi
    preSeq += process.makePatMetPuppi




# create the EventHypothesis
# and tweaks for special MC/data samples

from LatinoTrees.AnalysisStep.skimEventProducer_cfi import addEventHypothesis
process.skimEventProducer.triggerTag = cms.InputTag("TriggerResults","","HLT")

if doTauEmbed == True:
  process.skimEventProducer.triggerTag = cms.InputTag("TriggerResults","","EmbeddedRECO")
  process.skimEventProducer.mcGenWeightTag = cms.InputTag("generator:minVisPtFilter")

# this is where the "event" is built
addEventHypothesis(process,labelSetup,muon,ele,softmu,pho,preSeq)

if (wztth == True) or (doPDFvar == True):
    getattr(process,"ww%s"% (labelSetup)).mcGenEventInfoTag = "generator"
    getattr(process,"ww%s"% (labelSetup)).genParticlesTag = "prunedGen"

if doSusy == True :
    getattr(process,"ww%s"% (labelSetup)).genParticlesTag = "prunedGen"

if doHiggs == True :
    getattr(process,"ww%s"% (labelSetup)).genParticlesTag = "prunedGen"

if doLHE == True :
    getattr(process,"ww%s"% (labelSetup)).mcLHEEventInfoTag = "source"
    getattr(process,"ww%s"% (labelSetup)).whichLHE = cms.untracked.int32(typeLHEcomment)

if doGen == True :
    # Difference between "prunedGenParticles" and "packedGenParticles": https://twiki.cern.ch/twiki/bin/viewauth/CMS/MiniAOD#PackedGenParticles
    getattr(process,"ww%s"% (labelSetup)).genParticlesTag = "prunedGenParticles"
    getattr(process,"ww%s"% (labelSetup)).genMetTag = "slimmedMET" # "genMetTrue"
    getattr(process,"ww%s"% (labelSetup)).genJetTag = "slimmedGenJets"
    #cms.InputTag("ak5GenJetsNoElNoMuNoNu","","Yield")

if doGenVV == True :
    getattr(process,"ww%s"% (labelSetup)).mcLHEEventInfoTag = "source"
    getattr(process,"ww%s"% (labelSetup)).genParticlesTag = "prunedGen"
    getattr(process,"ww%s"% (labelSetup)).genMetTag = "genMetTrue"
    getattr(process,"ww%s"% (labelSetup)).genJetTag = cms.InputTag("ak5GenJetsNoElNoMuNoNu","","Yield")

#if id in ["036", "037", "037c0", "037c1", "037c2", "037c3", "037c4", "037c5", "037c6", "037c7", "037c8", "037c9", "042", "043", "045", "046" ]: # DY-Madgraph sample
    #getattr(process,"ww%s"% (labelSetup)).genParticlesTag = "prunedGen"



# if puppi, change the input jet collection
if options.runPUPPISequence:
    getattr(process,"ww%s"% (labelSetup)).secondJetTag    = "patJetsAK4selectedPatJetsPuppi"
    #getattr(process,"ww%s"% (labelSetup)).jetTag    = "patJetsAK4selectedPatJetsPuppi"
    #getattr(process,"ww%s"% (labelSetup)).tagJetTag = "patJetsAK4selectedPatJetsPuppi"

# latino trees construction

tree = process.stepBTree.clone(src = cms.InputTag("ww%s"% (labelSetup) ));
seq = cms.Sequence()
setattr(process, 'TreeSequence', seq)
setattr(process, "Nvtx", process.nverticesModule.clone(probes = cms.InputTag("ww%s"% (labelSetup))))
seq += getattr(process, "Nvtx")
tree.variables.nvtx = cms.InputTag("Nvtx")
if doIsoStudy: 
    addIsoStudyVariables(process,tree)


# pile-up information
process.nPU.puLabel = cms.InputTag(options.puInformation)

process.nPU = cms.EDProducer("PileUpMultiplicityCounter",
    puLabel = cms.InputTag("addPileupInfo")
)

if dataset[0] == 'MC':
    setattr(process, "NPU", process.nPU.clone(src = cms.InputTag("ww%s"% (labelSetup))))
    seq += getattr(process, "NPU")
    tree.variables.trpu = cms.InputTag("NPU:tr")
    tree.variables.itpu = cms.InputTag("NPU:it")
    tree.variables.ootpup1 = cms.InputTag("NPU:p1")
    tree.variables.ootpum1 = cms.InputTag("NPU:m1")
    tree.variables.ootpup2 = cms.InputTag("NPU:p2")
    tree.variables.ootpum2 = cms.InputTag("NPU:m2")
    tree.variables.ootpup3 = cms.InputTag("NPU:p3")
    tree.variables.ootpum3 = cms.InputTag("NPU:m3")
    tree.variables.ootpum4 = cms.InputTag("NPU:m4")
    tree.variables.ootpum5 = cms.InputTag("NPU:m5")


# muon id and iso variables
if doMuonIsoId:
  addMuonIdIsoVariables(process,tree)

# electron id and iso variables
if doEleIsoId:
  addEleIdIsoVariables(process,tree)

# PHOTON VARIABLES
if options.doPhotonID:
  addPhotonIDVariables(process,tree)
 
addPhotonVariables(process,tree)

# LHE information dumper
if doLHE:
  addLHEVariables(process,tree)

  tree.variables.numbLHE = cms.string("numberOfbQuarks()")
  tree.variables.numtLHE = cms.string("numberOftQuarks()")

  tree.variables.HEPMCweightScale0 = cms.string("HEPMCweightScale(0)")
  tree.variables.HEPMCweightScale1 = cms.string("HEPMCweightScale(1)")
  tree.variables.HEPMCweightScale2 = cms.string("HEPMCweightScale(2)")
  tree.variables.HEPMCweightScale3 = cms.string("HEPMCweightScale(3)")
  tree.variables.HEPMCweightScale4 = cms.string("HEPMCweightScale(4)")
  tree.variables.HEPMCweightScale5 = cms.string("HEPMCweightScale(5)")
  tree.variables.HEPMCweightScale6 = cms.string("HEPMCweightScale(6)")

  if typeLHEcomment == 1 :
     #import ROOT
     for i in range (70) :
         #ROOT.gROOT.ProcessLine("tree.variables.HEPMCweightScale" + str(i+7) + " = cms.string(\"HEPMCweightScale(" + str(i+7) + ")\")")
         exec("tree.variables.HEPMCweightScale" + str(i+7) + " = cms.string(\"HEPMCweightScale(" + str(i+7) + ")\")")

  tree.variables.HEPMCweightRen0 = cms.string("HEPMCweightRen(0)")
  tree.variables.HEPMCweightRen1 = cms.string("HEPMCweightRen(1)")
  tree.variables.HEPMCweightRen2 = cms.string("HEPMCweightRen(2)")
  tree.variables.HEPMCweightRen3 = cms.string("HEPMCweightRen(3)")
  tree.variables.HEPMCweightRen4 = cms.string("HEPMCweightRen(4)")
  tree.variables.HEPMCweightRen5 = cms.string("HEPMCweightRen(5)")
  tree.variables.HEPMCweightRen6 = cms.string("HEPMCweightRen(6)")

  tree.variables.HEPMCweightFac0 = cms.string("HEPMCweightFac(0)")
  tree.variables.HEPMCweightFac1 = cms.string("HEPMCweightFac(1)")
  tree.variables.HEPMCweightFac2 = cms.string("HEPMCweightFac(2)")
  tree.variables.HEPMCweightFac3 = cms.string("HEPMCweightFac(3)")
  tree.variables.HEPMCweightFac4 = cms.string("HEPMCweightFac(4)")
  tree.variables.HEPMCweightFac5 = cms.string("HEPMCweightFac(5)")
  tree.variables.HEPMCweightFac6 = cms.string("HEPMCweightFac(6)")

if doGen: addGenVariables(process,tree)

if doGenVV: addGenVVVariables(process,tree)

# add 5th and 6th, ... jets variables
addAdditionalJets(process,tree)

if options.doFatJet :
    addFatJets(process,tree)



if id in ["036", "037", "037c0", "037c1", "037c2", "037c3", "037c4", "037c5", "037c6", "037c7", "037c8", "037c9", "042", "043", "045", "046" ]: # DY-Madgraph sample
    tree.variables.mctruth = cms.string("getFinalStateMC()")

if id in ["077", "078", "074" ]:
    tree.variables.PtZ = cms.string("getZPt()")
    tree.variables.MZ = cms.string("getZMass()")
    tree.variables.WZchan = cms.string("getWZdecayMC()")

if doTauEmbed == True:
    tree.variables.mctruth = cms.string("mcGenWeight()")

if wztth == True:
    tree.variables.mctruth = cms.string("mcHiggsProd()")
    tree.variables.mcHWWdecay = cms.string("getWWdecayMC()")

if doSusy == True :
    tree.variables.susyMstop = cms.string("getSusyStopMass()")
    tree.variables.susyMLSP = cms.string("getSusyLSPMass()")

if doHiggs == True :
    tree.variables.MHiggs = cms.string("getHiggsMass()")
    tree.variables.PtHiggs = cms.string("getHiggsPt()")
    tree.variables.HEPMCweight = cms.string("HEPMCweight()")

if doPDFvar == True :
    tree.variables.pdfscalePDF = cms.string("getPDFscalePDF()")
    tree.variables.pdfx1 = cms.string("getPDFx1()")
    tree.variables.pdfx2 = cms.string("getPDFx2()")
    tree.variables.pdfid1 = cms.string("getPDFid1()")
    tree.variables.pdfid2 = cms.string("getPDFid2()")
    tree.variables.pdfx1PDF = cms.string("getPDFx1PDF()")
    tree.variables.pdfx2PDF = cms.string("getPDFx2PDF()")

setattr(process,"Tree", tree)
seq += tree


# path already set up
p = getattr(process,'sel'+labelSetup)
p += seq
setattr(process,'sel'+labelSetup,p)

# define output
process.TFileService = cms.Service("TFileService",fileName = cms.string(options.outputFile))


#if IsoStudy:
  ##getattr(process,"ww%s%s"% (X,labelSetup)).elTag = "wwEleIDMerge"
  ##getattr(process,"ww%s%s"% (X,labelSetup)).muTag = "wwMuonsMergeID"
  #getattr(process,"Tree").cut = cms.string("!isSTA(0) && !isSTA(1) && leptEtaCut(2.4,2.5) && ptMax > 20 && ptMin > 10 && passesIP && nExtraLep(10) == 0")
  ##prepend = process.isoStudySequence + process.wwEleIDMerge + process.wwMuonsMergeID
  ##getattr(process,"sel%s%s"% (X,labelSetup))._seq = prepend + getattr(process,"sel%s%s"% (X,labelSetup))._seq


if doSameSign:
    getattr(process,"Tree").cut = cms.string("q(0)*q(1) > 0 && !isSTA(0) && !isSTA(1) && leptEtaCut(2.4,2.5) && ptMax > 20 && ptMin > 10")


# save all events
if doNoFilter:
    print ">> Dump all events"
    getattr(process,"Tree").cut = cms.string("1")
    getattr(process,"skim%s"% (labelSetup)).cut = cms.string("nLep >= 0")
