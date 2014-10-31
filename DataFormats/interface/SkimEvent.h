#ifndef AnalysisDataFormats_SkimEvent_h
#define AnalysisDataFormats_SkimEvent_h

#include "FWCore/ParameterSet/interface/FileInPath.h"

#include "DataFormats/Common/interface/RefToBase.h"
#include "DataFormats/Common/interface/Ref.h"
#include "DataFormats/Common/interface/Ptr.h"
#include "DataFormats/PatCandidates/interface/Jet.h"
#include "DataFormats/PatCandidates/interface/Muon.h"
#include "DataFormats/PatCandidates/interface/Electron.h"
#include "DataFormats/PatCandidates/interface/MET.h"
#include "DataFormats/Math/interface/Point3D.h"
#include "DataFormats/METReco/interface/MET.h"
#include "DataFormats/METReco/interface/METFwd.h"
#include "DataFormats/METReco/interface/PFMET.h"
#include "DataFormats/METReco/interface/PFMETFwd.h"
#include "DataFormats/METReco/interface/GenMET.h"
#include "DataFormats/METReco/interface/GenMETFwd.h"
#include "DataFormats/JetReco/interface/PFJet.h"
#include "DataFormats/JetReco/interface/PFJetCollection.h"
#include "DataFormats/JetReco/interface/GenJet.h"
#include "DataFormats/JetReco/interface/GenJetCollection.h"
#include "DataFormats/VertexReco/interface/Vertex.h"
#include "DataFormats/VertexReco/interface/VertexFwd.h"
#include "DataFormats/Common/interface/ValueMap.h"
#include "DataFormats/HepMCCandidate/interface/GenParticle.h"
#include "DataFormats/HepMCCandidate/interface/GenParticleFwd.h"

#include "CondFormats/JetMETObjects/interface/JetCorrectorParameters.h"
#include "CondFormats/JetMETObjects/interface/FactorizedJetCorrector.h"

#include "SimDataFormats/GeneratorProducts/interface/GenFilterInfo.h"
#include "SimDataFormats/GeneratorProducts/interface/GenEventInfoProduct.h"
#include "SimDataFormats/GeneratorProducts/interface/LHEEventProduct.h"

#include "DataFormats/JetReco/interface/PileupJetIdentifier.h"

#include <vector>
#include <utility>

using std::size_t;

const double Mw = 80.450;
const double Mz = 91.188;

namespace reco {

    struct mwlSortByPtClass { // sorting struct
      bool operator() ( edm::RefToBase<reco::Candidate> a, edm::RefToBase<reco::Candidate> b) { return a->pt() > b->pt(); }
    };

    class SkimEvent : public LeafCandidate {

       public:
            
        typedef edm::Ptr<reco::RecoCandidate> refToCand; // pointer to a reco candidate
        enum primaryDatasetType { MC = 0, SingleMuon = 1, SingleElectron = 2, DoubleMuon = 3, MuEG = 4, DoubleElectron = 5, primaryDatasetTypeSize = 6, AllEmbed = 7}; // dataset
        enum metType { TCMET = 0, PFMET = 1, CHMET = 2, PUPPIMET = 3}; // met type

        SkimEvent(); // constructor

        // return user float
        float userFloat( const std::string & key ) const;
        float userFloat( const char* key ) const { return userFloat(std::string(key)); }
        // add User float
        void addUserFloat( const std::string & label, float data );
        // return list 
        const std::vector<std::string> & userFloatNames() const { return userFloatLabels_; }
        // Fin user float
        bool hasUserFloat( const std::string & key ) const {
                return std::find(userFloatLabels_.begin(), userFloatLabels_.end(), key) != userFloatLabels_.end();
        }
        bool hasUserFloat( const char* key ) const {return hasUserFloat( std::string(key) );}

        // same structure for user int
        int32_t userInt( const std::string & key ) const;
        void addUserInt( const std::string & label, int32_t data );
        const std::vector<std::string> & userIntNames() const { return userIntLabels_; }
        bool hasUserInt( const std::string & key ) const {
                return std::find(userIntLabels_.begin(), userIntLabels_.end(), key) != userIntLabels_.end();
        }

        // Set methods
        void setLepton       (const edm::Handle<edm::View<reco::RecoCandidate> > &h, size_t i);
        void setSoftMuon     (const edm::Handle<edm::View<reco::RecoCandidate> > &h, size_t i);
        void setExtraLepton  (const edm::Handle<edm::View<reco::RecoCandidate> > &h, size_t i);
        void setJetRhoIso(const edm::Handle<double> & h);
        void setJets(const edm::Handle<pat::JetCollection> &);
        void setPuppiJets(const edm::Handle<pat::JetCollection> &);
        void setFatJets(const edm::Handle<pat::JetCollection> &);
        void setFatPuppiJets(const edm::Handle<pat::JetCollection> &);
        void setTagJets(const edm::Handle<pat::JetCollection> &);
        void setPuppiTagJets(const edm::Handle<pat::JetCollection> &);

        void setTCMet(const edm::Handle<reco::METCollection> &);
        void setPFMet(const edm::Handle< std::vector<pat::MET> > &);
        void setPuppiPFMet(const edm::Handle< std::vector<pat::MET> > &);
        void setMvaMet(const reco::PFMET &met) {mvaMet_ = met;}
        void setChargedMet(const reco::PFMET &);
        void setChargedMetSmurf(const reco::MET& met) {chargedMetSmurf_ = met;}
        
        void setVertex(const edm::Handle<reco::VertexCollection> &);
        void setVtxSumPts(const edm::Handle<edm::ValueMap<float> > &s);
        void setVtxSumPt2s(const edm::Handle<edm::ValueMap<float> > &s);
        void setGenParticles(const edm::Handle<reco::GenParticleCollection> &h);
        void setLHEinfo(const edm::Handle<LHEEventProduct> & h);
        void setGenWeight(const edm::Handle<GenFilterInfo> &s);
        void setGenInfo(const edm::Handle<GenEventInfoProduct> &s);
        void setGenMet(const edm::Handle<reco::GenMETCollection> &);
        void setGenJets(const edm::Handle<reco::GenJetCollection> &h );

