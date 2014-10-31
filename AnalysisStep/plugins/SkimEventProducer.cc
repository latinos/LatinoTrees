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


SkimEventProducer::SkimEventProducer(const edm::ParameterSet& cfg):
  singleMuData_ ( cfg.getParameter<std::vector<std::string> >("singleMuDataPaths") ),
  singleElData_ ( cfg.getParameter<std::vector<std::string> >("singleElDataPaths") ),
  doubleMuData_ ( cfg.getParameter<std::vector<std::string> >("doubleMuDataPaths") ),
  doubleElData_ ( cfg.getParameter<std::vector<std::string> >("doubleElDataPaths") ),
  muEGData_ ( cfg.getParameter<std::vector<std::string> > ("muEGDataPaths") ),
  singleMuMC_ ( cfg.getParameter<std::vector<std::string> >("singleMuMCPaths") ),
  singleElMC_ ( cfg.getParameter<std::vector<std::string> >("singleElMCPaths") ),
  doubleMuMC_ ( cfg.getParameter<std::vector<std::string> >("doubleMuMCPaths") ),
  doubleElMC_ ( cfg.getParameter<std::vector<std::string> >("doubleElMCPaths") ),
  muEGMC_ ( cfg.getParameter<std::vector<std::string> >("muEGMCPaths") ),
  AllEmbed_ ( cfg.getParameter<std::vector<std::string> >("AllEmbedPaths") )
{
  
  if(cfg.existsAs<edm::InputTag>("triggerTag")) 
    triggerTag_  = cfg.getParameter<edm::InputTag>("triggerTag");  

  if(cfg.existsAs<edm::InputTag>("mcLHEEventInfoTag"))  
    mcLHEEventInfoTag_ = cfg.getParameter<edm::InputTag>("mcLHEEventInfoTag");
  if(cfg.existsAs<edm::InputTag>("mcGenEventInfoTag"))  
    mcGenEventInfoTag_ = cfg.getParameter<edm::InputTag>("mcGenEventInfoTag");
  if(cfg.existsAs<edm::InputTag>("mcGenWeightTag"))     
    mcGenWeightTag_    = cfg.getParameter<edm::InputTag>("mcGenWeightTag");
  if(cfg.existsAs<edm::InputTag>("genParticlesTag"))    
    genParticlesTag_   = cfg.getParameter<edm::InputTag>("genParticlesTag");
  if(cfg.existsAs<edm::InputTag>("genMetTag"))          
    genMetTag_         = cfg.getParameter<edm::InputTag>("genMetTag");
  if(cfg.existsAs<edm::InputTag>("genJetTag"))          
    genJetTag_         = cfg.getParameter<edm::InputTag>("genJetTag");

  if(cfg.existsAs<edm::InputTag>("muTag"))  
   muTag_  = cfg.getParameter<edm::InputTag>("muTag" ); 
  else throw cms::Exception("missing muon tag collection ");
  if(cfg.existsAs<edm::InputTag>("elTag"))  
   elTag_  = cfg.getParameter<edm::InputTag>("elTag" );
  else throw cms::Exception("missing electron tag collection ");
  if(cfg.existsAs<edm::InputTag>("softMuTag"))  
   softMuTag_  = cfg.getParameter<edm::InputTag>("softMuTag" );
  else throw cms::Exception("missing soft muon tag collection ");

  if(cfg.existsAs<edm::InputTag>("jetTag"))  
   jetTag_  = cfg.getParameter<edm::InputTag>("jetTag" );
  else throw cms::Exception("missing jet tag collection ");
  if(cfg.existsAs<edm::InputTag>("jetPuppiTag"))  
   jetPuppiTag_  = cfg.getParameter<edm::InputTag>("jetPuppiTag" );
  else throw cms::Exception("missing puppi jet tag collection ");

  if(cfg.existsAs<edm::InputTag>("tagJetTag"))  
   tagJetTag_      = cfg.getParameter<edm::InputTag>("tagJetTag" );
  else throw cms::Exception("missing tag jet tag collection ");
  if(cfg.existsAs<edm::InputTag>("tagJetPuppiTag"))  
   tagJetPuppiTag_  = cfg.getParameter<edm::InputTag>("tagJetPuppiTag" );
  else throw cms::Exception("missing tag puppi jet tag collection ");

  if(cfg.existsAs<edm::InputTag>("fatJetTag"))      
   fatJetTag_         = cfg.getParameter<edm::InputTag>("fatJetTag" );
  if(cfg.existsAs<edm::InputTag>("fatJetPuppiTag")) 
   fatJetPuppiTag_    = cfg.getParameter<edm::InputTag>("fatJetPuppiTag" );

  if(cfg.existsAs<edm::InputTag>("pfMetTag"))       
   pfMetTag_          = cfg.getParameter<edm::InputTag>("pfMetTag" );
  else throw cms::Exception("missing pf met tag collection ");
  if(cfg.existsAs<edm::InputTag>("pfPuppiMetTag"))  
   pfPuppiMetTag_     = cfg.getParameter<edm::InputTag>("pfPuppiMetTag" );
  else throw cms::Exception("missing pf puppi met tag collection ");

  if(cfg.existsAs<edm::InputTag>("tcMetTag"))       
   tcMetTag_ = cfg.getParameter<edm::InputTag>("tcMetTag" );
  if(cfg.existsAs<edm::InputTag>("chargedMetTag"))  
   chargedMetTag_ = cfg.getParameter<edm::InputTag>("chargedMetTag" );

  if(cfg.existsAs<edm::InputTag>("vtxTag"))  
   vtxTag_  = cfg.getParameter<edm::InputTag>("vtxTag" );
  else throw cms::Exception("missing vertex tag collection ");

  if(cfg.existsAs<edm::InputTag>("pfCandsTag"))  
   pfCandsTag_ = cfg.getParameter<edm::InputTag>("pfCandsTag" );
  else throw cms::Exception("missing pf candidate tag collection ");

  if(cfg.existsAs<edm::InputTag>("rhoTag"))      
   rhoTag_ = cfg.getParameter<edm::InputTag>("rhoTag" );
  else throw cms::Exception("missing rho tag collection ");

  if (cfg.existsAs<edm::InputTag>("sptTag")) 
     sptTag_ = cfg.getParameter<edm::InputTag>("sptTag" );
  else 
     sptTag_ = edm::InputTag("","","");
  if (cfg.existsAs<edm::InputTag>("spt2Tag" )) 
     spt2Tag_ = cfg.getParameter<edm::InputTag>("spt2Tag" );
  else 
     spt2Tag_ = edm::InputTag("","","");

  // produce output collection 
  produces<std::vector<reco::SkimEvent> >().setBranchAlias(cfg.getParameter<std::string>("@module_label"));

  // consumes
  rhoT_          = consumes<double>(rhoTag_);
  genParticlesT_ = consumes<reco::GenParticleCollection>(genParticlesTag_);

  if(!(fatJetTag_ == edm::InputTag("")))      fatJetHT_ = consumes<pat::JetCollection>(fatJetTag_);
  if(!(fatJetPuppiTag_ == edm::InputTag(""))) fatJetPuppiHT_ = consumes<pat::JetCollection>(fatJetPuppiTag_);

  jetHT_         = consumes<pat::JetCollection>(jetTag_);
  jetPuppiHT_    = consumes<pat::JetCollection>(jetPuppiTag_);
  tagJetPuppiHT_ = consumes<pat::JetCollection>(tagJetPuppiTag_);
  if(!(tagJetTag_  == edm::InputTag(""))) tagJetHT_      = consumes<pat::JetCollection>(tagJetTag_);
  if(!(pfPuppiMetTag_  == edm::InputTag(""))) pfPuppiMetHT_  = consumes<std::vector<pat::MET> >(pfPuppiMetTag_);
  pfMetHT_       = consumes<std::vector<pat::MET> >(pfMetTag_);
  vtxHT_         = consumes<reco::VertexCollection>(vtxTag_);
  pfCandsHT_     = consumes<reco::CandidateView>(pfCandsTag_);

  if(!(sptTag_  == edm::InputTag(""))) sptHT_   = consumes<edm::ValueMap<float> >(sptTag_);
  if(!(spt2Tag_ == edm::InputTag(""))) spt2HT_  = consumes<edm::ValueMap<float> >(spt2Tag_);
  if(!(triggerTag_ == edm::InputTag(""))) triggerResultsT_  = consumes<edm::TriggerResults>(triggerTag_);

  muonsT_        = consumes<edm::View<reco::RecoCandidate> > (muTag_);
  softMuonT_     = consumes<edm::View<reco::RecoCandidate> > (softMuTag_);
  electronsT_    = consumes<edm::View<reco::RecoCandidate> > (elTag_);

  if (!(mcGenEventInfoTag_ == edm::InputTag(""))) GenInfoT_     = consumes<GenEventInfoProduct>(mcGenEventInfoTag_);
  if (!(mcGenWeightTag_    == edm::InputTag(""))) mcGenWeightT_ = consumes<GenFilterInfo>(mcGenWeightTag_); 
  if (!(mcLHEEventInfoTag_ == edm::InputTag(""))) productLHET_  = consumes<LHEEventProduct>(mcLHEEventInfoTag_);
  if (!(genMetTag_ == edm::InputTag(""))) genMetHT_ = consumes<reco::GenMETCollection>(genMetTag_);
  if (!(genJetTag_ == edm::InputTag(""))) genJetHT_ = consumes<reco::GenJetCollection>(genJetTag_);

}


