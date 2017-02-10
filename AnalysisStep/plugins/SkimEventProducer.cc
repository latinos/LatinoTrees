#include "LatinoTrees/AnalysisStep/interface/SkimEventProducer.h"
#include "DataFormats/METReco/interface/MET.h"
#include "DataFormats/METReco/interface/METCollection.h"
#include "DataFormats/METReco/interface/PFMET.h"
#include "DataFormats/METReco/interface/PFMETCollection.h"
#include "DataFormats/METReco/interface/GenMET.h"
#include "DataFormats/METReco/interface/GenMETCollection.h"
#include "DataFormats/PatCandidates/interface/Jet.h"
#include "DataFormats/JetReco/interface/GenJet.h"
#include "DataFormats/JetReco/interface/GenJetCollection.h"
#include "DataFormats/PatCandidates/interface/Muon.h"
#include "DataFormats/PatCandidates/interface/Electron.h"
#include "DataFormats/VertexReco/interface/Vertex.h"
#include "DataFormats/VertexReco/interface/VertexFwd.h"
#include "DataFormats/Common/interface/ValueMap.h"

#include<vector>
#include "Math/VectorUtil.h"


SkimEventProducer::SkimEventProducer(const edm::ParameterSet& cfg) :
    triggerTag_           ( cfg.getParameter<edm::InputTag>("triggerTag")),
    singleMuData_ ( cfg.getParameter<std::vector<std::string> >("singleMuDataPaths") ),
    singleElData_ ( cfg.getParameter<std::vector<std::string> >("singleElDataPaths") ),
    doubleMuData_ ( cfg.getParameter<std::vector<std::string> >("doubleMuDataPaths") ),
    doubleElData_ ( cfg.getParameter<std::vector<std::string> >("doubleElDataPaths") ),
    muEGData_     ( cfg.getParameter<std::vector<std::string> >("muEGDataPaths") ),
    singleMuMC_   ( cfg.getParameter<std::vector<std::string> >("singleMuMCPaths") ),
    singleElMC_   ( cfg.getParameter<std::vector<std::string> >("singleElMCPaths") ),
    doubleMuMC_   ( cfg.getParameter<std::vector<std::string> >("doubleMuMCPaths") ),
    doubleElMC_   ( cfg.getParameter<std::vector<std::string> >("doubleElMCPaths") ),
    muEGMC_       ( cfg.getParameter<std::vector<std::string> >("muEGMCPaths") ),
    AllEmbed_     ( cfg.getParameter<std::vector<std::string> >("AllEmbedPaths") ),
    //---- for fake rate estimation
    FakeRate_El_     ( cfg.getParameter<std::vector<std::string> >("FakeRateElPaths") ),
    FakeRate_Mu_     ( cfg.getParameter<std::vector<std::string> >("FakeRateMuPaths") ),
    //---- selected paths
    SelectedPaths_   ( cfg.getParameter<std::vector<std::string> >("SelectedPaths") ),
    SpecialPaths_   ( cfg.getParameter<std::vector<std::string> >("SpecialPaths") ),    //---- no prescale options
    BadChCandFilterToken_(consumes<bool>(cfg.getParameter<edm::InputTag>("BadChargedCandidateFilterTag"))),
    BadPFMuonFilterToken_(consumes<bool>(cfg.getParameter<edm::InputTag>("BadPFMuonFilterTag")))
{
    triggerSpecialTag_  = cfg.getParameter<edm::InputTag>("triggerSpecialTag");
    triggerPrescaleTag_ = cfg.getParameter<edm::InputTag>("triggerPrescaleTag");
    triggerL1minPrescaleTag_ = cfg.getParameter<edm::InputTag>("triggerL1minPrescaleTag");
    triggerL1maxPrescaleTag_ = cfg.getParameter<edm::InputTag>("triggerL1maxPrescaleTag");    
    
    mcLHEEventInfoTag_ = cfg.getParameter<edm::InputTag>("mcLHEEventInfoTag");
    mcGenEventInfoTag_ = cfg.getParameter<edm::InputTag>("mcGenEventInfoTag");
    mcGenWeightTag_    = cfg.getParameter<edm::InputTag>("mcGenWeightTag");
    genParticlesTag_   = cfg.getParameter<edm::InputTag>("genParticlesTag");
    genMetTag_         = cfg.getParameter<edm::InputTag>("genMetTag");
    genJetTag_         = cfg.getParameter<edm::InputTag>("genJetTag");
    tauTag_            = cfg.getParameter<edm::InputTag>("tauTag");
    muTag_             = cfg.getParameter<edm::InputTag>("muTag" );
    elTag_             = cfg.getParameter<edm::InputTag>("elTag" );
    softMuTag_         = cfg.getParameter<edm::InputTag>("softMuTag" );
    jetTag_            = cfg.getParameter<edm::InputTag>("jetTag" );
    tagJetTag_         = cfg.getParameter<edm::InputTag>("tagJetTag" );
    secondJetTag_      = cfg.getParameter<edm::InputTag>("secondJetTag" );
    fatJetTag_         = cfg.getParameter<edm::InputTag>("fatJetTag" );
    pfMetTag_          = cfg.getParameter<edm::InputTag>("pfMetTag" );
    pfMetNoHfTag_      = cfg.getParameter<edm::InputTag>("pfMetNoHfTag" );
    pupMetTag_         = cfg.getParameter<edm::InputTag>("pupMetTag" );
    tcMetTag_          = cfg.getParameter<edm::InputTag>("tcMetTag" );
    chargedMetTag_     = cfg.getParameter<edm::InputTag>("chargedMetTag" );
    vtxTag_            = cfg.getParameter<edm::InputTag>("vtxTag" );
    chCandsTag_        = cfg.getParameter<edm::InputTag>("chCandsTag" );
    pfCandsTag_        = cfg.getParameter<edm::InputTag>("pfCandsTag" );
    rhoTag_            = cfg.getParameter<edm::InputTag>("rhoTag" );
    rhoCaloTag_        = cfg.getParameter<edm::InputTag>("rhoCaloTag" );
    rhoCentralNeutralTag_  = cfg.getParameter<edm::InputTag>("rhoCentralNeutralTag" );
    phoTag_	       = cfg.getParameter<edm::InputTag>("phoTag"); //Photon
    trackJetTag_       = cfg.getParameter<edm::InputTag>("trackJetTag");
    htxsTag_           = cfg.getParameter<edm::InputTag>("HTXSTag");
    dressedMuonTag_    = cfg.getParameter<edm::InputTag>("dressedMuonTag");
    dressedElectronTag_= cfg.getParameter<edm::InputTag>("dressedElectronTag");
    
    _electronId        = cfg.getUntrackedParameter<int>("electronId",1);
    _debug             = cfg.getUntrackedParameter<int>("debug",0);
    _isMC              = cfg.getUntrackedParameter<int>("isMC",1); //---- 1 = MC, 0 = data
    
    //---- list of electron ids: namely value maps!
    _electronIds       = cfg.getParameter<std::vector<std::string> >("electronIds");
    _electronMvaIds    = cfg.getParameter<std::vector<std::string> >("electronMvaIds");
    
    if (cfg.exists("sptTag" )) 
     sptTag_ = cfg.getParameter<edm::InputTag>("sptTag" );
    else 
     sptTag_ = edm::InputTag("","","");
    if (cfg.exists("spt2Tag" )) 
     spt2Tag_ = cfg.getParameter<edm::InputTag>("spt2Tag" );
    else 
     spt2Tag_ = edm::InputTag("","","");

    produces<std::vector<reco::SkimEvent> >().setBranchAlias(cfg.getParameter<std::string>("@module_label"));

    applyJetCleaning_       = cfg.getParameter<int>("applyJetCleaning");
    maxEtaForJets_          = cfg.getParameter<double>("maxEtaForJets");
    minPtForJets_	    = cfg.getParameter<double>("minPtForJets"); 
    dzCutForBtagJets_	    = cfg.getParameter<double>("dzCutForBtagJets");
    applyCorrectionForJets_ = cfg.getParameter<bool>("applyCorrectionForJets");
    apply50nsValues_        = cfg.getParameter<bool>("apply50nsValues");
    applyIDForJets_	    = cfg.getParameter<int>("applyIDForJets");
 
    _maxDrSoftMuonJet       = cfg.getParameter<double>("maxDrSoftMuonJet");
    _minPtSoftMuon          = cfg.getParameter<double>("minPtSoftMuon");
    
    
//     std::cout << " minPtForJets_= " << minPtForJets_ << std::endl;
    name_puJetIdDiscriminant_= cfg.getParameter<std::string>("namePuJetIdDiscriminant");
    
    // consumes
    tausT_         = consumes<pat::TauCollection>(tauTag_);
    genParticlesT_ = consumes<reco::GenParticleCollection>(genParticlesTag_);
    fatJetHT_      = consumes<pat::JetCollection>(fatJetTag_);
    jetHT_         = consumes<pat::JetCollection>(jetTag_);
    rhoT_          = consumes<double>(rhoTag_);
    rhoCaloT_      = consumes<double>(rhoCaloTag_);
    rhoCentralNeutralT_  = consumes<double>(rhoCentralNeutralTag_);
    tagJetHT_      = consumes<pat::JetCollection>(tagJetTag_);
    secondTagJetHT_= consumes<pat::JetCollection>(secondJetTag_);
    pfMetHT_       = consumes<std::vector<pat::MET> >(pfMetTag_);
    if(!(pfMetNoHfTag_ == edm::InputTag(""))) pfMetNoHfHT_ = consumes<std::vector<pat::MET> >(pfMetNoHfTag_);
    if(!(pupMetTag_ == edm::InputTag("")))    pupMetHT_    = consumes<std::vector<pat::MET> >(pupMetTag_);
    vtxHT_         = consumes<reco::VertexCollection>(vtxTag_);
    candsHT_       = consumes<reco::CandidateView>(chCandsTag_);
    pfCandsHT_     = consumes<pat::PackedCandidateCollection>(pfCandsTag_);
    if (!(sptTag_  == edm::InputTag(""))) sptHT_   = consumes<edm::ValueMap<float> >(sptTag_);
    if (!(spt2Tag_ == edm::InputTag(""))) spt2HT_  = consumes<edm::ValueMap<float> >(spt2Tag_);
    triggerT_         = consumes<edm::TriggerResults>(triggerTag_);
    triggerPrescaleT_ = consumes<pat::PackedTriggerPrescales>(triggerPrescaleTag_);
    triggerL1minPrescaleT_ = consumes<pat::PackedTriggerPrescales>(triggerL1minPrescaleTag_);
    triggerL1maxPrescaleT_ = consumes<pat::PackedTriggerPrescales>(triggerL1maxPrescaleTag_);
    if (!(triggerSpecialTag_ == edm::InputTag(""))) { //---- like for MET special paths
     triggerSpecialT_ = consumes<edm::TriggerResults>(triggerSpecialTag_);
    }
     
     
    edm::Service<TFileService> fs ;
    _selectedTriggers = fs -> make<TH1F>("selectedTriggers", "selectedTriggers", 1000,  0., 1000.);
     
     
    muonsT_        = consumes<edm::View<reco::RecoCandidate> > (muTag_);
    softMuonsT_    = consumes<edm::View<reco::RecoCandidate> > (softMuTag_);
    electronsT_    = consumes<edm::View<reco::RecoCandidate> > (elTag_);
    photonsT_      = consumes<edm::View<reco::RecoCandidate> > (phoTag_); // Photon
    trackJetT_     = consumes<pat::JetCollection> (trackJetTag_);

    if (_isMC){
      if ( !( dressedMuonTag_ == edm::InputTag("")) )
        dressedMuonT_     = consumes<edm::View<reco::Candidate> > (dressedMuonTag_    );  
      if ( !( dressedElectronTag_ == edm::InputTag("")) )  
        dressedElectronT_ = consumes<edm::View<reco::Candidate> > (dressedElectronTag_);  

      if (!(htxsTag_ == edm::InputTag("")) ) 
        htxsT_ = consumes<HTXS::HiggsClassification>(htxsTag_); 
    }


    if (!(mcGenEventInfoTag_ == edm::InputTag(""))) GenInfoT_     = consumes<GenEventInfoProduct>(mcGenEventInfoTag_);
    if (!(mcGenWeightTag_    == edm::InputTag(""))) mcGenWeightT_ = consumes<GenFilterInfo>(mcGenWeightTag_); 
    if (!(mcLHEEventInfoTag_ == edm::InputTag(""))) {
     productLHET_         = consumes<LHEEventProduct>(mcLHEEventInfoTag_);
//      productLHERunInfoT_  = consumes<LHERunInfoProduct>(mcLHEEventInfoTag_);
    }
    if (!(genMetTag_ == edm::InputTag(""))) genMetHT_ = consumes<reco::GenMETCollection>(genMetTag_);
    if (!(genJetTag_ == edm::InputTag(""))) genJetHT_ = consumes<reco::GenJetCollection>(genJetTag_);

    if (_electronIds.size() != 0) {
     _vector_electronIdsTags.clear();
     for (unsigned int iEleIdName = 0; iEleIdName < _electronIds.size(); iEleIdName++) {
      if (_debug >= 2) std::cout << " electron id: " << iEleIdName << " : = " << _electronIds.at(iEleIdName) << std::endl;
      edm::EDGetTokenT<edm::ValueMap<bool> > temp_tag = consumes<edm::ValueMap<bool> >(_electronIds.at(iEleIdName));
      _vector_electronIdsTags.push_back(temp_tag);
      if (_debug >= 2) std::cout << " done! " << std::endl;
     }
    }
    if (_electronMvaIds.size() != 0) {
     _vector_electronMvaIdsTags.clear();
     for (unsigned int iEleIdName = 0; iEleIdName < _electronMvaIds.size(); iEleIdName++) {
      if (_debug >= 2) std::cout << " electron Mva id: " << iEleIdName << " : = " << _electronMvaIds.at(iEleIdName) << std::endl;
      edm::EDGetTokenT<edm::ValueMap<float> > temp_tag = consumes<edm::ValueMap<float> >(_electronMvaIds.at(iEleIdName));
      _vector_electronMvaIdsTags.push_back(temp_tag);
      if (_debug >= 2) std::cout << " done! " << std::endl;
     }
    }
}