        // star with get methods
        const unsigned int run()  const { return run_; }
        const unsigned int lumi() const { return lumi_; }
        const unsigned int evt()  const { return evt_; } 
        // generator weight 
        const float mcGenWeight () const { return mcGenWeight_.filterEfficiency(); } // Monte carlo weight

        // lepton sector
        const bool isElectron(size_t a) const; // if is an electron
        const bool isMuon(size_t a) const; // if is a muon
        const bool isElectron(const refToCand &) const;
        const bool isMuon(const refToCand &) const;

        const pat::Electron * getElectron(size_t a) const;
        const pat::Muon *     getMuon(size_t a) const;
        const pat::Electron * getElectron(const refToCand&) const;
        const pat::Muon *     getMuon(const refToCand&) const;

        // lepton get and is methods
        const int   getChannel() const ;
        const int   getNLep(float a = -1) const;
        const int   getNExtraLep(float a = -1) const;
        const int   getNSoftMu (float a = -1, float vetoJets=-1, float dRCut = 0.3) const;
        const int   getPdgId(size_t a = 0) const;
        const float getPt(size_t a = 0) const;
        const int   getPassCustom(size_t a = 0, const std::string & muStr = "1", const std::string & elStr = "1" ) const;
        const float getLeptId(size_t i, std::string idele = "", std::string idmu = "") const;             
        const float getLeptBdt(size_t a = 0) const;
        const float getPtMax() const { return getPtByPt(0); }
        const float getPtMin() const { return getPtByPt(1); }
        const float getEta(size_t a = 0) const;
        const float getNBrem(size_t a = 0) const;
        const float getEtaSC(size_t a = 0) const; //returns isMuon ? eta : ele.sc.eta
        const float getPhi(size_t a = 0) const;
        const int   getQ(size_t a = 0) const;
        const bool  peaking() const;

        const reco::GenParticle *genParticle(size_t i) const;
        const reco::GenParticleRef getMotherID(size_t a=0) const;
	
        //Jet variables
        const int nJets(float pt , int applyCorrection , int applyJetID, int applyPileUpJetID) const;
        const int nCentralJets(float pt , float eta, int applyCorrection = true, int applyJetID = 0, int applyPileUpJetID = 0) const;

        const int nPuppiJets(float pt , int applyCorrection , int applyJetID, int applyPileUpJetID) const;
        const int nCentralPuppiJets(float pt , float eta, int applyCorrection = true, int applyJetID = 0, int applyPileUpJetID = 0) const;

        const math::XYZTLorentzVector jet(size_t index, float minPt,float eta,int applyCorrection,int applyJetID, int applyPileUpJetID) const;
        const math::XYZTLorentzVector puppiJet(size_t index, float minPt,float eta,int applyCorrection,int applyJetID, int applyPileUpJetID) const;
        const size_t indexJetByPt(size_t i, float minPt, float eta, int applyCorrection, int applyJetID, int applyPileUpJetID) const;
        const size_t indexPuppiJetByPt(size_t i, float minPt, float eta, int applyCorrection, int applyJetID, int applyPileUpJetID) const;
        const pat::Jet* leadingJet  (size_t a, float pt ,float eta,int applyCorrection, int applyJetID, int applyPileUpJetID) const;
        const pat::Jet* leadingPuppiJet  (size_t a, float pt ,float eta,int applyCorrection, int applyJetID, int applyPileUpJetID) const;

        const float leadingVBFJetPt (size_t a, float pt ,float eta,int applyCorrection, int applyJetID, int applyPileUpID) const;
        const float leadingVBFJetEta(size_t a, float pt ,float eta,int applyCorrection, int applyJetID, int applyPileUpJetID) const;
        const float leadingVBFJetPhi(size_t a, float pt ,float eta,int applyCorrection, int applyJetID, int applyPileUpJetID) const;
        const int   leadingVBFJetId (size_t a, float pt ,float eta,int applyCorrection, int applyJetID, int applyPileUpJetID) const;
        const float leadingVBFPileUpJetIdValue (size_t a, float pt ,float eta,int applyCorrection, int applyJetID, int applyPileUpJetID) const;
        const int   leadingVBFPileUpJetIdFlag (size_t a, float pt ,float eta,int applyCorrection, int applyJetID, int applyPileUpJetID) const;
        const float leadingVBFJetMva(size_t a, float pt ,float eta,int applyCorrection, int applyJetID, int applyPileUpJetID) const;
        const float leadingJetPt    (size_t a, float pt ,float eta,int applyCorrection, int applyJetID, int applyPileUpJetID) const;
        const float leadingJetEta   (size_t a, float pt ,float eta,int applyCorrection, int applyJetID, int applyPileUpJetID) const;
        const float leadingJetMass  (size_t a, float pt ,float eta,int applyCorrection, int applyJetID, int applyPileUpJetID) const;
        const float leadingJetPhi   (size_t a, float pt ,float eta,int applyCorrection, int applyJetID, int applyPileUpJetID) const;

