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
        edm::InputTag chCandsTag_;
        edm::InputTag sptTag_;
        edm::InputTag spt2Tag_;
        edm::InputTag rhoTag_;

	edm::EDGetTokenT<reco::GenParticleCollection> genParticlesT_;
	edm::EDGetTokenT<pat::JetCollection> fatJetHT_ ;
	edm::EDGetTokenT<pat::JetCollection> jetHT_ ;
	edm::EDGetTokenT<double> rhoT_  ;
	edm::EDGetTokenT<pat::JetCollection> tagJetHT_ ;
	edm::EDGetTokenT<std::vector<pat::MET> > pfMetHT_;
	edm::EDGetTokenT<reco::VertexCollection> vtxHT_;
	edm::EDGetTokenT<reco::CandidateView> candsHT_;
	edm::EDGetTokenT<edm::ValueMap<float> > sptHT_;
	edm::EDGetTokenT<edm::ValueMap<float> > spt2HT_;
	edm::EDGetTokenT<edm::TriggerResults> triggerResultsT_;
	edm::EDGetTokenT<edm::View<reco::RecoCandidate> > muonsT_;
	edm::EDGetTokenT<edm::View<reco::RecoCandidate> > softsT_;
	edm::EDGetTokenT<edm::View<reco::RecoCandidate> > electronsT_;
	edm::EDGetTokenT<GenEventInfoProduct> GenInfoT_ ;
	edm::EDGetTokenT<GenFilterInfo> mcGenWeightT_;
	edm::EDGetTokenT<LHEEventProduct> productLHET_ ;
	edm::EDGetTokenT<reco::GenMETCollection> genMetHT_;
	edm::EDGetTokenT<reco::GenJetCollection> genJetHT_;
	edm::EDGetTokenT<edm::TriggerResults> triggerT_;

        void addDYMVA(reco::SkimEvent* event);
        std::vector<std::pair<LorentzVector,double> > lPFInfo;
        std::vector<Vector> lVtxInfo;

        void makeJets       (const edm::Handle<pat::JetCollection> &jH, reco::VertexCollection &iVertices);
        void makeCandidates (std::vector<std::pair<LorentzVector,double> > &iPFInfo, edm::Handle<reco::CandidateView> cH, reco::Vertex *iPV);
        void makeVertices   (std::vector<Vector> &iPVInfo,reco::VertexCollection &iVertices);
        reco::PFMET getMvaMet(const reco::Candidate *cand1,const reco::Candidate *cand2,reco::Vertex *iPV,reco::PFMETCollection thePfMet);

};


#endif
