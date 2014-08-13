#ifndef AnalysisDataFormats_SkimEvent_h
#define AnalysisDataFormats_SkimEvent_h

// #include "FWCore/Framework/interface/Event.h"
// #include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/FileInPath.h"
// #include "FWCore/Framework/interface/Frameworkfwd.h"
// #include "FWCore/Framework/interface/EDAnalyzer.h"


#include "DataFormats/Candidate/interface/LeafCandidate.h"
#include "DataFormats/Common/interface/RefToBase.h"
#include "DataFormats/Common/interface/Ref.h"
#include <DataFormats/Common/interface/Ptr.h>
#include "DataFormats/PatCandidates/interface/Jet.h"
#include "DataFormats/PatCandidates/interface/Muon.h"
#include "DataFormats/PatCandidates/interface/Electron.h"
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

//---- pat
#include <DataFormats/PatCandidates/interface/MET.h>

#include "DataFormats/HepMCCandidate/interface/GenParticle.h"
#include "DataFormats/HepMCCandidate/interface/GenParticleFwd.h"

#include "CondFormats/JetMETObjects/interface/JetCorrectorParameters.h"
#include "CondFormats/JetMETObjects/interface/FactorizedJetCorrector.h"
#include "SimDataFormats/GeneratorProducts/interface/GenFilterInfo.h"
#include "SimDataFormats/GeneratorProducts/interface/GenEventInfoProduct.h"
#include "SimDataFormats/GeneratorProducts/interface/LHEEventProduct.h"

#include <vector>
#include <utility>

using std::size_t;

const double Mw = 80.450;
const double Mz = 91.188;

namespace reco {

    struct mwlSortByPtClass {
        bool operator() ( edm::RefToBase<Candidate> a, edm::RefToBase<Candidate> b) { return a->pt() > b->pt(); }
    };

    /*
struct sortPatJetByPtClass {
bool operator() ( pat::JetRef a, pat::JetRef b) { return a.get()->pt() > b.get()->pt(); }
};
*/

    class SkimEvent : public LeafCandidate {

        public:

            // our base object for the leptons now
            //typedef edm::RefToBase<reco::RecoCandidate> refToCand;
            typedef edm::Ptr<reco::RecoCandidate> refToCand;

//             enum hypoType {undefined = 0, WELNU = 1, WMUNU=2, WWELEL=3, WWELMU=4, WWMUEL=5, WWMUMU=6, hypoTypeSize=7};
            enum primaryDatasetType {MC = 0, SingleMuon=1, SingleElectron=2, DoubleMuon=3, MuEG=4, DoubleElectron=5, primaryDatasetTypeSize=6, AllEmbed=7};
            //enum metType { TCMET=0, PFMET=1, CHMET=2, MINMET=3 };
            enum metType { TCMET=0, PFMET=1, CHMET=2};

//             static const std::string hypoTypeNames[];

            /// static functions used to convert to&from hypoType and strings
//             static std::string hypoTypeName(SkimEvent::hypoType);
//             static hypoType hypoTypeByName(const std::string &name);


            SkimEvent();
//             SkimEvent(const hypoType &);

            //
            float userFloat( const std::string & key ) const;
            float userFloat( const char* key ) const { return userFloat( std::string(key) ); }

            void addUserFloat( const std::string & label, float data );
            const std::vector<std::string> & userFloatNames() const { return userFloatLabels_; }
            bool hasUserFloat( const std::string & key ) const {
                return std::find(userFloatLabels_.begin(), userFloatLabels_.end(), key) != userFloatLabels_.end();
            }
            bool hasUserFloat( const char* key ) const {return hasUserFloat( std::string(key) );}

            int32_t userInt( const std::string & key ) const;
            void addUserInt( const std::string & label, int32_t data );
            const std::vector<std::string> & userIntNames() const { return userIntLabels_; }
            bool hasUserInt( const std::string & key ) const {
                return std::find(userIntLabels_.begin(), userIntLabels_.end(), key) != userIntLabels_.end();
            }
 