        const float leadingVBFPuppiJetPt (size_t a, float pt ,float eta,int applyCorrection, int applyJetID, int applyPileUpID) const;
        const float leadingVBFPuppiJetEta(size_t a, float pt ,float eta,int applyCorrection, int applyJetID, int applyPileUpJetID) const;
        const float leadingVBFPuppiJetPhi(size_t a, float pt ,float eta,int applyCorrection, int applyJetID, int applyPileUpJetID) const;
        const int   leadingVBFPuppiJetId (size_t a, float pt ,float eta,int applyCorrection, int applyJetID, int applyPileUpJetID) const;
        const float leadingVBFPuppiPileUpJetIdValue (size_t a, float pt ,float eta,int applyCorrection, int applyJetID, int applyPileUpJetID) const;
        const int   leadingVBFPuppiPileUpJetIdFlag (size_t a, float pt ,float eta,int applyCorrection, int applyJetID, int applyPileUpJetID) const;
        const float leadingVBFPuppiJetMva(size_t a, float pt ,float eta,int applyCorrection, int applyJetID, int applyPileUpJetID) const;
        const float leadingPuppiJetPt    (size_t a, float pt ,float eta,int applyCorrection, int applyJetID, int applyPileUpJetID) const;
        const float leadingPuppiJetEta   (size_t a, float pt ,float eta,int applyCorrection, int applyJetID, int applyPileUpJetID) const;
        const float leadingPuppiJetMass  (size_t a, float pt ,float eta,int applyCorrection, int applyJetID, int applyPileUpJetID) const;
        const float leadingPuppiJetPhi   (size_t a, float pt ,float eta,int applyCorrection, int applyJetID, int applyPileUpJetID) const;

        const float leadingFatJetPt  (size_t a, float pt ,float eta,int applyCorrection, int applyJetID, int applyPileUpJetID) const;
        const float leadingFatJetEta (size_t a, float pt ,float eta,int applyCorrection, int applyJetID, int applyPileUpJetID) const;
        const float leadingFatJetPhi (size_t a, float pt ,float eta,int applyCorrection, int applyJetID, int applyPileUpJetID) const;
        const float leadingFatJetTrimmedMass  (size_t a, float pt ,float eta,int applyCorrection, int applyJetID, int applyPileUpJetID) const;
        const float leadingFatJetFilteredMass (size_t a, float pt ,float eta,int applyCorrection, int applyJetID, int applyPileUpJetID) const;
        const float leadingFatJetPrunedMass   (size_t a, float pt ,float eta,int applyCorrection, int applyJetID, int applyPileUpJetID) const;
        const float leadingFatJetMassDrop (size_t a, float pt ,float eta,int applyCorrection, int applyJetID, int applyPileUpJetID) const;
        const float leadingFatJetPrunedTau2Tau1 (size_t a, float pt ,float eta,int applyCorrection, int applyJetID, int applyPileUpJetID) const;
        const float leadingFatJetPrunedTau1 (size_t a, float pt ,float eta,int applyCorrection, int applyJetID, int applyPileUpJetID) const;
        const float leadingFatJetPrunedTau2 (size_t a, float pt ,float eta,int applyCorrection, int applyJetID, int applyPileUpJetID) const;
        const float leadingFatJetPrunedTau3 (size_t a, float pt ,float eta,int applyCorrection, int applyJetID, int applyPileUpJetID) const;
        const float leadingFatJetPrunedTau4 (size_t a, float pt ,float eta,int applyCorrection, int applyJetID, int applyPileUpJetID) const;

        const float leadingFatPuppiJetPt  (size_t a, float pt ,float eta,int applyCorrection, int applyJetID, int applyPileUpJetID) const;
        const float leadingFatPuppiJetEta (size_t a, float pt ,float eta,int applyCorrection, int applyJetID, int applyPileUpJetID) const;
        const float leadingFatPuppiJetPhi (size_t a, float pt ,float eta,int applyCorrection, int applyJetID, int applyPileUpJetID) const;
        const float leadingFatPuppiJetTrimmedMass  (size_t a, float pt ,float eta,int applyCorrection, int applyJetID, int applyPileUpJetID) const;
        const float leadingFatPuppiJetFilteredMass (size_t a, float pt ,float eta,int applyCorrection, int applyJetID, int applyPileUpJetID) const;
        const float leadingFatPuppiJetPrunedMass   (size_t a, float pt ,float eta,int applyCorrection, int applyJetID, int applyPileUpJetID) const;
        const float leadingFatPuppiJetMassDrop (size_t a, float pt ,float eta,int applyCorrection, int applyJetID, int applyPileUpJetID) const;
        const float leadingFatPuppiJetPrunedTau2Tau1 (size_t a, float pt ,float eta,int applyCorrection, int applyJetID, int applyPileUpJetID) const;
        const float leadingFatPuppiJetPrunedTau1 (size_t a, float pt ,float eta,int applyCorrection, int applyJetID, int applyPileUpJetID) const;
        const float leadingFatPuppiJetPrunedTau2 (size_t a, float pt ,float eta,int applyCorrection, int applyJetID, int applyPileUpJetID) const;
        const float leadingFatPuppiJetPrunedTau3 (size_t a, float pt ,float eta,int applyCorrection, int applyJetID, int applyPileUpJetID) const;
        const float leadingFatPuppiJetPrunedTau4 (size_t a, float pt ,float eta,int applyCorrection, int applyJetID, int applyPileUpJetID) const;