void SkimEventProducer::produce(edm::Event& iEvent, const edm::EventSetup& iSetup) {

    if (_debug >= 1) {
     std::cout << " SkimEventProducer::produce " << std::endl;
    }
    
    std::auto_ptr<std::vector<reco::SkimEvent> > skimEvent(new std::vector<reco::SkimEvent> );

    edm::Handle<reco::GenParticleCollection> genParticles;
    if(!(genParticlesTag_ == edm::InputTag(""))) {
     iEvent.getByToken(genParticlesT_,genParticles);
    }

    edm::Handle<reco::CandidateView> dressedMuonsH;
    edm::Handle<reco::CandidateView> dressedElectronsH;
    edm::Handle<HTXS::HiggsClassification> htxs;
    if (_isMC){
      if (!(dressedMuonTag_ == edm::InputTag("")))
        iEvent.getByToken(dressedMuonT_, dressedMuonsH);
      if (!(dressedElectronTag_ == edm::InputTag("")))  
        iEvent.getByToken(dressedElectronT_, dressedElectronsH);
      if (!(htxsTag_ == edm::InputTag("")))
        iEvent.getByToken(htxsT_,htxs);
    }

    
    edm::Handle<pat::TauCollection> tausH;
    iEvent.getByToken(tausT_,tausH);
    
    edm::Handle<pat::JetCollection> fatJetH;
    iEvent.getByToken(fatJetHT_,fatJetH);

    edm::Handle<pat::JetCollection> jetH;
    iEvent.getByToken(jetHT_,jetH);

    edm::Handle<double> rhoJetIso;
    iEvent.getByToken(rhoT_,rhoJetIso);

    edm::Handle<double> rhoCalo;
    iEvent.getByToken(rhoCaloT_,rhoCalo);

    edm::Handle<double> rhoCentralNeutral;
    iEvent.getByToken(rhoCentralNeutralT_,rhoCentralNeutral);
    
    edm::Handle<pat::JetCollection> tagJetH;
    if(!(tagJetTag_==edm::InputTag(""))) iEvent.getByToken(tagJetHT_,tagJetH);

    edm::Handle<pat::JetCollection> secondTagJetH;
    if(!(secondJetTag_==edm::InputTag(""))) iEvent.getByToken(secondTagJetHT_,secondTagJetH);
    
    edm::Handle<pat::JetCollection> tagTrackJetH;
    if(!(trackJetTag_==edm::InputTag(""))) iEvent.getByToken(trackJetT_,tagTrackJetH);    
    
    edm::Handle< std::vector<pat::MET> > pfMetH;
    iEvent.getByToken(pfMetHT_,pfMetH);

    edm::Handle< std::vector<pat::MET> > pfMetNoHfH;
    if(!(pfMetNoHfTag_ == edm::InputTag(""))) iEvent.getByToken(pfMetNoHfHT_,pfMetNoHfH);

    edm::Handle< std::vector<pat::MET> > pupMetH;
    if(!(pupMetTag_ == edm::InputTag(""))) iEvent.getByToken(pupMetHT_,pupMetH);

    edm::Handle<reco::VertexCollection> vtxH;
    iEvent.getByToken(vtxHT_,vtxH);

    edm::Handle<reco::CandidateView> candsH;
    iEvent.getByToken(candsHT_,candsH);
    edm::Handle<pat::PackedCandidateCollection> pfCandsH;
    iEvent.getByToken(pfCandsHT_,pfCandsH);

    edm::Handle<edm::ValueMap<float> > sptH;
    if(!(sptTag_ == edm::InputTag(""))) iEvent.getByToken(sptHT_,sptH);

    edm::Handle<edm::ValueMap<float> > spt2H;
    if(!(spt2Tag_ == edm::InputTag(""))) iEvent.getByToken(spt2HT_,spt2H);

    edm::Handle<edm::TriggerResults> triggerResults;
    iEvent.getByToken(triggerT_,triggerResults);
    // May God have mercy on my soul ...
    std::vector<bool> passBits;
    passBits.push_back( singleMuData_.check(iEvent,*triggerResults) );
    passBits.push_back( singleElData_.check(iEvent,*triggerResults) );
    passBits.push_back( doubleMuData_.check(iEvent,*triggerResults) );
    passBits.push_back( doubleElData_.check(iEvent,*triggerResults) );
    passBits.push_back( muEGData_.check( iEvent,*triggerResults) );
    passBits.push_back( singleMuMC_.check(iEvent,*triggerResults) );
    passBits.push_back( singleElMC_.check(iEvent,*triggerResults) );
    passBits.push_back( doubleMuMC_.check(iEvent,*triggerResults) );
    passBits.push_back( doubleElMC_.check(iEvent,*triggerResults) );
    passBits.push_back( muEGMC_.check( iEvent,*triggerResults) );
    passBits.push_back( AllEmbed_.check( iEvent,*triggerResults) );
    passBits.push_back( FakeRate_El_.check( iEvent,*triggerResults) );
    passBits.push_back( FakeRate_Mu_.check( iEvent,*triggerResults) );
    
    
    edm::Handle<pat::PackedTriggerPrescales> triggerPrescales;
    iEvent.getByToken(triggerPrescaleT_, triggerPrescales);
    edm::Handle<pat::PackedTriggerPrescales> triggerL1minPrescales;
    iEvent.getByToken(triggerL1minPrescaleT_, triggerL1minPrescales);
    edm::Handle<pat::PackedTriggerPrescales> triggerL1maxPrescales;
    iEvent.getByToken(triggerL1maxPrescaleT_, triggerL1maxPrescales);
    
    
    //---- check selected list of trigger paths
    std::vector<float> passBitsSelected;
    std::vector<float> prescaleBitsSelected;
    std::vector<float> prescaleL1minBitsSelected;
    std::vector<float> prescaleL1maxBitsSelected;
    const edm::TriggerNames &names = iEvent.triggerNames(*triggerResults);
    for (unsigned int iPath = 0; iPath < SelectedPaths_.size(); iPath++) {
     bool foundPath = false;

     //---- save names of triggers     
     if (iPath<1000) {
       _selectedTriggers->GetXaxis()->SetBinLabel (iPath +1, SelectedPaths_.at(iPath).c_str());
      }
     
//      std::cout << " SelectedPaths[" << iPath << "] = " << SelectedPaths_.at(iPath) << std::endl;
     
     //---- remove "*" in the name
     std::string wantedTrigger = SelectedPaths_.at(iPath);
     std::size_t found = wantedTrigger.find("*");
     if (found != std::string::npos) {
      wantedTrigger = ( wantedTrigger.substr(0, wantedTrigger.size()-1) );
     }
     

     for (unsigned int jPath = 0, nmax = triggerResults->size(); jPath < nmax; jPath++) {
      std::string nameTrigger = names.triggerName(jPath);
      
      std::size_t foundInTrigger = nameTrigger.find(wantedTrigger);
      if (foundInTrigger != std::string::npos) {
       //---- see https://twiki.cern.ch/twiki/bin/view/CMSPublic/WorkBookMiniAOD2015#Trigger for instructions details
       passBitsSelected.push_back (1.0 * triggerResults->accept(jPath));
       prescaleBitsSelected.push_back (1.0 * triggerPrescales->getPrescaleForIndex(jPath) );
       prescaleL1minBitsSelected.push_back (1.0 * triggerL1minPrescales->getPrescaleForIndex(jPath) );
       prescaleL1maxBitsSelected.push_back (1.0 * triggerL1maxPrescales->getPrescaleForIndex(jPath) );
       //        std::cout << " >>  1.0 * triggerResults.push_back (1.0 * triggerPrescales->getPrescaleForIndex(jPath) );->accept(" << jPath << ") = " << 1.0 * triggerResults->accept(jPath) << std::endl;
       foundPath = true;
       break;
      }
     }
     if (!foundPath) {
      passBitsSelected.push_back (-1);
      prescaleBitsSelected.push_back (-1);
      prescaleL1minBitsSelected.push_back ( -1 );
      prescaleL1maxBitsSelected.push_back ( -1 );
     }
    }
    
    
    //---- check special list of trigger paths -> no prescale!   Like "metFilter" triggers
    
    
    edm::Handle<edm::TriggerResults> triggerSpecialResults;
    if (!(triggerSpecialTag_ == edm::InputTag(""))) {
     iEvent.getByToken(triggerSpecialT_,triggerSpecialResults);
    }
    
    std::vector<float> passBitsSpecial;

    if (!(triggerSpecialTag_ == edm::InputTag(""))) {
     
     const edm::TriggerNames &specialNames = iEvent.triggerNames(*triggerSpecialResults);
     
     for (unsigned int iPath = 0; iPath < SpecialPaths_.size(); iPath++) {
      
      bool foundPath = false;
      for (unsigned int jPath = 0, nmax = triggerSpecialResults->size(); jPath < nmax; jPath++) {
       std::string nameTrigger = specialNames.triggerName(jPath);
       
       if (nameTrigger == SpecialPaths_.at(iPath)) {
        passBitsSpecial.push_back (1.0 * triggerSpecialResults->accept(jPath));
        //        std::cout << " >>  1.0 * triggerSpecialResults->accept(" << jPath << ") = " << 1.0 * triggerSpecialResults->accept(jPath) << std::endl;
        foundPath = true;
       }
      }
      if (!foundPath) {
       passBitsSpecial.push_back (-1);
      }
     }
    }

    edm::Handle<bool> ifilterbadChCand;
    iEvent.getByToken(BadChCandFilterToken_, ifilterbadChCand);
    bool  filterbadChCandidate = *ifilterbadChCand;
    
    if (filterbadChCandidate) passBitsSpecial.push_back (1);
    else passBitsSpecial.push_back (0);

    edm::Handle<bool> ifilterbadPFMuon;
    iEvent.getByToken(BadPFMuonFilterToken_, ifilterbadPFMuon);
    bool filterbadPFMuon = *ifilterbadPFMuon;

    if (filterbadPFMuon) passBitsSpecial.push_back (1);
    else passBitsSpecial.push_back (0);
    
    
    
    //---- RecoCandidate in order to be used by SKimEvent later in a template way
    edm::Handle<edm::View<reco::RecoCandidate> > muons;
    iEvent.getByToken(muonsT_,muons);
    edm::Handle<edm::View<reco::RecoCandidate> > softMuons;
    iEvent.getByToken(softMuonsT_,softMuons);
    edm::Handle<edm::View<reco::RecoCandidate> > electrons;
    iEvent.getByToken(electronsT_,electrons);
    edm::Handle<edm::View<reco::RecoCandidate> > photons; // Photon
    iEvent.getByToken(photonsT_, photons);

    edm::Handle<GenFilterInfo> mcGenWeight;
    if (!(mcGenWeightTag_ == edm::InputTag(""))) {
     iEvent.getByToken(mcGenWeightT_, mcGenWeight);
    }

    edm::Handle<GenEventInfoProduct> GenInfoHandle;
    if (!(mcGenEventInfoTag_ == edm::InputTag(""))) {
      iEvent.getByToken(GenInfoT_, GenInfoHandle);
    }
    edm::Handle<LHEEventProduct>   productLHEHandle;
    edm::Handle<LHERunInfoProduct> productLHERunInfoHandle;
    if (!(mcLHEEventInfoTag_ == edm::InputTag(""))) {
     iEvent.getByToken(productLHET_, productLHEHandle);
//      iEvent.getByToken(productLHERunInfoT_, productLHERunInfoHandle);
//      iEvent.getByLabel(mcLHEEventInfoTag_, productLHERunInfoHandle);
//      iEvent.getByLabel("source", productLHERunInfoHandle);
    }

    edm::Handle<reco::GenMETCollection> genMetH;
    if (!(genMetTag_ == edm::InputTag(""))) {
    iEvent.getByToken(genMetHT_,genMetH);
    }

    edm::Handle<reco::GenJetCollection> genJetH;
    if (!(genJetTag_ == edm::InputTag(""))) {
    iEvent.getByToken(genJetHT_,genJetH);
    }

    skimEvent->push_back( *(new reco::SkimEvent() ) );

    skimEvent->back().setEventInfo(iEvent);

    skimEvent->back().setPFCollection(pfCandsH);

    //---- list of electron ids
    //----    save directly as vectors of "bool"
    //----    see: https://github.com/ikrav/EgammaWork/blob/ntupler_and_VID_demos/ElectronNtupler/plugins/ElectronNtuplerVIDDemo.cc
    //----         https://twiki.cern.ch/twiki/bin/view/CMS/CutBasedElectronIdentificationRun2#Recipe_for_regular_users_for_74X
    skimEvent->back().setElectronIds(_electronIds);
    std::vector < std::vector<bool> > v_electronIds;
    for (unsigned int iEleIdName = 0; iEleIdName < _electronIds.size(); iEleIdName++) {
     edm::Handle<edm::ValueMap<bool> > electronIdsHandle;
     if (_debug >= 2) std::cout << " electron id: get by token: " << iEleIdName << " [" << _electronIds.size() << "] : = " << _electronIds.at(iEleIdName) << std::endl;
     iEvent.getByToken(_vector_electronIdsTags.at(iEleIdName), electronIdsHandle);
     if (_debug >= 2) std::cout << " done " << std::endl;
     std::vector<bool> eleid_values;
     for(size_t i=0;i<electrons->size();++i) {
      if (isGoodElectron(electrons->ptrAt(i), rhoJetIso)){ 
       const auto patEle = electrons->ptrAt(i);
       eleid_values.push_back( (*electronIdsHandle)[patEle] ); 
      }
     }
     for(size_t k=0;k<muons->size();++k) {
      eleid_values.push_back( true );  //---- true by default for muons :-)
     } 
     if (_debug >= 2) std::cout << " addElectronId :: " << _electronIds.at(iEleIdName) << std::endl;
     skimEvent->back().addElectronId(eleid_values,_electronIds.at(iEleIdName));
    }

    std::vector < std::vector<bool> > v_electronMvaIds;
    for (unsigned int iEleIdName = 0; iEleIdName < _electronMvaIds.size(); iEleIdName++) {
     edm::Handle<edm::ValueMap<float> > electronMvaIdsHandle;
     if (_debug >= 2) std::cout << " electron Mva id: get by token: " << iEleIdName << " [" << _electronMvaIds.size() << "] : = " << _electronMvaIds.at(iEleIdName) << std::endl;
     iEvent.getByToken(_vector_electronMvaIdsTags.at(iEleIdName), electronMvaIdsHandle);
     if (_debug >= 2) std::cout << " done " << std::endl;
     std::vector<float> eleMvaid_values;
     for(size_t i=0;i<electrons->size();++i) {
      if (isGoodElectron(electrons->ptrAt(i), rhoJetIso)){ 
       const auto patEle = electrons->ptrAt(i);
       eleMvaid_values.push_back( (*electronMvaIdsHandle)[patEle] ); 
      }
     }
     for(size_t k=0;k<muons->size();++k) {
      eleMvaid_values.push_back( true );  //---- true by default for muons :-)
     } 
     if (_debug >= 2) std::cout << " addElectronId :: " << _electronMvaIds.at(iEleIdName) << std::endl;
     skimEvent->back().addElectronMvaId(eleMvaid_values,_electronMvaIds.at(iEleIdName));
    }
    

    //---- electrons
    if (_debug >= 1) {
     std::cout << " SkimEventProducer::produce:electrons->size () = " << electrons->size () << std::endl;
    }
    for(size_t i=0;i<electrons->size();++i) {
     if (isGoodElectron(electrons->ptrAt(i), rhoJetIso)){ 
      skimEvent->back().setLepton(electrons,i);
     }
    }

    //---- muons
    if (_debug >= 1) {
     std::cout << " SkimEventProducer::produce:muons->size () = " << muons->size () << std::endl;
    }
    for(size_t k=0;k<muons->size();++k) {
     skimEvent->back().setLepton(muons,k);
    }

    for(size_t k=0;k<softMuons->size();++k) {
     skimEvent->back().setSoftMuon(softMuons,k);
    }

    for(size_t k=0; k<photons->size();++k){ // Photon
     skimEvent->back().setPhoton(photons,k);
    }

    // Everything else
    skimEvent->back().setTaus(tausH);
    
    skimEvent->back().setTriggerBits(passBits);
    skimEvent->back().setSelectedTriggerBits(passBitsSelected);
    skimEvent->back().setSelectedTriggerBitsPrescales(prescaleBitsSelected);
    skimEvent->back().setSelectedTriggerL1minBitsPrescales(prescaleL1minBitsSelected);
    skimEvent->back().setSelectedTriggerL1maxBitsPrescales(prescaleL1maxBitsSelected);
    skimEvent->back().setSpecialTriggerBits(passBitsSpecial);
    
    
    
    skimEvent->back().setJets(jetH);
    skimEvent->back().setFatJets(fatJetH);
    skimEvent->back().setJetRhoIso(rhoJetIso);
    skimEvent->back().setJetRhoCaloIso(rhoCalo);
    skimEvent->back().setJetRhoCentralNeutralIso(rhoCentralNeutral);
    
    skimEvent->back().setPFMet(pfMetH);
    if (pfMetNoHfH.isValid() ) skimEvent->back().setPFMetNoHf(pfMetNoHfH);
    if (pupMetH.isValid() ) skimEvent->back().setPUpMet(pupMetH);
    skimEvent->back().setVertex(vtxH);
    if (sptH.isValid() ) skimEvent->back().setVtxSumPts(sptH);
    if (spt2H.isValid() ) skimEvent->back().setVtxSumPt2s(spt2H);
    if (tagJetH.isValid()) skimEvent->back().setTagJets(tagJetH);
    else skimEvent->back().setTagJets(jetH);

    if (secondTagJetH.isValid()) skimEvent->back().setSecondJets(secondTagJetH);
    if (tagTrackJetH.isValid()) skimEvent->back().setTrackJets(tagTrackJetH);
    
    if(genParticles.isValid()) {
     skimEvent->back().setGenParticles(genParticles);
     skimEvent->back().setGenLeptonIndices();
     skimEvent->back().setGenDiLeptFromZGstar(genParticles);

    }
    if(!(mcGenWeightTag_ == edm::InputTag(""))) {
     skimEvent->back().setGenWeight(mcGenWeight);
    }
    if(!(mcGenEventInfoTag_ == edm::InputTag(""))) {
     skimEvent->back().setGenInfo(GenInfoHandle);
    }
    if(!(mcLHEEventInfoTag_ == edm::InputTag(""))) {
     skimEvent->back().setLHEinfo(productLHEHandle);
     //      skimEvent->back().setLHEinfo(productLHEHandle,productLHERunInfoHandle);
    }
    
    if(!(genMetTag_ == edm::InputTag(""))) {
     skimEvent->back().setGenMet(genMetH);
    } 
    else {
     if (_isMC) {
      skimEvent->back().setGenMet(pfMetH); //---- in miniAOD met and genmet are linked
     }
    }
    const reco::VertexCollection *pvCol = vtxH.product();
    const reco::Vertex* pv = &(*pvCol->begin());
    reco::MET trkMet = computeTrkMet(*pv, pfCandsH);
    //std::cout<<"trkMetOut: "<<trkMet.pt()<<std::endl;
    skimEvent->back().setTrkMet(trkMet);


    // MET XY shift variables
    SetMetXYshiftVariables(pfCandsH);

    skimEvent->back().set_XYshift_counts(
	hEtaPlus_counts, hEtaMinus_counts, h0Barrel_counts,
	h0EndcapPlus_counts, h0EndcapMinus_counts, gammaBarrel_counts,
	gammaEndcapPlus_counts, gammaEndcapMinus_counts, hHFPlus_counts,
	hHFMinus_counts, egammaHFPlus_counts, egammaHFMinus_counts );


    skimEvent->back().set_XYshift_sumPt(
	hEtaPlus_sumPt, hEtaMinus_sumPt, h0Barrel_sumPt,
	h0EndcapPlus_sumPt, h0EndcapMinus_sumPt, gammaBarrel_sumPt,
	gammaEndcapPlus_sumPt, gammaEndcapMinus_sumPt, hHFPlus_sumPt,
	hHFMinus_sumPt, egammaHFPlus_sumPt, egammaHFMinus_sumPt );

    
    if(!(genJetTag_==edm::InputTag(""))) {
     skimEvent->back().setGenJets(genJetH);
    }

    //---- apply 50ns values
    skimEvent->back().setApply50nsValues(apply50nsValues_);
    //---- apply jet cleaning using lepton collection
    skimEvent->back().setApplyJetCleaning(applyJetCleaning_);
    //---- maximum eta for jets for default values
    skimEvent->back().setMaxEtaForJets(maxEtaForJets_);
    //---- maximum eta for jets for default values
    skimEvent->back().setMinPtForJets(minPtForJets_);
    //---- dz cut for btag jets for default values
    skimEvent->back().setDzCutForBtagJets(dzCutForBtagJets_);
    //---- apply correction for jets for default values
    skimEvent->back().setApplyCorrectionForJets(applyCorrectionForJets_);
    //---- apply ID for jets for default values
    skimEvent->back().setApplyIDForJets(applyIDForJets_);
    //---- pile up jet id discimanot name (it depends on the options of the jettoolbox!)
    skimEvent->back().setPuJetIdDiscriminantName(name_puJetIdDiscriminant_);
    
    //---- default values for soft muons from b-jets
    //----     max DR between jet and soft muon
    skimEvent->back().setMaxDrSoftMuonJet(_maxDrSoftMuonJet);
    //----     min pt of soft muon
    skimEvent->back().setMinPtSoftMuon(_minPtSoftMuon);

    if (_isMC) {
      if (!(dressedMuonTag_ == edm::InputTag(""))){
        for(size_t k=0; k<dressedMuonsH->size();++k){
          skimEvent->back().setDressedLepton(dressedMuonsH, k);
        }  
      }    
      if (!(dressedElectronTag_ == edm::InputTag(""))){
        for(size_t k=0; k<dressedElectronsH->size();++k){
          skimEvent->back().setDressedLepton(dressedElectronsH, k);
        }  
      }
      if (!(htxsTag_ == edm::InputTag("")))
        skimEvent->back().setHTXS(htxs);
    }
    
    iEvent.put(skimEvent);
}