            //Lepton variables
            //const bool passMuID0() const;
            //const bool passMuID1() const;
            //const bool passMuID2() const;
            //const bool passMuID3() const;
            //const bool passMuID4() const;
            //const bool passMuID5() const;
            //const bool passMuID6() const;
            //at userFloat( const std::string & key ) const;
            //00316 float userFloat( const char* key ) const { return userFloat( std::string(key) ); }
            //00317
            //00319 void addUserFloat( const std::string & label, float data );
            //00321 const std::vector<std::string> & userFloatNames() const { return userFloatLabels_; }
            //00323 bool hasUserFloat( const std::string & key ) const {
            //00324 return std::find(userFloatLabels_.begin(), userFloatLabels_.end(), key) != userFloatLabels_.end();
            //00325 }
            //00327 bool hasUserFloat( const char* key ) const {return hasUserFloat( std::string(key) );}
            //00328
            //00331 int32_t userInt( const std::string & key ) const;
            //00333 void addUserInt( const std::string & label, int32_t data );
            //00335 const std::vector<std::string> & userIntNames() const { return userIntLabels_; }
            //00337 bool hasUserInt( const std::string & key ) const {
            //00338 return std::find(userIntLabels_.begin(), userIntLabels_.end(), key) != userIntLabels_.end();
            //00339 }
            //00340
            //const bool passMuID7() const;
            //const bool passMuID8() const;

// const float mcGenWeight () const { return mcGenWeight_; }
            const float mcGenWeight () const { return mcGenWeight_.filterEfficiency(); }

            const bool isElectron(size_t a) const;
            const bool isMuon(size_t a) const;
            const bool isElectron(const refToCand&) const;
            const bool isMuon(const refToCand&) const;

            const pat::Electron * getElectron(size_t a) const;
            const pat::Muon * getMuon(size_t a) const;
            const pat::Electron * getElectron(const refToCand&) const;
            const pat::Muon * getMuon(const refToCand&) const;

//             const int hypo() const { return hypo_; }
            const int channel() const ;
            const int nLep(float a = -1) const;
            const int nExtraLep(float a = -1) const;
            const int nSoftMu(float a = -1, float vetoJets=-1, float dRCut = 0.3) const;
            const int pdgId(size_t a = 0) const;

            void FindDaughterParticles(const reco::Candidate** pCurrent, std::vector<const reco::Candidate*>* pFinal = 0) const;
            const float getFinalStateMC() const;
            const float getWWdecayMC() const;
            const float mcHiggsProd() const;

            const float HEPMCweight() const ;
            const float HEPMCweightScale(size_t i) const ;
            const float HEPMCweightRen(size_t i) const ;
            const float HEPMCweightFac(size_t i) const ;

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

            //const pat::Muon& mu(size_t a=0) const;
            //const pat::Electron& el(size_t a=0) const;

            const float pt(size_t a = 0) const;
            const int passCustom(size_t a = 0,const std::string &muStr="1", const std::string &elStr="1" ) const;
            const float leptBdt(size_t a = 0) const;
            const float leptLH(size_t a = 0) const;
            const float ptMax() const {return std::max(pt(0),pt(1));}
            const float ptMin() const {return std::min(pt(0),pt(1));}
            const float eta(size_t a = 0) const;
            const float nBrem(size_t a = 0) const;
            const float etaSC(size_t a = 0) const; //returns isMuon ? eta : ele.sc.eta
            const float phi(size_t a = 0) const;
            const int q(size_t a = 0) const;

            const bool peaking() const;
            const reco::GenParticle *genParticle(size_t i) const;
            const reco::GenParticleRef getMotherID(size_t a=0) const;