        const float leadingJetPtd(size_t index, float minPt,float eta,int applyCorrection,int applyJetID, int applyPileUpJetID) const ;
        const float leadingJetPtD(size_t index, float minPt,float eta,int applyCorrection,int applyJetID, int applyPileUpJetID, int QualityCut) const ;
        const float leadingJetQGaxis1(size_t index, float minPt,float eta,int applyCorrection,int applyJetID, int applyPileUpJetID, int QualityCut) const ;
        const float leadingJetQGaxis2(size_t index, float minPt,float eta,int applyCorrection,int applyJetID, int applyPileUpJetID, int QualityCut) const ;
        const float leadingJetQGRMScand(size_t index, float minPt,float eta,int applyCorrection,int applyJetID, int applyPileUpJetID, int QualityCut) const ;
        const float leadingJetQGRmax(size_t index, float minPt,float eta,int applyCorrection,int applyJetID, int applyPileUpJetID, int QualityCut) const ;
        const float leadingJetNChgQC(size_t index, float minPt,float eta,int applyCorrection,int applyJetID, int applyPileUpJetID) const ;
        const float leadingJetNChgptCut(size_t index, float minPt,float eta,int applyCorrection,int applyJetID, int applyPileUpJetID) const ;
        const float leadingJetNNeutralptCut(size_t index, float minPt,float eta,int applyCorrection,int applyJetID, int applyPileUpJetID) const ;
        const float leadingJetChargedHadronMultiplicity(size_t index, float minPt,float eta,int applyCorrection,int applyJetID, int applyPileUpJetID) const ;
        const float leadingJetNeutralHadronMultiplicity(size_t index, float minPt,float eta,int applyCorrection,int applyJetID, int applyPileUpJetID) const ;
        const float leadingJetPhotonMultiplicity(size_t index, float minPt,float eta,int applyCorrection,int applyJetID, int applyPileUpJetID) const ;
        const float getJetRhoIso() const ;
        const int   leadingJetId(size_t a, float pt ,float eta,int applyCorrection, int applyJetID, int applyPileUpJetID) const;
        const float leadingPileUpJetIdValue (size_t a, float pt ,float eta,int applyCorrection, int applyJetID, int applyPileUpJetID) const;
        const int   leadingPileUpJetIdFlag (size_t a, float pt ,float eta,int applyCorrection, int applyJetID, int applyPileUpJetID) const;
        const float leadingJetMva(size_t a, float pt ,float eta,int applyCorrection, int applyJetID, int applyPileUpJetID) const;
        const bool  passesDPhillJet(float pt,float eta,int applyCorrection, int applyJetID, int applyPileUpJetID) const;


        const float leadingPuppiJetPtd(size_t index, float minPt,float eta,int applyCorrection,int applyJetID, int applyPileUpJetID) const ;
        const float leadingPuppiJetPtD(size_t index, float minPt,float eta,int applyCorrection,int applyJetID, int applyPileUpJetID, int QualityCut) const ;
        const float leadingPuppiJetQGaxis1(size_t index, float minPt,float eta,int applyCorrection,int applyJetID, int applyPileUpJetID, int QualityCut) const ;
        const float leadingPuppiJetQGaxis2(size_t index, float minPt,float eta,int applyCorrection,int applyJetID, int applyPileUpJetID, int QualityCut) const ;
        const float leadingPuppiJetQGRMScand(size_t index, float minPt,float eta,int applyCorrection,int applyJetID, int applyPileUpJetID, int QualityCut) const ;
        const float leadingPuppiJetQGRmax(size_t index, float minPt,float eta,int applyCorrection,int applyJetID, int applyPileUpJetID, int QualityCut) const ;
        const float leadingPuppiJetNChgQC(size_t index, float minPt,float eta,int applyCorrection,int applyJetID, int applyPileUpJetID) const ;
        const float leadingPuppiJetNChgptCut(size_t index, float minPt,float eta,int applyCorrection,int applyJetID, int applyPileUpJetID) const ;
        const float leadingPuppiJetNNeutralptCut(size_t index, float minPt,float eta,int applyCorrection,int applyJetID, int applyPileUpJetID) const ;
        const float leadingPuppiJetChargedHadronMultiplicity(size_t index, float minPt,float eta,int applyCorrection,int applyJetID, int applyPileUpJetID) const ;
        const float leadingPuppiJetNeutralHadronMultiplicity(size_t index, float minPt,float eta,int applyCorrection,int applyJetID, int applyPileUpJetID) const ;
        const float leadingPuppiJetPhotonMultiplicity(size_t index, float minPt,float eta,int applyCorrection,int applyJetID, int applyPileUpJetID) const ;
        const int   leadingPuppiJetId(size_t a, float pt ,float eta,int applyCorrection, int applyJetID, int applyPileUpJetID) const;
        const float leadingPuppiPileUpJetIdValue (size_t a, float pt ,float eta,int applyCorrection, int applyJetID, int applyPileUpJetID) const;
        const int   leadingPuppiPileUpJetIdFlag (size_t a, float pt ,float eta,int applyCorrection, int applyJetID, int applyPileUpJetID) const;
        const float leadingPuppiJetMva(size_t a, float pt ,float eta,int applyCorrection, int applyJetID, int applyPileUpJetID) const;
        const bool  passesDPhillPuppiJet(float pt,float eta,int applyCorrection, int applyJetID, int applyPileUpJetID) const;

        const float jetPt   (size_t a = 0,int = 0) const;
        const float fatJetPt(size_t a = 0,int = 0) const;
        const float tagJetPt(size_t a = 0,int = 0) const;
        const float jetPt   (const pat::Jet *j,int = 0) const;

        const float puppiJetPt   (size_t a = 0,int = 0) const;
        const float fatPuppiJetPt(size_t a = 0,int = 0) const;
        const float tagPuppiJetPt(size_t a = 0,int = 0) const;

        static void setupJEC(const std::string&, const std::string&, const std::string&);

        const pat::JetRef matchedJet(size_t alepton, float minDr = 0.4) const;
        const float nearestJet(int i = 0, float minPt = 25, float eta = 5.0,bool applyCorrection = 1, int applyJetID = 0, int applyPileUpJetID = 0) const;
        const float matchedJetPt(size_t alepton, float minDr = 0.4, bool applyCorrection = 1) const;
        const bool  isThisJetALepton(pat::JetRef jet, float drCut = 0.3) const ;

        const pat::JetRef matchedPuppiJet(size_t alepton, float minDr = 0.4) const;
        const float nearestPuppiJet(int i = 0, float minPt = 25, float eta = 5.0,bool applyCorrection = 1, int applyJetID = 0, int applyPileUpJetID = 0) const;
        const float matchedPuppiJetPt(size_t alepton, float minDr = 0.4, bool applyCorrection = 1) const;