//---- leptons filters

bool SkimEventProducer::isGoodElectron( const edm::Ptr<reco::RecoCandidate> electron,  const edm::Handle<double> &rho)
{
 const pat::Electron* patEle = static_cast<const pat::Electron*> (electron.get());
 double rhoIso = double (*rho);
 double sumChargedHadronPt = patEle->pfIsolationVariables().sumChargedHadronPt;
 double sumPhotonEt = patEle->pfIsolationVariables().sumPhotonEt;
 double sumNeutralHadronEt = patEle->pfIsolationVariables().sumNeutralHadronEt;
 
 //PHYS14 Cut based Loose WP 
 
 float SCeta=abs(patEle->superCluster()->eta());
 float el_pt=patEle->pt();
 
 if (_electronId == 1) {
  if(patEle->isEB() &&
   abs(patEle->dB(pat::Electron::PV2D)) < 0.035904 &&
   abs( sqrt( patEle->dB(pat::Electron::PV3D)*patEle->dB(pat::Electron::PV3D) - patEle->dB(pat::Electron::PV2D)*patEle->dB(pat::Electron::PV2D) ) ) < 0.075496 &&
   abs(patEle->deltaEtaSuperClusterTrackAtVtx()) < 0.009277 &&
   abs(patEle->deltaPhiSuperClusterTrackAtVtx()) < 0.094739 &&
   patEle->gsfTrack()->hitPattern().numberOfLostHits(reco::HitPattern::MISSING_INNER_HITS) <=1 &&
   patEle->full5x5_sigmaIetaIeta() < 0.010331 &&
   (1.0/patEle->ecalEnergy() - patEle->eSuperClusterOverP()/patEle->ecalEnergy()) < 0.189968 &&
   patEle->hcalOverEcal() < 0.093068 &&
   patEle->passConversionVeto() && 
   (  
   (SCeta >= 0.000 && SCeta < 0.800 && (sumChargedHadronPt+ std::max(0.0,sumNeutralHadronEt+sumPhotonEt - 0.1013 * rhoIso))/el_pt < 0.130136)||
   (SCeta >= 0.8 && SCeta < 1.3 && (sumChargedHadronPt+ std::max(0.0,sumNeutralHadronEt+sumPhotonEt - 0.0988 * rhoIso))/el_pt < 0.130136)||
   (SCeta >= 1.3 && SCeta < 2.0 && (sumChargedHadronPt+ std::max(0.0,sumNeutralHadronEt+sumPhotonEt - 0.0572 * rhoIso))/el_pt < 0.130136)||
   (SCeta >= 2.0 && SCeta < 2.2 && (sumChargedHadronPt+ std::max(0.0,sumNeutralHadronEt+sumPhotonEt - 0.0842 * rhoIso))/el_pt < 0.130136)||
   (SCeta >= 2.2 && SCeta < 2.5 && (sumChargedHadronPt+ std::max(0.0,sumNeutralHadronEt+sumPhotonEt - 0.1530 * rhoIso))/el_pt < 0.130136))){
   
   return true;
   
   }
   
   else if(
    abs(patEle->dB(pat::Electron::PV2D)) < 0.099266 &&
    abs( sqrt( patEle->dB(pat::Electron::PV3D)*patEle->dB(pat::Electron::PV3D) - patEle->dB(pat::Electron::PV2D)*patEle->dB(pat::Electron::PV2D) ) ) < 0.197897 &&
    abs(patEle->deltaEtaSuperClusterTrackAtVtx()) < 0.009833 &&
    abs(patEle->deltaPhiSuperClusterTrackAtVtx()) < 0.149934 &&
    patEle->gsfTrack()->hitPattern().numberOfLostHits(reco::HitPattern::MISSING_INNER_HITS)<=1 &&
    patEle->hadronicOverEm() < 0.115754 &&
    patEle->full5x5_sigmaIetaIeta() < 0.031838 &&
    (1.0/patEle->ecalEnergy() - patEle->eSuperClusterOverP()/patEle->ecalEnergy()) < 0.140662 &&
    patEle->hcalOverEcal() < 0.115754 &&
    patEle->passConversionVeto() && 
    (  
    (SCeta >= 0.000 && SCeta < 0.800 && (sumChargedHadronPt+ std::max(0.0,sumNeutralHadronEt+sumPhotonEt - 0.1013 * rhoIso))/el_pt < 0.163368)||
    (SCeta >= 0.8 && SCeta < 1.3 && (sumChargedHadronPt+ std::max(0.0,sumNeutralHadronEt+sumPhotonEt - 0.0988 * rhoIso))/el_pt < 0.163368 )||
    (SCeta >= 1.3 && SCeta < 2.0 && (sumChargedHadronPt+ std::max(0.0,sumNeutralHadronEt+sumPhotonEt - 0.0572 * rhoIso))/el_pt < 0.163368 )|| 
    (SCeta >= 2.0 && SCeta < 2.2 && (sumChargedHadronPt+ std::max(0.0,sumNeutralHadronEt+sumPhotonEt - 0.0842 * rhoIso))/el_pt < 0.163368 )|| 
    (SCeta >= 2.2 && SCeta < 2.5 && (sumChargedHadronPt+ std::max(0.0,sumNeutralHadronEt+sumPhotonEt - 0.1530 * rhoIso))/el_pt < 0.163368 )))
   {
    return true;
   }
   else {
    return false;
   }
 }
 
 else if (_electronId == -1) { //---- no selections
  return true;
 }
 else {
  return true;
 }
  
}