            //Jet variables
            const int nJets(float pt , int applyCorrection , int applyID) const;
            const int nCentralJets(float pt , float eta,int applyCorrection=true, int applyID=0) const;
            const float leadingVBFJetPt(size_t a, float pt ,float eta,int applyCorrection, int applyID) const;
            const float leadingVBFJetEta(size_t a, float pt ,float eta,int applyCorrection, int applyID) const;
            const float leadingVBFJetPhi(size_t a, float pt ,float eta,int applyCorrection, int applyID) const;
            const int leadingVBFJetId(size_t a, float pt ,float eta,int applyCorrection, int applyID) const;
            const float leadingVBFJetMva(size_t a, float pt ,float eta,int applyCorrection, int applyID) const;
            const pat::Jet* leadingJet(size_t a, float pt ,float eta,int applyCorrection, int applyID) const;
            const float leadingJetPt(size_t a, float pt ,float eta,int applyCorrection, int applyID) const;
            const float leadingJetEta(size_t a, float pt ,float eta,int applyCorrection, int applyID) const;
            const float leadingJetMass(size_t a, float pt ,float eta,int applyCorrection, int applyID) const;
            const float leadingJetPhi(size_t a, float pt ,float eta,int applyCorrection, int applyID) const;

            const float leadingFatJetPt(size_t a, float pt ,float eta,int applyCorrection, int applyID) const;
            const float leadingFatJetEta(size_t a, float pt ,float eta,int applyCorrection, int applyID) const;
            const float leadingFatJetPhi(size_t a, float pt ,float eta,int applyCorrection, int applyID) const;

            const float leadingFatJetTrimmedMass (size_t a, float pt ,float eta,int applyCorrection, int applyID) const;
            const float leadingFatJetFilteredMass (size_t a, float pt ,float eta,int applyCorrection, int applyID) const;
            const float leadingFatJetPrunedMass (size_t a, float pt ,float eta,int applyCorrection, int applyID) const;

            const float leadingFatJetMassDrop (size_t a, float pt ,float eta,int applyCorrection, int applyID) const;
            const float leadingFatJetPrunedTau2Tau1 (size_t a, float pt ,float eta,int applyCorrection, int applyID) const;
            const float leadingFatJetPrunedTau1 (size_t a, float pt ,float eta,int applyCorrection, int applyID) const;
            const float leadingFatJetPrunedTau2 (size_t a, float pt ,float eta,int applyCorrection, int applyID) const;
            const float leadingFatJetPrunedTau3 (size_t a, float pt ,float eta,int applyCorrection, int applyID) const;
            const float leadingFatJetPrunedTau4 (size_t a, float pt ,float eta,int applyCorrection, int applyID) const;

            const float leadingJetPtd(size_t index, float minPt,float eta,int applyCorrection,int applyID) const ;
            const float leadingJetPtD(size_t index, float minPt,float eta,int applyCorrection,int applyID, int QualityCut) const ;

            const float leadingJetQGaxis1(size_t index, float minPt,float eta,int applyCorrection,int applyID, int QualityCut) const ;
            const float leadingJetQGaxis2(size_t index, float minPt,float eta,int applyCorrection,int applyID, int QualityCut) const ;
            const float leadingJetQGRMScand(size_t index, float minPt,float eta,int applyCorrection,int applyID, int QualityCut) const ;
            const float leadingJetQGRmax(size_t index, float minPt,float eta,int applyCorrection,int applyID, int QualityCut) const ;

            const float leadingJetNChgQC(size_t index, float minPt,float eta,int applyCorrection,int applyID) const ;
            const float leadingJetNChgptCut(size_t index, float minPt,float eta,int applyCorrection,int applyID) const ;
            const float leadingJetNNeutralptCut(size_t index, float minPt,float eta,int applyCorrection,int applyID) const ;

            const float leadingJetChargedHadronMultiplicity(size_t index, float minPt,float eta,int applyCorrection,int applyID) const ;
            const float leadingJetNeutralHadronMultiplicity(size_t index, float minPt,float eta,int applyCorrection,int applyID) const ;
            const float leadingJetPhotonMultiplicity(size_t index, float minPt,float eta,int applyCorrection,int applyID) const ;
            const float getJetRhoIso() const ;
            const int leadingJetId(size_t a, float pt ,float eta,int applyCorrection, int applyID) const;
            const float leadingJetMva(size_t a, float pt ,float eta,int applyCorrection, int applyID) const;
            const float dPhillLeadingJet(float eta,int applyCorrection, int applyID) const;
            const bool passesDPhillJet(float pt,float eta,int applyCorrection, int applyID) const;
            const float jetPt(size_t a = 0,int = 0) const;
            const float fatJetPt(size_t a = 0,int = 0) const;