        const bool passJetID       (pat::JetRef jet,int) const ;
        const bool passPileUpJetID (pat::JetRef jet,int) const ;
        const bool passFatJetID    (pat::JetRef jet,int) const ;

        const int bTaggedJetsBetween(const float & minPt,const float & maxPt, const float& discrCut, std::string discriminator = "trackCountingHighEffBJetTags", 
                                     int applyJetID = 0, int applyPileUpJetID =0, float dzCut=9999.) const;
        const int bTaggedJetsUnder(const float& maxPt, const float& discrCut, std::string discriminator="trackCountingHighEffBJetTags", int applyJetID = 0, 
                                   int applyPileUpJetID = 0, float dzCut=9999.) const;
        const int bTaggedJetsOver(const float& maxPt,  const float& discrCut, std::string discriminator="trackCountingHighEffBJetTags", int applyJetID =0, 
                                  int applyPileUpJetID=0, float dzCut=9999.) const;
        const float leadingJetBtag(size_t a, std::string discriminator="trackCountingHighEffBJetTags",float pt=30.0 ,float eta=5.0,int applyCorrection=true, int applyJetID = 0, 
                                   int applyPileUpJetID=0, float dzCut=9999.) const;
        const float highestBDiscRange(const float& minPt, const float& maxPt, std::string discriminator="trackCountingHighEffBJetTags", int applyJetID = 0, int applyPileUpJetID=0, 
                                      float dzCut=9999., int minPtApplyCorrection =1) const;
        const float highestHardBDisc(const float& maxPt, std::string discriminator="trackCountingHighEffBJetTags", int applyJetID = 0, int applyPileUpJetID=0, float dzCut=9999.) const;
        const float highestSoftBDisc(const float& maxPt, std::string discriminator="trackCountingHighEffBJetTags", int applyJetID = 0, int applyPileUpJetID=0, float dzCut=9999.) const;

        const int bTaggedPuppiJetsBetween(const float & minPt,const float & maxPt, const float& discrCut, std::string discriminator = "trackCountingHighEffBJetTags", 
                                     int applyJetID = 0, int applyPileUpJetID =0, float dzCut=9999.) const;
        const int bTaggedPuppiJetsUnder(const float& maxPt, const float& discrCut, std::string discriminator="trackCountingHighEffBJetTags", int applyJetID = 0, 
                                   int applyPileUpJetID = 0, float dzCut=9999.) const;
        const int bTaggedPuppiJetsOver(const float& maxPt,  const float& discrCut, std::string discriminator="trackCountingHighEffBJetTags", int applyJetID =0, 
                                  int applyPileUpJetID=0, float dzCut=9999.) const;
        const float leadingPuppiJetBtag(size_t a, std::string discriminator="trackCountingHighEffBJetTags",float pt=30.0 ,float eta=5.0,int applyCorrection=true, int applyJetID = 0, 
                                   int applyPileUpJetID=0, float dzCut=9999.) const;
        const float highestBPuppiDiscRange(const float& minPt, const float& maxPt, std::string discriminator="trackCountingHighEffBJetTags", int applyJetID = 0, int applyPileUpJetID=0, 
                                      float dzCut=9999., int minPtApplyCorrection =0) const;
        const float highestHardBPuppiDisc(const float& maxPt, std::string discriminator="trackCountingHighEffBJetTags", int applyJetID = 0, int applyPileUpJetID=0, float dzCut=9999.) const;
        const float highestSoftBPuppiDisc(const float& maxPt, std::string discriminator="trackCountingHighEffBJetTags", int applyJetID = 0, int applyPileUpJetID=0, float dzCut=9999.) const;

        const float dPhillLeadingJet(float eta,int applyCorrection, int applyJetID, int applyPileUpJetID) const;
        const float dPhiJetllInDegrees(size_t a,float pt ,float eta,int applyCorrection, int applyJetID, int applyPileUpJetID) const;
        const float dPhiJetll(size_t a,float pt ,float eta,int applyCorrection, int applyJetID, int applyPileUpJetID) const;
        const float dPhilljetjet(float eta,int applyCorrection,int applyJetID, int applyPileUpJetID) const;

        const float dPhillLeadingPuppiJet(float eta,int applyCorrection, int applyJetID, int applyPileUpJetID) const;
        const float dPhiPuppiJetllInDegrees(size_t a,float pt ,float eta,int applyCorrection, int applyJetID, int applyPileUpJetID) const;
        const float dPhiPuppiJetll(size_t a,float pt ,float eta,int applyCorrection, int applyJetID, int applyPileUpJetID) const;
        const float dPhillPuppijetjet(float eta,int applyCorrection,int applyJetID, int applyPileUpJetID) const;

        // MET info
        const float met(metType metToUse = TCMET) const;
        const float pfSumEt() const;
        const float pfMet() const;
        const float pfMetPhi() const;
        const float pfPuppiSumEt() const;
        const float pfPuppiMet() const;
        const float pfPuppiMetPhi() const;
        const float mvaMet() const{return mvaMet_.pt();}
        const float mvaMetPhi() const{return mvaMet_.phi();}
        const float tcSumEt() const;
        const float tcMet() const;
        const float tcMetPhi() const;
        
        const float chargedSumEt() const;
        const float chargedMet() const;
        const float chargedMetSmurfSumEt() const;
        const float chargedMetSmurf() const{return chargedMetSmurf_.pt();}
        const float chargedMetSmurfPhi() const{return chargedMetSmurf_.phi();}
        
        const float pfMetSignificance() const;
        const float pfMetMEtSig() const;