int SkimEventProducer::translateTypeToAbsPdgId( reco::PFCandidate::ParticleType type ) {
  switch( type ) {
    case reco::PFCandidate::ParticleType::h: return 211; // pi+
    case reco::PFCandidate::ParticleType::e: return 11;
    case reco::PFCandidate::ParticleType::mu: return 13;
    case reco::PFCandidate::ParticleType::gamma: return 22;
    case reco::PFCandidate::ParticleType::h0: return 130; // K_L0
    case reco::PFCandidate::ParticleType::h_HF: return 1; // dummy pdg code
    case reco::PFCandidate::ParticleType::egamma_HF: return 2; // dummy pdg code
    case reco::PFCandidate::ParticleType::X:
    default: return 0;
  }
}

reco::MET SkimEventProducer::computeTrkMet(const reco::Vertex &pv,
                      edm::Handle<pat::PackedCandidateCollection> candsH
        )
{
 using namespace std;
 //cout<<"pv.x:"<<pv.x()<<endl;
 reco::Candidate::LorentzVector totalP4;
 for(pat::PackedCandidateCollection::const_iterator it= candsH->begin(), ed =candsH->end(); it != ed; ++it){
  if( it->charge() == 0 ) continue;
  //cout<<"it.dz: "<<it->dz()<<endl;
  //if( it->trackRef().isNonnull() && pv.trackWeight(it->trackRef())>0)
  if( fabs(it->dz()) <0.1){
   totalP4 += it->p4();
  }
 }
 reco::Candidate::LorentzVector invertedP4(-totalP4);
 reco::MET met(invertedP4,reco::Candidate::Point(0,0,0));
 return met;
}