            const float jetPt(const pat::Jet *j,int = 0) const;
            const float tagJetPt(size_t a = 0,int = 0) const;
            static void setupJEC(const std::string&, const std::string&, const std::string&);
            const float nearestJet(int i=0,float minPt=25, float eta=5.0,bool applyCorrection=1, int applyID=0 ) const;
            const pat::JetRef matchedJet(size_t alepton, float minDr=0.4) const;
            const float matchedJetPt(size_t alepton, float minDr=0.4, bool applyCorrection=1) const;
            const bool isThisJetALepton(pat::JetRef jet, float drCut=0.3) const ;

            const bool passJetID (pat::JetRef jet,int) const ;
            const bool passFatJetID (pat::JetRef jet,int) const ;

            const float dPhiJetllInDegrees(size_t a,float pt ,float eta,int applyCorrection, int applyID) const;
            const float dPhiJetll(size_t a,float pt ,float eta,int applyCorrection, int applyID) const;
            const int leadingJetIndex(size_t index,float minPt,float eta,int applyCorrection,int applyID) const;
            const float dPhilljetjet(float eta,int applyCorrection,int applyID) const;
            //Event variables
            const float mTHiggs(metType metToUse=TCMET) const;
            const float tcMetX() const;
            const float pXll() const;
            const float tcMetY() const;
            const float pYll() const;
            const float mTll() const;
            const bool leptEtaCut(float maxAbsEtaMu=2.4,float maxAbsEtaEl=2.5) const;
            void setTriggerBits( const std::vector<bool> &);
            const bool triggerBitsCut(SkimEvent::primaryDatasetType pdType) const;
            const bool guillelmoTrigger(SkimEvent::primaryDatasetType pdType) const;
            const bool triggerMatchingCut(SkimEvent::primaryDatasetType pdType) const;
            bool passTriggerSingleMu(size_t i, bool isData=true) const;
            bool passTriggerDoubleMu(size_t i, bool isData=true) const;
            bool passTriggerElMu(size_t i, bool isData=true) const;
            bool passTriggerDoubleEl(size_t i, bool isData=true) const;

            const float met(metType metToUse=TCMET) const;
            const float pfSumEt() const;
            const float pfMet() const;
            const float pfMetPhi() const;
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
            //const float minMet() const;
            //const math::XYZTLorentzVector minMetP4() const;
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
            const float dPhillTcMet() const;
            const float dPhillChargedMet() const;
            //const float dPhillMinMet() const;
            const float mT(size_t a = 0, metType metToUse=TCMET) const;
            const float dPhilPfMet(size_t a) const;
            const float dPhilMvaMet(size_t a) const;
            const float dPhilTcMet(size_t a) const;
            const float dPhilChargedMet(size_t a) const;
            const float dPhilChargedMetSmurf(size_t a) const;
            //const float dPhilMinMet(size_t a) const;
            const float dPhilMet(metType metToUse=TCMET) const;
            const float dPhilMet(size_t a, metType metToUse=TCMET) const;
            const float dPhilPfMet() const;
            const float dPhilMvaMet() const;
            const float dPhilTcMet() const;
            const float dPhilChargedMet() const;
            const float dPhilChargedMetSmurf() const;
            //const float dPhilMinMet() const;
            const float projMet(metType metToUse=TCMET) const;
            const float projPfMet() const;
            const float projMvaMet() const;
            const float projTcMet() const;
            const float projChargedMet() const;
            const float projChargedMetSmurf() const;
            //const float projMinMet() const;
            //const float pfMT(size_t a = 0) const;
            //const float tcMT(size_t a = 0) const;
            //const float pfMTll(size_t a = 0, size_t b = 1) const;
            //const float tcMTll(size_t a = 0, size_t b = 1) const;
            //const float llType(size_t a = 0, size_t b = 1) const;
            const float nTracks() const;
            //const float dPhiJetTcMet(size_t a = 0, size_t b = 1) const;
            //const float dPhiJetPfMet(size_t a = 0, size_t b = 1) const;
            const float cosThetaStar(size_t a = 0) const;
            const float afb(size_t a = 0) const;
            //const size_t dPhiPfMetMin(size_t a=0, size_t b=0) const;
            //const size_t dPhiTcMetMin(size_t a=0, size_t b=0) const;
            const float mRStar() const;
            const float gamma() const;
            const float gammaMRStar() const;

