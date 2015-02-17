#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/Framework/interface/ModuleFactory.h"
#include "FWCore/Framework/interface/SourceFactory.h"
#include "CondCore/PluginSystem/interface/registration_macros.h"
#include "FWCore/PluginManager/interface/ModuleDef.h"

#include "DataFormats/PatCandidates/interface/Jet.h"

#include "JetMETCorrections/Objects/interface/JetCorrector.h"
#include "JetMETCorrections/Objects/interface/JetCorrectionsRecord.h"
#include "JetMETCorrections/Modules/interface/JetCorrectionESProducer.h"
#include "JetMETCorrections/Modules/interface/JetCorrectionESSource.h"
#include "JetMETCorrections/Modules/interface/JetCorrectionESChain.h"
#include "JetMETCorrections/Modules/interface/JetCorrectionProducer.h"
#include "JetMETCorrections/Modules/interface/QGLikelihoodESProducer.h"
#include "JetMETCorrections/Modules/interface/QGLikelihoodSystematicsESProducer.h"
#include "JetMETCorrections/Algorithms/interface/LXXXCorrector.h"
#include "JetMETCorrections/Algorithms/interface/L1OffsetCorrector.h"
#include "JetMETCorrections/Algorithms/interface/L1JPTOffsetCorrector.h"
#include "JetMETCorrections/Algorithms/interface/L1FastjetCorrector.h"
#include "JetMETCorrections/Algorithms/interface/L6SLBCorrector.h"
#include "DataFormats/JetReco/interface/CaloJet.h"
#include "DataFormats/JetReco/interface/PFJet.h"
#include "DataFormats/JetReco/interface/JPTJet.h"
#include "DataFormats/JetReco/interface/TrackJet.h"
#include "DataFormats/JetReco/interface/GenJet.h"
#include "CondFormats/JetMETObjects/interface/JetCorrectorParameters.h"
#include "CondFormats/JetMETObjects/interface/QGLikelihoodObject.h"

typedef cms::JetCorrectionProducer<pat::Jet> PatJetCorrectionProducer;

DEFINE_FWK_MODULE(PatJetCorrectionProducer);