void SkimEventProducer::SetMetXYshiftVariables(
                      edm::Handle<pat::PackedCandidateCollection> pfCandsH)
{
    hEtaPlus_counts=0;
    hEtaPlus_sumPt=0;
    hEtaMinus_counts=0;
    hEtaMinus_sumPt=0;
    h0Barrel_counts=0;
    h0Barrel_sumPt=0;
    h0EndcapPlus_counts=0;
    h0EndcapPlus_sumPt=0;
    h0EndcapMinus_counts=0;
    h0EndcapMinus_sumPt=0;
    gammaBarrel_counts=0;
    gammaBarrel_sumPt=0;
    gammaEndcapPlus_counts=0;
    gammaEndcapPlus_sumPt=0;
    gammaEndcapMinus_counts=0;
    gammaEndcapMinus_sumPt=0;
    hHFPlus_counts=0;
    hHFPlus_sumPt=0;
    hHFMinus_counts=0;
    hHFMinus_sumPt=0;
    egammaHFPlus_counts=0;
    egammaHFPlus_sumPt=0;
    egammaHFMinus_counts=0;
    egammaHFMinus_sumPt=0;

    for( unsigned i(0); i <  pfCandsH->size() ; ++i){
    //for( pat::PackedCandidateCollection::const_iterator it= pfCandsH->begin(), ed =pfCandsH->end(); it != ed; ++it)
      const reco::Candidate& c = pfCandsH->at(i);
      if (abs(c.pdgId())== translateTypeToAbsPdgId(reco::PFCandidate::ParticleType( 1 ))) {
	if ( (c.eta()>0) and (c.eta()< 2.7) ){
	  hEtaPlus_counts +=1;
	  hEtaPlus_sumPt  +=c.pt();
	}
      }
      if (abs(c.pdgId())== translateTypeToAbsPdgId(reco::PFCandidate::ParticleType( 1 ))) {
	if ( (c.eta()> -2.7) and (c.eta()< 0.) ){
	  hEtaMinus_counts +=1;
	  hEtaMinus_sumPt  +=c.pt();
	}
      }
      if (abs(c.pdgId())== translateTypeToAbsPdgId(reco::PFCandidate::ParticleType( 5 ))) {
	if ( (c.eta()> -1.392) and (c.eta()< 1.392) ){
	  h0Barrel_counts +=1;
	  h0Barrel_sumPt  +=c.pt();
	}
      }
      if (abs(c.pdgId())== translateTypeToAbsPdgId(reco::PFCandidate::ParticleType( 5 ))) {
	if ( (c.eta()> 1.392) and (c.eta()< 3.) ){
	  h0EndcapPlus_counts +=1;
	  h0EndcapPlus_sumPt  +=c.pt();
	}
      }
      if (abs(c.pdgId())== translateTypeToAbsPdgId(reco::PFCandidate::ParticleType( 5 ))) {
	if ( (c.eta()> -3.) and (c.eta()< -1.392) ){
	  h0EndcapMinus_counts +=1;
	  h0EndcapMinus_sumPt  +=c.pt();
	}
      }
      if (abs(c.pdgId())== translateTypeToAbsPdgId(reco::PFCandidate::ParticleType( 4 ))) {
	if ( (c.eta()> -1.479) and (c.eta()< 1.479) ){
	  gammaBarrel_counts +=1;
	  gammaBarrel_sumPt  +=c.pt();
	}
      }
      if (abs(c.pdgId())== translateTypeToAbsPdgId(reco::PFCandidate::ParticleType( 4 ))) {
	if ( (c.eta()> 1.479) and (c.eta()< 3.0) ){
	  gammaEndcapPlus_counts +=1;
	  gammaEndcapPlus_sumPt  +=c.pt();
	}
      }
      if (abs(c.pdgId())== translateTypeToAbsPdgId(reco::PFCandidate::ParticleType( 4 ))) {
	if ( (c.eta()> -3.0) and (c.eta()< -1.479) ){
	  gammaEndcapMinus_counts +=1;
	  gammaEndcapMinus_sumPt  +=c.pt();
	}
      }
      if (abs(c.pdgId())== translateTypeToAbsPdgId(reco::PFCandidate::ParticleType( 6 ))) {
	if ( (c.eta()> 2.901376) and (c.eta()< 5.2) ){
	  hHFPlus_counts +=1;
	  hHFPlus_sumPt  +=c.pt();
	}
      }
      if (abs(c.pdgId())== translateTypeToAbsPdgId(reco::PFCandidate::ParticleType( 6 ))) {
	if ( (c.eta()> -5.2) and (c.eta()< -2.901376) ){
	  hHFMinus_counts +=1;
	  hHFMinus_sumPt  +=c.pt();
	}
      }
      if (abs(c.pdgId())== translateTypeToAbsPdgId(reco::PFCandidate::ParticleType( 7 ))) {
	if ( (c.eta()> 2.901376) and (c.eta()< 5.2) ){
	  egammaHFPlus_counts +=1;
	  egammaHFPlus_sumPt  +=c.pt();
	}
      }
      if (abs(c.pdgId())== translateTypeToAbsPdgId(reco::PFCandidate::ParticleType( 7 ))) {
	if ( (c.eta()> -5.2) and (c.eta()< -2.901376) ){
	  egammaHFMinus_counts +=1;
	  egammaHFMinus_sumPt  +=c.pt();
	}
      }
    }

}