void SkimEventProducer::produce(edm::Event& iEvent, const edm::EventSetup& iSetup) {
  
    std::auto_ptr<std::vector<reco::SkimEvent> > skimEvent(new std::vector<reco::SkimEvent> );

    edm::Handle<pat::JetCollection> jetH;
    iEvent.getByToken(jetHT_,jetH);

    edm::Handle<pat::JetCollection> jetPuppiH;
    if(!(jetPuppiTag_==edm::InputTag(""))) iEvent.getByToken(jetPuppiHT_,jetPuppiH);

    edm::Handle<double> rhoJetIso;
    iEvent.getByToken(rhoT_,rhoJetIso);

    edm::Handle<pat::JetCollection> tagJetH;
    if(!(tagJetTag_==edm::InputTag(""))) iEvent.getByToken(tagJetHT_,tagJetH);

    edm::Handle<pat::JetCollection> tagJetPuppiH;
    if(!(tagJetPuppiTag_==edm::InputTag(""))) iEvent.getByToken(tagJetPuppiHT_,tagJetPuppiH);

    edm::Handle< std::vector<pat::MET> > pfMetH;
    iEvent.getByToken(pfMetHT_,pfMetH);

    edm::Handle< std::vector<pat::MET> > pfPuppiMetH;
    if(!(pfPuppiMetTag_ == edm::InputTag(""))){
     iEvent.getByToken(pfPuppiMetHT_,pfPuppiMetH);
    }

    edm::Handle<reco::VertexCollection> vtxH;
    iEvent.getByToken(vtxHT_,vtxH);

    edm::Handle<reco::CandidateView> pfCandsH;
    iEvent.getByToken(pfCandsHT_,pfCandsH);

    edm::Handle<reco::GenParticleCollection> genParticles;
    if(!(genParticlesTag_ == edm::InputTag(""))) {
     iEvent.getByToken(genParticlesT_,genParticles);
    }

    edm::Handle<pat::JetCollection> fatJetH;
    if(!(fatJetTag_ == edm::InputTag(""))) {   
     iEvent.getByToken(fatJetHT_,fatJetH);
    }

    edm::Handle<pat::JetCollection> fatJetPuppiH;
    if(!(fatJetPuppiTag_ == edm::InputTag(""))) {   
     iEvent.getByToken(fatJetPuppiHT_,fatJetPuppiH);
    }

    edm::Handle<edm::ValueMap<float> > sptH;
    if(!(sptTag_ == edm::InputTag(""))) iEvent.getByToken(sptHT_,sptH);

    edm::Handle<edm::ValueMap<float> > spt2H;
    if(!(spt2Tag_ == edm::InputTag(""))) iEvent.getByToken(spt2HT_,spt2H);

    edm::Handle<edm::TriggerResults> triggerResults;
    if(!(triggerTag_ == edm::InputTag(""))){
     iEvent.getByToken(triggerResultsT_,triggerResults);
    }
 
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

    //---- RecoCandidate in order to be used by SKimEvent later in a template way
    edm::Handle<edm::View<reco::RecoCandidate> > muons;
    iEvent.getByToken(muonsT_,muons);
    edm::Handle<edm::View<reco::RecoCandidate> > softMu;
    iEvent.getByToken(softMuonT_,softMu);
    edm::Handle<edm::View<reco::RecoCandidate> > electrons;
    iEvent.getByToken(electronsT_,electrons);

    edm::Handle<GenFilterInfo> mcGenWeight;
    if (!(mcGenWeightTag_ == edm::InputTag(""))) {
     iEvent.getByToken(mcGenWeightT_, mcGenWeight);
    }

    edm::Handle<GenEventInfoProduct> GenInfoHandle;
    if (!(mcGenEventInfoTag_ == edm::InputTag(""))) {
      iEvent.getByToken(GenInfoT_, GenInfoHandle);
    }
    edm::Handle<LHEEventProduct> productLHEHandle;
    if (!(mcLHEEventInfoTag_ == edm::InputTag(""))) {
     iEvent.getByToken(productLHET_, productLHEHandle);
    }

    edm::Handle<reco::GenMETCollection> genMetH;
    if (!(genMetTag_ == edm::InputTag(""))) {
    iEvent.getByToken(genMetHT_,genMetH);
    }

    edm::Handle<reco::GenJetCollection> genJetH;
    if (!(genJetTag_ == edm::InputTag(""))) {
    iEvent.getByToken(genJetHT_,genJetH);
    }

    // new event in the output vector and set leptons
    skimEvent->push_back( *(new reco::SkimEvent() ) );

    if(electrons.isValid()){
     for(size_t i = 0; i < electrons->size(); ++i)
      skimEvent->back().setLepton(electrons,i);
    }

    if(muons.isValid()){
     for(size_t k = 0; k<muons->size(); ++k) 
      skimEvent->back().setLepton(muons,k);
    }

    if(softMu.isValid()){
     for(size_t k=0;k<softMu->size();++k) 
      skimEvent->back().setSoftMuon(softMu,k);
    }
 
    // Everything else 
    skimEvent->back().setTriggerBits(passBits);

    // set jet info      
    if(jetH.isValid())      skimEvent->back().setJets(jetH);
    if(jetPuppiH.isValid()) skimEvent->back().setPuppiJets(jetPuppiH);
    if(fatJetH.isValid() )  skimEvent->back().setFatJets(fatJetH);
    if(fatJetPuppiH.isValid()) skimEvent->back().setFatPuppiJets(fatJetPuppiH);
    if(rhoJetIso.isValid())    skimEvent->back().setJetRhoIso(rhoJetIso);
    if(pfMetH.isValid())       skimEvent->back().setPFMet(pfMetH);
    if(pfPuppiMetH.isValid())  skimEvent->back().setPuppiPFMet(pfPuppiMetH);
    if(vtxH.isValid())         skimEvent->back().setVertex(vtxH);

    if(sptH.isValid() )   skimEvent->back().setVtxSumPts(sptH);
    if(spt2H.isValid() )  skimEvent->back().setVtxSumPt2s(spt2H);

    if(tagJetH.isValid()) skimEvent->back().setTagJets(tagJetH);
    else if(jetH.isValid()) skimEvent->back().setTagJets(jetH);

    if(tagJetPuppiH.isValid()) skimEvent->back().setTagJets(tagJetPuppiH);
    else if(jetPuppiH.isValid()) skimEvent->back().setTagJets(jetPuppiH);

    if(genParticles.isValid())  skimEvent->back().setGenParticles(genParticles);
    
    if(mcGenWeight.isValid())   skimEvent->back().setGenWeight(mcGenWeight);
    
    if(GenInfoHandle.isValid()) skimEvent->back().setGenInfo(GenInfoHandle);
    
    if(productLHEHandle.isValid()) skimEvent->back().setLHEinfo(productLHEHandle);
    
    if(genMetH.isValid()) skimEvent->back().setGenMet(genMetH);
    
    if(genJetH.isValid()) skimEvent->back().setGenJets(genJetH);
    
    iEvent.put(skimEvent);
}