        const float mll() const;
        const float pTll() const;
        const float dPhill() const;
        const float dPhillInDegrees() const {return dPhill() / M_PI * 180.;}
        const float dRll() const;
        const float dEtall() const;
        const float etall() const;
        const float yll() const;
        const float dPhillMet(metType metToUse=TCMET) const;
        const float dPhillPfMet() const;
        const float dPhillPuppiMet() const;
        const float dPhillTcMet() const;
        const float dPhillChargedMet() const;
        
        const float mT(size_t a = 0, metType metToUse=TCMET) const;
        const float dPhilPfMet(size_t a) const;
        const float dPhilPuppiPfMet(size_t a) const;
        const float dPhilMvaMet(size_t a) const;
        const float dPhilTcMet(size_t a) const;
        const float dPhilChargedMet(size_t a) const;
        const float dPhilChargedMetSmurf(size_t a) const;

        const float dPhilMet(metType metToUse=TCMET) const;
        const float dPhilMet(size_t a, metType metToUse=TCMET) const;
        const float dPhilPfMet() const;
        const float dPhilPuppiPfMet() const;
        const float dPhilMvaMet() const;
        const float dPhilTcMet() const;
        const float dPhilChargedMet() const;
        const float dPhilChargedMetSmurf() const;
        
        const float projMet(metType metToUse=TCMET) const;
        const float projPfMet() const;
        const float projPuppiPfMet() const;
        const float projMvaMet() const;
        const float projTcMet() const;
        const float projChargedMet() const;
        const float projChargedMetSmurf() const;
            
        const float nTracks() const;
        const float cosThetaStar(size_t a = 0) const;
        const float afb(size_t a = 0) const;
        const float mRStar() const;
        const float gamma() const;
        const float gammaMRStar() const;
        
        //Event variables
        const float mTHiggs(metType metToUse = TCMET) const;
        const float tcMetX() const;
        const float tcMetY() const;
        const float pYll() const;
        const float pXll() const;
        const float pZll() const;
        const float mTll() const;
        const float getFinalStateMC() const;
        const float getWWdecayMC() const;
        const float getMCHiggsProd() const;
        const float getHEPMCweight() const ;
        const float getHEPMCweightScale(size_t i) const ;
        const float getHEPMCweightRen(size_t i) const ;
        const float getHEPMCweightFac(size_t i) const ;
        void FindDaughterParticles(const reco::Candidate** pCurrent, std::vector<const reco::Candidate*>* pFinal = 0) const;

        const float getHiggsMass() const;
        const float getHiggsPt() const;
        const float getSusyStopMass() const;
        const float getSusyLSPMass() const;

        const float getPDFscalePDF() const;
        const float getPDFx1() const;
        const float getPDFx2() const;
        const float getPDFid1() const;
        const float getPDFid2() const;
        const float getPDFx1PDF() const;
        const float getPDFx2PDF() const;

        // Trigger bits
        void setTriggerBits(const std::vector<bool> &);
        const bool triggerBitsCut(SkimEvent::primaryDatasetType pdType) const;
        const bool guillelmoTrigger(SkimEvent::primaryDatasetType pdType) const;
        const bool triggerMatchingCut(SkimEvent::primaryDatasetType pdType) const;
        bool passTriggerSingleMu(size_t i, bool isData = true) const;
        bool passTriggerDoubleMu(size_t i, bool isData = true) const;
        bool passTriggerElMu(size_t i,     bool isData = true) const;
        bool passTriggerDoubleEl(size_t i, bool isData = true) const;

        //Selection Functions
        const bool leptEtaCut(float maxAbsEtaMu = 2.4, float maxAbsEtaEl = 2.5) const;
        const bool passesIP() const;
        const bool passesIP(const refToCand &c) const;
        const bool hasGoodVertex() const;
        const double d0Reco(size_t a = 0) const;
        const double dZReco(size_t a = 0) const;
        const bool passesConversion(size_t a=0) const;
        const bool isMuTriggered(size_t a = 0) const;

        const int numberOfbQuarks() const;
        const int numberOftQuarks() const;

        const math::XYZTLorentzVector lepton(size_t a) const;
        const bool passesVtxSel(size_t a = 0) const;
        const reco::Vertex* highestPtVtx() const;

        // muon ID
        const bool isSTA(size_t i = 0) const;
        const bool isSTA(const refToCand &c) const;
        const bool isTightMuon(size_t i = 0) const;
        const bool isTightMuon(const refToCand &c) const;
        const bool isLooseMuon(size_t i = 0) const;
        const bool isLooseMuon(const refToCand &c) const;
        const bool isSoftMuon(size_t i = 0) const;
        const bool isSoftMuon(const refToCand &c) const;
        const bool isHighPtMuon(size_t i = 0) const;
        const bool isHighPtMuon(const refToCand &c) const;

        // electron ID
        const bool isTightElectron(size_t i = 0) const;
        const bool isTightElectron(const refToCand &c) const;
        const bool isLooseElectron(size_t i = 0) const;
        const bool isLooseElectron(const refToCand &c) const;
        const bool isLooseRobustElectron(size_t i = 0) const;
        const bool isLooseRobustElectron(const refToCand &c) const;
        const bool isTightRobustElectron(size_t i = 0) const;
        const bool isTightRobustElectron(const refToCand &c) const;
        const bool isRobustHighEnergyElectron(size_t i = 0) const;
        const bool isRobustHighEnergyElectron(const refToCand &c) const;

	// muons and electron isolation
        const float pfNeutralHadronsIso  (size_t i = 0) const;
        const float pfParticleAllIso      (size_t i = 0) const;
        const float pfPUChargedHadronIso (size_t i = 0) const;
        const float pfChargedHadronsIso  (size_t i = 0) const;
        const float pfPhotonsIso         (size_t i = 0) const;