reco::MET SkimEventProducer::doChMET(edm::Handle<reco::CandidateView> candsH,
        const reco::Candidate* cand1,const reco::Candidate* cand2)
{
 using namespace std;
 reco::Candidate::LorentzVector totalP4;
 for(reco::CandidateView::const_iterator it= candsH->begin(), ed =candsH->end(); it != ed; ++it){
  if( it->charge() == 0 ) continue;
  if(fabs(ROOT::Math::VectorUtil::DeltaR(it->p4(),cand1->p4())) <=0.1) continue;
  if(fabs(ROOT::Math::VectorUtil::DeltaR(it->p4(),cand2->p4())) <=0.1) continue;
  totalP4 += it->p4();
 }
 totalP4 +=cand1->p4();
 totalP4 +=cand2->p4();
 reco::Candidate::LorentzVector invertedP4(-totalP4);
 reco::MET met(invertedP4,reco::Candidate::Point(0,0,0));
 return met;
}


void SkimEventProducer::addDYMVA(reco::SkimEvent* event){
  float dymva0 = -999;
  float dymva1 = -999;

  if (event->nLep() == 2) {

    size_t index = 0;
    float minPt = 0;
    float eta = 4.7;
    int applyCorrection = 1;
    int applyID = 4;

    float jet1pt = event->leadingJetPt (index, minPt, eta, applyCorrection, applyID);
//     float jet1phi = event->leadingJetPhi(index, minPt, eta, applyCorrection, applyID);

//     double dPhiDiLepJet1 = fabs(event->dPhillLeadingJet(eta, applyCorrection, applyID));
//     double dPhiJet1MET = fabs(deltaPhi(jet1phi, event->pfMetPhi()));
//     double dPhillPfMET = fabs(event->dPhillPfMet());

    if (jet1pt < 15) {
      jet1pt = 15;
//       dPhiDiLepJet1 = -0.1;
//       dPhiJet1MET = -0.1;
    }

//     std::cout << " LT:: dPhiDiLepJet1 = " << dPhiDiLepJet1 << std::endl;
//     std::cout << " LT:: dPhiJet1MET = " << dPhiJet1MET << std::endl;
//     std::cout << " LT:: dPhillPfMET = " << dPhillPfMET << std::endl;

//     float px_rec = event->pfMet()*cos(event->pfMetPhi()) + event->pXll();
//     float py_rec = event->pfMet()*sin(event->pfMetPhi()) + event->pYll();
//     double recoil = sqrt(px_rec*px_rec + py_rec*py_rec);

//     std::cout << " LT:: recoil = " << recoil << std::endl;

//     dymva0 = getDYMVA_v0->getValue(event->nCentralJets(30.0, eta, applyCorrection,applyID),
//                    event->pfMet(),
//                    event->chargedMetSmurf(),
//                    jet1pt,
//                    event->pfMetSignificance(),
//                    dPhiDiLepJet1,
//                    dPhiJet1MET,
//                    event->mTHiggs(event->PFMET));
// 
//     dymva1 = getDYMVA_v1->getValue(event->nCentralJets(30.0, eta, applyCorrection, applyID),
//                    event->projPfMet(),
//                    event->projChargedMetSmurf(),
//                    event->nGoodVertices(),
//                    event->pTll(),
//                    jet1pt,
//                    event->pfMetMEtSig(),
//                    dPhiDiLepJet1,
//                    dPhillPfMET,
//                    dPhiJet1MET,
//                    recoil,
//                    event->mTHiggs(event->PFMET));
  
  }

  event->addUserFloat("dymva0", dymva0);
  event->addUserFloat("dymva1", dymva1);
}