            //Selection Functions
            const bool passesIP() const;
            const bool passesIP(const refToCand &c) const;
            const bool hasGoodVertex() const;
            const double d0Reco(size_t a=0) const;
            const double dZReco(size_t a=0) const;
            // const bool passesIDV1(size_t a=0) const;
            const bool passesConversion(size_t a=0) const;
            const bool isSTA(size_t a=0) const;
            const bool isSTA(const refToCand &c) const;
            const bool isMuTriggered(size_t a=0) const;

            const int numberOfbQuarks() const;
            const int numberOftQuarks() const;

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
            
//void setLepton(const pat::Electron& ele);
            //void setLepton(const pat::Muon& mu);
            //void setSoftMuon(const pat::Muon& mu);
            //void setExtraLepton(const pat::Electron& ele);
            //void setExtraLepton(const pat::Muon& mu);
//             void setEventInfo (const edm::Event &e) { run_ = e.id().run(); lumi_ = e.id().luminosityBlock(); evt_ = e.id().event(); }
            void setLepton (const edm::Handle<edm::View<reco::RecoCandidate> > &h, size_t i);
            void setSoftMuon (const edm::Handle<edm::View<reco::RecoCandidate> > &h, size_t i);
            void setExtraLepton(const edm::Handle<edm::View<reco::RecoCandidate> > &h, size_t i);

            void setJetRhoIso(const edm::Handle<double> & h);

            void setJets(const edm::Handle<pat::JetCollection> &);
            void setFatJets(const edm::Handle<pat::JetCollection> &);
            void setTagJets(const edm::Handle<pat::JetCollection> &);
            void setTCMet(const edm::Handle<reco::METCollection> &);
            void setPFMet(const edm::Handle< std::vector<pat::MET> > &);
            void setMvaMet(const reco::PFMET &met) {mvaMet_ = met;}
            void setChargedMet(const reco::PFMET &);
            void setChargedMetSmurf(const reco::MET& met) {chargedMetSmurf_ = met;}
            void setVertex(const edm::Handle<reco::VertexCollection> &);
            void setVtxSumPts(const edm::Handle<edm::ValueMap<float> > &s);
            void setVtxSumPt2s(const edm::Handle<edm::ValueMap<float> > &s);
            void setGenParticles(const edm::Handle<reco::GenParticleCollection> &h);
            void setLHEinfo(const edm::Handle<LHEEventProduct> & h);

// void setGenWeight(const edm::Handle<double> &s);
            void setGenWeight(const edm::Handle<GenFilterInfo> &s);

            void setGenInfo(const edm::Handle<GenEventInfoProduct> &s);
            void setGenMet(const edm::Handle<reco::GenMETCollection> &);
            void setGenJets(const edm::Handle<reco::GenJetCollection> &h );
        
            //void sortJetsByPt() { std::sort(jets_.begin(), jets_.end(), sortPatJetByPt); }
            //void sortTagJetsByPt() { std::sort(tagJets_.begin(), tagJets_.end(), sortPatJetByPt); }

            const unsigned int run() const { return run_; }
            const unsigned int lumi() const { return lumi_; }
            const unsigned int evt() const { return evt_; }

            const bool passesVtxSel(size_t a=0) const;
            const reco::Vertex highestPtVtx() const;
            const int bTaggedJetsBetween(const float& minPt,const float& maxPt, const float& discrCut, std::string discriminator="trackCountingHighEffBJetTags", int applyID=0, float dzCut=9999.) const;
            const int bTaggedJetsUnder(const float& maxPt, const float& discrCut, std::string discriminator="trackCountingHighEffBJetTags", int applyID=0, float dzCut=9999.) const;
            const int bTaggedJetsOver(const float& maxPt, const float& discrCut, std::string discriminator="trackCountingHighEffBJetTags", int applyID=0, float dzCut=9999.) const;