        const float tkIso(size_t a = 0) const;
        const float ecalIso(size_t a = 0) const;
        const float hcalIso(size_t a = 0) const;
        const float allLeptonIso(size_t a = 0) const;
        const float mvaIso(size_t a = 0) const;
        const float tkVeto(size_t a = 0) const;
        const float ecalVeto(size_t a = 0) const;
        const float hcalVeto(size_t a = 0) const;
        const float allVeto(size_t a = 0) const;
        const float isSTAByPt (size_t i = 0)   const { return isSTA (indexLeptonByPt (i)); }
        const float tkIsoByPt (size_t i = 0)   const { return tkIso (indexLeptonByPt (i)); }
        const float tkIsoByIso (size_t i = 0)  const { return tkIso (indexLeptonByIso(i)); }
        const float ecalIsoByPt (size_t i = 0) const { return ecalIso(indexLeptonByPt (i)); }
        const float ecalIsoByIso(size_t i = 0) const { return ecalIso(indexLeptonByIso(i)); }
        const float hcalIsoByPt (size_t i = 0) const { return hcalIso(indexLeptonByPt (i)); }
        const float hcalIsoByIso(size_t i = 0) const { return hcalIso(indexLeptonByIso(i)); }
        const float allLeptonIsoByPt (size_t i = 0)  const { return allLeptonIso (indexLeptonByPt (i)); }
        const float allLeptonIsoByIso (size_t i = 0) const { return allLeptonIso (indexLeptonByIso(i)); }
        const float mvaIsoByPt (size_t i = 0)  const { return mvaIso (indexLeptonByPt (i)); }
        const float mvaIsoByIso (size_t i = 0) const { return mvaIso (indexLeptonByIso(i)); }
        const float tkVetoByPt (size_t i = 0)  const { return tkVeto (indexLeptonByPt (i)); }
        const float tkVetoByIso (size_t i = 0) const { return tkVeto (indexLeptonByIso(i)); }
        const float ecalVetoByPt (size_t i = 0) const { return ecalVeto(indexLeptonByPt (i)); }
        const float ecalVetoByIso(size_t i = 0) const { return ecalVeto(indexLeptonByIso(i)); }
        const float hcalVetoByPt (size_t i = 0) const { return hcalVeto(indexLeptonByPt (i)); }
        const float hcalVetoByIso(size_t i = 0) const { return hcalVeto(indexLeptonByIso(i)); }
        const float allVetoByPt (size_t i = 0)  const { return allVeto (indexLeptonByPt (i)); }
        const float allVetoByIso (size_t i = 0) const { return allVeto (indexLeptonByIso(i)); }


        // LHE Info
        const float leadingLHEJetPt(size_t a) const;
        const float leadingLHEJetPID(size_t a) const;
        const float leadingLHEJetPhi(size_t a) const;
        const float leadingLHEJetEta(size_t a) const;
        const float leadingLHELeptonPt(size_t a) const;
        const float leadingLHELeptonPID(size_t a) const;
        const float leadingLHELeptonPhi(size_t a) const;
        const float leadingLHELeptonEta(size_t a) const;
        const float leadingLHENeutrinoPt(size_t a) const;
        const float leadingLHENeutrinoPID(size_t a) const;
        const float leadingLHENeutrinoPhi(size_t a) const;
        const float leadingLHENeutrinoEta(size_t a) const;
           
        const float LHEMetPt() const;
        const float LHEMetPhi() const;
        const float LHEMetEta() const;
        const float higgsLHEPt() const;
            
        // Gen particle Info
        const float leadingGenJetPartonPt(size_t a) const;
        const float leadingGenJetPartonPID(size_t a) const;
        const float leadingGenJetPartonEta(size_t a) const;
        const float leadingGenJetPartonPhi(size_t a) const;
        const float leadingGenLeptonPt(size_t a) const;
        const float leadingGenLeptonPID(size_t a) const;
        const float leadingGenLeptonEta(size_t a) const;
        const float leadingGenLeptonPhi(size_t a) const;
        const float leadingGenNeutrinoPt(size_t a) const;
        const float leadingGenNeutrinoPID(size_t a) const;
        const float leadingGenNeutrinoEta(size_t a) const;
        const float leadingGenNeutrinoPhi(size_t a) const;
        const float leadingGenJetPt(size_t a) const;
        const float leadingGenJetEta(size_t a) const;
        const float leadingGenJetPhi(size_t a) const;

        const float genMetPt() const;
        const float genMetPhi() const;
        const float genMetEta() const;
            
        //Iso Functions
        const bool   isEB(size_t a = 0) const;
        const bool   isEE(size_t a = 0) const;
        const float  tkPt(size_t a = 0) const;
        const size_t indexLeptonByPt(size_t a = 0) const;
        const size_t indexLeptonByIso(size_t a = 0) const;
        const float getRho(size_t a = 0) const;
        const reco::Candidate & candByPt(size_t i) const { return *leps_[indexLeptonByPt(i)]; }
        const int pdgIdByPt (size_t i = 0)  const { return getPdgId (indexLeptonByPt (i)); }
        const int pdgIdByIso (size_t i = 0) const { return getPdgId (indexLeptonByIso(i)); }
        const float getPtByPt (size_t i = 0) const { return getPt (indexLeptonByPt (i)); }
        const float leptIdByPt (size_t i = 0, std::string idele = "", std::string idmu = "") const { return getLeptId (indexLeptonByPt (i), idele, idmu); }
        const float leptBdtByPt (size_t i = 0) const { return getLeptBdt (indexLeptonByPt (i)); }
        const float nBremByPt (size_t i = 0)   const { return getNBrem (indexLeptonByPt (i)); }
        const float ptByIso (size_t i = 0)     const { return getPt (indexLeptonByIso(i)); }
        const float etaByPt (size_t i = 0)     const { return getEta (indexLeptonByPt (i)); }
        const float etaByIso (size_t i = 0)    const { return getEta (indexLeptonByIso(i)); }
        const float etaSCByPt (size_t i = 0)   const { return getEtaSC (indexLeptonByPt (i)); }
        const float etaSCByIso (size_t i = 0)  const { return getEtaSC (indexLeptonByIso(i)); }
        const float phiByPt (size_t i = 0)     const { return getPhi (indexLeptonByPt (i)); }
        const float phiByIso (size_t i = 0)    const { return getPhi (indexLeptonByIso(i)); }
        const int qByPt (size_t i = 0)         const { return getQ (indexLeptonByPt (i)); }
        const int qByIso (size_t i = 0)      const { return getQ (indexLeptonByIso(i)); }
        const bool isEBByPt (size_t i = 0)   const { return isEB (indexLeptonByPt (i)); }
        const bool isEBByIso (size_t i = 0)  const { return isEB (indexLeptonByIso(i)); }
        const float tkPtByPt (size_t i = 0)  const { return tkPt (indexLeptonByPt (i)); }
        const float tkPtByIso (size_t i = 0) const { return tkPt (indexLeptonByIso(i)); }
        const float mTByPt(size_t i = 0, metType metToUse=TCMET) const { return mT(indexLeptonByPt(i), metToUse); }
        const float dPhilMetByPt(size_t i = 0, metType metToUse=TCMET) const { return dPhilMet(indexLeptonByPt(i),metToUse); }
        const int passCustomByPt(size_t i,std::string &a,const std::string &b) const { return getPassCustom(indexLeptonByPt(i),a,b); }