//------------------------------------------------------------------------------
// makeJets
//------------------------------------------------------------------------------
void SkimEventProducer::makeJets(
//                  std::vector<MetUtilities::JetInfo> &iJetInfo,
                 const edm::Handle<pat::JetCollection> &jH,
                 reco::VertexCollection &iVertices)
{
//   iJetInfo.clear();

  pat::JetRefVector jrv;

  jrv.clear();

  for (size_t ijet=0; ijet<jH->size(); ++ijet) {

    jrv.push_back(pat::JetRef(jH,ijet));

    double eta = jrv[ijet]->correctedJet("Uncorrected", "none").eta();
    double energy = jrv[ijet]->correctedJet("Uncorrected", "none").energy();
    double neutralHadronEnergy = jrv[ijet]->correctedJet("Uncorrected", "none").neutralHadronEnergy();
    double neutralEmEnergy = jrv[ijet]->correctedJet("Uncorrected", "none").neutralEmEnergy();
    double chargedHadronEnergy = jrv[ijet]->correctedJet("Uncorrected", "none").chargedHadronEnergy();
    double chargedEmEnergy = jrv[ijet]->correctedJet("Uncorrected", "none").chargedEmEnergy();
    // int nConstituents = jrv[ijet]->correctedJet("Uncorrected", "none").nConstituents();
    int chargedMultiplicity = jrv[ijet]->correctedJet("Uncorrected", "none").chargedMultiplicity();

    if (energy == 0) continue;
    if (neutralHadronEnergy / energy > 0.99) continue;
    if (neutralEmEnergy / energy > 0.99) continue;
    // if (nConstituents < 2) continue;
    if (chargedHadronEnergy / energy <= 0 && fabs(eta) < 2.4) continue;
    if (chargedEmEnergy / energy > 0.99 && fabs(eta) < 2.4) continue;
    if (chargedMultiplicity < 1 && fabs(eta) < 2.4) continue;

//     double lNeuFrac = (energy > 0) ? (neutralEmEnergy + neutralHadronEnergy) / energy : 9999;

//     MetUtilities::JetInfo pJetObject;

//     pJetObject.p4 = jrv[ijet]->p4();
//     pJetObject.mva = jrv[ijet]->userFloat("jetMva");
//     pJetObject.neutFrac = lNeuFrac;

//     iJetInfo.push_back(pJetObject);
  }
}