            const float leadingJetBtag(size_t a, std::string discriminator="trackCountingHighEffBJetTags",float pt=30.0 ,float eta=5.0,int applyCorrection=true, int applyID=0, float dzCut=9999.) const;
            const float highestBDiscRange(const float& minPt, const float& maxPt, std::string discriminator="trackCountingHighEffBJetTags", int applyID=0, float dzCut=9999., int minPtApplyCorrection =0) const;
            const float highestHardBDisc(const float& maxPt, std::string discriminator="trackCountingHighEffBJetTags", int applyID=0, float dzCut=9999.) const;
            const float highestSoftBDisc(const float& maxPt, std::string discriminator="trackCountingHighEffBJetTags", int applyID=0, float dzCut=9999.) const;

            //Iso Functions
            const bool isEB(size_t a = 0) const;
            const bool isEE(size_t a = 0) const;
            const float tkPt(size_t a = 0) const;

            const size_t indexByPt(size_t a = 0) const;
            const size_t indexByIso(size_t a = 0) const;

            const float tkIso(size_t a = 0) const;
            const float ecalIso(size_t a = 0) const;
            const float hcalIso(size_t a = 0) const;
            const float getRho(size_t a = 0) const;
            const float allIso(size_t a = 0) const;
            const float mvaIso(size_t a = 0) const;

            const float tkVeto(size_t a = 0) const;
            const float ecalVeto(size_t a = 0) const;
            const float hcalVeto(size_t a = 0) const;
            const float allVeto(size_t a = 0) const;

            const reco::Candidate & candByPt(size_t i) const { return *leps_[indexByPt(i)]; }

            const float isSTAByPt (size_t i = 0) const { return isSTA (indexByPt (i)); }
            const float tkIsoByPt (size_t i = 0) const { return tkIso (indexByPt (i)); }
            const float tkIsoByIso (size_t i = 0) const { return tkIso (indexByIso(i)); }
            const float ecalIsoByPt (size_t i = 0) const { return ecalIso(indexByPt (i)); }
            const float ecalIsoByIso(size_t i = 0) const { return ecalIso(indexByIso(i)); }
            const float hcalIsoByPt (size_t i = 0) const { return hcalIso(indexByPt (i)); }
            const float hcalIsoByIso(size_t i = 0) const { return hcalIso(indexByIso(i)); }
            const float allIsoByPt (size_t i = 0) const { return allIso (indexByPt (i)); }
            const float allIsoByIso (size_t i = 0) const { return allIso (indexByIso(i)); }
            const float mvaIsoByPt (size_t i = 0) const { return mvaIso (indexByPt (i)); }
            const float mvaIsoByIso (size_t i = 0) const { return mvaIso (indexByIso(i)); }
            const float tkVetoByPt (size_t i = 0) const { return tkVeto (indexByPt (i)); }
            const float tkVetoByIso (size_t i = 0) const { return tkVeto (indexByIso(i)); }
            const float ecalVetoByPt (size_t i = 0) const { return ecalVeto(indexByPt (i)); }
            const float ecalVetoByIso(size_t i = 0) const { return ecalVeto(indexByIso(i)); }
            const float hcalVetoByPt (size_t i = 0) const { return hcalVeto(indexByPt (i)); }
            const float hcalVetoByIso(size_t i = 0) const { return hcalVeto(indexByIso(i)); }
            const float allVetoByPt (size_t i = 0) const { return allVeto (indexByPt (i)); }
            const float allVetoByIso (size_t i = 0) const { return allVeto (indexByIso(i)); }
            const int pdgIdByPt (size_t i = 0) const { return pdgId (indexByPt (i)); }
            const int pdgIdByIso (size_t i = 0) const { return pdgId (indexByIso(i)); }
            const float ptByPt (size_t i = 0) const { return pt (indexByPt (i)); }
            const float leptBdtByPt (size_t i = 0) const { return leptBdt (indexByPt (i)); }
            const float leptLHByPt (size_t i = 0) const { return leptLH (indexByPt (i)); }
            const float nBremByPt (size_t i = 0) const { return nBrem (indexByPt (i)); }
            const float ptByIso (size_t i = 0) const { return pt (indexByIso(i)); }
            const float etaByPt (size_t i = 0) const { return eta (indexByPt (i)); }
            const float etaByIso (size_t i = 0) const { return eta (indexByIso(i)); }
            const float etaSCByPt (size_t i = 0) const { return etaSC (indexByPt (i)); }
            const float etaSCByIso (size_t i = 0) const { return etaSC (indexByIso(i)); }
            const float phiByPt (size_t i = 0) const { return phi (indexByPt (i)); }
            const float phiByIso (size_t i = 0) const { return phi (indexByIso(i)); }
            const int qByPt (size_t i = 0) const { return q (indexByPt (i)); }
            const int qByIso (size_t i = 0) const { return q (indexByIso(i)); }
            const bool isEBByPt (size_t i = 0) const { return isEB (indexByPt (i)); }
            const bool isEBByIso (size_t i = 0) const { return isEB (indexByIso(i)); }
            const float tkPtByPt (size_t i = 0) const { return tkPt (indexByPt (i)); }
            const float tkPtByIso (size_t i = 0) const { return tkPt (indexByIso(i)); }
            const float mTByPt(size_t i = 0, metType metToUse=TCMET) const { return mT(indexByPt(i), metToUse); }
            const float dPhilMetByPt(size_t i = 0, metType metToUse=TCMET) const { return dPhilMet(indexByPt(i),metToUse); }
            const int passCustomByPt(size_t i,std::string &a,const std::string &b) const { return passCustom(indexByPt (i),a,b); }

