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

// BDT ElectronID
// #include "HiggsAnalysis/HiggsToWW2Leptons/interface/ElectronIDMVA.h"

// DYMVA
#include "DataFormats/Math/interface/deltaR.h"
// #include "DYMvaInCMSSW/GetDYMVA/interface/GetDYMVA.h"

// MVAMet
#include <TLorentzVector.h>
#include "DataFormats/Math/interface/LorentzVector.h"
#include "DataFormats/Math/interface/Vector.h"
// #include "pharris/MVAMet/interface/MetUtilities.h"
// #include "pharris/MVAMet/interface/MVAMet.h"

//---- pat
#include <DataFormats/PatCandidates/interface/MET.h>

// MC information Gen level
#include "SimDataFormats/GeneratorProducts/interface/GenEventInfoProduct.h"

class SkimEventProducer : public edm::EDProducer {
    public:
        explicit SkimEventProducer(const edm::ParameterSet&);
        ~SkimEventProducer();


// MVAMet
typedef math::XYZTLorentzVector LorentzVector;
typedef math::XYZVector Vector;
  
struct JetInfo {
LorentzVector p4;
double mva;
double neutFrac;
};


    private:
        virtual void beginJob() ;
        virtual void produce(edm::Event&, const edm::EventSetup&);
        virtual void endJob() ;
reco::MET doChMET(edm::Handle<reco::CandidateView> candsH,
const reco::Candidate* cand1,const reco::Candidate* cand2);
 
        std::string branchAlias_;
//      reco::SkimEvent::hypoType hypoType_;

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

        edm::InputTag mcLHEEventInfoTag_;
        edm::InputTag mcGenEventInfoTag_;
        edm::InputTag mcGenWeightTag_;
        edm::InputTag genParticlesTag_;
        edm::InputTag genMetTag_;
        edm::InputTag genJetTag_;
        edm::InputTag muTag_;
        edm::InputTag elTag_;
        edm::InputTag softMuTag_;
        edm::InputTag jetTag_;
        edm::InputTag fatJetTag_;
        edm::InputTag tagJetTag_;
        edm::InputTag pfMetTag_;
        edm::InputTag tcMetTag_;
        edm::InputTag chargedMetTag_;
        edm::InputTag vtxTag_;
// edm::InputTag allCandsTag_; // Needed for MVAMet
        edm::InputTag chCandsTag_;
        edm::InputTag sptTag_;
        edm::InputTag spt2Tag_;
        edm::InputTag rhoTag_;

// std::string l2File_;
// std::string l3File_;
// std::string resFile_;
      
// GetDYMVA *getDYMVA_v0;
// GetDYMVA *getDYMVA_v1;

void addDYMVA(reco::SkimEvent* event);

// MVAMet *fMVAMet;

std::vector<std::pair<LorentzVector,double> > lPFInfo;
// std::vector<MetUtilities::JetInfo> lJetInfo;
std::vector<Vector> lVtxInfo;

void makeJets (
//    std::vector<MetUtilities::JetInfo> &iJetInfo,
   const edm::Handle<pat::JetCollection> &jH,
   reco::VertexCollection &iVertices);

void makeCandidates (std::vector<std::pair<LorentzVector,double> > &iPFInfo,
edm::Handle<reco::CandidateView> cH,
reco::Vertex *iPV);

void makeVertices (std::vector<Vector> &iPVInfo,
reco::VertexCollection &iVertices);

reco::PFMET getMvaMet(const reco::Candidate *cand1,
const reco::Candidate *cand2,
reco::Vertex *iPV,
reco::PFMETCollection thePfMet);
};


#endif