pat::MET SkimEventProducer::doChMET(edm::Handle<reco::CandidateView> pfCandsH, const reco::Candidate* cand1, const reco::Candidate* cand2){
    reco::Candidate::LorentzVector totalP4;
    for(reco::CandidateView::const_iterator it= pfCandsH->begin();  it !=  pfCandsH->end(); ++it){
        if( it->charge() == 0 ) continue;
        if(fabs(ROOT::Math::VectorUtil::DeltaR(it->p4(),cand1->p4())) <= 0.1) continue;
        if(fabs(ROOT::Math::VectorUtil::DeltaR(it->p4(),cand2->p4())) <= 0.1) continue;
        totalP4 += it->p4();
    }
    totalP4 +=cand1->p4();
    totalP4 +=cand2->p4();
    reco::Candidate::LorentzVector invertedP4(-totalP4);
    pat::MET met(reco::MET(invertedP4,reco::Candidate::Point(0,0,0)));
    return met;
}


//------------------------------------------------------------------------------
// makeJets
//------------------------------------------------------------------------------

void SkimEventProducer::makeJets( const edm::Handle<pat::JetCollection> & jH, const reco::VertexCollection & iVertices){

  pat::JetRefVector jrv;
  jrv.clear();

  for (size_t ijet=0; ijet < jH->size(); ++ijet) {

    jrv.push_back(pat::JetRef(jH,ijet));

    double eta    = jrv[ijet]->correctedJet("Uncorrected","none").eta();
    double energy = jrv[ijet]->correctedJet("Uncorrected","none").energy();
    double neutralHadronEnergy = jrv[ijet]->correctedJet("Uncorrected", "none").neutralHadronEnergy();
    double neutralEmEnergy     = jrv[ijet]->correctedJet("Uncorrected", "none").neutralEmEnergy();
    double chargedHadronEnergy = jrv[ijet]->correctedJet("Uncorrected", "none").chargedHadronEnergy();
    double chargedEmEnergy     = jrv[ijet]->correctedJet("Uncorrected", "none").chargedEmEnergy();
    //int nConstituents          = jrv[ijet]->correctedJet("Uncorrected", "none").nConstituents();
    int chargedMultiplicity    = jrv[ijet]->correctedJet("Uncorrected", "none").chargedMultiplicity();

    if (energy == 0) continue;
    if (neutralHadronEnergy / energy > 0.99) continue;
    if (neutralEmEnergy / energy > 0.99) continue;
    if (chargedHadronEnergy / energy <= 0 && fabs(eta) < 2.4) continue;
    if (chargedEmEnergy / energy > 0.99 && fabs(eta) < 2.4) continue;
    if (chargedMultiplicity < 1 && fabs(eta) < 2.4) continue;
  }

}


//------------------------------------------------------------------------------
// makeCandidates
//------------------------------------------------------------------------------
void SkimEventProducer::makeCandidates(std::vector<std::pair<LorentzVector,double> > &iPFInfo,
                                       const edm::Handle<reco::CandidateView> & cH,
                                       reco::Vertex *iPV){

  iPFInfo.clear();
  for (reco::CandidateView::const_iterator it = cH->begin(), ed = cH->end(); it!=ed; ++it) {

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
void SkimEventProducer::makeVertices(std::vector<Vector> &iPVInfo,const reco::VertexCollection &iVertices){
  iPVInfo.clear();
  for (int i0 = 0; i0<(int)iVertices.size(); i0++) {

    const reco::Vertex *pVertex = &(iVertices.at(i0));
    Vector pVec;
    pVec.SetCoordinates(pVertex->x(), pVertex->y(), pVertex->z());
    iPVInfo.push_back(pVec);
  }
}




SkimEventProducer::~SkimEventProducer() {}

void SkimEventProducer::beginJob() { }
void SkimEventProducer::endJob() { }

DEFINE_FWK_MODULE(SkimEventProducer);