            const int vtxSize() const { return vtxs_.size(); }
            const int nGoodVertices() const;
            const int mitType() const;

            const bool passesSmurfMuonID() const;
            const bool isHardMuID(size_t a) const;

            //Matt's
            // const int nExtraLepMatt(float a = -1) const;
            // const int nSoftMuMatt(float a = -1) const;

            // VBF Variables
            const int nJetVBF(float pt ,float eta,int applyCorrection, int applyID) const;
            const float mjj(float pt ,float eta,int applyCorrection, int applyID) const;
            const float dEtajj(float pt ,float eta,int applyCorrection, int applyID) const;
            const float zeppenfeld(size_t a,float pt ,float eta,int applyCorrection, int applyID) const;




        private:
            // User float values
            std::vector<std::string> userFloatLabels_;
            std::vector<float> userFloats_;
            // User int values
            std::vector<std::string> userIntLabels_;
            std::vector<int32_t> userInts_;

            static mwlSortByPtClass mwlSortByPt;
            //static sortPatJetByPtClass sortPatJetByPt;
            static std::vector<std::string> jecFiles_;

//             int hypo_;
            reco::VertexRefVector vtxs_;
            std::vector<double> sumPts_;
            std::vector<double> sumPt2s_;
            reco::METRef tcMet_;
//             reco::PFMETRef pfMet_;
            pat::METRef pfMet_;
            reco::PFMET mvaMet_;
            reco::PFMET chargedMet_;
            reco::MET chargedMetSmurf_;
            std::vector<refToCand> leps_;
            std::vector<refToCand> extraLeps_;
            std::vector<refToCand> softMuons_;
            //edm::RefToBaseVector<reco::RecoCandidate> leps_;
            //edm::RefToBaseVector<reco::RecoCandidate> extraLeps_;
            //edm::RefToBaseVector<reco::RecoCandidate> softMuons_;
            pat::JetRefVector jets_;
            pat::JetRefVector fatJets_;
            pat::JetRefVector tagJets_;
            reco::GenParticleRefVector genParticles_;
            reco::GenMETRef genMet_;
            reco::GenJetRefVector genJets_;
// float mcGenWeight_;
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

            //JEC


    };

}

#endif