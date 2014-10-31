#ifndef MYPRODUCERS_SKIMEVENTPRODUCER_SKIMEVENTPRODUCER_H
#define MYPRODUCERS_SKIMEVENTPRODUCER_SKIMEVENTPRODUCER_H

#include <memory>

#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "LatinoTrees/DataFormats/interface/SkimEvent.h"
#include "LatinoTrees/AnalysisStep/interface/TriggerBitChecker.h"

#include "CommonTools/Utils/interface/StringCutObjectSelector.h"

// DYMVA
#include "DataFormats/Math/interface/deltaR.h"

// MVAMet
#include <TLorentzVector.h>
#include "DataFormats/Math/interface/LorentzVector.h"
#include "DataFormats/Math/interface/Vector.h"

//---- pat
#include <DataFormats/PatCandidates/interface/MET.h>

// MC information Gen level
#include "SimDataFormats/GeneratorProducts/interface/GenEventInfoProduct.h"

class SkimEventProducer : public edm::EDProducer {

  public:

     explicit SkimEventProducer(const edm::ParameterSet&);
      ~SkimEventProducer();
     virtual void beginJob() ;
     virtual void endJob() ;
     virtual void produce(edm::Event&, const edm::EventSetup&);

     // MVAMet
     typedef math::XYZTLorentzVector LorentzVector;
     typedef math::XYZVector Vector;
  
     struct JetInfo {
        LorentzVector p4;
        double mva;
        double neutFrac;
     };

     pat::MET doChMET(edm::Handle<reco::CandidateView> candsH, const reco::Candidate* cand1,const reco::Candidate* cand2);
     void makeJets       (const edm::Handle<pat::JetCollection> &jH, const reco::VertexCollection &iVertices);
     void makeCandidates (std::vector<std::pair<LorentzVector,double> > &iPFInfo, const edm::Handle<reco::CandidateView> & cH, reco::Vertex *iPV);
     void makeVertices   (std::vector<Vector> &iPVInfo, const reco::VertexCollection &iVertices);
 
  private:

   std::string branchAlias_;
   edm::InputTag triggerTag_;

   TriggerBitChecker singleMuData_;
   TriggerBitChecker singleElData_;
   TriggerBitChecker doubleMuData_;
   TriggerBitChecker doubleElData_;
   TriggerBitChecker muEGData_;
   TriggerBitChecker singleMuMC_;
   TriggerBitChecker singleElMC_;
   TriggerBitChecker doubleMuMC_;
   TriggerBitChecker doubleElMC_;
   TriggerBitChecker muEGMC_;
   TriggerBitChecker AllEmbed_;

   edm::InputTag mcLHEEventInfoTag_; // LHE Info 
   edm::InputTag mcGenEventInfoTag_; // Generator info
   edm::InputTag mcGenWeightTag_;    // Generator weight
   edm::InputTag genParticlesTag_;   // Gen Particles
   edm::InputTag genMetTag_;         // Gen Met
   edm::InputTag genJetTag_;         // Gen Jets

   edm::InputTag muTag_; // reco muon
   edm::InputTag elTag_; // reco ele
   edm::InputTag softMuTag_; // reco soft mu


   edm::InputTag jetTag_; // standard jet
   edm::InputTag jetPuppiTag_; // puppi jet
   edm::InputTag fatJetTag_;
   edm::InputTag fatJetPuppiTag_; // puppi fat jet
   edm::InputTag tagJetTag_; // tag vbf
   edm::InputTag tagJetPuppiTag_; // tag vbf puppi

   edm::InputTag pfMetTag_; // pfMet
   edm::InputTag pfPuppiMetTag_; // pf puppi met
   edm::InputTag tcMetTag_; // tracker met
   edm::InputTag chargedMetTag_; // charged met

   edm::InputTag vtxTag_; // verteces
   edm::InputTag pfCandsTag_; // pfCandidates

   edm::InputTag sptTag_;
   edm::InputTag spt2Tag_;
   edm::InputTag rhoTag_; // rho

   edm::EDGetTokenT<double> rhoT_  ;
   edm::EDGetTokenT<reco::GenParticleCollection> genParticlesT_; // gen particles
   edm::EDGetTokenT<pat::JetCollection> fatJetHT_ ;              // fat jets
   edm::EDGetTokenT<pat::JetCollection> fatJetPuppiHT_ ;         
   edm::EDGetTokenT<pat::JetCollection> jetHT_ ;
   edm::EDGetTokenT<pat::JetCollection> jetPuppiHT_ ;  
   edm::EDGetTokenT<pat::JetCollection> tagJetHT_ ; 
   edm::EDGetTokenT<pat::JetCollection> tagJetPuppiHT_ ;

   edm::EDGetTokenT<std::vector<pat::MET> > pfMetHT_;
   edm::EDGetTokenT<std::vector<pat::MET> > pfPuppiMetHT_;

   edm::EDGetTokenT<reco::VertexCollection> vtxHT_;
   edm::EDGetTokenT<reco::CandidateView>   pfCandsHT_;
   edm::EDGetTokenT<edm::ValueMap<float> > sptHT_;
   edm::EDGetTokenT<edm::ValueMap<float> > spt2HT_;
   edm::EDGetTokenT<edm::TriggerResults> triggerResultsT_;

   edm::EDGetTokenT<edm::View<reco::RecoCandidate> > muonsT_;
   edm::EDGetTokenT<edm::View<reco::RecoCandidate> > softMuonT_;
   edm::EDGetTokenT<edm::View<reco::RecoCandidate> > electronsT_;

   edm::EDGetTokenT<GenEventInfoProduct> GenInfoT_ ;
   edm::EDGetTokenT<GenFilterInfo> mcGenWeightT_;
   edm::EDGetTokenT<LHEEventProduct> productLHET_ ;

   edm::EDGetTokenT<reco::GenMETCollection> genMetHT_;
   edm::EDGetTokenT<reco::GenJetCollection> genJetHT_;

   std::vector<std::pair<LorentzVector,double> > lPFInfo;
   std::vector<Vector> lVtxInfo;

};


#endif