        const int vtxSize() const { return vtxs_.size(); }
        const int nGoodVertices() const;
        const int mitType() const;

        //---- electron id
        const float deltaEtaSuperClusterTrackAtVtx(size_t i) const;           
        const float deltaPhiSuperClusterTrackAtVtx(size_t i) const;          
        const float sigmaIetaIeta(size_t i) const;
        const float hadronicOverEm(size_t i) const;
        const float numberOfHits(size_t i) const;
            
        const float deltaEtaSuperClusterTrackAtVtxByPt(size_t i) const { return deltaEtaSuperClusterTrackAtVtx (indexLeptonByPt (i)); }          
        const float deltaPhiSuperClusterTrackAtVtxByPt(size_t i) const { return deltaPhiSuperClusterTrackAtVtx (indexLeptonByPt (i)); }
        const float sigmaIetaIetaByPt(size_t i) const { return sigmaIetaIeta (indexLeptonByPt (i)); }
        const float hadronicOverEmByPt(size_t i) const { return hadronicOverEm (indexLeptonByPt (i)); }
        const float numberOfHitsByPt(size_t i) const { return hadronicOverEm (indexLeptonByPt (i)); }
                        
        // VBF Variables
        const int nJetVBF(float pt ,float eta,int applyCorrection, int applyJetID, int applyPileUpJetID) const;
        const float mjj(float pt ,float eta,int applyCorrection, int applyJetID, int applyPileUpJetID) const;
        const float dEtajj(float pt ,float eta,int applyCorrection, int applyJetID, int applyPileUpJetID) const;
        const float zeppenfeld(size_t a,float pt ,float eta,int applyCorrection, int applyJetID, int applyPileUpJetID) const;

        const int nPuppiJetVBF(float pt ,float eta,int applyCorrection, int applyJetID, int applyPileUpJetID) const;
        const float puppiMjj(float pt ,float eta,int applyCorrection, int applyJetID, int applyPileUpJetID) const;
        const float puppidEtajj(float pt ,float eta,int applyCorrection, int applyJetID, int applyPileUpJetID) const;
        const float zeppenfeldPuppi(size_t a,float pt ,float eta,int applyCorrection, int applyJetID, int applyPileUpJetID) const;

        private:
            // User float values
            std::vector<std::string> userFloatLabels_;
            std::vector<float> userFloats_;
            // User int values
            std::vector<std::string> userIntLabels_;
            std::vector<int32_t> userInts_;

            static mwlSortByPtClass mwlSortByPt;
            // JEC to be applied
            static std::vector<std::string> jecFiles_;


            reco::VertexRefVector vtxs_;
            std::vector<double>   sumPts_;
            std::vector<double>   sumPt2s_;
            // MET
            reco::METRef tcMet_;
            pat::METRef  pfMet_;
            pat::METRef  puppiPFMet_;
            reco::PFMET  mvaMet_;
            reco::PFMET  chargedMet_;
            reco::MET    chargedMetSmurf_;
            // Leptons
            std::vector<refToCand> leps_;
            std::vector<refToCand> extraLeps_;
            std::vector<refToCand> softMuons_;

            pat::JetRefVector jets_;
            pat::JetRefVector puppiJets_;
            pat::JetRefVector fatJets_;
            pat::JetRefVector fatPuppiJets_;
            pat::JetRefVector tagJets_;
            pat::JetRefVector tagPuppiJets_;

            reco::GenParticleRefVector genParticles_;
            reco::GenMETRef genMet_;
            reco::GenJetRefVector genJets_;

            GenFilterInfo mcGenWeight_;
            GenEventInfoProduct GenInfoHandle_;
            lhef::HEPEUP LHEhepeup_;

            std::vector< std::string > comments_LHE_;
            std::vector< float > comments_LHE_weight_;
            std::vector< float > comments_LHE_rfac_;
            std::vector< float > comments_LHE_ffac_;

            double rhoJetIso_;

            unsigned int run_;
            unsigned int lumi_;
            unsigned int evt_;

            // trigger bits
            bool passesSingleMuData_;
            bool passesSingleElData_;
            bool passesDoubleMuData_;
            bool passesDoubleElData_;
            bool passesMuEGData_ ;
            bool passesSingleMuMC_ ;
            bool passesSingleElMC_ ;
            bool passesDoubleMuMC_ ;
            bool passesDoubleElMC_ ;
            bool passesMuEGMC_ ;
            bool passesAllEmbed_ ;

    };

};

#endif