//------------------------------------------------------------------------------
// makeCandidates
//------------------------------------------------------------------------------
void SkimEventProducer::makeCandidates(std::vector<std::pair<LorentzVector,double> > &iPFInfo,
                       edm::Handle<reco::CandidateView> cH,
                       reco::Vertex *iPV)
{
  iPFInfo.clear();

  for (reco::CandidateView::const_iterator it=cH->begin(), ed=cH->end(); it!=ed; ++it) {

    double pDZ = -999;

    if (iPV != 0) {

      double bsx = iPV->x();
      double bsy = iPV->y();
      double bsz = iPV->z();

      double vx = it->vx();
      double vy = it->vy();
      double vz = it->vz();

      if (vx != 0 || vy != 0 || vz != 0) {

    double px = it->p4().px();
    double py = it->p4().py();
    double pz = it->p4().pz();
    double pt = it->p4().pt();
    
    pDZ = fabs((vz - bsz) - ((vx - bsx)*px + (vy - bsy)*py)/pt * pz/pt);

    if (pDZ == 0) pDZ = -999;
      }
    }

    std::pair<LorentzVector,double> pPFObject(it->p4(), pDZ);

    iPFInfo.push_back(pPFObject);
  }
}


//------------------------------------------------------------------------------
// makeVertices
//------------------------------------------------------------------------------
void SkimEventProducer::makeVertices(std::vector<Vector> &iPVInfo,
                     reco::VertexCollection &iVertices)
{
  iPVInfo.clear();

  for (int i0 = 0; i0<(int)iVertices.size(); i0++) {

    const reco::Vertex *pVertex = &(iVertices.at(i0));

    Vector pVec;

    pVec.SetCoordinates(pVertex->x(), pVertex->y(), pVertex->z());

    iPVInfo.push_back(pVec);
  }
}


//------------------------------------------------------------------------------
// getMvaMet
//------------------------------------------------------------------------------
reco::PFMET SkimEventProducer::getMvaMet(const reco::Candidate *cand1,
                     const reco::Candidate *cand2,
                     reco::Vertex *iPV,
                     reco::PFMETCollection thePfMet)
{
  LorentzVector lVis1 = cand1->p4();
  LorentzVector lVis2 = cand2->p4();

  std::vector<LorentzVector > theLeptons;

  theLeptons.push_back(lVis1);
  theLeptons.push_back(lVis2);

//   std::pair<LorentzVector,TMatrixD> lMVAMetInfo = fMVAMet->GetMet(theLeptons,
//                                   lJetInfo,
//                                   lPFInfo,
//                                   lVtxInfo,
//                                   false);
// 
//   reco::PFMET lDummy;
// 
//   reco::PFMET lMVAMet(lDummy.getSpecific(),
//               thePfMet.at(0).sumEt(),
//               lMVAMetInfo.first,
//               iPV->position());

  reco::PFMET lDummy;

  reco::PFMET lMVAMet = lDummy;

  return lMVAMet;
}


SkimEventProducer::~SkimEventProducer() {
//   delete getDYMVA_v0;
//   delete getDYMVA_v1;
}
void SkimEventProducer::beginJob() { }
void SkimEventProducer::endJob() { }
DEFINE_FWK_MODULE(SkimEventProducer);
