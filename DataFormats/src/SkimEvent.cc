#include "DataFormats/PatCandidates/interface/JetCorrFactors.h"
#include "LatinoTrees/DataFormats/interface/SkimEvent.h"
#include "CommonTools/Utils/interface/StringCutObjectSelector.h"
#include "Math/VectorUtil.h"

#include <TVector3.h>

#include <iostream>
#include <algorithm>
#include <sstream>

// order the collection of lepton asaf of a value, typically pt

struct indexValueStruct { // independent struct with a float value (like pt) and a index (position in the original collection)
    indexValueStruct(const float & v, const size_t & i) : value(v), index(i) {}
    float value;
    size_t index;
};

bool lowToHigh(const indexValueStruct & a, const indexValueStruct & b) { // order from lower to higher
    return a.value < b.value;
}

bool highToLow(const indexValueStruct &a, const indexValueStruct &b) { // reverse sort
    return a.value > b.value;
}

// get method implementation
const bool reco::SkimEvent::isMuon(size_t i) const {
  if(i < leps_.size()) return isMuon(leps_[i]); // refToCandidate
  else return false;
}

const bool reco::SkimEvent::isElectron(size_t i) const {
  if(i < leps_.size()) return isElectron(leps_[i]); // refToCandidate
  else return false;
}

const bool reco::SkimEvent::isMuon(const refToCand &c) const {
  return (abs(c->pdgId()) == 13);
}

const bool reco::SkimEvent::isElectron(const refToCand &c) const {
  return (abs(c->pdgId()) == 11);
}

const pat::Muon * reco::SkimEvent::getMuon(size_t i) const {
  return getMuon(leps_[i]);
}

const pat::Electron * reco::SkimEvent::getElectron(size_t i) const {
  return getElectron(leps_[i]);
}

const pat::Muon * reco::SkimEvent::getMuon(const refToCand &c) const {
  if(isMuon(c)) return static_cast<const pat::Muon*>(c.get());
  else return NULL;    
}

const pat::Electron * reco::SkimEvent::getElectron(const refToCand &c) const {
  if(isElectron(c)) return static_cast<const pat::Electron*>(c.get());
  return NULL;
}


const float reco::SkimEvent::allLeptonIso(size_t i) const {    
    if( i >= leps_.size() ) return -9999.0;
    if( isElectron(i) ){
      if(getElectron(i)->hasUserFloat("eleSmurfPF04"))
        return getElectron(i)->userFloat("eleSmurfPF04");
      else return 9999.0;
    } 
    else if( isMuon(i) ){
      if(getMuon(i)->hasUserFloat("muSmurfPF"))
      return getMuon(i)->userFloat("muSmurfPF");
      else return 9999.0;
    } 
    else {
        std::cout << " Do I ever friggin get here?" << std::endl;
        return 9999.0;
    }
}

const size_t reco::SkimEvent::indexLeptonByPt(size_t i) const { // size of the lepton collection
   
    if( i >= leps_.size() ) return 9999; //--> lepton index by pt
    std::vector<indexValueStruct> a;
    for(size_t j = 0;j < leps_.size(); ++j ) 
      a.push_back(indexValueStruct(leps_.at(j)->pt(),j));

    std::sort(a.begin(),a.end(),highToLow); // sort in case is not sorted and retun the index

    return a[i].index; 
}


const size_t reco::SkimEvent::indexLeptonByIso(size_t i) const {
    
    if( i >= leps_.size() ) return 9999;
    std::vector<indexValueStruct> a;

    for(size_t j = 0; j < leps_.size(); ++j) 
     a.push_back(indexValueStruct(allLeptonIso(j),j));
    std::sort(a.begin(),a.end(),lowToHigh);
    return a[i].index;
}

const int reco::SkimEvent::getPdgId(size_t i) const {
    if(i < leps_.size()) return leps_[i]->pdgId();
    else return -9999;
}


const int reco::SkimEvent::getChannel() const { // take the two leading lepton if they exisist and return a value asaf of their flavour

      if ((pdgIdByPt(0) != -9999) && (pdgIdByPt(1) != -9999)) {
      if      (abs(pdgIdByPt(0)) == 13 && abs(pdgIdByPt(1)) == 13 ) return 0; //---- mm
      else if (abs(pdgIdByPt(0)) == 11 && abs(pdgIdByPt(1)) == 11 ) return 1; //---- ee
      else if (abs(pdgIdByPt(0)) == 11 && abs(pdgIdByPt(1)) == 13 ) return 2; //---- em
      else if (abs(pdgIdByPt(0)) == 13 && abs(pdgIdByPt(1)) == 11 ) return 3; //---- me
      else return -1;  //--> new c++ constraint! need all options return!
    }
    else {
     return -1;
    }
}

std::vector<std::string> reco::SkimEvent::jecFiles_;


float reco::SkimEvent::userFloat( const std::string &key ) const {
  std::vector<std::string>::const_iterator it = std::find(userFloatLabels_.begin(), userFloatLabels_.end(), key);
  if (it != userFloatLabels_.end()) { return userFloats_[it - userFloatLabels_.begin()]; }
  return 0.0;
}

void reco::SkimEvent::addUserFloat( const std::string & label, float data ){
  userFloatLabels_.push_back(label);
  userFloats_.push_back( data );
}


int reco::SkimEvent::userInt( const std::string & key ) const{
  std::vector<std::string>::const_iterator it = std::find(userIntLabels_.begin(), userIntLabels_.end(), key);
  if (it != userIntLabels_.end()) {
      return userInts_[it - userIntLabels_.begin()];
  }
  return 0;
}

void reco::SkimEvent::addUserInt( const std::string &label, int data ){
  userIntLabels_.push_back(label);
  userInts_.push_back( data );
}


const bool reco::SkimEvent::peaking() const {
    if(leps_.size() < 2) return false;
    if (getMotherID(0).isNonnull() && getMotherID(1).isNonnull() && getMotherID(0) == getMotherID(1)) return true;

    return false;
}

const reco::GenParticle *reco::SkimEvent::genParticle(size_t i) const { // need to embed gen particles in pat objects
    return isMuon(i) ? getMuon(i)->genLepton() : getElectron(i)->genLepton();
}

const reco::GenParticleRef reco::SkimEvent::getMotherID(size_t i) const { // get mother ref given a lepton

  if(i >= leps_.size()) return reco::GenParticleRef();  // return empty ref  
 
    const reco::GenParticle *match = isMuon(i) ? getMuon(i)->genLepton() : getElectron(i)->genLepton();

    if( !match ) return reco::GenParticleRef(); // no reference to gen particle
    if( !match->numberOfMothers() ) return reco::GenParticleRef(); // no mothers

    int pdgId = match->pdgId();

    reco::GenParticleRef mother = match->motherRefVector()[0]; // take all the mothers, zero position
    if( !mother.isNonnull() ) return reco::GenParticleRef();
    if( !mother->numberOfMothers() ) return reco::GenParticleRef();
    while(mother->pdgId() == pdgId) {
        mother = mother->motherRefVector()[0];
        if( !mother.isNonnull() ) return reco::GenParticleRef();
        if( !mother->numberOfMothers() ) return reco::GenParticleRef();
    }
    return mother;
}

// default constructor
reco::SkimEvent::SkimEvent() : sumPts_(0) { }


// set GenParticles
void reco::SkimEvent::setGenParticles(const edm::Handle<reco::GenParticleCollection> & h) {
   genParticles_.clear();
   for(size_t i = 0; i < h->size(); ++i)
    genParticles_.push_back( reco::GenParticleRef(h,i) );
   
}

// set GenJets
void reco::SkimEvent::setGenJets(const edm::Handle<reco::GenJetCollection> & h) {
   genJets_.clear();
   for(size_t i=0; i < h->size(); ++i)
    genJets_.push_back( reco::GenJetRef(h,i) );
   
}

// set LHEinfo
void reco::SkimEvent::setLHEinfo(const edm::Handle<LHEEventProduct> & h) {

 LHEhepeup_ = (*(h.product())).hepeup();
 std::vector<std::string>::const_iterator it_end = (*(h.product())).comments_end();
 std::vector<std::string>::const_iterator it = (*(h.product())).comments_begin();
 for(; it != it_end; it++)
  comments_LHE_.push_back (*it);
 

 for (unsigned int iComm = 0; iComm < comments_LHE_.size(); iComm++) {
  std::stringstream line( comments_LHE_.at(iComm));
  std::string dummy;
  line >> dummy; // #new weight,renfact,facfact,pdf1,pdf2
  float dummy_float;
  line >> dummy_float; // 32.2346904790193
  comments_LHE_weight_.push_back(dummy_float);
  line >> dummy_float; // 1.00000000000000
  comments_LHE_rfac_.push_back(dummy_float);
  line >> dummy_float; // 1.00000000000000
  comments_LHE_ffac_.push_back(dummy_float);
 }

}


const float reco::SkimEvent::getHEPMCweightScale(size_t i) const {
 if (i < comments_LHE_weight_.size()) return comments_LHE_weight_.at(i);
 else return -1;
}

const float reco::SkimEvent::getHEPMCweightRen(size_t i) const {
 if (i < comments_LHE_rfac_.size()) return comments_LHE_rfac_.at(i);
 else return -1;
}

const float reco::SkimEvent::getHEPMCweightFac(size_t i) const {
 if (i < comments_LHE_ffac_.size()) return comments_LHE_ffac_.at(i);
 else return -1;
}


//EDM RefToBase implementation
void reco::SkimEvent::setLepton(const edm::Handle<edm::View<reco::RecoCandidate> > & h,size_t i){
  leps_.push_back( h->ptrAt(i) );
}

void reco::SkimEvent::setExtraLepton(const edm::Handle<edm::View<reco::RecoCandidate> > & h,size_t i){
  extraLeps_.push_back( h->ptrAt(i) );
}

void reco::SkimEvent::setSoftMuon(const edm::Handle<edm::View<reco::RecoCandidate> > & h,size_t i){
  softMuons_.push_back(h->ptrAt(i) );
}

void reco::SkimEvent::setJets(const edm::Handle<pat::JetCollection> & jH) {
    jets_.clear();
    for(size_t i = 0; i < jH->size(); ++i)
      jets_.push_back(pat::JetRef(jH,i));
}

void reco::SkimEvent::setPuppiJets(const edm::Handle<pat::JetCollection> & jH) {
    puppiJets_.clear();
    for(size_t i = 0; i < jH->size(); ++i)
      puppiJets_.push_back(pat::JetRef(jH,i));
}


void reco::SkimEvent::setFatJets(const edm::Handle<pat::JetCollection> & jH) {
 fatJets_.clear();
 for(size_t i=0;i<jH->size();++i)
  fatJets_.push_back(pat::JetRef(jH,i));
}

void reco::SkimEvent::setFatPuppiJets(const edm::Handle<pat::JetCollection> & jH) {
 fatPuppiJets_.clear();
 for(size_t i=0;i<jH->size();++i)
  fatPuppiJets_.push_back(pat::JetRef(jH,i));
}


void reco::SkimEvent::setTagJets(const edm::Handle<pat::JetCollection> & jH) {

    tagJets_.clear();
    for(size_t i=0;i<jH->size();++i)
      tagJets_.push_back(pat::JetRef(jH,i));
}

void reco::SkimEvent::setPuppiTagJets(const edm::Handle<pat::JetCollection> & jH) {
    tagPuppiJets_.clear();
    for(size_t i=0;i<jH->size();++i)
      tagPuppiJets_.push_back(pat::JetRef(jH,i));
}

void reco::SkimEvent::setTCMet(const edm::Handle<reco::METCollection> & mH) {
    tcMet_ = reco::METRef(mH,0);
}

void reco::SkimEvent::setPFMet(const edm::Handle< std::vector<pat::MET> > & mH) {
    pfMet_ = pat::METRef(mH,0);
}

void reco::SkimEvent::setPuppiPFMet(const edm::Handle< std::vector<pat::MET> > & mH) {
    puppiPFMet_ = pat::METRef(mH,0);
}

void reco::SkimEvent::setChargedMet(const reco::PFMET & chMET) {
    chargedMet_ = chMET;
}

void reco::SkimEvent::setGenMet(const edm::Handle<reco::GenMETCollection> & mH) {
    genMet_ = reco::GenMETRef(mH,0);
}


void reco::SkimEvent::setVtxSumPts(const edm::Handle<edm::ValueMap<float> > &s) {
    for(size_t i=0 ; i < vtxs_.size();++i) 
      sumPts_.push_back( (*s)[vtxs_[i]] );
}

void reco::SkimEvent::setVtxSumPt2s(const edm::Handle<edm::ValueMap<float> > &s) {
    for(size_t i=0;i<vtxs_.size();++i) sumPt2s_.push_back( (*s)[vtxs_[i]] );
}

void reco::SkimEvent::setVertex(const edm::Handle<reco::VertexCollection> & vtxH) {
    for(size_t i=0;i<vtxH->size();++i) vtxs_.push_back(reco::VertexRef(vtxH,i));
}


void reco::SkimEvent::setGenWeight(const edm::Handle<GenFilterInfo> &mcGenWeight) {
  mcGenWeight_ = *(mcGenWeight.product());
}

void reco::SkimEvent::setGenInfo(const edm::Handle<GenEventInfoProduct> &GenInfoHandle) {
  GenInfoHandle_ = *(GenInfoHandle.product());
}

//Lepton variables
const float reco::SkimEvent::getPt(size_t i) const {
  if(i >= leps_.size()) return -9999.0;
  return leps_[i]->pt();
}

const int reco::SkimEvent::getNLep(float minPt) const { // simple count over a pt threshold
    int count = 0;
    if(minPt < 0) count = leps_.size();
    else for(size_t i=0; i<leps_.size(); ++i) if(leps_[i]->pt() > minPt) count++;
    return count;
}

const int reco::SkimEvent::getNExtraLep(float minPt) const { // extra lepton counting over a pt threshold
    int count = 0;
     if(minPt < 0) count = extraLeps_.size();
     else for(size_t i=0;i<extraLeps_.size();++i) if(extraLeps_[i]->pt() > minPt) count++;
    return count;
}


const int reco::SkimEvent::getNSoftMu(float minPt, float vetoJets, float dRCut) const { // number of softMuons
    int count = 0;
    if(minPt < 0 && vetoJets < 0) count = softMuons_.size();
    else if(vetoJets < 0) {
      for(size_t i=0; i < softMuons_.size();++i){
        if(softMuons_[i]->pt() < minPt) continue;
	if(!isSoftMuon(i)) continue ;
        count++; //  minPt is the minimum pt threshold
      }
    } else {
        for(size_t i=0;i < softMuons_.size();++i) {
            bool toCount = true;
            for(size_t j=0; j < jets_.size();++j){
	      if(jetPt(jets_[j].get(),true) <= vetoJets) continue; // take all the jets over the miniimum veto threshold
	      if(!isSoftMuon(i)) continue ;
	       double dR = fabs(ROOT::Math::VectorUtil::DeltaR(softMuons_[i]->p4(),jets_[j]->p4()) ); // dR between soft muon and jet and apply a cut, if overlap not consider
                if(dR < dRCut) toCount = false;
            }
            if(toCount && softMuons_[i]->pt() > minPt) count++;
        }
    }  
    return count;
}


const math::XYZTLorentzVector reco::SkimEvent::lepton(size_t i) const {
 if(indexLeptonByPt (i) >= leps_.size()) return math::XYZTLorentzVector(0,0,0,0); 
 return leps_[indexLeptonByPt(i)]->p4();
}

// apply custom cuts
const int reco::SkimEvent::getPassCustom(size_t i, const std::string & muStr, const std::string & elStr) const {
  if(i >= leps_.size()) return 0;
  else if( isElectron(i) ) return StringCutObjectSelector<pat::Electron>(elStr,true)( *getElectron(i) );
  else if( isMuon(i) )     return StringCutObjectSelector<pat::Muon >   (muStr,true)( *getMuon(i) );
  else return 0;
}


const float reco::SkimEvent::getLeptId(size_t i, std::string idele, std::string idmu) const { // get lepton id as userfloat
 if(i >= leps_.size()) return -9999.0;

 if( isElectron(i) ) 
   if(getElectron(i)->hasUserFloat(idele)) return getElectron(i)->userFloat(idele); //---- "idele" is the name of the id for electrons
   else return getElectron(i)->electronID(idele);
 else{ 
   if(getMuon(i)->hasUserFloat(idmu)) return getMuon(i)->userFloat(idmu);
   else return getMuon(i)->muonID(idmu);
  }
}  


const float reco::SkimEvent::getLeptBdt(size_t i) const {
  if(i >= leps_.size()) return -9999.0;
  if( isElectron(i) ){
    if(getElectron(i)->hasUserFloat("bdttrig")) return getElectron(i)->userFloat("bdttrig"); // 
    else return 999999.;
  }
  else return 999999.;
}

const float reco::SkimEvent::getEta(size_t i) const {
  if(i < leps_.size()) return leps_[i]->eta();
  else return -9999.0;
}

const float reco::SkimEvent::getEtaSC(size_t i) const {
  if(i >= leps_.size()) return -9999.0;
  if( isElectron(i) ) return getElectron(i)->superCluster()->eta();
  else return leps_[i]->eta();
}

const float reco::SkimEvent::getNBrem(size_t i) const {
  if(i >= leps_.size()) return -9999.0;
  if( isElectron(i) ) return getElectron(i)->numberOfBrems();
  else return 0;
}

const float reco::SkimEvent::getPhi(size_t i) const {
  if(i < leps_.size()) return leps_[i]->phi();
  else return -9999.0;
}


const int reco::SkimEvent::getQ(size_t i) const{
  if(i < leps_.size()) return leps_[i]->charge();
  else return -9999.0;
}

const float reco::SkimEvent::deltaEtaSuperClusterTrackAtVtx(size_t i) const {
 if(i >= leps_.size()) return -9999.0;
 if( isElectron(i) ) return getElectron(i)->deltaEtaSuperClusterTrackAtVtx();
 else return -999.0;
}

const float reco::SkimEvent::deltaPhiSuperClusterTrackAtVtx(size_t i) const {
 if(i >= leps_.size()) return -9999.0;
 if( isElectron(i) ) return getElectron(i)->deltaPhiSuperClusterTrackAtVtx();
 else return -999.0;
}

const float reco::SkimEvent::sigmaIetaIeta(size_t i) const {
 if(i >= leps_.size()) return -9999.0;
 if( isElectron(i) ) return getElectron(i)->sigmaIetaIeta();
 else return -999.0;
}

const float reco::SkimEvent::hadronicOverEm(size_t i) const {
 if(i >= leps_.size()) return -9999.0;
 if( isElectron(i) ) return getElectron(i)->hadronicOverEm();
 else return -999.0;
}

const float reco::SkimEvent::numberOfHits(size_t i) const {
 if(i >= leps_.size()) return -9999.0;
 if( isElectron(i) ) return getElectron(i)->gsfTrack()->trackerExpectedHitsInner().numberOfHits();
 else return -999.0;
}


// New emanuele gamma mr star thingy
const float reco::SkimEvent::mRStar() const {

  if(leps_.size() < 2) return -9999.;
  float A  = leps_[indexLeptonByPt(0)]->p();
  float B  = leps_[indexLeptonByPt(1)]->p();
  float az = leps_[indexLeptonByPt(0)]->pz();
  float bz = leps_[indexLeptonByPt(1)]->pz();
  TVector3 jaT, jbT;
  jaT.SetXYZ(leps_[indexLeptonByPt(0)]->px(),leps_[indexLeptonByPt(0)]->py(),0.0);
  jbT.SetXYZ(leps_[indexLeptonByPt(1)]->px(),leps_[indexLeptonByPt(1)]->py(),0.0);

  float temp = sqrt((A+B)*(A+B)-(az+bz)*(az+bz)-(jbT.Dot(jbT)-jaT.Dot(jaT))*(jbT.Dot(jbT)-jaT.Dot(jaT))/(jaT+jbT).Mag2());

  return temp;
}

const float reco::SkimEvent::gamma() const {
  if(leps_.size() < 2) return -9999.;
  float A  = leps_[indexLeptonByPt(0)]->p();
  float B  = leps_[indexLeptonByPt(1)]->p();
  float az = leps_[indexLeptonByPt(0)]->pz();
  float bz = leps_[indexLeptonByPt(1)]->pz();
  TVector3 jaT, jbT;
  jaT.SetXYZ(leps_[indexLeptonByPt(0)]->px(),leps_[indexLeptonByPt(0)]->py(),0.0);
  jbT.SetXYZ(leps_[indexLeptonByPt(1)]->px(),leps_[indexLeptonByPt(1)]->py(),0.0);

  float ATBT = (jaT+jbT).Mag2();
  double mybeta = (jbT.Dot(jbT)-jaT.Dot(jaT))/sqrt(ATBT*((A+B)*(A+B)-(az+bz)*(az+bz)));

  return 1./sqrt(1.-mybeta*mybeta);
}

const float reco::SkimEvent::gammaMRStar() const {
    return gamma() * mRStar();
}



//Jet variables
void reco::SkimEvent::setupJEC(const std::string &l2File, const std::string &l3File, const std::string &residualFile) {
    jecFiles_.clear();
    jecFiles_.push_back(l2File);
    jecFiles_.push_back(l3File);
    if(residualFile.compare("")) jecFiles_.push_back(residualFile);
}

const float reco::SkimEvent::jetPt(size_t i, int applyCorrection) const {
    return jetPt(jets_[i].get(),applyCorrection);
}

const float reco::SkimEvent::puppiJetPt(size_t i, int applyCorrection) const {
    return jetPt(puppiJets_[i].get(),applyCorrection);
}

const float reco::SkimEvent::jetPt(const pat::Jet *j, int applyCorrection) const {
  if(applyCorrection) return j->pt();
  else return j->correctedJet("Uncorrected","none").pt();
}


const int reco::SkimEvent::nJets(float minPt, int applyCorrection, int applyJetID, int applyPileUpJetID) const {
  return nCentralJets(minPt,99.9, applyCorrection,applyJetID,applyPileUpJetID);
}

const int reco::SkimEvent::nPuppiJets(float minPt, int applyCorrection, int applyJetID, int applyPileUpJetID) const {
  return nCentralPuppiJets(minPt,99.9, applyCorrection,applyJetID,applyPileUpJetID);
}

const bool reco::SkimEvent::isThisJetALepton(pat::JetRef jet, float drCut) const { // check if overlap with lepton
    bool thisJetIsLepton(false);
    for(size_t j = 0; j < std::min((uint) 2,(uint) leps_.size());++j){
        double dR = fabs(ROOT::Math::VectorUtil::DeltaR(jet->p4(),leps_[j]->p4()));
        if(dR < drCut){
            thisJetIsLepton = true;
            break;
        }
    }

    return thisJetIsLepton;
}

const bool reco::SkimEvent::passJetID(pat::JetRef jet, int applyID) const{ // 0 noID, 1 is default cut based
  // no ID
  if(applyID == 0) return true;  
  // old ID
  else if(applyID == 1) {
    unsigned int multiplicity = jet->neutralMultiplicity () + jet->chargedMultiplicity ();
    if(jet->neutralEmEnergyFraction() >=0.99 || jet->neutralHadronEnergyFraction() >=0.99 || multiplicity==0 ) return false;
    if(fabs(jet->eta())<2.4){
    if(jet->chargedEmEnergyFraction() >=0.99 || jet->chargedHadronEnergyFraction() == 0 || jet->chargedMultiplicity()==0 ) return false;
   }
   return true;
  }

  // MVA ID loose
  else if(applyID == 4 and jet->hasUserInt("jetId")) {
    if(jet->userInt("jetId") >= 4) return true;
    else return false;
  }
  
  // MVA ID medium
  else if(applyID == 5 and jet->hasUserInt("jetId"))  {
    if(jet->userInt("jetId") >= 6) return true;
    else return false;
  }
  
  // MVA ID tight
  else if(applyID == 6 and jet->hasUserInt("jetId")) {
    if(jet->userInt("jetId") >= 7) return true;
    else return false;
  }
  
  return false;
}


const bool reco::SkimEvent::passPileUpJetID(pat::JetRef jet, int applyID) const{ // 0 noID, 1 loose WP, 2 medium WP, 3 tight

  // no ID
  if(applyID == 0 or applyID > 3) return true;  
  // old ID
  else if(jet->hasUserInt("pileupJetId:fullId")){
    if(applyID == 1 and PileupJetIdentifier::passJetId(jet->userInt("pileupJetId:fullId"),PileupJetIdentifier::kLoose) == true ) return true ;
    else if(applyID == 2 and PileupJetIdentifier::passJetId(jet->userInt("pileupJetId:fullId"),PileupJetIdentifier::kMedium) == true ) return true ;
    else if(applyID == 3 and PileupJetIdentifier::passJetId(jet->userInt("pileupJetId:fullId"),PileupJetIdentifier::kTight) == true ) return true ;
    else return false;
  }
  else return true ;
}


const bool reco::SkimEvent::passFatJetID(pat::JetRef jet, int applyID) const{
  // no ID
 if(applyID == 0) return true;  
  // old ID
 else if(applyID == 1) { ///----> AN 2013/139 Sec 3.1.3
  unsigned int multiplicity = jet->neutralMultiplicity () + jet->chargedMultiplicity ();
  ///---- Loose from https://twiki.cern.ch/twiki/bin/view/CMS/JetID
  if( jet->neutralEmEnergyFraction() >=0.99 || jet->neutralHadronEnergyFraction() >=0.99 || multiplicity<=1 ) return false;
  if(fabs(jet->eta())<2.4){
   if(jet->chargedEmEnergyFraction() >=0.99 || jet->chargedHadronEnergyFraction() == 0 || jet->chargedMultiplicity()==0) return false;
  }
  return true;
 }
 return false;
}


const float reco::SkimEvent::dPhiJetllInDegrees(size_t leadingIndex,float minPt,float eta,int applyCorrection,int applyJetID, int applyPileUpJetID) const {
  return dPhiJetll(leadingIndex,minPt,eta,applyCorrection,applyJetID,applyPileUpJetID)/M_PI * 180.;
}

const float reco::SkimEvent::dPhiPuppiJetllInDegrees(size_t leadingIndex,float minPt,float eta,int applyCorrection,int applyJetID, int applyPileUpJetID) const {
  return dPhiPuppiJetll(leadingIndex,minPt,eta,applyCorrection,applyJetID,applyPileUpJetID)/M_PI * 180.;
}

const float reco::SkimEvent::dPhiJetll(size_t leadingIndex,float minPt,float eta,int applyCorrection,int applyJetID, int applyPileUpJetID) const { // phi jet ll

    if(leps_.size() < 2) return -9999.0;
 
    size_t count = 0, newIndex = 0;
    for(size_t i = 0; i < jets_.size();++i) { // loop on jets
      if(!(passJetID(jets_[i],applyJetID)) ) continue; // apply jet id
      if(!(passPileUpJetID(jets_[i],applyPileUpJetID)) ) continue; // apply pile-up jet id
      if( std::fabs(jets_[i]->eta()) >= eta) continue; //eta range
      if( jetPt(jets_[i].get(),applyCorrection) <= minPt) continue; // min pt for jet

      if(isThisJetALepton(jets_[i])) continue; // cleaning with lepton
      // When count becomes higher than leadingIndex, we've found the leadingIndexth jet that passes all the cuts
      if(++count > leadingIndex) {
            newIndex = i;
            break;
        }
    }
    
    return fabs(ROOT::Math::VectorUtil::DeltaPhi(leps_[indexLeptonByPt(0)]->p4() + leps_[indexLeptonByPt(1)]->p4(), jets_[newIndex]->p4()) ); // delta phi returned
}


const float reco::SkimEvent::dPhiPuppiJetll(size_t leadingIndex,float minPt,float eta,int applyCorrection,int applyJetID, int applyPileUpJetID) const { // phi jet ll

    if(leps_.size() < 2) return -9999.0;
 
    size_t count = 0, newIndex = 0;
    for(size_t i = 0; i < puppiJets_.size();++i) { // loop on jets
      if(!(passJetID(puppiJets_[i],applyJetID)) ) continue; // apply jet id
      if(!(passPileUpJetID(puppiJets_[i],applyPileUpJetID)) ) continue; // apply pile-up jet id
      if( std::fabs(puppiJets_[i]->eta()) >= eta) continue; //eta range
      if( jetPt(puppiJets_[i].get(),applyCorrection) <= minPt) continue; // min pt for jet

      if(isThisJetALepton(puppiJets_[i])) continue; // cleaning with lepton
      // When count becomes higher than leadingIndex, we've found the leadingIndexth jet that passes all the cuts
      if(++count > leadingIndex) {
            newIndex = i;
            break;
        }
    }
    
    return fabs(ROOT::Math::VectorUtil::DeltaPhi(leps_[indexLeptonByPt(0)]->p4() + leps_[indexLeptonByPt(1)]->p4(), puppiJets_[newIndex]->p4()) ); // delta phi returned
}



const math::XYZTLorentzVector reco::SkimEvent::jet(size_t index, float minPt,float eta,int applyCorrection,int applyJetID, int applyPileUpJetID) const {
 unsigned int index_jet_ordered = indexJetByPt(index, minPt, eta, applyCorrection, applyJetID, applyPileUpJetID);
 if (index_jet_ordered >= jets_.size()) return math::XYZTLorentzVector(0,0,0,0);
 return jets_[index_jet_ordered]->p4();
}

const math::XYZTLorentzVector reco::SkimEvent::puppiJet(size_t index, float minPt,float eta,int applyCorrection,int applyJetID, int applyPileUpJetID) const {
 unsigned int index_jet_ordered = indexPuppiJetByPt(index, minPt, eta, applyCorrection, applyJetID, applyPileUpJetID);
 if (index_jet_ordered >= puppiJets_.size()) return math::XYZTLorentzVector(0,0,0,0);
 return puppiJets_[index_jet_ordered]->p4();
}


const pat::Jet* reco::SkimEvent::leadingJet(size_t index, float minPt,float eta,int applyCorrection,int applyJetID, int applyPileUpJetID) const {

    size_t count = 0;
    for(size_t i = 0; i < jets_.size();++i) {
      if(!(passJetID(jets_[i],applyJetID)) ) continue;
      if(!(passPileUpJetID(jets_[i],applyPileUpJetID)) ) continue;
      if( std::fabs(jets_[i]->eta()) >= eta) continue;
      if( jetPt(jets_[i].get(),applyCorrection) <= minPt) continue;
      if(isThisJetALepton(jets_[i])) continue;
      if(++count > index) return jets_[i].get(); // return the pointer to the leading jet over the min pt, inside the eta acceptance (should be ordered in pT)
    }
    return new pat::Jet();
}

const pat::Jet* reco::SkimEvent::leadingPuppiJet(size_t index, float minPt,float eta,int applyCorrection,int applyJetID, int applyPileUpJetID) const {

    size_t count = 0;
    for(size_t i = 0; i < puppiJets_.size();++i) {
      if(!(passJetID(puppiJets_[i],applyJetID)) ) continue;
      if(!(passPileUpJetID(puppiJets_[i],applyPileUpJetID)) ) continue;
      if( std::fabs(puppiJets_[i]->eta()) >= eta) continue;
      if( jetPt(puppiJets_[i].get(),applyCorrection) <= minPt) continue;
      if(isThisJetALepton(puppiJets_[i])) continue;
      if(++count > index) return puppiJets_[i].get(); // return the pointer to the leading jet over the min pt, inside the eta acceptance (should be ordered in pT)
    }
    return new pat::Jet();
}


const float reco::SkimEvent::leadingJetBtag(size_t index, std::string discriminator, float minPt,float eta,int applyCorrection,int applyJetID, int applyPileUpJetID, float dzCut) const {

    size_t count = 0;
    for(size_t i=0;i<jets_.size();++i) { // loop on jets 
      if(!(passJetID(jets_[i],applyJetID)) ) continue;
      if(!(passPileUpJetID(jets_[i],applyPileUpJetID)) ) continue;
      if( std::fabs(jets_[i]->eta()) >= eta) continue;
      if( jetPt(jets_[i].get(),applyCorrection) <= minPt) continue;
      if(isThisJetALepton(jets_[i])) continue;
      if(jets_[i]->hasUserFloat("dz")){
	if(fabs(jets_[i]->userFloat("dz")) > dzCut) continue;
      }
      if(++count > index) return jets_[i]->bDiscriminator(discriminator); // take the discriminator value
    }
    return -9999.9;
}

const float reco::SkimEvent::leadingPuppiJetBtag(size_t index, std::string discriminator, float minPt,float eta,int applyCorrection,int applyJetID, int applyPileUpJetID, float dzCut) const {

    size_t count = 0;

    for(size_t i=0;i<puppiJets_.size();++i) { // loop on jets 
      if(!(passJetID(puppiJets_[i],applyJetID)) ) continue;
      if(!(passPileUpJetID(puppiJets_[i],applyPileUpJetID)) ) continue;
      if( std::fabs(puppiJets_[i]->eta()) >= eta) continue;
      if( jetPt(puppiJets_[i].get(),applyCorrection) <= minPt) continue;
      if(isThisJetALepton(puppiJets_[i])) continue;
      if(puppiJets_[i]->hasUserFloat("dz")){
	if(fabs(puppiJets_[i]->userFloat("dz")) > dzCut) continue;
      }
      if(++count > index) return puppiJets_[i]->bDiscriminator(discriminator); // take the discriminator value
    }
    return -9999.9;
}


const float reco::SkimEvent::leadingJetMass(size_t index, float minPt,float eta,int applyCorrection,int applyJetID, int applyPileUpJetID) const {

 size_t count = 0;
 for(size_t i=0;i<jets_.size();++i) {
  if(!(passJetID(jets_[i],applyJetID)) ) continue;
  if(!(passPileUpJetID(jets_[i],applyPileUpJetID)) ) continue;
  if( std::fabs(jets_[i]->eta()) >= eta) continue;
  if( jetPt(jets_[i].get(),applyCorrection) <= minPt) continue;

  if(isThisJetALepton(jets_[i])) continue;
  if(++count > index) return jets_[i]->mass();
 }
 return -9999.9;

}

const float reco::SkimEvent::leadingPuppiJetMass(size_t index, float minPt,float eta,int applyCorrection,int applyJetID, int applyPileUpJetID) const {

 size_t count = 0;
 for(size_t i=0;i<puppiJets_.size();++i) {
  if(!(passJetID(puppiJets_[i],applyJetID)) ) continue;
  if(!(passPileUpJetID(puppiJets_[i],applyPileUpJetID)) ) continue;
  if( std::fabs(puppiJets_[i]->eta()) >= eta) continue;
  if( jetPt(puppiJets_[i].get(),applyCorrection) <= minPt) continue;

  if(isThisJetALepton(puppiJets_[i])) continue;
  if(++count > index) return puppiJets_[i]->mass();
 }
 return -9999.9;

}


const float reco::SkimEvent::leadingJetPhi(size_t index, float minPt,float eta,int applyCorrection,int applyJetID, int applyPileUpJetID) const {

    size_t count = 0;
    for(size_t i=0;i<jets_.size();++i) {
      if(!(passJetID(jets_[i],applyJetID)) ) continue;
      if(!(passPileUpJetID(jets_[i],applyPileUpJetID)) ) continue;
      if( std::fabs(jets_[i]->eta()) >= eta) continue;
      if( jetPt(jets_[i].get(),applyCorrection) <= minPt) continue;

      if(isThisJetALepton(jets_[i])) continue;
      if(++count > index) return jets_[i]->phi();
    }
    return -9999.9;

}

const float reco::SkimEvent::leadingPuppiJetPhi(size_t index, float minPt,float eta,int applyCorrection,int applyJetID, int applyPileUpJetID) const {

    size_t count = 0;
    for(size_t i=0;i<puppiJets_.size();++i) {
      if(!(passJetID(puppiJets_[i],applyJetID)) ) continue;
      if(!(passPileUpJetID(puppiJets_[i],applyPileUpJetID)) ) continue;
      if( std::fabs(puppiJets_[i]->eta()) >= eta) continue;
      if( jetPt(puppiJets_[i].get(),applyCorrection) <= minPt) continue;

      if(isThisJetALepton(puppiJets_[i])) continue;
      if(++count > index) return puppiJets_[i]->phi();
    }
    return -9999.9;

}

const float reco::SkimEvent::leadingJetEta(size_t index, float minPt,float eta,int applyCorrection,int applyJetID, int applyPileUpJetID) const {

    size_t count = 0;
    for(size_t i=0;i<jets_.size();++i) {
      if(!(passJetID(jets_[i],applyJetID)) ) continue;
      if(!(passPileUpJetID(jets_[i],applyPileUpJetID)) ) continue;
      if( std::fabs(jets_[i]->eta()) >= eta) continue;
      if( jetPt(jets_[i].get(),applyCorrection) <= minPt) continue;

      if(isThisJetALepton(jets_[i])) continue;
      if(++count > index) return jets_[i]->eta();
    }
    return -9999.9;

}

const float reco::SkimEvent::leadingPuppiJetEta(size_t index, float minPt,float eta,int applyCorrection,int applyJetID, int applyPileUpJetID) const {

    size_t count = 0;
    for(size_t i=0;i<puppiJets_.size();++i) {
      if(!(passJetID(puppiJets_[i],applyJetID)) ) continue;
      if(!(passPileUpJetID(puppiJets_[i],applyPileUpJetID)) ) continue;
      if( std::fabs(puppiJets_[i]->eta()) >= eta) continue;
      if( jetPt(puppiJets_[i].get(),applyCorrection) <= minPt) continue;
      if(isThisJetALepton(puppiJets_[i])) continue;
      if(++count > index) return puppiJets_[i]->eta();
    }
    return -9999.9;

}

const int reco::SkimEvent::leadingJetId(size_t index, float minPt,float eta,int applyCorrection,int applyJetID, int applyPileUpJetID) const {

    size_t count = 0;
    for(size_t i=0;i<jets_.size();++i) {
      if(!(passJetID(jets_[i],applyJetID)) ) continue;
      if(!(passPileUpJetID(jets_[i],applyPileUpJetID)) ) continue;
      if( std::fabs(jets_[i]->eta()) >= eta) continue;
      if( jetPt(jets_[i].get(),applyCorrection) <= minPt) continue;

      if(isThisJetALepton(jets_[i])) continue;
      if(++count > index and jets_[i]->hasUserInt("jetId")) return jets_[i]->userInt("jetId");
    }
    return -9999.9;
}

const int reco::SkimEvent::leadingPuppiJetId(size_t index, float minPt,float eta,int applyCorrection,int applyJetID, int applyPileUpJetID) const {

    size_t count = 0;
    for(size_t i=0;i<puppiJets_.size();++i) {       
      if(!(passJetID(puppiJets_[i],applyJetID)) ) continue;
      if(!(passPileUpJetID(puppiJets_[i],applyPileUpJetID)) ) continue;
      if( std::fabs(puppiJets_[i]->eta()) >= eta) continue;
      if( jetPt(puppiJets_[i].get(),applyCorrection) <= minPt) continue;

      if(isThisJetALepton(puppiJets_[i])) continue;
      if(++count > index and puppiJets_[i]->hasUserInt("jetId")) return puppiJets_[i]->userInt("jetId");
    }
    return -9999.9;
}


const float reco::SkimEvent::leadingPileUpJetIdValue(size_t index, float minPt,float eta,int applyCorrection,int applyJetID, int applyPileUpJetID) const {

    size_t count = 0;
    for(size_t i=0;i<jets_.size();++i) {
      if(!(passJetID(jets_[i],applyJetID)) ) continue;
      if(!(passPileUpJetID(jets_[i],applyPileUpJetID)) ) continue;
      if( std::fabs(jets_[i]->eta()) >= eta) continue;
      if( jetPt(jets_[i].get(),applyCorrection) <= minPt) continue;

      if(isThisJetALepton(jets_[i])) continue;
      if(++count > index and jets_[i]->hasUserFloat("pileupJetId:fullDiscriminant")) return jets_[i]->userFloat("pileupJetId:fullDiscriminant");
    }
    return -9999.9;
}

const int reco::SkimEvent::leadingPileUpJetIdFlag(size_t index, float minPt,float eta,int applyCorrection,int applyJetID, int applyPileUpJetID) const {

    size_t count = 0;
    for(size_t i=0;i<jets_.size();++i) {
      if(!(passJetID(jets_[i],applyJetID)) ) continue;
      if(!(passPileUpJetID(jets_[i],applyPileUpJetID)) ) continue;
      if( std::fabs(jets_[i]->eta()) >= eta) continue;
      if( jetPt(jets_[i].get(),applyCorrection) <= minPt) continue;

      if(isThisJetALepton(jets_[i])) continue;
      if(++count > index and jets_[i]->hasUserInt("pileupJetId:fullId")) return jets_[i]->userInt("pileupJetId:fullId");
    }
    return -9999.9;
}

const float reco::SkimEvent::leadingPuppiPileUpJetIdValue(size_t index, float minPt,float eta,int applyCorrection,int applyJetID, int applyPileUpJetID) const {

    size_t count = 0;
    for(size_t i=0;i<puppiJets_.size();++i) {
      if(!(passJetID(puppiJets_[i],applyJetID)) ) continue;
      if(!(passPileUpJetID(puppiJets_[i],applyPileUpJetID)) ) continue;
      if( std::fabs(puppiJets_[i]->eta()) >= eta) continue;
      if( jetPt(puppiJets_[i].get(),applyCorrection) <= minPt) continue;

      if(isThisJetALepton(puppiJets_[i])) continue;
      if(++count > index and puppiJets_[i]->hasUserFloat("AK5pileupJetIdPuppi:fullDiscriminant")) return puppiJets_[i]->userFloat("AK5pileupJetIdPuppi:fullDiscriminant");
    }
    return -9999.9;
}

const int reco::SkimEvent::leadingPuppiPileUpJetIdFlag(size_t index, float minPt,float eta,int applyCorrection,int applyJetID, int applyPileUpJetID) const {

    size_t count = 0;
    for(size_t i=0;i<puppiJets_.size();++i) {
      if(!(passJetID(puppiJets_[i],applyJetID)) ) continue;
      if(!(passPileUpJetID(puppiJets_[i],applyPileUpJetID)) ) continue;
      if( std::fabs(puppiJets_[i]->eta()) >= eta) continue;
      if( jetPt(puppiJets_[i].get(),applyCorrection) <= minPt) continue;

      if(isThisJetALepton(puppiJets_[i])) continue;
      if(++count > index and puppiJets_[i]->hasUserInt("AK5pileupJetIdPuppi:fullId")) return puppiJets_[i]->userInt("AK5pileupJetIdPuppi:fullId");
    }
    return -9999.9;
}

const float reco::SkimEvent::leadingJetMva(size_t index, float minPt,float eta,int applyCorrection,int applyJetID, int applyPileUpJetID) const {

    size_t count = 0;
    for(size_t i=0;i<jets_.size();++i) {
      if(!(passJetID(jets_[i],applyJetID)) ) continue;
      if(!(passPileUpJetID(jets_[i],applyPileUpJetID)) ) continue;
      if( std::fabs(jets_[i]->eta()) >= eta) continue;
      if( jetPt(jets_[i].get(),applyCorrection) <= minPt) continue;
      if(isThisJetALepton(jets_[i])) continue;
      if(++count > index and jets_[i]->hasUserFloat("jetMva") ) return jets_[i]->userFloat("jetMva");
    }
    return -9999.9;
}

const float reco::SkimEvent::leadingPuppiJetMva(size_t index, float minPt,float eta,int applyCorrection,int applyJetID, int applyPileUpJetID) const {

    size_t count = 0;
    for(size_t i=0;i<puppiJets_.size();++i) {
      if(!(passJetID(puppiJets_[i],applyJetID)) ) continue;
      if(!(passPileUpJetID(puppiJets_[i],applyPileUpJetID)) ) continue;
      if( std::fabs(puppiJets_[i]->eta()) >= eta) continue;
      if( jetPt(puppiJets_[i].get(),applyCorrection) <= minPt) continue;

      if(isThisJetALepton(puppiJets_[i])) continue;
      if(++count > index and puppiJets_[i]->hasUserFloat("jetMva") ) return puppiJets_[i]->userFloat("jetMva");
    }
    return -9999.9;
}


const float reco::SkimEvent::leadingJetPt(size_t index, float minPt,float eta,int applyCorrection,int applyJetID, int applyPileUpJetID) const {

    size_t count = 0;
    for(size_t i=0;i<jets_.size();++i) {
      if(!(passJetID(jets_[i],applyJetID)) ) continue;
      if(!(passPileUpJetID(jets_[i],applyPileUpJetID)) ) continue;
      if( std::fabs(jets_[i]->eta()) >= eta) continue;
      if( jetPt(jets_[i].get(),applyCorrection) <= minPt) continue;

      if(isThisJetALepton(jets_[i])) continue;
      if(++count > index) return jetPt(jets_[i].get(),applyCorrection);
    }
    return -9999.9;
}

const float reco::SkimEvent::leadingPuppiJetPt(size_t index, float minPt,float eta,int applyCorrection,int applyJetID, int applyPileUpJetID) const {

    size_t count = 0;
    for(size_t i=0;i<puppiJets_.size();++i) {
      if(!(passJetID(puppiJets_[i],applyJetID)) ) continue;
      if(!(passPileUpJetID(puppiJets_[i],applyPileUpJetID)) ) continue;
      if( std::fabs(puppiJets_[i]->eta()) >= eta) continue;
      if( jetPt(puppiJets_[i].get(),applyCorrection) <= minPt) continue;

      if(isThisJetALepton(puppiJets_[i])) continue;
      if(++count > index) return jetPt(puppiJets_[i].get(),applyCorrection);
    }
    return -9999.9;
}


const float reco::SkimEvent::leadingJetPtd(size_t index, float minPt,float eta,int applyCorrection,int applyJetID, int applyPileUpJetID) const {

 size_t count = 0;
 for(size_t i=0;i<jets_.size();++i) {
  if(!(passJetID(jets_[i],applyJetID)) ) continue;
  if(!(passPileUpJetID(jets_[i],applyPileUpJetID)) ) continue;
  if( std::fabs(jets_[i]->eta()) >= eta) continue;
  if( jetPt(jets_[i].get(),applyCorrection) <= minPt) continue;

  if(isThisJetALepton(jets_[i])) continue;
  if(++count > index and jets_[i]->hasUserFloat("ptd")) {
   return jets_[i]->userFloat("ptd");
  }
 }
 return -9999.9;
}

const float reco::SkimEvent::leadingPuppiJetPtd(size_t index, float minPt,float eta,int applyCorrection,int applyJetID, int applyPileUpJetID) const {

 size_t count = 0;
 for(size_t i=0;i<puppiJets_.size();++i) {
  if(!(passJetID(puppiJets_[i],applyJetID)) ) continue;
  if(!(passPileUpJetID(puppiJets_[i],applyPileUpJetID)) ) continue;
  if( std::fabs(puppiJets_[i]->eta()) >= eta) continue;
  if( jetPt(puppiJets_[i].get(),applyCorrection) <= minPt) continue;

  if(isThisJetALepton(puppiJets_[i])) continue;
  if(++count > index and puppiJets_[i]->hasUserFloat("ptd")) {
   return puppiJets_[i]->userFloat("ptd");
  }
 }
 return -9999.9;
}


const float reco::SkimEvent::leadingPuppiJetNChgQC(size_t index, float minPt,float eta,int applyCorrection,int applyJetID, int applyPileUpJetID) const {

 size_t count = 0;
 for(size_t i=0;i<puppiJets_.size();++i) {
  if(!(passJetID(puppiJets_[i],applyJetID)) ) continue;
  if(!(passPileUpJetID(puppiJets_[i],applyPileUpJetID)) ) continue;
  if( std::fabs(puppiJets_[i]->eta()) >= eta) continue;
  if( jetPt(puppiJets_[i].get(),applyCorrection) <= minPt) continue;

  if(isThisJetALepton(puppiJets_[i])) continue;
  if(++count > index and puppiJets_[i]->hasUserFloat("nChgQC")) {
   return puppiJets_[i]->userFloat("nChgQC");
  }
 }
 return -9999.9;

}

const float reco::SkimEvent::leadingJetNChgQC(size_t index, float minPt,float eta,int applyCorrection,int applyJetID, int applyPileUpJetID) const {

 size_t count = 0;
 for(size_t i=0;i<jets_.size();++i) {
  if(!(passJetID(jets_[i],applyJetID)) ) continue;
  if(!(passPileUpJetID(jets_[i],applyPileUpJetID)) ) continue;
  if( std::fabs(jets_[i]->eta()) >= eta) continue;
  if( jetPt(jets_[i].get(),applyCorrection) <= minPt) continue;

  if(isThisJetALepton(jets_[i])) continue;
  if(++count > index and jets_[i]->hasUserFloat("nChgQC")) {
   return jets_[i]->userFloat("nChgQC");
  }
 }
 return -9999.9;

}


const float reco::SkimEvent::leadingPuppiJetNChgptCut(size_t index, float minPt,float eta,int applyCorrection,int applyJetID, int applyPileUpJetID) const {

 size_t count = 0;
 for(size_t i=0;i<puppiJets_.size();++i) {
  if(!(passJetID(puppiJets_[i],applyJetID)) ) continue;
  if(!(passPileUpJetID(puppiJets_[i],applyPileUpJetID)) ) continue;
  if( std::fabs(puppiJets_[i]->eta()) >= eta) continue;
  if( jetPt(puppiJets_[i].get(),applyCorrection) <= minPt) continue;

  if(isThisJetALepton(puppiJets_[i])) continue;
  if(++count > index and puppiJets_[i]->hasUserFloat("nChgptCut")) {
   return puppiJets_[i]->userFloat("nChgptCut");
  }
 }
 return -9999.9;

}

const float reco::SkimEvent::leadingJetNChgptCut(size_t index, float minPt,float eta,int applyCorrection,int applyJetID, int applyPileUpJetID) const {

 size_t count = 0;
 for(size_t i=0;i<jets_.size();++i) {
  if(!(passJetID(jets_[i],applyJetID)) ) continue;
  if(!(passPileUpJetID(jets_[i],applyPileUpJetID)) ) continue;
  if( std::fabs(jets_[i]->eta()) >= eta) continue;
  if( jetPt(jets_[i].get(),applyCorrection) <= minPt) continue;

  if(isThisJetALepton(jets_[i])) continue;
  if(++count > index and jets_[i]->hasUserFloat("nChgptCut")) {
   return jets_[i]->userFloat("nChgptCut");
  }
 }
 return -9999.9;

}


const float reco::SkimEvent::leadingJetNNeutralptCut(size_t index, float minPt,float eta,int applyCorrection,int applyJetID, int applyPileUpJetID) const {

 size_t count = 0;
 for(size_t i=0;i<jets_.size();++i) {
  if(!(passJetID(jets_[i],applyJetID)) ) continue;
  if(!(passPileUpJetID(jets_[i],applyPileUpJetID)) ) continue;
  if( std::fabs(jets_[i]->eta()) >= eta) continue;
  if( jetPt(jets_[i].get(),applyCorrection) <= minPt) continue;

  if(isThisJetALepton(jets_[i])) continue;
  if(++count > index and jets_[i]->hasUserFloat("nNeutralptCut")) {
   return jets_[i]->userFloat("nNeutralptCut");
  }
 }
 return -9999.9;

}

const float reco::SkimEvent::leadingPuppiJetNNeutralptCut(size_t index, float minPt,float eta,int applyCorrection,int applyJetID, int applyPileUpJetID) const {

 size_t count = 0;
 for(size_t i=0;i<puppiJets_.size();++i) {
  if(!(passJetID(puppiJets_[i],applyJetID)) ) continue;
  if(!(passPileUpJetID(puppiJets_[i],applyPileUpJetID)) ) continue;
  if( std::fabs(puppiJets_[i]->eta()) >= eta) continue;
  if( jetPt(puppiJets_[i].get(),applyCorrection) <= minPt) continue;

  if(isThisJetALepton(puppiJets_[i])) continue;
  if(++count > index and puppiJets_[i]->hasUserFloat("nNeutralptCut")) {
   return puppiJets_[i]->userFloat("nNeutralptCut");
  }
 }
 return -9999.9;

}


const float reco::SkimEvent::leadingPuppiJetPtD(size_t index, float minPt,float eta,int applyCorrection,int applyJetID, int applyPileUpJetID, int QualityCut) const {

 size_t count = 0;
 for(size_t i=0;i<puppiJets_.size();++i) {
  if(!(passJetID(puppiJets_[i],applyJetID)) ) continue;
  if(!(passPileUpJetID(puppiJets_[i],applyPileUpJetID)) ) continue;
  if( std::fabs(puppiJets_[i]->eta()) >= eta) continue;
  if( jetPt(puppiJets_[i].get(),applyCorrection) <= minPt) continue;

  if(isThisJetALepton(puppiJets_[i])) continue;
  if(++count > index) {
    if (QualityCut == 1 and puppiJets_[i]->hasUserFloat("QCptD")) return puppiJets_[i]->userFloat("QCptD");
    else if(puppiJets_[i]->hasUserFloat("ptD")) return puppiJets_[i]->userFloat("ptD");
  }
 }
 return -9999.9;

}

const float reco::SkimEvent::leadingJetPtD(size_t index, float minPt,float eta,int applyCorrection,int applyJetID, int applyPileUpJetID, int QualityCut) const {

 size_t count = 0;
 for(size_t i=0;i<jets_.size();++i) {
  if(!(passJetID(jets_[i],applyJetID)) ) continue;
  if(!(passPileUpJetID(jets_[i],applyPileUpJetID)) ) continue;
  if( std::fabs(jets_[i]->eta()) >= eta) continue;
  if( jetPt(jets_[i].get(),applyCorrection) <= minPt) continue;

  if(isThisJetALepton(jets_[i])) continue;
  if(++count > index) {
    if (QualityCut == 1 and jets_[i]->hasUserFloat("QCptD")) return jets_[i]->userFloat("QCptD");
    else if(jets_[i]->hasUserFloat("ptD")) return jets_[i]->userFloat("ptD");
  }
 }
 return -9999.9;

}


const float reco::SkimEvent::leadingJetQGRmax(size_t index, float minPt,float eta,int applyCorrection,int applyJetID, int applyPileUpJetID, int QualityCut) const {

 size_t count = 0;
 for(size_t i=0;i<jets_.size();++i) {
  if(!(passJetID(jets_[i],applyJetID)) ) continue;
  if(!(passPileUpJetID(jets_[i],applyPileUpJetID)) ) continue;
  if( std::fabs(jets_[i]->eta()) >= eta) continue;
  if( jetPt(jets_[i].get(),applyCorrection) <= minPt) continue;

  if(isThisJetALepton(jets_[i])) continue;
  if(++count > index) {
    if (QualityCut == 1 and jets_[i]->hasUserFloat("QCRmax")) return jets_[i]->userFloat("QCRmax");
    else if(jets_[i]->hasUserFloat("Rmax")) return jets_[i]->userFloat("Rmax");
  }
 }
 return -9999.9;

}

const float reco::SkimEvent::leadingPuppiJetQGRmax(size_t index, float minPt,float eta,int applyCorrection,int applyJetID, int applyPileUpJetID, int QualityCut) const {

 size_t count = 0;
 for(size_t i=0;i<puppiJets_.size();++i) {
  if(!(passJetID(puppiJets_[i],applyJetID)) ) continue;
  if(!(passPileUpJetID(puppiJets_[i],applyPileUpJetID)) ) continue;
  if( std::fabs(puppiJets_[i]->eta()) >= eta) continue;
  if( jetPt(puppiJets_[i].get(),applyCorrection) <= minPt) continue;

  if(isThisJetALepton(puppiJets_[i])) continue;
  if(++count > index) {
    if (QualityCut == 1 and puppiJets_[i]->hasUserFloat("QCRmax")) return puppiJets_[i]->userFloat("QCRmax");
    else if(puppiJets_[i]->hasUserFloat("Rmax")) return puppiJets_[i]->userFloat("Rmax");
  }
 }
 return -9999.9;

}


const float reco::SkimEvent::leadingJetQGRMScand(size_t index, float minPt,float eta,int applyCorrection,int applyJetID, int applyPileUpJetID, int QualityCut) const {

 size_t count = 0;
 for(size_t i=0;i<jets_.size();++i) {
  if(!(passJetID(jets_[i],applyJetID)) ) continue;
  if(!(passPileUpJetID(jets_[i],applyPileUpJetID)) ) continue;
  if( std::fabs(jets_[i]->eta()) >= eta) continue;
  if( jetPt(jets_[i].get(),applyCorrection) <= minPt) continue;

  if(isThisJetALepton(jets_[i])) continue;
  if(++count > index) {
    if (QualityCut == 1 and jets_[i]->hasUserFloat("QCRMScand")) return jets_[i]->userFloat("QCRMScand");
    else if(jets_[i]->hasUserFloat("RMScand")) return jets_[i]->userFloat("RMScand");
  }
 }
 return -9999.9;

}

const float reco::SkimEvent::leadingPuppiJetQGRMScand(size_t index, float minPt,float eta,int applyCorrection,int applyJetID, int applyPileUpJetID, int QualityCut) const {

 size_t count = 0;
 for(size_t i=0;i<puppiJets_.size();++i) {
  if(!(passJetID(puppiJets_[i],applyJetID)) ) continue;
  if(!(passPileUpJetID(puppiJets_[i],applyPileUpJetID)) ) continue;
  if( std::fabs(puppiJets_[i]->eta()) >= eta) continue;
  if( jetPt(puppiJets_[i].get(),applyCorrection) <= minPt) continue;

  if(isThisJetALepton(puppiJets_[i])) continue;
  if(++count > index) {
    if (QualityCut == 1 and puppiJets_[i]->hasUserFloat("QCRMScand")) return puppiJets_[i]->userFloat("QCRMScand");
    else if(puppiJets_[i]->hasUserFloat("RMScand")) return puppiJets_[i]->userFloat("RMScand");
  }
 }
 return -9999.9;

}


const float reco::SkimEvent::leadingJetQGaxis1(size_t index, float minPt,float eta,int applyCorrection,int applyJetID, int applyPileUpJetID, int QualityCut) const {

 size_t count = 0;
 for(size_t i=0;i<jets_.size();++i) {
  if(!(passJetID(jets_[i],applyJetID)) ) continue;
  if(!(passPileUpJetID(jets_[i],applyPileUpJetID)) ) continue;
  if( std::fabs(jets_[i]->eta()) >= eta) continue;
  if( jetPt(jets_[i].get(),applyCorrection) <= minPt) continue;

  if(isThisJetALepton(jets_[i])) continue;
  if(++count > index) {
    if (QualityCut == 1 and jets_[i]->hasUserFloat("QCaxis1"))   return jets_[i]->userFloat("QCaxis1");
    else if(QualityCut == 1 and jets_[i]->hasUserFloat("axis1")) return jets_[i]->userFloat("axis1");
  }
 }
 return -9999.9;

}

const float reco::SkimEvent::leadingPuppiJetQGaxis1(size_t index, float minPt,float eta,int applyCorrection,int applyJetID, int applyPileUpJetID, int QualityCut) const {

 size_t count = 0;
 for(size_t i=0;i<puppiJets_.size();++i) {
  if(!(passJetID(puppiJets_[i],applyJetID)) ) continue;
  if(!(passPileUpJetID(puppiJets_[i],applyPileUpJetID)) ) continue;
  if( std::fabs(puppiJets_[i]->eta()) >= eta) continue;
  if( jetPt(puppiJets_[i].get(),applyCorrection) <= minPt) continue;

  if(isThisJetALepton(puppiJets_[i])) continue;
  if(++count > index) {
    if (QualityCut == 1 and puppiJets_[i]->hasUserFloat("QCaxis1"))   return puppiJets_[i]->userFloat("QCaxis1");
    else if(QualityCut == 1 and puppiJets_[i]->hasUserFloat("axis1")) return puppiJets_[i]->userFloat("axis1");
  }
 }
 return -9999.9;

}


const float reco::SkimEvent::leadingJetQGaxis2(size_t index, float minPt,float eta,int applyCorrection,int applyJetID, int applyPileUpJetID, int QualityCut) const {

 size_t count = 0;
 for(size_t i=0;i<jets_.size();++i) {
  if(!(passJetID(jets_[i],applyJetID)) ) continue;
  if(!(passPileUpJetID(jets_[i],applyPileUpJetID)) ) continue;
  if( std::fabs(jets_[i]->eta()) >= eta) continue;
  if( jetPt(jets_[i].get(),applyCorrection) <= minPt) continue;

  if(isThisJetALepton(jets_[i])) continue;
  if(++count > index) {
    if (QualityCut == 1 and jets_[i]->hasUserFloat("QCaxis2")) return jets_[i]->userFloat("QCaxis2");
    else if(jets_[i]->userFloat("axis2")) return jets_[i]->userFloat("axis2");
  }
 }
 return -9999.9;

}

const float reco::SkimEvent::leadingPuppiJetQGaxis2(size_t index, float minPt,float eta,int applyCorrection,int applyJetID, int applyPileUpJetID, int QualityCut) const {

 size_t count = 0;
 for(size_t i=0;i<puppiJets_.size();++i) {
  if(!(passJetID(puppiJets_[i],applyJetID)) ) continue;
  if(!(passPileUpJetID(puppiJets_[i],applyPileUpJetID)) ) continue;
  if( std::fabs(puppiJets_[i]->eta()) >= eta) continue;
  if( jetPt(puppiJets_[i].get(),applyCorrection) <= minPt) continue;

  if(isThisJetALepton(puppiJets_[i])) continue;
  if(++count > index) {
    if (QualityCut == 1 and puppiJets_[i]->hasUserFloat("QCaxis2")) return puppiJets_[i]->userFloat("QCaxis2");
    else if(puppiJets_[i]->userFloat("axis2")) return puppiJets_[i]->userFloat("axis2");
  }
 }
 return -9999.9;

}


const float reco::SkimEvent::leadingJetChargedHadronMultiplicity(size_t index, float minPt,float eta,int applyCorrection,int applyJetID, int applyPileUpJetID) const {

 size_t count = 0;
 for(size_t i=0;i<jets_.size();++i) {
  if(!(passJetID(jets_[i],applyJetID)) ) continue;
  if(!(passPileUpJetID(jets_[i],applyPileUpJetID)) ) continue;
  if( std::fabs(jets_[i]->eta()) >= eta) continue;
  if( jetPt(jets_[i].get(),applyCorrection) <= minPt) continue;

  if(isThisJetALepton(jets_[i])) continue;
  if(++count > index) return jets_[i]->chargedHadronMultiplicity();
 }
 return -9999.9;

}

const float reco::SkimEvent::leadingPuppiJetChargedHadronMultiplicity(size_t index, float minPt,float eta,int applyCorrection,int applyJetID, int applyPileUpJetID) const {

 size_t count = 0;
 for(size_t i=0;i<puppiJets_.size();++i) {
  if(!(passJetID(puppiJets_[i],applyJetID)) ) continue;
  if(!(passPileUpJetID(puppiJets_[i],applyPileUpJetID)) ) continue;
  if( std::fabs(puppiJets_[i]->eta()) >= eta) continue;
  if( jetPt(puppiJets_[i].get(),applyCorrection) <= minPt) continue;

  if(isThisJetALepton(puppiJets_[i])) continue;
  if(++count > index) return puppiJets_[i]->chargedHadronMultiplicity();
 }
 return -9999.9;

}

const float reco::SkimEvent::leadingPuppiJetNeutralHadronMultiplicity(size_t index, float minPt,float eta,int applyCorrection,int applyJetID, int applyPileUpJetID) const {

 size_t count = 0;
 for(size_t i=0;i<puppiJets_.size();++i) {
  if(!(passJetID(puppiJets_[i],applyJetID)) ) continue;
  if(!(passPileUpJetID(puppiJets_[i],applyPileUpJetID)) ) continue;
  if( std::fabs(puppiJets_[i]->eta()) >= eta) continue;
  if( jetPt(puppiJets_[i].get(),applyCorrection) <= minPt) continue;

  if(isThisJetALepton(puppiJets_[i])) continue;
  if(++count > index) return puppiJets_[i]->neutralHadronMultiplicity();
 }
 return -9999.9;

}

const float reco::SkimEvent::leadingJetNeutralHadronMultiplicity(size_t index, float minPt,float eta,int applyCorrection,int applyJetID, int applyPileUpJetID) const {

 size_t count = 0;
 for(size_t i=0;i<jets_.size();++i) {
  if(!(passJetID(jets_[i],applyJetID)) ) continue;
  if(!(passPileUpJetID(jets_[i],applyPileUpJetID)) ) continue;
  if( std::fabs(jets_[i]->eta()) >= eta) continue;
  if( jetPt(jets_[i].get(),applyCorrection) <= minPt) continue;

  if(isThisJetALepton(jets_[i])) continue;
  if(++count > index) return jets_[i]->neutralHadronMultiplicity();
 }
 return -9999.9;

}

const float reco::SkimEvent::leadingJetPhotonMultiplicity(size_t index, float minPt,float eta,int applyCorrection,int applyJetID, int applyPileUpJetID) const {

 size_t count = 0;
 for(size_t i=0;i<jets_.size();++i) {
  if(!(passJetID(jets_[i],applyJetID)) ) continue;
  if(!(passPileUpJetID(jets_[i],applyPileUpJetID)) ) continue;
  if( std::fabs(jets_[i]->eta()) >= eta) continue;
  if( jetPt(jets_[i].get(),applyCorrection) <= minPt) continue;

  if(isThisJetALepton(jets_[i])) continue;
  if(++count > index) return jets_[i]->photonMultiplicity();
 }
 return -9999.9;

}

const float reco::SkimEvent::leadingPuppiJetPhotonMultiplicity(size_t index, float minPt,float eta,int applyCorrection,int applyJetID, int applyPileUpJetID) const {

 size_t count = 0;
 for(size_t i=0;i<puppiJets_.size();++i) {
  if(!(passJetID(puppiJets_[i],applyJetID)) ) continue;
  if(!(passPileUpJetID(puppiJets_[i],applyPileUpJetID)) ) continue;
  if( std::fabs(puppiJets_[i]->eta()) >= eta) continue;
  if( jetPt(puppiJets_[i].get(),applyCorrection) <= minPt) continue;

  if(isThisJetALepton(puppiJets_[i])) continue;
  if(++count > index) return puppiJets_[i]->photonMultiplicity();
 }
 return -9999.9;

}


// set and get Rho for Jet
void reco::SkimEvent::setJetRhoIso(const edm::Handle<double> & h) {
 rhoJetIso_ = (double) (*h);
}

const float reco::SkimEvent::getJetRhoIso() const {
 return rhoJetIso_;
}


const float reco::SkimEvent::leadingVBFJetPhi(size_t index, float minPt,float eta,int applyCorrection,int applyJetID, int applyPileUpJetID) const { // two leading jet, not from PU
    if(jets_.size() < 2) return -9999.;
    float jetEta1 = leadingJetEta(0,minPt,eta,applyCorrection,applyJetID,applyPileUpJetID);
    float jetEta2 = leadingJetEta(1,minPt,eta,applyCorrection,applyJetID,applyPileUpJetID);
    float jetEtaMax = std::max(jetEta1,jetEta2);
    float jetEtaMin = std::min(jetEta1,jetEta2);

    size_t count = 0;
    for(size_t i=0;i<jets_.size();++i) {
      if(!(passJetID(jets_[i],applyJetID)) ) continue;
      if(!(passPileUpJetID(jets_[i],applyPileUpJetID)) ) continue;
      if( std::fabs(jets_[i]->eta()) >= eta) continue;
      if( jetPt(jets_[i].get(),applyCorrection) <= minPt) continue;

      if(isThisJetALepton(jets_[i])) continue;
      if( jets_[i]->eta() >= jetEtaMax ) continue;
      if( jets_[i]->eta() <= jetEtaMin ) continue;
      if(++count > index) return jets_[i]->phi();
    }
    return -9999.9;
}

const float reco::SkimEvent::leadingVBFPuppiJetPhi(size_t index, float minPt,float eta,int applyCorrection,int applyJetID, int applyPileUpJetID) const { // two leading jet, not from PU
    if(puppiJets_.size() < 2) return -9999.;
    float jetEta1 = leadingPuppiJetEta(0,minPt,eta,applyCorrection,applyJetID,applyPileUpJetID);
    float jetEta2 = leadingPuppiJetEta(1,minPt,eta,applyCorrection,applyJetID,applyPileUpJetID);
    float jetEtaMax = std::max(jetEta1,jetEta2);
    float jetEtaMin = std::min(jetEta1,jetEta2);

    size_t count = 0;
    for(size_t i=0;i<puppiJets_.size();++i) {
      if(!(passJetID(puppiJets_[i],applyJetID)) ) continue;
      if(!(passPileUpJetID(puppiJets_[i],applyPileUpJetID)) ) continue;
      if( std::fabs(puppiJets_[i]->eta()) >= eta) continue;
      if( jetPt(puppiJets_[i].get(),applyCorrection) <= minPt) continue;

      if(isThisJetALepton(puppiJets_[i])) continue;
      if( puppiJets_[i]->eta() >= jetEtaMax ) continue;
      if( puppiJets_[i]->eta() <= jetEtaMin ) continue;
      if(++count > index) return puppiJets_[i]->phi();
    }
    return -9999.9;
}

const float reco::SkimEvent::leadingVBFJetEta(size_t index, float minPt,float eta,int applyCorrection,int applyJetID, int applyPileUpJetID) const {
    if(jets_.size() < 2) return -9999.;
    float jetEta1 = leadingJetEta(0,minPt,eta,applyCorrection,applyJetID,applyPileUpJetID);
    float jetEta2 = leadingJetEta(1,minPt,eta,applyCorrection,applyJetID,applyPileUpJetID);
    float jetEtaMax = std::max(jetEta1,jetEta2);
    float jetEtaMin = std::min(jetEta1,jetEta2);

    size_t count = 0;
    for(size_t i=0;i<jets_.size();++i) {
      if(!(passJetID(jets_[i],applyJetID)) ) continue;
      if(!(passPileUpJetID(jets_[i],applyPileUpJetID)) ) continue;
      if( std::fabs(jets_[i]->eta()) >= eta) continue;
      if( jetPt(jets_[i].get(),applyCorrection) <= minPt) continue;

      if(isThisJetALepton(jets_[i])) continue;
      if( jets_[i]->eta() >= jetEtaMax ) continue;
      if( jets_[i]->eta() <= jetEtaMin ) continue;
      if(++count > index) return jets_[i]->eta();
    }
    return -9999.9;

}

const float reco::SkimEvent::leadingVBFPuppiJetEta(size_t index, float minPt,float eta,int applyCorrection,int applyJetID, int applyPileUpJetID) const {
    if(puppiJets_.size() < 2) return -9999.;
    float jetEta1 = leadingPuppiJetEta(0,minPt,eta,applyCorrection,applyJetID,applyPileUpJetID);
    float jetEta2 = leadingPuppiJetEta(1,minPt,eta,applyCorrection,applyJetID,applyPileUpJetID);
    float jetEtaMax = std::max(jetEta1,jetEta2);
    float jetEtaMin = std::min(jetEta1,jetEta2);

    size_t count = 0;
    for(size_t i=0;i<puppiJets_.size();++i) {
      if(!(passJetID(puppiJets_[i],applyJetID)) ) continue;
      if(!(passPileUpJetID(puppiJets_[i],applyPileUpJetID)) ) continue;
      if( std::fabs(puppiJets_[i]->eta()) >= eta) continue;
      if( jetPt(puppiJets_[i].get(),applyCorrection) <= minPt) continue;

      if(isThisJetALepton(puppiJets_[i])) continue;
      if( puppiJets_[i]->eta() >= jetEtaMax ) continue;
      if( puppiJets_[i]->eta() <= jetEtaMin ) continue;
      if(++count > index) return puppiJets_[i]->eta();
    }
    return -9999.9;

}

const int reco::SkimEvent::leadingVBFJetId(size_t index, float minPt,float eta,int applyCorrection,int applyJetID, int applyPileUpJetID) const {
    if(jets_.size() < 2) return -9999.;
    float jetEta1 = leadingJetEta(0,minPt,eta,applyCorrection,applyJetID,applyPileUpJetID);
    float jetEta2 = leadingJetEta(1,minPt,eta,applyCorrection,applyJetID,applyPileUpJetID);
    float jetEtaMax = std::max(jetEta1,jetEta2);
    float jetEtaMin = std::min(jetEta1,jetEta2);

    size_t count = 0;
    for(size_t i=0;i<jets_.size();++i) {
      if(!(passJetID(jets_[i],applyJetID)) ) continue;
      if(!(passPileUpJetID(jets_[i],applyPileUpJetID)) ) continue;
      if( std::fabs(jets_[i]->eta()) >= eta) continue;
      if( jetPt(jets_[i].get(),applyCorrection) <= minPt) continue;

      if(isThisJetALepton(jets_[i])) continue;
      if( jets_[i]->eta() >= jetEtaMax ) continue;
      if( jets_[i]->eta() <= jetEtaMin ) continue;
      if(++count > index and jets_[i]->hasUserInt("jetId")) return jets_[i]->userInt("jetId");
    }
    return -9999.9;
}

const int reco::SkimEvent::leadingVBFPuppiJetId(size_t index, float minPt,float eta,int applyCorrection,int applyJetID, int applyPileUpJetID) const {
    if(puppiJets_.size() < 2) return -9999.;
    float jetEta1 = leadingPuppiJetEta(0,minPt,eta,applyCorrection,applyJetID,applyPileUpJetID);
    float jetEta2 = leadingPuppiJetEta(1,minPt,eta,applyCorrection,applyJetID,applyPileUpJetID);
    float jetEtaMax = std::max(jetEta1,jetEta2);
    float jetEtaMin = std::min(jetEta1,jetEta2);

    size_t count = 0;
    for(size_t i=0;i<puppiJets_.size();++i) {
      if(!(passJetID(puppiJets_[i],applyJetID)) ) continue;
      if(!(passPileUpJetID(puppiJets_[i],applyPileUpJetID)) ) continue;
      if( std::fabs(puppiJets_[i]->eta()) >= eta) continue;
      if( jetPt(puppiJets_[i].get(),applyCorrection) <= minPt) continue;

      if(isThisJetALepton(puppiJets_[i])) continue;
      if( puppiJets_[i]->eta() >= jetEtaMax ) continue;
      if( puppiJets_[i]->eta() <= jetEtaMin ) continue;
      if(++count > index and puppiJets_[i]->hasUserInt("jetId")) return puppiJets_[i]->userInt("jetId");
    }
    return -9999.9;
}


const float reco::SkimEvent::leadingVBFPileUpJetIdValue(size_t index, float minPt,float eta,int applyCorrection,int applyJetID, int applyPileUpJetID) const {
    if(jets_.size() < 2) return -9999.;
    float jetEta1 = leadingJetEta(0,minPt,eta,applyCorrection,applyJetID,applyPileUpJetID);
    float jetEta2 = leadingJetEta(1,minPt,eta,applyCorrection,applyJetID,applyPileUpJetID);
    float jetEtaMax = std::max(jetEta1,jetEta2);
    float jetEtaMin = std::min(jetEta1,jetEta2);

    size_t count = 0;
    for(size_t i=0;i<jets_.size();++i) {
      if(!(passJetID(jets_[i],applyJetID)) ) continue;
      if(!(passPileUpJetID(jets_[i],applyPileUpJetID)) ) continue;
      if( std::fabs(jets_[i]->eta()) >= eta) continue;
      if( jetPt(jets_[i].get(),applyCorrection) <= minPt) continue;

      if(isThisJetALepton(jets_[i])) continue;
      if( jets_[i]->eta() >= jetEtaMax ) continue;
      if( jets_[i]->eta() <= jetEtaMin ) continue;
      if(++count > index and jets_[i]->hasUserFloat("pileupJetId:fullDiscriminant")) return jets_[i]->userFloat("pileupJetId:fullDiscriminant");
    }
    return -9999.9;
}


const int reco::SkimEvent::leadingVBFPileUpJetIdFlag(size_t index, float minPt,float eta,int applyCorrection,int applyJetID, int applyPileUpJetID) const {
    if(jets_.size() < 2) return -9999.;
    float jetEta1 = leadingJetEta(0,minPt,eta,applyCorrection,applyJetID,applyPileUpJetID);
    float jetEta2 = leadingJetEta(1,minPt,eta,applyCorrection,applyJetID,applyPileUpJetID);
    float jetEtaMax = std::max(jetEta1,jetEta2);
    float jetEtaMin = std::min(jetEta1,jetEta2);

    size_t count = 0;
    for(size_t i=0;i<jets_.size();++i) {
      if(!(passJetID(jets_[i],applyJetID)) ) continue;
      if(!(passPileUpJetID(jets_[i],applyPileUpJetID)) ) continue;
      if( std::fabs(jets_[i]->eta()) >= eta) continue;
      if( jetPt(jets_[i].get(),applyCorrection) <= minPt) continue;

      if(isThisJetALepton(jets_[i])) continue;
      if( jets_[i]->eta() >= jetEtaMax ) continue;
      if( jets_[i]->eta() <= jetEtaMin ) continue;
      if(++count > index and jets_[i]->hasUserInt("pileupJetId:fullId")) return jets_[i]->userInt("pileupJetId:fullId");
    }
    return -9999.9;
}

const float reco::SkimEvent::leadingVBFPuppiPileUpJetIdValue(size_t index, float minPt,float eta,int applyCorrection,int applyJetID, int applyPileUpJetID) const {
    if(puppiJets_.size() < 2) return -9999.;
    float jetEta1 = leadingPuppiJetEta(0,minPt,eta,applyCorrection,applyJetID,applyPileUpJetID);
    float jetEta2 = leadingPuppiJetEta(1,minPt,eta,applyCorrection,applyJetID,applyPileUpJetID);
    float jetEtaMax = std::max(jetEta1,jetEta2);
    float jetEtaMin = std::min(jetEta1,jetEta2);

    size_t count = 0;
    for(size_t i=0;i<puppiJets_.size();++i) {
      if(!(passJetID(puppiJets_[i],applyJetID)) ) continue;
      if(!(passPileUpJetID(puppiJets_[i],applyPileUpJetID)) ) continue;
      if( std::fabs(puppiJets_[i]->eta()) >= eta) continue;
      if( jetPt(puppiJets_[i].get(),applyCorrection) <= minPt) continue;

      if(isThisJetALepton(puppiJets_[i])) continue;
      if( puppiJets_[i]->eta() >= jetEtaMax ) continue;
      if( puppiJets_[i]->eta() <= jetEtaMin ) continue;
      if(++count > index and puppiJets_[i]->hasUserFloat("AK5pileupJetIdPuppi:fullDiscriminant")) return puppiJets_[i]->userFloat("AK5pileupJetIdPuppi:fullDiscriminant");
    }
    return -9999.9;
}

const int reco::SkimEvent::leadingVBFPuppiPileUpJetIdFlag(size_t index, float minPt,float eta,int applyCorrection,int applyJetID, int applyPileUpJetID) const {
    if(puppiJets_.size() < 2) return -9999.;
    float jetEta1 = leadingPuppiJetEta(0,minPt,eta,applyCorrection,applyJetID,applyPileUpJetID);
    float jetEta2 = leadingPuppiJetEta(1,minPt,eta,applyCorrection,applyJetID,applyPileUpJetID);
    float jetEtaMax = std::max(jetEta1,jetEta2);
    float jetEtaMin = std::min(jetEta1,jetEta2);

    size_t count = 0;
    for(size_t i=0;i<puppiJets_.size();++i) {
      if(!(passJetID(puppiJets_[i],applyJetID)) ) continue;
      if(!(passPileUpJetID(puppiJets_[i],applyPileUpJetID)) ) continue;
      if( std::fabs(puppiJets_[i]->eta()) >= eta) continue;
      if( jetPt(puppiJets_[i].get(),applyCorrection) <= minPt) continue;

      if(isThisJetALepton(puppiJets_[i])) continue;
      if( puppiJets_[i]->eta() >= jetEtaMax ) continue;
      if( puppiJets_[i]->eta() <= jetEtaMin ) continue;
      if(++count > index and puppiJets_[i]->hasUserInt("AK5pileupJetIdPuppi:fullId")) return puppiJets_[i]->userInt("AK5pileupJetIdPuppi:fullId");
    }
    return -9999.9;
}

const float reco::SkimEvent::leadingVBFJetMva(size_t index, float minPt,float eta,int applyCorrection,int applyJetID, int applyPileUpJetID) const {
    if(jets_.size() < 2) return -9999.;
    float jetEta1 = leadingJetEta(0,minPt,eta,applyCorrection,applyJetID,applyPileUpJetID);
    float jetEta2 = leadingJetEta(1,minPt,eta,applyCorrection,applyJetID,applyPileUpJetID);
    float jetEtaMax = std::max(jetEta1,jetEta2);
    float jetEtaMin = std::min(jetEta1,jetEta2);

    size_t count = 0;
    for(size_t i=0;i<jets_.size();++i) {
      if(!(passJetID(jets_[i],applyJetID)) ) continue;
      if(!(passPileUpJetID(jets_[i],applyPileUpJetID)) ) continue;
      if( std::fabs(jets_[i]->eta()) >= eta) continue;
      if( jetPt(jets_[i].get(),applyCorrection) <= minPt) continue;

      if(isThisJetALepton(jets_[i])) continue;
      if( jets_[i]->eta() >= jetEtaMax ) continue;
      if( jets_[i]->eta() <= jetEtaMin ) continue;
      if(++count > index and jets_[i]->hasUserInt("jetMva")) return jets_[i]->userInt("jetMva");
    }
    return -9999.9;
}


const float reco::SkimEvent::leadingVBFPuppiJetMva(size_t index, float minPt,float eta,int applyCorrection,int applyJetID, int applyPileUpJetID) const {
    if(puppiJets_.size() < 2) return -9999.;
    float jetEta1 = leadingPuppiJetEta(0,minPt,eta,applyCorrection,applyJetID,applyPileUpJetID);
    float jetEta2 = leadingPuppiJetEta(1,minPt,eta,applyCorrection,applyJetID,applyPileUpJetID);
    float jetEtaMax = std::max(jetEta1,jetEta2);
    float jetEtaMin = std::min(jetEta1,jetEta2);

    size_t count = 0;
    for(size_t i=0;i<puppiJets_.size();++i) {
      if(!(passJetID(puppiJets_[i],applyJetID)) ) continue;
      if(!(passPileUpJetID(puppiJets_[i],applyPileUpJetID)) ) continue;
      if( std::fabs(puppiJets_[i]->eta()) >= eta) continue;
      if( jetPt(puppiJets_[i].get(),applyCorrection) <= minPt) continue;

      if(isThisJetALepton(puppiJets_[i])) continue;
      if( puppiJets_[i]->eta() >= jetEtaMax ) continue;
      if( puppiJets_[i]->eta() <= jetEtaMin ) continue;
      if(++count > index and puppiJets_[i]->hasUserInt("jetMva")) return puppiJets_[i]->userInt("jetMva");
    }
    return -9999.9;
}

const float reco::SkimEvent::leadingVBFJetPt(size_t index, float minPt,float eta,int applyCorrection,int applyJetID, int applyPileUpJetID) const {
    if(jets_.size() < 2) return -9999.;
    float jetEta1 = leadingJetEta(0,minPt,eta,applyCorrection,applyJetID,applyPileUpJetID);
    float jetEta2 = leadingJetEta(1,minPt,eta,applyCorrection,applyJetID,applyPileUpJetID);
    float jetEtaMax = std::max(jetEta1,jetEta2);
    float jetEtaMin = std::min(jetEta1,jetEta2);

    size_t count = 0;
    for(size_t i=0;i<jets_.size();++i) {
      if(!(passJetID(jets_[i],applyJetID)) ) continue;
      if(!(passPileUpJetID(jets_[i],applyPileUpJetID)) ) continue;
      if( std::fabs(jets_[i]->eta()) >= eta) continue;
      if( jetPt(jets_[i].get(),applyCorrection) <= minPt) continue;

      if(isThisJetALepton(jets_[i])) continue;
      if( jets_[i]->eta() >= jetEtaMax ) continue;
      if( jets_[i]->eta() <= jetEtaMin ) continue;
      if(++count > index) return jetPt(jets_[i].get(),applyCorrection);
    }
    return -9999.9;
}

const float reco::SkimEvent::leadingVBFPuppiJetPt(size_t index, float minPt,float eta,int applyCorrection,int applyJetID, int applyPileUpJetID) const {
    if(puppiJets_.size() < 2) return -9999.;
    float jetEta1 = leadingPuppiJetEta(0,minPt,eta,applyCorrection,applyJetID,applyPileUpJetID);
    float jetEta2 = leadingPuppiJetEta(1,minPt,eta,applyCorrection,applyJetID,applyPileUpJetID);
    float jetEtaMax = std::max(jetEta1,jetEta2);
    float jetEtaMin = std::min(jetEta1,jetEta2);

    size_t count = 0;
    for(size_t i=0;i<puppiJets_.size();++i) {
      if(!(passJetID(puppiJets_[i],applyJetID)) ) continue;
      if(!(passPileUpJetID(puppiJets_[i],applyPileUpJetID)) ) continue;
      if( std::fabs(puppiJets_[i]->eta()) >= eta) continue;
      if( jetPt(puppiJets_[i].get(),applyCorrection) <= minPt) continue;

      if(isThisJetALepton(puppiJets_[i])) continue;
      if( puppiJets_[i]->eta() >= jetEtaMax ) continue;
      if( puppiJets_[i]->eta() <= jetEtaMin ) continue;
      if(++count > index) return jetPt(puppiJets_[i].get(),applyCorrection);
    }
    return -9999.9;
}

const int reco::SkimEvent::nCentralJets(float minPt,float eta,int applyCorrection,int applyJetID, int applyPileUpJetID) const {

    int count = 0;
    for(size_t i=0;i<jets_.size();++i) {    
      if(!(passJetID(jets_[i],applyJetID)) ) continue;
      if(!(passPileUpJetID(jets_[i],applyPileUpJetID)) ) continue;
      if( std::fabs(jets_[i]->eta()) >= eta) continue;
      if( jetPt(jets_[i].get(),applyCorrection) <= minPt) continue;

      if(isThisJetALepton(jets_[i])) continue;
      count++;
    }
    return count;
}

const int reco::SkimEvent::nCentralPuppiJets(float minPt,float eta,int applyCorrection,int applyJetID, int applyPileUpJetID) const {

    int count = 0;
    for(size_t i=0;i<puppiJets_.size();++i) {
      if(!(passJetID(puppiJets_[i],applyJetID)) ) continue;
      if(!(passPileUpJetID(puppiJets_[i],applyPileUpJetID)) ) continue;
      if( std::fabs(puppiJets_[i]->eta()) >= eta) continue;
      if( jetPt(puppiJets_[i].get(),applyCorrection) <= minPt) continue;

      if(isThisJetALepton(puppiJets_[i])) continue;
      count++;
    }
    return count;
}

const bool reco::SkimEvent::passesDPhillJet(float ptMin, float eta,int applyCorrection,int applyJetID, int applyPileUpJetID) const {
    if(leps_.size() < 2) return -9999.0;   
    float dphi = 0, ptMax = 0;
    for(size_t i=0;i<jets_.size();++i) {
      if(!(passJetID(jets_[i],applyJetID)) ) continue;
      if(!(passPileUpJetID(jets_[i],applyPileUpJetID)) ) continue;
      if( std::fabs(jets_[i]->eta()) >= eta) continue;

      if(isThisJetALepton(jets_[i])) continue;
      float pt = jetPt(jets_[i].get(),applyCorrection);
      if (pt > ptMax) {
        ptMax = pt;
        dphi = fabs(ROOT::Math::VectorUtil::DeltaPhi(leps_[indexLeptonByPt(0)]->p4()+leps_[indexLeptonByPt(1)]->p4(), jets_[i]->p4()) );
      }
    }
    return (ptMax <= ptMin || dphi / M_PI * 180. < 165.0);
}

const bool reco::SkimEvent::passesDPhillPuppiJet(float ptMin, float eta,int applyCorrection,int applyJetID, int applyPileUpJetID) const {
    if(leps_.size() < 2) return -9999.0;   
    float dphi = 0, ptMax = 0;
    for(size_t i=0;i<puppiJets_.size();++i) {
      if(!(passJetID(puppiJets_[i],applyJetID)) ) continue;
      if(!(passPileUpJetID(puppiJets_[i],applyPileUpJetID)) ) continue;
      if( std::fabs(puppiJets_[i]->eta()) >= eta) continue;

      if(isThisJetALepton(puppiJets_[i])) continue;
      float pt = jetPt(puppiJets_[i].get(),applyCorrection);
      if (pt > ptMax) {
        ptMax = pt;
        dphi = fabs(ROOT::Math::VectorUtil::DeltaPhi(leps_[indexLeptonByPt(0)]->p4()+leps_[indexLeptonByPt(1)]->p4(), puppiJets_[i]->p4()) );
      }
    }
    return (ptMax <= ptMin || dphi / M_PI * 180. < 165.0);
}

const float reco::SkimEvent::dPhillLeadingJet(float eta,int applyCorrection,int applyJetID, int applyPileUpJetID) const { // no min pt
    if(leps_.size() < 2) return -9999.0;
    float dphi = 0, ptMax = 0;
    for(size_t i=0;i<jets_.size();++i) {
      if(!(passJetID(jets_[i],applyJetID)) ) continue;
      if(!(passPileUpJetID(jets_[i],applyPileUpJetID)) ) continue;
      if( std::fabs(jets_[i]->eta()) >= eta) continue;

      if(isThisJetALepton(jets_[i])) continue;
      float pt = jetPt(jets_[i].get(),applyCorrection);
      if (pt > ptMax) {
        ptMax = pt;
        dphi = fabs(ROOT::Math::VectorUtil::DeltaPhi(leps_[indexLeptonByPt(0)]->p4()+leps_[indexLeptonByPt(1)]->p4(), jets_[i]->p4()) );
      }
    }
    return dphi;
}

const float reco::SkimEvent::dPhillLeadingPuppiJet(float eta,int applyCorrection,int applyJetID, int applyPileUpJetID) const { // no min pt
    if(leps_.size() < 2) return -9999.0;
    float dphi = 0, ptMax = 0;
    for(size_t i=0;i<puppiJets_.size();++i) {
      if(!(passJetID(puppiJets_[i],applyJetID)) ) continue;
      if(!(passPileUpJetID(puppiJets_[i],applyPileUpJetID)) ) continue;
      if( std::fabs(puppiJets_[i]->eta()) >= eta) continue;

      if(isThisJetALepton(puppiJets_[i])) continue;
      float pt = jetPt(puppiJets_[i].get(),applyCorrection);
      if (pt > ptMax) {
        ptMax = pt;
        dphi = fabs(ROOT::Math::VectorUtil::DeltaPhi(leps_[indexLeptonByPt(0)]->p4()+leps_[indexLeptonByPt(1)]->p4(), puppiJets_[i]->p4()) );
      }
    }
    return dphi;
}

 
const float reco::SkimEvent::dPhilljetjet(float eta,int applyCorrection,int applyJetID, int applyPileUpJetID) const {
    if(leps_.size() < 2) return -9999.0;
    float dphi = -1;
    int jet1 = indexJetByPt(0,0,eta,applyCorrection,applyJetID,applyPileUpJetID);
    int jet2 = indexJetByPt(1,0,eta,applyCorrection,applyJetID,applyPileUpJetID);

    if (jet1 != 9999 && jet2 != 9999 and jet1 < int(jets_.size()) and jet2 < int(jets_.size())) 
      dphi = fabs(ROOT::Math::VectorUtil::DeltaPhi(leps_[indexLeptonByPt(0)]->p4()+leps_[indexLeptonByPt(1)]->p4(), jets_[jet1]->p4()+jets_[jet2]->p4()) );

    return dphi;
}

const float reco::SkimEvent::dPhillPuppijetjet(float eta,int applyCorrection,int applyJetID, int applyPileUpJetID) const {
    if(leps_.size() < 2) return -9999.0;
    float dphi = -1;
    int jet1 = indexPuppiJetByPt(0,0,eta,applyCorrection,applyJetID,applyPileUpJetID);
    int jet2 = indexPuppiJetByPt(1,0,eta,applyCorrection,applyJetID,applyPileUpJetID);

    if (jet1 != 9999 && jet2 != 9999 and jet1 < int(puppiJets_.size()) and jet2 < int(puppiJets_.size())) 
      dphi = fabs(ROOT::Math::VectorUtil::DeltaPhi(leps_[indexLeptonByPt(0)]->p4()+leps_[indexLeptonByPt(1)]->p4(), puppiJets_[jet1]->p4()+puppiJets_[jet2]->p4()) );

    return dphi;
}

/// fat jet section 

const float reco::SkimEvent::fatJetPt(size_t i, int applyCorrection) const {
 return jetPt(fatJets_[i].get(),applyCorrection);
}

const float reco::SkimEvent::fatPuppiJetPt(size_t i, int applyCorrection) const {
 return jetPt(fatPuppiJets_[i].get(),applyCorrection);
}


const float reco::SkimEvent::tagJetPt(size_t i, int applyCorrection) const {
  if(applyCorrection) return tagJets_[i]->pt();
  else return tagJets_[i]->correctedJet("Uncorrected","none").pt();
}

const float reco::SkimEvent::tagPuppiJetPt(size_t i, int applyCorrection) const {
  if(applyCorrection) return tagPuppiJets_[i]->pt();
  else return tagPuppiJets_[i]->correctedJet("Uncorrected","none").pt();
}

const float reco::SkimEvent::leadingFatJetPt(size_t index, float minPt,float eta,int applyCorrection,int applyJetID, int applyPileUpJetID) const {

 size_t count = 0;
 for(size_t i=0;i<fatJets_.size();++i) {
  if(!(passFatJetID(fatJets_[i],applyJetID)) ) continue;
  if(!(passPileUpJetID(fatJets_[i],applyPileUpJetID)) ) continue;
  if( std::fabs(fatJets_[i]->eta()) >= eta) continue;
  if( jetPt(fatJets_[i].get(),applyCorrection) <= minPt) continue;
  if(isThisJetALepton(fatJets_[i])) continue;
  if(++count > index) return jetPt(i,applyCorrection);
 }
 return -9999.9;
}

const float reco::SkimEvent::leadingFatPuppiJetPt(size_t index, float minPt,float eta,int applyCorrection,int applyJetID, int applyPileUpJetID) const {

 size_t count = 0;
 for(size_t i=0;i<fatPuppiJets_.size();++i) {
  if(!(passFatJetID(fatPuppiJets_[i],applyJetID)) ) continue;
  if(!(passPileUpJetID(fatPuppiJets_[i],applyPileUpJetID)) ) continue;
  if( std::fabs(fatPuppiJets_[i]->eta()) >= eta) continue;
  if( jetPt(fatPuppiJets_[i].get(),applyCorrection) <= minPt) continue;
  if(isThisJetALepton(fatPuppiJets_[i])) continue;
  if(++count > index) return jetPt(fatPuppiJets_[i].get(),applyCorrection);
 }
 return -9999.9;
}

const float reco::SkimEvent::leadingFatJetEta(size_t index, float minPt,float eta,int applyCorrection,int applyJetID, int applyPileUpJetID) const {
 size_t count = 0;
 for(size_t i=0;i<fatJets_.size();++i) {
  if(!(passFatJetID(fatJets_[i],applyJetID)) ) continue;
  if(!(passPileUpJetID(fatJets_[i],applyPileUpJetID)) ) continue;
  if( std::fabs(fatJets_[i]->eta()) >= eta) continue;
  if( jetPt(fatJets_[i].get(),applyCorrection) <= minPt) continue;
  if(isThisJetALepton(fatJets_[i])) continue;
  if(++count > index) return fatJets_[i]->eta();
 }
 return -9999.9;
}

const float reco::SkimEvent::leadingFatPuppiJetEta(size_t index, float minPt,float eta,int applyCorrection,int applyJetID, int applyPileUpJetID) const {
 size_t count = 0;
 for(size_t i=0;i<fatPuppiJets_.size();++i) {
  if(!(passFatJetID(fatPuppiJets_[i],applyJetID)) ) continue;
  if(!(passPileUpJetID(fatPuppiJets_[i],applyPileUpJetID)) ) continue;
  if( std::fabs(fatPuppiJets_[i]->eta()) >= eta) continue;
  if( jetPt(fatPuppiJets_[i].get(),applyCorrection) <= minPt) continue;
  if(isThisJetALepton(fatPuppiJets_[i])) continue;
  if(++count > index) return fatPuppiJets_[i]->eta();
 }
 return -9999.9;
}

const float reco::SkimEvent::leadingFatJetPhi(size_t index, float minPt,float eta,int applyCorrection,int applyJetID, int applyPileUpJetID) const {
 size_t count = 0;
 for(size_t i=0;i<fatJets_.size();++i) {
  if(!(passFatJetID(fatJets_[i],applyJetID)) ) continue;
  if(!(passPileUpJetID(fatJets_[i],applyPileUpJetID)) ) continue;
  if( std::fabs(fatJets_[i]->eta()) >= eta) continue;
  if( jetPt(fatJets_[i].get(),applyCorrection) <= minPt) continue;
  if(isThisJetALepton(fatJets_[i])) continue;
  if(++count > index) return fatJets_[i]->phi();
 }
 return -9999.9;
}

const float reco::SkimEvent::leadingFatPuppiJetPhi(size_t index, float minPt,float eta,int applyCorrection,int applyJetID, int applyPileUpJetID) const {
 size_t count = 0;
 for(size_t i=0;i<fatPuppiJets_.size();++i) {
  if(!(passFatJetID(fatPuppiJets_[i],applyJetID)) ) continue;
  if(!(passPileUpJetID(fatPuppiJets_[i],applyPileUpJetID)) ) continue;
  if( std::fabs(fatPuppiJets_[i]->eta()) >= eta) continue;
  if( jetPt(fatPuppiJets_[i].get(),applyCorrection) <= minPt) continue;
  if(isThisJetALepton(fatPuppiJets_[i])) continue;
  if(++count > index) return fatPuppiJets_[i]->phi();
 }
 return -9999.9;
}

const float reco::SkimEvent::leadingFatJetTrimmedMass(size_t index, float minPt,float eta,int applyCorrection,int applyJetID, int applyPileUpJetID) const {
 size_t count = 0;
 for(size_t i=0;i<fatJets_.size();++i) {
  if(!(passFatJetID(fatJets_[i],applyJetID)) ) continue;
  if(!(passPileUpJetID(fatJets_[i],applyPileUpJetID)) ) continue;
  if( std::fabs(fatJets_[i]->eta()) >= eta) continue;
  if( jetPt(fatJets_[i].get(),applyCorrection) <= minPt) continue;
  if(isThisJetALepton(fatJets_[i])) continue;
  if(++count > index and fatJets_[i]->hasUserFloat("TrimmedMass")) return fatJets_[i]->userFloat("TrimmedMass");
 }
 return -9999.9;
}

const float reco::SkimEvent::leadingFatPuppiJetTrimmedMass(size_t index, float minPt,float eta,int applyCorrection,int applyJetID, int applyPileUpJetID) const {
 size_t count = 0;
 for(size_t i=0;i<fatPuppiJets_.size();++i) {
  if(!(passFatJetID(fatPuppiJets_[i],applyJetID)) ) continue;
  if(!(passPileUpJetID(fatPuppiJets_[i],applyPileUpJetID)) ) continue;
  if( std::fabs(fatPuppiJets_[i]->eta()) >= eta) continue;
  if( jetPt(fatPuppiJets_[i].get(),applyCorrection) <= minPt) continue;
  if(isThisJetALepton(fatPuppiJets_[i])) continue;
  if(++count > index and fatPuppiJets_[i]->hasUserFloat("TrimmedMass")) return fatPuppiJets_[i]->userFloat("TrimmedMass");
 }
 return -9999.9;
}


const float reco::SkimEvent::leadingFatJetFilteredMass(size_t index, float minPt,float eta,int applyCorrection,int applyJetID, int applyPileUpJetID) const {
 size_t count = 0;
 for(size_t i=0;i<fatJets_.size();++i) {
  if(!(passJetID(fatJets_[i],applyJetID)) ) continue;
  if(!(passPileUpJetID(fatJets_[i],applyPileUpJetID)) ) continue;
  if( std::fabs(fatJets_[i]->eta()) >= eta) continue;
  if( jetPt(fatJets_[i].get(),applyCorrection) <= minPt) continue;
  if(isThisJetALepton(fatJets_[i])) continue;
  if(++count > index and fatJets_[i]->hasUserFloat("FilteredMass")) return fatJets_[i]->userFloat("FilteredMass");
 }
 return -9999.9;
}

const float reco::SkimEvent::leadingFatPuppiJetFilteredMass(size_t index, float minPt,float eta,int applyCorrection,int applyJetID, int applyPileUpJetID) const {
 size_t count = 0;
 for(size_t i=0;i<fatPuppiJets_.size();++i) {
  if(!(passJetID(fatPuppiJets_[i],applyJetID)) ) continue;
  if(!(passPileUpJetID(fatPuppiJets_[i],applyPileUpJetID)) ) continue;
  if( std::fabs(fatPuppiJets_[i]->eta()) >= eta) continue;
  if( jetPt(fatPuppiJets_[i].get(),applyCorrection) <= minPt) continue;
  if(isThisJetALepton(fatPuppiJets_[i])) continue;
  if(++count > index and fatPuppiJets_[i]->hasUserFloat("FilteredMass")) return fatPuppiJets_[i]->userFloat("FilteredMass");
 }
 return -9999.9;
}


const float reco::SkimEvent::leadingFatJetPrunedMass(size_t index, float minPt,float eta,int applyCorrection,int applyJetID, int applyPileUpJetID) const {
 size_t count = 0;
 for(size_t i=0;i<fatJets_.size();++i) {
  if(!(passJetID(fatJets_[i],applyJetID)) ) continue;
  if(!(passPileUpJetID(fatJets_[i],applyPileUpJetID)) ) continue;
  if( std::fabs(fatJets_[i]->eta()) >= eta) continue;
  if( jetPt(fatJets_[i].get(),applyCorrection) <= minPt) continue;
  if(isThisJetALepton(fatJets_[i])) continue;
  if(++count > index and fatJets_[i]->hasUserFloat("PrunedMass")) return fatJets_[i]->userFloat("PrunedMass");
 }
 return -9999.9;
}

const float reco::SkimEvent::leadingFatPuppiJetPrunedMass(size_t index, float minPt,float eta,int applyCorrection,int applyJetID, int applyPileUpJetID) const {
 size_t count = 0;
 for(size_t i=0;i<fatPuppiJets_.size();++i) {
  if(!(passJetID(fatPuppiJets_[i],applyJetID)) ) continue;
  if(!(passPileUpJetID(fatPuppiJets_[i],applyPileUpJetID)) ) continue;
  if( std::fabs(fatPuppiJets_[i]->eta()) >= eta) continue;
  if( jetPt(fatPuppiJets_[i].get(),applyCorrection) <= minPt) continue;
  if(isThisJetALepton(fatPuppiJets_[i])) continue;
  if(++count > index and fatPuppiJets_[i]->hasUserFloat("PrunedMass")) return fatPuppiJets_[i]->userFloat("PrunedMass");
 }
 return -9999.9;
}



const float reco::SkimEvent::leadingFatJetMassDrop(size_t index, float minPt,float eta,int applyCorrection,int applyJetID, int applyPileUpJetID) const {

 size_t count = 0;
 for(size_t i=0;i<fatJets_.size();++i) {
  if(!(passJetID(fatJets_[i],applyJetID)) ) continue;
  if(!(passPileUpJetID(fatJets_[i],applyPileUpJetID)) ) continue;
  if( std::fabs(fatJets_[i]->eta()) >= eta) continue;
  if( jetPt(fatJets_[i].get(),applyCorrection) <= minPt) continue;
  if(isThisJetALepton(fatJets_[i])) continue;
  if(++count > index and fatJets_[i]->hasUserFloat("massDrop")) return fatJets_[i]->userFloat("massDrop");
 }
 return -9999.9;
}

const float reco::SkimEvent::leadingFatPuppiJetMassDrop(size_t index, float minPt,float eta,int applyCorrection,int applyJetID, int applyPileUpJetID) const {

 size_t count = 0;
 for(size_t i=0;i<fatPuppiJets_.size();++i) {
  if(!(passJetID(fatPuppiJets_[i],applyJetID)) ) continue;
  if(!(passPileUpJetID(fatPuppiJets_[i],applyPileUpJetID)) ) continue;
  if( std::fabs(fatPuppiJets_[i]->eta()) >= eta) continue;
  if( jetPt(fatPuppiJets_[i].get(),applyCorrection) <= minPt) continue;
  if(isThisJetALepton(fatPuppiJets_[i])) continue;
  if(++count > index and fatPuppiJets_[i]->hasUserFloat("massDrop")) return fatPuppiJets_[i]->userFloat("massDrop");
 }
 return -9999.9;
}


const float reco::SkimEvent::leadingFatJetPrunedTau2Tau1(size_t index, float minPt,float eta,int applyCorrection,int applyJetID, int applyPileUpJetID) const {

 size_t count = 0;
 for(size_t i=0;i<fatJets_.size();++i) {
  if(!(passJetID(fatJets_[i],applyJetID)) ) continue;
  if(!(passPileUpJetID(fatJets_[i],applyPileUpJetID)) ) continue;
  if( std::fabs(fatJets_[i]->eta()) >= eta) continue;
  if( jetPt(fatJets_[i].get(),applyCorrection) <= minPt) continue;
  if(isThisJetALepton(fatJets_[i])) continue;
  if(++count > index and fatJets_[i]->hasUserFloat("Prunedtau2tau1") ) return fatJets_[i]->userFloat("Prunedtau2tau1");
 }
 return -9999.9;
}

const float reco::SkimEvent::leadingFatPuppiJetPrunedTau2Tau1(size_t index, float minPt,float eta,int applyCorrection,int applyJetID, int applyPileUpJetID) const {

 size_t count = 0;
 for(size_t i=0;i<fatPuppiJets_.size();++i) {
  if(!(passJetID(fatPuppiJets_[i],applyJetID)) ) continue;
  if(!(passPileUpJetID(fatPuppiJets_[i],applyPileUpJetID)) ) continue;
  if( std::fabs(fatPuppiJets_[i]->eta()) >= eta) continue;
  if( jetPt(fatPuppiJets_[i].get(),applyCorrection) <= minPt) continue;
  if(isThisJetALepton(fatPuppiJets_[i])) continue;
  if(++count > index and fatPuppiJets_[i]->hasUserFloat("Prunedtau2tau1") ) return fatPuppiJets_[i]->userFloat("Prunedtau2tau1");
 }
 return -9999.9;
}


const float reco::SkimEvent::leadingFatJetPrunedTau1(size_t index, float minPt,float eta,int applyCorrection,int applyJetID, int applyPileUpJetID) const {
 size_t count = 0;
 for(size_t i=0;i<fatJets_.size();++i) {
  if(!(passJetID(fatJets_[i],applyJetID)) ) continue;
  if(!(passPileUpJetID(fatJets_[i],applyPileUpJetID)) ) continue;
  if( std::fabs(fatJets_[i]->eta()) >= eta) continue;
  if( jetPt(fatJets_[i].get(),applyCorrection) <= minPt) continue;
  if(isThisJetALepton(fatJets_[i])) continue;
  if(++count > index and fatJets_[i]->hasUserFloat("Prunedtau1")) return fatJets_[i]->userFloat("Prunedtau1");
 }
 return -9999.9;
}

const float reco::SkimEvent::leadingFatPuppiJetPrunedTau1(size_t index, float minPt,float eta,int applyCorrection,int applyJetID, int applyPileUpJetID) const {
 size_t count = 0;
 for(size_t i=0;i<fatPuppiJets_.size();++i) {
  if(!(passJetID(fatPuppiJets_[i],applyJetID)) ) continue;
  if(!(passPileUpJetID(fatPuppiJets_[i],applyPileUpJetID)) ) continue;
  if( std::fabs(fatPuppiJets_[i]->eta()) >= eta) continue;
  if( jetPt(fatPuppiJets_[i].get(),applyCorrection) <= minPt) continue;
  if(isThisJetALepton(fatPuppiJets_[i])) continue;
  if(++count > index and fatPuppiJets_[i]->hasUserFloat("Prunedtau1")) return fatPuppiJets_[i]->userFloat("Prunedtau1");
 }
 return -9999.9;
}


const float reco::SkimEvent::leadingFatJetPrunedTau2(size_t index, float minPt,float eta,int applyCorrection,int applyJetID, int applyPileUpJetID) const {

 size_t count = 0;
 for(size_t i=0;i<fatJets_.size();++i) {
  if(!(passJetID(fatJets_[i],applyJetID)) ) continue;
  if(!(passPileUpJetID(fatJets_[i],applyPileUpJetID)) ) continue;
  if( std::fabs(fatJets_[i]->eta()) >= eta) continue;
  if( jetPt(fatJets_[i].get(),applyCorrection) <= minPt) continue;
  if(isThisJetALepton(fatJets_[i])) continue;
  if(++count > index and fatJets_[i]->hasUserFloat("Prunedtau2")) return fatJets_[i]->userFloat("Prunedtau2");
 }
 return -9999.9;
}

const float reco::SkimEvent::leadingFatPuppiJetPrunedTau2(size_t index, float minPt,float eta,int applyCorrection,int applyJetID, int applyPileUpJetID) const {

 size_t count = 0;
 for(size_t i=0;i<fatPuppiJets_.size();++i) {
  if(!(passJetID(fatPuppiJets_[i],applyJetID)) ) continue;
  if(!(passPileUpJetID(fatPuppiJets_[i],applyPileUpJetID)) ) continue;
  if( std::fabs(fatPuppiJets_[i]->eta()) >= eta) continue;
  if( jetPt(fatPuppiJets_[i].get(),applyCorrection) <= minPt) continue;
  if(isThisJetALepton(fatPuppiJets_[i])) continue;
  if(++count > index and fatPuppiJets_[i]->hasUserFloat("Prunedtau2")) return fatPuppiJets_[i]->userFloat("Prunedtau2");
 }
 return -9999.9;
}


const float reco::SkimEvent::leadingFatJetPrunedTau3(size_t index, float minPt,float eta,int applyCorrection,int applyJetID, int applyPileUpJetID) const {

 size_t count = 0;
 for(size_t i=0;i<fatJets_.size();++i) {
  if(!(passJetID(fatJets_[i],applyJetID)) ) continue;
  if(!(passPileUpJetID(fatJets_[i],applyPileUpJetID)) ) continue;
  if( std::fabs(fatJets_[i]->eta()) >= eta) continue;
  if( jetPt(fatJets_[i].get(),applyCorrection) <= minPt) continue;
  if(isThisJetALepton(fatJets_[i])) continue;
  if(++count > index and fatJets_[i]->hasUserFloat("Prunedtau3")) return fatJets_[i]->userFloat("Prunedtau3");
 }
 return -9999.9;
}

const float reco::SkimEvent::leadingFatPuppiJetPrunedTau3(size_t index, float minPt,float eta,int applyCorrection,int applyJetID, int applyPileUpJetID) const {

 size_t count = 0;
 for(size_t i=0;i<fatPuppiJets_.size();++i) {
  if(!(passJetID(fatPuppiJets_[i],applyJetID)) ) continue;
  if(!(passPileUpJetID(fatPuppiJets_[i],applyPileUpJetID)) ) continue;
  if( std::fabs(fatPuppiJets_[i]->eta()) >= eta) continue;
  if( jetPt(fatPuppiJets_[i].get(),applyCorrection) <= minPt) continue;
  if(isThisJetALepton(fatPuppiJets_[i])) continue;
  if(++count > index and fatPuppiJets_[i]->hasUserFloat("Prunedtau3")) return fatPuppiJets_[i]->userFloat("Prunedtau3");
 }
 return -9999.9;
}


const float reco::SkimEvent::leadingFatJetPrunedTau4(size_t index, float minPt,float eta,int applyCorrection,int applyJetID, int applyPileUpJetID) const {

 size_t count = 0;
 for(size_t i=0;i<fatJets_.size();++i) {
  if(!(passJetID(fatJets_[i],applyJetID)) ) continue;
  if(!(passPileUpJetID(fatJets_[i],applyPileUpJetID)) ) continue;
  if( std::fabs(fatJets_[i]->eta()) >= eta) continue;
  if( jetPt(fatJets_[i].get(),applyCorrection) <= minPt) continue;
  if(isThisJetALepton(fatJets_[i])) continue;
  if(++count > index and fatJets_[i]->hasUserFloat("Prunedtau4")) return fatJets_[i]->userFloat("Prunedtau4");
 }
 return -9999.9;
}

const float reco::SkimEvent::leadingFatPuppiJetPrunedTau4(size_t index, float minPt,float eta,int applyCorrection,int applyJetID, int applyPileUpJetID) const {

 size_t count = 0;
 for(size_t i=0;i<fatPuppiJets_.size();++i) {
  if(!(passJetID(fatPuppiJets_[i],applyJetID)) ) continue;
  if(!(passPileUpJetID(fatPuppiJets_[i],applyPileUpJetID)) ) continue;
  if( std::fabs(fatPuppiJets_[i]->eta()) >= eta) continue;
  if( jetPt(fatPuppiJets_[i].get(),applyCorrection) <= minPt) continue;
  if(isThisJetALepton(fatPuppiJets_[i])) continue;
  if(++count > index and fatPuppiJets_[i]->hasUserFloat("Prunedtau4")) return fatPuppiJets_[i]->userFloat("Prunedtau4");
 }
 return -9999.9;
}


const float reco::SkimEvent::highestHardBDisc(const float& maxPt, std::string discriminator, int applyJetID, int applyPileUpJetID, float dzCut) const {

    float disc=-9999.9;

    for(size_t i=0;i<tagJets_.size();++i) {
        if( tagJetPt(i,true) < maxPt ) continue;
        if(!(passJetID(tagJets_[i],applyJetID)) ) continue;
        if(!(passPileUpJetID(tagJets_[i],applyPileUpJetID)) ) continue;
        if(isThisJetALepton(tagJets_[i])) continue;
        if(jets_[i]->hasUserFloat("dz")){
	  if(fabs(jets_[i]->userFloat("dz")) > dzCut) continue;
	}
        if( tagJets_[i]->bDiscriminator(discriminator) > disc ) disc = tagJets_[i]->bDiscriminator(discriminator);
    }

    return disc;

}

const float reco::SkimEvent::highestHardBPuppiDisc(const float& maxPt, std::string discriminator, int applyJetID, int applyPileUpJetID, float dzCut) const {

    float disc=-9999.9;

    for(size_t i=0;i<tagPuppiJets_.size();++i) {
        if( tagPuppiJetPt(i,true) < maxPt ) continue;
        if(!(passJetID(tagPuppiJets_[i],applyJetID)) ) continue;
        if(!(passPileUpJetID(tagPuppiJets_[i],applyPileUpJetID)) ) continue;
        if(isThisJetALepton(tagPuppiJets_[i])) continue;
        if(jets_[i]->hasUserFloat("dz")){
	  if(fabs(jets_[i]->userFloat("dz")) > dzCut) continue;
	}
        if( tagPuppiJets_[i]->bDiscriminator(discriminator) > disc ) disc = tagPuppiJets_[i]->bDiscriminator(discriminator);
    }

    return disc;

}


const float reco::SkimEvent::highestBDiscRange(const float& minPt, const float& maxPt, std::string discriminator, int applyJetID, int applyPileUpJetID, float dzCut, int minPtApplyCorrection) const {

    float disc=-9999.9;

    for(size_t i=0;i<tagJets_.size();++i) {      
        if( tagJetPt(i,true) > maxPt ) continue;
        if( tagJetPt(i,true) <= minPt ) continue;        
        if(!(passJetID(tagJets_[i],applyJetID)) ) continue;
        if(!(passPileUpJetID(tagJets_[i],applyPileUpJetID)) ) continue;
        if(isThisJetALepton(tagJets_[i])) continue;
        if(tagJets_[i]->hasUserFloat("dz")){
	  if(fabs(tagJets_[i]->userFloat("dz")) > dzCut) continue;
	}
        if( tagJets_[i]->bDiscriminator(discriminator) > disc ) disc = tagJets_[i]->bDiscriminator(discriminator);
      
    }

    return disc;

}


const float reco::SkimEvent::highestBPuppiDiscRange(const float& minPt, const float& maxPt, std::string discriminator, int applyJetID, int applyPileUpJetID, float dzCut, int minPtApplyCorrection) const {

    float disc=-9999.9;

    for(size_t i=0;i<tagPuppiJets_.size();++i) {
        if( tagPuppiJetPt(i,true) > maxPt ) continue;
        if( tagPuppiJetPt(i,minPtApplyCorrection) <= minPt ) continue;
        if(!(passJetID(tagPuppiJets_[i],applyJetID)) ) continue;
        if(!(passPileUpJetID(tagPuppiJets_[i],applyPileUpJetID)) ) continue;
        if(isThisJetALepton(tagPuppiJets_[i])) continue;
        if(jets_[i]->hasUserFloat("dz")){
	  if(fabs(jets_[i]->userFloat("dz")) > dzCut) continue;
	}
        if( tagPuppiJets_[i]->bDiscriminator(discriminator) > disc ) disc = tagPuppiJets_[i]->bDiscriminator(discriminator);
    }

    return disc;
}

const float reco::SkimEvent::highestSoftBDisc(const float& maxPt, std::string discriminator, int applyJetID, int applyPileUpJetID, float dzCut) const {

    float disc=-9999.9;

    for(size_t i=0;i<tagJets_.size();++i) {
        if( tagJetPt(i,true) > maxPt ) continue;
        if(!(passJetID(tagJets_[i],applyJetID)) ) continue;
        if(!(passPileUpJetID(tagJets_[i],applyPileUpJetID)) ) continue;
        if(isThisJetALepton(tagJets_[i])) continue;
        if(jets_[i]->hasUserFloat("dz")){
	  if(fabs(jets_[i]->userFloat("dz")) > dzCut) continue;
	}
        if( tagJets_[i]->bDiscriminator(discriminator) > disc ) disc = tagJets_[i]->bDiscriminator(discriminator);
    }

    return disc;

}

const float reco::SkimEvent::highestSoftBPuppiDisc(const float& maxPt, std::string discriminator, int applyJetID, int applyPileUpJetID, float dzCut) const {

    float disc=-9999.9;

    for(size_t i=0;i<tagPuppiJets_.size();++i) {
        if( tagPuppiJetPt(i,true) > maxPt ) continue;
        if(!(passJetID(tagPuppiJets_[i],applyJetID)) ) continue;
        if(!(passPileUpJetID(tagPuppiJets_[i],applyPileUpJetID)) ) continue;
        if(isThisJetALepton(tagPuppiJets_[i])) continue;
        if(jets_[i]->hasUserFloat("dz")){
	  if(fabs(jets_[i]->userFloat("dz")) > dzCut) continue;
	}
        if( tagPuppiJets_[i]->bDiscriminator(discriminator) > disc ) disc = tagPuppiJets_[i]->bDiscriminator(discriminator);
    }

    return disc;

}

const int reco::SkimEvent::bTaggedJetsBetween(const float& minPt, const float& maxPt, const float& cut, std::string discriminator, int applyJetID, int applyPileUpJetID , float dzCut) const {

    int count=0;

    for(size_t i=0;i<tagJets_.size();++i) {
        if( tagJetPt(i,true) > maxPt ) continue;
        if( tagJetPt(i,true) <= minPt ) continue;
        if(!(passJetID(tagJets_[i],applyJetID)) ) continue;
        if(!(passPileUpJetID(tagJets_[i],applyPileUpJetID)) ) continue;
        if(tagJets_[i]->bDiscriminator(discriminator) <= cut ) continue;   
        if(jets_[i]->hasUserFloat("dz")){
	  if(fabs(jets_[i]->userFloat("dz")) > dzCut) continue;
	}    
       if(isThisJetALepton(tagJets_[i])) continue;
        count++;
    }

    return count;
}

const int reco::SkimEvent::bTaggedPuppiJetsBetween(const float& minPt, const float& maxPt, const float& cut, std::string discriminator, int applyJetID, int applyPileUpJetID , float dzCut) const {

    int count=0;

    for(size_t i=0;i<tagPuppiJets_.size();++i) {
        if( tagPuppiJetPt(i,true) > maxPt ) continue;
        if( tagPuppiJetPt(i,false) <= minPt ) continue;
        if(!(passJetID(tagPuppiJets_[i],applyJetID)) ) continue;
        if(!(passPileUpJetID(tagPuppiJets_[i],applyPileUpJetID)) ) continue;
        if( tagPuppiJets_[i]->bDiscriminator(discriminator) <= cut ) continue;   
        if(jets_[i]->hasUserFloat("dz")){
          if(fabs(jets_[i]->userFloat("dz")) > dzCut) continue;
	}
        if(isThisJetALepton(tagPuppiJets_[i])) continue;
        count++;
    }

    return count;
}

const int reco::SkimEvent::bTaggedJetsUnder(const float& maxPt, const float& cut, std::string discriminator, int applyJetID, int applyPileUpJetID, float dzCut) const {

    int count=0;

    for(size_t i=0;i<tagJets_.size();++i) {
        if( tagJetPt(i,true) > maxPt ) continue;
        if(!(passJetID(tagJets_[i],applyJetID)) ) continue;
        if(!(passPileUpJetID(tagJets_[i],applyPileUpJetID)) ) continue;
        if( tagJets_[i]->bDiscriminator(discriminator) <= cut ) continue;   
        if(jets_[i]->hasUserFloat("dz")){
          if(fabs(jets_[i]->userFloat("dz")) > dzCut) continue;
        }
        if(isThisJetALepton(tagJets_[i])) continue;
        count++;
    }

    return count;
}

const int reco::SkimEvent::bTaggedPuppiJetsUnder(const float& maxPt, const float& cut, std::string discriminator, int applyJetID, int applyPileUpJetID, float dzCut) const {

    int count=0;

    for(size_t i=0;i<tagPuppiJets_.size();++i) {
        if( tagPuppiJetPt(i,true) > maxPt ) continue;
        if(!(passJetID(tagPuppiJets_[i],applyJetID)) ) continue;
        if(!(passPileUpJetID(tagPuppiJets_[i],applyPileUpJetID)) ) continue;
        if( tagPuppiJets_[i]->bDiscriminator(discriminator) <= cut ) continue;   
        if(jets_[i]->hasUserFloat("dz")){
	  if(fabs(jets_[i]->userFloat("dz")) > dzCut) continue;
	}
        if(isThisJetALepton(tagPuppiJets_[i])) continue;
        count++;
    }

    return count;
}

const int reco::SkimEvent::bTaggedJetsOver(const float& maxPt, const float& cut, std::string discriminator, int applyJetID, int applyPileUpJetID, float dzCut) const {

    int count=0;

    for(size_t i=0;i<tagJets_.size();++i) {
        if( tagJetPt(i,true) <= maxPt ) continue;
        if(!(passJetID(tagJets_[i],applyJetID)) ) continue;
        if(!(passPileUpJetID(tagJets_[i],applyJetID)) ) continue;
        if( tagJets_[i]->bDiscriminator(discriminator) <= cut ) continue;
        if(tagJets_[i]->hasUserFloat("dz")){ 
	  if(fabs(tagJets_[i]->userFloat("dz")) > dzCut) continue;
	}
        if(isThisJetALepton(tagJets_[i])) continue;
        count++;
    }

    return count;
}

const int reco::SkimEvent::bTaggedPuppiJetsOver(const float& maxPt, const float& cut, std::string discriminator, int applyJetID, int applyPileUpJetID, float dzCut) const {

    int count=0;

    for(size_t i=0;i<tagPuppiJets_.size();++i) {
        if( tagPuppiJetPt(i,true) <= maxPt ) continue;
        if(!(passJetID(tagPuppiJets_[i],applyJetID)) ) continue;
        if( tagPuppiJets_[i]->bDiscriminator(discriminator) <= cut ) continue;
        if(jets_[i]->hasUserFloat("dz")){
	  if(fabs(jets_[i]->userFloat("dz")) > dzCut) continue;
	}
        if(isThisJetALepton(tagPuppiJets_[i])) continue;
        count++;
    }

    return count;
}


const float reco::SkimEvent::nearestJet(int i,float minPt, float eta, bool applyCorrection , int applyJetID, int applyPileUpJetID ) const { // nearest jet to one of the two lepton

    if (i >= (int)std::min((uint) 2,(uint) leps_.size()) || i < -1) return -9999.9;

    float dR = 9999;
    for(size_t j=0;j<jets_.size();++j) {
        if(!(passJetID(jets_[j],applyJetID)) ) continue;
        if(!(passPileUpJetID(jets_[j],applyPileUpJetID)) ) continue;
        if( std::fabs(jets_[j]->eta()) >= eta) continue;
        if( jetPt(jets_[j].get(),applyCorrection) <= minPt) continue;
        if(isThisJetALepton(jets_[j])) continue;
    
        float tempdR;
        if(i != -1) {
            tempdR = fabs(ROOT::Math::VectorUtil::DeltaR(jets_[j]->p4(),leps_[i]->p4()) );
            if( tempdR < dR ) dR = tempdR;
        } else {
            for(size_t k=0; k<std::min((uint) 2,(uint) leps_.size());++k){
               tempdR = fabs(ROOT::Math::VectorUtil::DeltaR(jets_[j]->p4(),leps_[k]->p4()) );
               if( tempdR < dR ) dR = tempdR;
            }
        }
    }
    return dR;
}

const float reco::SkimEvent::nearestPuppiJet(int i,float minPt, float eta, bool applyCorrection , int applyJetID, int applyPileUpJetID ) const { // nearest jet to one of the two lepton

    if (i >= (int)std::min((uint) 2,(uint) leps_.size()) || i < -1) return -9999.9;

    float dR = 9999;
    for(size_t j=0;j<puppiJets_.size();++j) {
        if(!(passJetID(puppiJets_[j],applyJetID)) ) continue;
        if(!(passPileUpJetID(puppiJets_[j],applyPileUpJetID)) ) continue;
        if( std::fabs(puppiJets_[j]->eta()) >= eta) continue;
        if( jetPt(puppiJets_[j].get(),applyCorrection) <= minPt) continue;
        if(isThisJetALepton(puppiJets_[j])) continue;
    
        float tempdR;
        if(i != -1) {
            tempdR = fabs(ROOT::Math::VectorUtil::DeltaR(puppiJets_[j]->p4(),leps_[i]->p4()) );
            if( tempdR < dR ) dR = tempdR;
        } else {
            for(size_t k=0; k<std::min((uint) 2,(uint) leps_.size());++k){
               tempdR = fabs(ROOT::Math::VectorUtil::DeltaR(puppiJets_[j]->p4(),leps_[k]->p4()) );
               if( tempdR < dR ) dR = tempdR;
            }
        }
    }
    return dR;
}

const pat::JetRef reco::SkimEvent::matchedJet(size_t i, float minDr) const {
    pat::JetRef ret;
    if (i >= std::min((uint) 2,(uint) leps_.size())) return ret;

    float dR = minDr;
    for(size_t j=0;j<jets_.size();++j) {
        float tempdR = fabs(ROOT::Math::VectorUtil::DeltaR(jets_[j]->p4(),leps_[i]->p4()) );
        if( tempdR < dR ) {
            dR = tempdR;
            ret = jets_[j];
        }
    }
    return ret;
}

const pat::JetRef reco::SkimEvent::matchedPuppiJet(size_t i, float minDr) const {
    pat::JetRef ret;
    if (i >= std::min((uint) 2,(uint) leps_.size())) return ret;

    float dR = minDr;
    for(size_t j=0;j<puppiJets_.size();++j) {
        float tempdR = fabs(ROOT::Math::VectorUtil::DeltaR(puppiJets_[j]->p4(),leps_[i]->p4()) );
        if( tempdR < dR ) {
            dR = tempdR;
            ret = puppiJets_[j];
        }
    }
    return ret;
}

const float reco::SkimEvent::matchedJetPt(size_t i, float minDr, bool applyCorrection) const {
    if (i >= std::min((uint) 2,(uint) leps_.size())) return -9999.9;

    float dR = minDr, pt = 0;
    for(size_t j=0;j<jets_.size();++j) {
        float tempdR = fabs(ROOT::Math::VectorUtil::DeltaR(jets_[j]->p4(),leps_[i]->p4()) );
        if( tempdR < dR ) {
            dR = tempdR;
            pt = jetPt(jets_[j].get(),applyCorrection);
        }
    }
    return pt;
}

const float reco::SkimEvent::matchedPuppiJetPt(size_t i, float minDr, bool applyCorrection) const {
    if (i >= std::min((uint) 2,(uint) leps_.size())) return -9999.9;

    float dR = minDr, pt = 0;
    for(size_t j=0;j<puppiJets_.size();++j) {
        float tempdR = fabs(ROOT::Math::VectorUtil::DeltaR(puppiJets_[j]->p4(),leps_[i]->p4()) );
        if( tempdR < dR ) {
            dR = tempdR;
            pt = jetPt(puppiJets_[j].get(),applyCorrection);
        }
    }
    return pt;
}

const size_t reco::SkimEvent::indexJetByPt(size_t i, float minPt,float eta,int applyCorrection,int applyJetID, int applyPileUpJetID) const {
 
 if( i >= jets_.size() ) return 9999; //--> big number then it will fail other tests later! good!
 std::vector<indexValueStruct> a;
 for(size_t j=0;j<jets_.size();++j) {
  if(!(passJetID(jets_[j],applyJetID)) ) continue;
  if(!(passPileUpJetID(jets_[j],applyPileUpJetID)) ) continue;
  if(std::fabs(jets_[j]->eta()) >= eta) continue;
  if(jetPt(jets_[j].get(),applyCorrection) <= minPt) continue;
  if(isThisJetALepton(jets_[j])) continue;
  a.push_back(indexValueStruct(jets_[j]->pt(),j));
 }
 
 std::sort(a.begin(),a.end(),highToLow);
 
 if(i < a.size()) return a[i].index;
 else  return 9999;
}

const size_t reco::SkimEvent::indexPuppiJetByPt(size_t i, float minPt,float eta,int applyCorrection,int applyJetID, int applyPileUpJetID) const {
 
 if( i >= puppiJets_.size() ) return 9999; //--> big number then it will fail other tests later! good!
 std::vector<indexValueStruct> a;
 
 for(size_t j=0;j<puppiJets_.size();++j) {
  if(!(passJetID(puppiJets_[j],applyJetID)) ) continue;
  if(!(passPileUpJetID(puppiJets_[j],applyPileUpJetID)) ) continue;
  if(std::fabs(puppiJets_[j]->eta()) >= eta) continue;
  if(jetPt(puppiJets_[j].get(),applyCorrection) <= minPt) continue;
  if(isThisJetALepton(puppiJets_[j])) continue;
  a.push_back(indexValueStruct(puppiJets_[j]->pt(),j));
 }
 
 std::sort(a.begin(),a.end(),highToLow);
 
 if( i < a.size() ) return a[i].index;
 else  return 9999;
}


//Event variables
const float reco::SkimEvent::pfSumEt() const {
    if(pfMet_.isNonnull()) return pfMet_->sumEt();
    else return -9999.0;
}

const float reco::SkimEvent::pfMet() const {
    if(pfMet_.isNonnull()) return pfMet_->pt();
    else return -9999.0;
}

const float reco::SkimEvent::pfMetPhi() const {
    if(pfMet_.isNonnull()) return pfMet_->phi();
    else return -9999.0;
}

const float reco::SkimEvent::pfPuppiSumEt() const {
    if(puppiPFMet_.isNonnull()) return puppiPFMet_->sumEt();
    else return -9999.0;
}

const float reco::SkimEvent::pfPuppiMet() const {
    if(puppiPFMet_.isNonnull()) return puppiPFMet_->pt();
    else return -9999.0;
}

const float reco::SkimEvent::pfPuppiMetPhi() const {
    if(puppiPFMet_.isNonnull()) return puppiPFMet_->phi();
    else return -9999.0;
}

const float reco::SkimEvent::tcSumEt() const {

    if(pfMet_.isNonnull()) return tcMet_->sumEt();
    else return -9999.0;
}

const float reco::SkimEvent::tcMet() const {

    if(tcMet_.isNonnull()) return tcMet_->pt();
    else return -9999.0;
}

const float reco::SkimEvent::tcMetPhi() const {

    if(tcMet_.isNonnull()) return tcMet_->phi();
    else return -9999.0;
}


const float reco::SkimEvent::chargedSumEt() const {

    return chargedMet_.sumEt();
}

const float reco::SkimEvent::chargedMet() const {
    return chargedMet_.pt();
}

const float reco::SkimEvent::pfMetSignificance() const {

    if(pfMet_.isNonnull()) return pfMet_->significance();
    else return -9999.0;
}

const float reco::SkimEvent::pfMetMEtSig() const {

    if(pfMet_.isNonnull()) return pfMet_->mEtSig();
    else return -9999.0;
}



const float reco::SkimEvent::chargedMetSmurfSumEt() const {

    return chargedMetSmurf_.sumEt();
}


const float reco::SkimEvent::tcMetX() const {

    if(tcMet_.isNonnull()) return tcMet_->px();
    else return -9999.0;
}

const float reco::SkimEvent::tcMetY() const {

    if(tcMet_.isNonnull()) return tcMet_->py();
    else return -9999.0;
}

const float reco::SkimEvent::mll() const {
  if(leps_.size() < 2) return -9999.0;
  return (leps_[indexLeptonByPt(0)]->p4() + leps_[indexLeptonByPt(1)]->p4()).mass();
}

const float reco::SkimEvent::pTll() const {
  if(leps_.size() < 2) return -9999.0;
  return (leps_[indexLeptonByPt(0)]->p4() + leps_[indexLeptonByPt(1)]->p4()).pt();
}

const float reco::SkimEvent::dPhill() const {
  if(leps_.size() < 2) return -9999.0;
  return fabs(ROOT::Math::VectorUtil::DeltaPhi(leps_[indexLeptonByPt(0)]->p4(),leps_[indexLeptonByPt(1)]->p4()) );
}

const float reco::SkimEvent::dRll() const {
  if(leps_.size() < 2) return -9999.0;
  return ROOT::Math::VectorUtil::DeltaR(leps_[indexLeptonByPt(0)]->p4(),leps_[indexLeptonByPt(1)]->p4());
}

const float reco::SkimEvent::dEtall() const {
  if(leps_.size() < 2) return -9999.0;
  return fabs(leps_[indexLeptonByPt(0)]->eta() - leps_[indexLeptonByPt(1)]->eta());
}

const float reco::SkimEvent::etall() const {
  if(leps_.size() < 2) return -9999.0;
  return (leps_[indexLeptonByPt(0)]->p4() + leps_[indexLeptonByPt(1)]->p4()).eta();
}

const float reco::SkimEvent::yll() const {
  if(leps_.size() < 2) return -9999.0;
  return (leps_[indexLeptonByPt(0)]->p4() + leps_[indexLeptonByPt(1)]->p4()).Rapidity();
}

const float reco::SkimEvent::dPhillMet(metType metToUse) const {
  switch (metToUse) {
    case TCMET: return dPhillTcMet();
    case PFMET: return dPhillPfMet();
    case CHMET: return dPhillChargedMet();
    case PUPPIMET:  return dPhillPuppiMet();
  }
return 0;
}

const float reco::SkimEvent::dPhillPfMet() const {
  if(leps_.size() < 2 || pfMet_.isNull()) return -9999.0;
  return fabs(ROOT::Math::VectorUtil::DeltaPhi(leps_[indexLeptonByPt(0)]->p4()+leps_[indexLeptonByPt(1)]->p4(),pfMet_->p4()) );
}

const float reco::SkimEvent::dPhillTcMet() const {
  if(leps_.size() < 2 || tcMet_.isNull()) return -9999.0;
  return fabs(ROOT::Math::VectorUtil::DeltaPhi(leps_[indexLeptonByPt(0)]->p4()+leps_[indexLeptonByPt(1)]->p4(),tcMet_->p4()) );
}

const float reco::SkimEvent::dPhillChargedMet() const {
  if(leps_.size() < 2) return -9999.0;
  return fabs(ROOT::Math::VectorUtil::DeltaPhi(leps_[indexLeptonByPt(0)]->p4()+leps_[indexLeptonByPt(1)]->p4(),chargedMet_.p4()) );
}

const float reco::SkimEvent::dPhillPuppiMet() const {
  if(leps_.size() < 2) return -9999.0;
  return fabs(ROOT::Math::VectorUtil::DeltaPhi(leps_[indexLeptonByPt(0)]->p4()+leps_[indexLeptonByPt(1)]->p4(),puppiPFMet_->p4()) );
}


const float reco::SkimEvent::mTHiggs(metType metToUse) const {
    // AN 2011/155, v2, p19
    return sqrt( 2 * pTll() * met(metToUse) * ( 1 - cos(dPhillMet(metToUse)) ) );
}

const float reco::SkimEvent::pXll() const {
  if(leps_.size() < 2) return -9999.0;
  return (leps_[indexLeptonByPt(0)]->p4() + leps_[indexLeptonByPt(1)]->p4()).px();
}

const float reco::SkimEvent::pYll() const {
  if(leps_.size() < 2) return -9999.0;
  return (leps_[indexLeptonByPt(0)]->p4() + leps_[indexLeptonByPt(1)]->p4()).py();
}

const float reco::SkimEvent::pZll() const {
  if(leps_.size() < 2) return -9999.0;
  return (leps_[indexLeptonByPt(0)]->p4() + leps_[indexLeptonByPt(1)]->p4()).pz();
}

const float reco::SkimEvent::mTll() const {
  if(leps_.size() < 2) return -9999.0;
  return (leps_[indexLeptonByPt(0)]->p4() + leps_[indexLeptonByPt(1)]->p4()).mt();
}

const float reco::SkimEvent::mT(size_t i, metType metToUse) const {
  if(i>=leps_.size()) return -9999.0;
  return sqrt(2*getPt(i)*met(metToUse)*(1 - cos(dPhilMet(i, metToUse))));
}

const float reco::SkimEvent::met(metType metToUse) const {
    switch (metToUse) {
        case TCMET: return tcMet();
        case PFMET: return pfMet();
        case CHMET: return chargedMet();
        case PUPPIMET: return pfPuppiMet();
//case MINMET: return minMet();
    }
return 0;
}

const float reco::SkimEvent::projMet(metType metToUse) const {
    float dphi = dPhilMet(metToUse);
    if(dphi < M_PI/2.) return met(metToUse)*sin(dphi);
    else return met(metToUse);
}

const float reco::SkimEvent::projPfMet() const {
    float dphi = dPhilPfMet();
    if(dphi < M_PI/2.) return pfMet()*sin(dphi);
    else return pfMet();
}

const float reco::SkimEvent::projPuppiPfMet() const {
    float dphi = dPhilPuppiPfMet();
    if(dphi < M_PI/2.) return pfPuppiMet()*sin(dphi);
    else return pfPuppiMet();
}

const float reco::SkimEvent::projMvaMet() const {
    float dphi = dPhilMvaMet();
    if(dphi < M_PI/2.) return mvaMet()*sin(dphi);
    else return mvaMet();
}

const float reco::SkimEvent::projTcMet() const {
    float dphi = dPhilTcMet();
    if(dphi < M_PI/2.) return tcMet()*sin(dphi);
    else return tcMet();
}

const float reco::SkimEvent::projChargedMet() const {
    float dphi = dPhilChargedMet();
    if(dphi < M_PI/2.) return chargedMet()*sin(dphi);
    else return chargedMet();
}

const float reco::SkimEvent::projChargedMetSmurf() const {
    float dphi = dPhilChargedMetSmurf();
    if(dphi < M_PI/2.) return chargedMetSmurf()*sin(dphi);
    else return chargedMetSmurf();
}


const float reco::SkimEvent::dPhilMet(metType metToUse) const {
    switch (metToUse) {
        case TCMET: return dPhilTcMet();
        case PFMET: return dPhilPfMet();
        case CHMET: return dPhilChargedMet();
        case PUPPIMET: return dPhilPuppiPfMet();
    }
return 0;
}


const float reco::SkimEvent::dPhilTcMet() const {
    float smallestDphi = 9999.;
    for(size_t l=0; l<std::min((uint) 2,(uint) leps_.size());++l){
        float dphi = dPhilTcMet(l);
        if( dphi < smallestDphi) smallestDphi = dphi;
    }
    return smallestDphi;
}

const float reco::SkimEvent::dPhilPfMet() const {
    float smallestDphi = 9999.;
    for(size_t l=0; l<std::min((uint) 2,(uint) leps_.size());++l){
        float dphi = dPhilPfMet(l);
        if( dphi < smallestDphi) smallestDphi = dphi;
    }
    return smallestDphi;
}

const float reco::SkimEvent::dPhilPuppiPfMet() const {
    float smallestDphi = 9999.;
    for(size_t l=0; l<std::min((uint) 2,(uint) leps_.size());++l){
        float dphi = dPhilPuppiPfMet(l);
        if( dphi < smallestDphi) smallestDphi = dphi;
    }
    return smallestDphi;
}

const float reco::SkimEvent::dPhilMvaMet() const {
    float smallestDphi = 9999.;
    for(size_t l=0; l<std::min((uint) 2,(uint) leps_.size());++l){
        float dphi = dPhilMvaMet(l);
        if( dphi < smallestDphi) smallestDphi = dphi;
    }
    return smallestDphi;
}

const float reco::SkimEvent::dPhilChargedMet() const {
    float smallestDphi = 9999.;
    for(size_t l=0; l<std::min((uint) 2,(uint) leps_.size());++l){
        float dphi = dPhilChargedMet(l);
        if( dphi < smallestDphi) smallestDphi = dphi;
    }
    return smallestDphi;
}

const float reco::SkimEvent::dPhilChargedMetSmurf() const {
    float smallestDphi = 9999.;
    for(size_t l=0; l<std::min((uint) 2,(uint) leps_.size());++l){
        float dphi = dPhilChargedMetSmurf(l);
        if( dphi < smallestDphi) smallestDphi = dphi;
    }
    return smallestDphi;
}


const float reco::SkimEvent::dPhilMet(size_t i, metType metToUse) const {
   if( i >= std::min((uint) 2,(uint) leps_.size()) ) return -9999.0;
    switch (metToUse) {
        case TCMET: return dPhilTcMet(i);
        case PFMET: return dPhilPfMet(i);
        case CHMET: return dPhilChargedMet(i);
        case PUPPIMET: return dPhilPuppiPfMet(i);
//case MINMET: return dPhilMinMet(i);
    }
return -9999.0;
}

const float reco::SkimEvent::dPhilTcMet(size_t i) const {
    if( i >= std::min((uint) 2,(uint) leps_.size()) ) return -9999.0;
    return fabs(ROOT::Math::VectorUtil::DeltaPhi(tcMet_->p4(),leps_[i]->p4()) );
}

const float reco::SkimEvent::dPhilPfMet(size_t i) const {
    if( i >= std::min((uint) 2,(uint) leps_.size()) ) return -9999.0;
    return fabs(ROOT::Math::VectorUtil::DeltaPhi(pfMet_->p4(),leps_[i]->p4()) );
}

const float reco::SkimEvent::dPhilPuppiPfMet(size_t i) const {
    if( i >= std::min((uint) 2,(uint) leps_.size()) ) return -9999.0;
    return fabs(ROOT::Math::VectorUtil::DeltaPhi(puppiPFMet_->p4(),leps_[i]->p4()) );
}

const float reco::SkimEvent::dPhilMvaMet(size_t i) const {
    if( i >= std::min((uint) 2,(uint) leps_.size()) ) return -9999.0;
    return fabs(ROOT::Math::VectorUtil::DeltaPhi(mvaMet_.p4(),leps_[i]->p4()) );
}

const float reco::SkimEvent::dPhilChargedMet(size_t i) const {
    if( i >= std::min((uint) 2,(uint) leps_.size()) ) return -9999.0;
    return fabs(ROOT::Math::VectorUtil::DeltaPhi(chargedMet_.p4(),leps_[i]->p4()) );
}

const float reco::SkimEvent::dPhilChargedMetSmurf(size_t i) const {
    if( i >= std::min((uint) 2,(uint) leps_.size()) ) return -9999.0;
    return fabs(ROOT::Math::VectorUtil::DeltaPhi(chargedMetSmurf_.p4(),leps_[i]->p4()) );
}


const float reco::SkimEvent::nTracks() const {
    return 0;
}

const float reco::SkimEvent::cosThetaStar(size_t i) const {
    return 0;
}

const float reco::SkimEvent::afb(size_t i) const {
    return 0;
}


const bool reco::SkimEvent::leptEtaCut(float maxAbsEtaMu,float maxAbsEtaEl) const{
  bool check0(true);
  bool check1(true);

  if(leps_.size() < 2) return true;
  if(abs(leps_[0]->pdgId())==11 && fabs(leps_[0]->eta())>=maxAbsEtaEl) check0=false;
  if(abs(leps_[0]->pdgId())==13 && fabs(leps_[0]->eta())>=maxAbsEtaMu) check0=false;
  if(abs(leps_[1]->pdgId())==11 && fabs(leps_[1]->eta())>=maxAbsEtaEl) check1=false;
  if(abs(leps_[1]->pdgId())==13 && fabs(leps_[1]->eta())>=maxAbsEtaMu) check1=false;
  
  return (check0 && check1);
  
}


// ... in spite my egregious programming
void reco::SkimEvent::setTriggerBits( const std::vector<bool> &bits) {
    
    passesSingleMuData_ = bits[0];
    passesSingleElData_ = bits[1];
    passesDoubleMuData_ = bits[2];
    passesDoubleElData_ = bits[3];
    passesMuEGData_     = bits[4];
    passesSingleMuMC_   = bits[5];
    passesSingleElMC_   = bits[6];
    passesDoubleMuMC_   = bits[7];
    passesDoubleElMC_   = bits[8];
    passesMuEGMC_       = bits[9];
    passesAllEmbed_     = bits[10];

}

const bool reco::SkimEvent::triggerBitsCut( SkimEvent::primaryDatasetType pdType) const{
  if (pdType == MC) return true; // no cut on the MC
  return false;
}

const bool reco::SkimEvent::guillelmoTrigger( SkimEvent::primaryDatasetType pdType ) const {

    //Guillelmo's Implementation:
    if(pdType == MC) return true;

    if(pdType == MuEG) { return ( passesMuEGData_ );
    } else if(pdType == DoubleMuon) { return ( !passesMuEGData_ && passesDoubleMuData_ );
    } else if(pdType == SingleMuon) { return ( !passesMuEGData_ && !passesDoubleMuData_ && passesSingleMuData_ );
    } else if(pdType == DoubleElectron) { return ( !passesMuEGData_ && !passesDoubleMuData_ && !passesSingleMuData_ && passesDoubleElData_ );
    } else if(pdType == SingleElectron) { return ( !passesMuEGData_ && !passesDoubleMuData_ && !passesSingleMuData_ && !passesDoubleElData_ && passesSingleElData_ );
    } else if(pdType == AllEmbed) { return ( passesMuEGData_ || passesDoubleMuData_ || passesSingleMuData_ || passesDoubleElData_ || passesSingleElData_ );
    }

    return false;
}

bool reco::SkimEvent::passTriggerSingleMu(size_t i, bool isData) const{
  bool result(false);

  if( !isMuon(i) ) return false;
  const pat::Muon * mu = getMuon(i);

  const pat::TriggerObjectStandAlone * match = mu->triggerObjectMatchByCollection("hltL3MuonCandidates");
  if(isData){
    if(match) result= (match->hasPathName("HLT_Mu24_v*",false) || match->hasPathName("HLT_IsoMu17_v*",false) );
  }
  else{
    if(match) result=(match->hasPathName("HLT_Mu21_v*",false) && match->pt()>24.0);
  }

  return result;
}

bool reco::SkimEvent::passTriggerDoubleMu(size_t i, bool isData) const{
  using namespace std;
  bool result(false);
  
  if( !isMuon(i) ) return false;
  const pat::Muon *mu = getMuon(i);

  const pat::TriggerObjectStandAlone * match = mu->triggerObjectMatchByCollection("hltL3MuonCandidates");
  if(isData){
    if(match) result=match->hasPathName("HLT_DoubleMu7_v*",false); }
  else{
    if(match) result=(match->hasPathName("HLT_DoubleMu5_v*",false) && match->pt()>7.0);
  }
  return result;
}


bool reco::SkimEvent::passTriggerDoubleEl(size_t i, bool isData) const{
  bool result(false);
  
  if( !isElectron(i) ) return false;
  const pat::Electron* el = getElectron(i);
    
  if(isData){
    if(el->triggerObjectMatchesByPath("HLT_Ele17_CaloIdL_CaloIsoVL_Ele8_CaloIdL_CaloIsoVL_v*").size() ||
       el->triggerObjectMatchesByPath("HLT_Ele17_CaloIdT_TrkIdVL_CaloIsoVL_TrkIsoVL_Ele8_CaloIdT_TrkIdVL_CaloIsoVL_TrkIsoVL_v*").size() )
      result=true;
  }else{
    if(el->triggerObjectMatchesByPath("HLT_Ele17_SW_TightCaloEleId_Ele8HE_L1R_v*").size() )
      result=true;
  }
  
  return result;

}

bool reco::SkimEvent::passTriggerElMu(size_t i, bool isData) const{
  bool result(false);
  using namespace std;
  if( isMuon(i) ) {
    const pat::Muon* mu = getMuon(i);
    const pat::TriggerObjectStandAlone * match = mu->triggerObjectMatchByCollection("hltL3MuonCandidates");
    
    if(match){
      if(isData){
        bool res1 = match->hasPathName("HLT_Mu8_Ele17_CaloIdL_v*",false);
        bool res2 = match->hasPathName("HLT_Mu17_Ele8_CaloIdL_v*",false);
        result=( res1 || res2);
      }
      else{
        bool res1 = (match->hasPathName("HLT_Mu5_Ele17_v*",false) && match->pt()>8.0);
        bool res2 = (match->hasPathName("HLT_Mu11_Ele8_v*",false) && match->pt()>17.0);
        result=( res1 || res2);
      }
    }
    return result;
  }

  if( isElectron(i) ) {
    const pat::Electron* el = getElectron(i);
    if(isData){
      if(el->triggerObjectMatchesByPath("HLT_Mu8_Ele17_CaloIdL*").size() ||
         el->triggerObjectMatchesByPath("HLT_Mu17_Ele8_CaloIdL*").size() )
         result=true;
    }
    else{
      const pat::TriggerObjectStandAlone * match1 = el->triggerObjectMatchByPath("HLT_Mu5_Ele17_v*",true);
      const pat::TriggerObjectStandAlone * match2 = el->triggerObjectMatchByPath("HLT_Mu11_Ele8_v*",true);
      result=( match1 || match2 );
    }
  }

  return result;

}


const bool reco::SkimEvent::triggerMatchingCut(SkimEvent::primaryDatasetType pdType) const{
  if( pdType == MC ) return true;
  bool result(false);
  return result;
}


//Iso Functions
const float reco::SkimEvent::tkIso(size_t i) const {

    if( i >= leps_.size() ) return -9999.0;
    if( isElectron(i) ) {
        return getElectron(i)->dr03TkSumPt();
    } else if ( isMuon(i) ) {
        return getMuon(i)->isolationR03().sumPt;
    } else {
        return -9999.0;
    }
}

const float reco::SkimEvent::ecalIso(size_t i) const {

    if( i >= leps_.size() ) return -9999.0;

    if( isElectron(i) ) {
        return getElectron(i)->dr03EcalRecHitSumEt();
    } else if ( isMuon(i) ) {
        return getMuon(i)->isolationR03().emEt;
    } else {
        return -9999.0;
    }
}

const float reco::SkimEvent::hcalIso(size_t i) const {

    if( i >= leps_.size() ) return -9999.0;

    if( isElectron(i)){
      if(getElectron(i)->hasUserFloat("hcalFull") ) return getElectron(i)->userFloat("hcalFull");
      else return getElectron(i)->dr03HcalDepth1TowerSumEt()+getElectron(i)->dr03HcalDepth2TowerSumEt() ;
    } else if ( isMuon(i) ) {
        return getMuon(i)->isolationR03().hadEt;
    } else {
        return -9999.0;
    }
}

const float reco::SkimEvent::getRho(size_t i) const {

    if( i >= leps_.size() ) return -9999.0;
    
    if( isElectron(i)){
      if(getElectron(i)->hasUserFloat("rhoEl")) return getElectron(i)->userFloat("rhoEl");
      else return -9999.0;
    } else if ( isMuon(i)) {
      if(getMuon(i)->hasUserFloat("rhoMu")) return getMuon(i)->userFloat("rhoMu");
      else return -9999.0;
    } else {
        return -9999.0;
    }
}


const float reco::SkimEvent::mvaIso(size_t i) const {

    if ( i >= leps_.size() ) return -9999.0;
    if ( isElectron(i) and getElectron(i)->hasUserFloat("pfCombRelIso04EACorr") ) return getElectron(i)->userFloat("pfCombRelIso04EACorr");
    else if ( isMuon(i) and getMuon(i)->hasUserFloat("bdtisonontrigDZ")) return getMuon(i)->userFloat("bdtisonontrigDZ");
    else return -9999.0;
}


const float reco::SkimEvent::tkVeto(size_t i) const {

    if( i >= leps_.size() ) return -9999.0;
    if( isElectron(i) ) {
        return 0;
    } else if ( isMuon(i) ) {
        return getMuon(i)->isolationR03().trackerVetoPt;
    } else {
        return -9999.0;
    }
}

const float reco::SkimEvent::ecalVeto(size_t i) const {

    if( i >= leps_.size() ) return -9999.0;

    if( isElectron(i) ) {
        return 0;
    } else if ( isMuon(i) ) {
        return getMuon(i)->isolationR03().emVetoEt;
    } else {
        return -9999.0;
    }
}

const float reco::SkimEvent::hcalVeto(size_t i) const {

    if( i >= leps_.size() ) return -9999.0;

    if( isElectron(i) ) {
        return 0;
    } else if ( isMuon(i) ) {
        return getMuon(i)->isolationR03().hadVetoEt;
    } else {
        return -9999.0;
    }
}

const float reco::SkimEvent::allVeto(size_t i) const {
    
    if( i >= leps_.size() ) return -9999.0;

    if( isElectron(i) ) {
        return 0;
    } else if( isMuon(i) ) {
        return tkVeto(i) + ecalVeto(i) + hcalVeto(i);
    } else {
        return -9999.0;
    }
}



const bool reco::SkimEvent::isEB(size_t i) const {

    if( isElectron(i) ) {
        return getElectron(i)->isEB();
    } else {
        return false;
    }
}

const bool reco::SkimEvent::isEE(size_t i) const {

    if( isElectron(i) ) {
        return getElectron(i)->isEE();
    } else {
        return false;
    }
}

const float reco::SkimEvent::tkPt(size_t i) const {
    if( isElectron(i) ) {
        return getElectron(i)->gsfTrack()->pt();
    } else if( isMuon(i) && !isSTA(i)) {
        return getMuon(i)->track()->pt();
    } else {
        return -9999.9;
    }
}

const bool reco::SkimEvent::passesVtxSel(size_t i) const {
    if(i >= vtxs_.size()) {
        return false;
    } else {
        return ( vtxs_[i]->isValid() && !vtxs_[i]->isFake() && vtxs_[i]->ndof() >= 4.0 &&
                 fabs(vtxs_[i]->position().Rho()) < 2.0 && abs(vtxs_[i]->z()) < 24.0 );
    }
    return false;
}

const int reco::SkimEvent::nGoodVertices() const {

    int count = 0;
    for(size_t i=0;i<vtxs_.size();++i) if( passesVtxSel(i) ) count++;

    return count;
}

const bool reco::SkimEvent::hasGoodVertex() const {

    return (nGoodVertices() > 0);
}

const reco::Vertex* reco::SkimEvent::highestPtVtx() const {
    if(vtxs_.size() == 0)   return(new reco::Vertex()) ;
    if(sumPts_.size() == 0) return vtxs_[0].get();
    double sum = 0;
    size_t high = 0;
    for(size_t i=0;i<vtxs_.size();++i) {
        if( sumPts_[i] > sum && passesVtxSel(i) ) {
            high = i;
            sum = sumPts_[i];
        }
    }
    return vtxs_[high].get();
}

const bool reco::SkimEvent::passesIP() const {
    if(leps_.size() < 2) return -9999.0;
    return (passesIP(leps_[indexLeptonByPt(0)]) && passesIP(leps_[indexLeptonByPt(1)]));
}

const bool reco::SkimEvent::passesIP(const refToCand &c) const {

    if( isElectron(c) ) {
      if(getElectron(c)->hasUserFloat("ip2")){
	if( fabs(getElectron(c)->userFloat("ip2")) >= 0.03) return false;
      }
    } else if( isMuon(c) && !isSTA(c) ) {

      if( getMuon(c)->hasUserFloat("tip2") and getMuon(c)->hasUserFloat("dzPV")){
        if( fabs(getMuon(c)->userFloat("tip2")) >= 0.01) return false;
        if( fabs(getMuon(c)->userFloat("dzPV")) >= 0.05) return false;
      }
    }
    
    return true;
}

const double reco::SkimEvent::d0Reco(size_t i) const {
  double dxyPV = 9999;
  if( isElectron(i) ) {
    if(getElectron(i)->hasUserFloat("dxyPV"))
      dxyPV = getElectron(i)->userFloat("dxyPV");
  } else if( isMuon(i) && !isSTA(i) ) {
    if(getMuon(i)->hasUserFloat("dxyPV"))
     dxyPV = getMuon(i)->userFloat("dxyPV");
  }
  return dxyPV;
}

const double reco::SkimEvent::dZReco(size_t i) const {
  double dzPV = 9999;
  if( isElectron(i) ) {
    if(getElectron(i)->hasUserFloat("dzPV"))
     dzPV = getElectron(i)->userFloat("dzPV");
  } else if( isMuon(i) && !isSTA(i) ) {
    if(getMuon(i)->hasUserFloat("dzPV"))  
     dzPV = getMuon(i)->userFloat("dzPV");
  }
  return dzPV;
}
 
const bool reco::SkimEvent::passesConversion(size_t i) const {
  if( isElectron(i) ) {
    const pat::Electron *e = getElectron(i);
    if(e->hasUserFloat("convValueMapProd:dist") and e->hasUserFloat("convValueMapProd:dcot")){
     if( fabs(e->userFloat("convValueMapProd:dist")) < 0.02 && fabs(e->userFloat("convValueMapProd:dcot")) < 0.02 ) 
      return false;
    }
    
    if( e->gsfTrack()->trackerExpectedHitsInner().numberOfLostHits() > 0 ) {
      return false;
    }
    return true;
  } else if( isMuon(i) ) {
    return true;
  } else {
    return false;
  }
}


// muon ID                                                                                                                                                                          
const bool reco::SkimEvent::isSTA(size_t i ) const {
  if(i < leps_.size()) return isSTA(leps_[i]);
  return false;
}

const bool reco::SkimEvent::isSTA(const refToCand &c) const {
  if( isMuon(c) ) return (getMuon(c)->type() == 8);  
  else return false;  
}

const bool reco::SkimEvent::isTightMuon(size_t i ) const {
  if(i < leps_.size()) isTightMuon(leps_[i]);
  return false;
}

const bool reco::SkimEvent::isTightMuon(const refToCand &c) const {
  if( isMuon(c) and  highestPtVtx()){
   return (muon::isTightMuon(*getMuon(c), *highestPtVtx()));
  }
  else return false;
}

const bool reco::SkimEvent::isLooseMuon(size_t i ) const {
  if(i < leps_.size()) isLooseMuon(leps_[i]);
  return false;
}

const bool reco::SkimEvent::isLooseMuon(const refToCand &c) const {
  if( isMuon(c) ) return (muon::isLooseMuon(*getMuon(c)));
  else return false;
}

const bool reco::SkimEvent::isSoftMuon(size_t i ) const {
  if(i < leps_.size()) isSoftMuon(leps_[i]);
  return false;
}

const bool reco::SkimEvent::isSoftMuon(const refToCand &c) const {
  if( isMuon(c) and highestPtVtx()) return (muon::isSoftMuon(*getMuon(c),*highestPtVtx()));
  else return false;
}

const bool reco::SkimEvent::isHighPtMuon(size_t i ) const {
  if(i < leps_.size()) isHighPtMuon(leps_[i]);
  return false;
}

const bool reco::SkimEvent::isHighPtMuon(const refToCand &c) const {
  if( isMuon(c) and highestPtVtx()) return (muon::isHighPtMuon(*getMuon(c),*highestPtVtx()));
  else return false;
}

// electron ID       
const bool reco::SkimEvent::isTightElectron(size_t i ) const {
  if(i < leps_.size()) isTightElectron(leps_[i]);
  return false;
}

const bool reco::SkimEvent::isTightElectron(const refToCand &c) const {
  if( isElectron(c) ){
    if(getElectron(c)->isElectronIDAvailable("eidTight")) return getElectron(c)->electronID("eidTight");
    else return false;
  }
  else return false;
}

const bool reco::SkimEvent::isLooseElectron(size_t i ) const {
  if(i < leps_.size()) isLooseElectron(leps_[i]);
  return false;
}

const bool reco::SkimEvent::isLooseElectron(const refToCand &c) const {
  if( isElectron(c) ){
    if(getElectron(c)->isElectronIDAvailable("eidLoose")) return getElectron(c)->electronID("eidLoose");
    else return false;
  }
  else return false;
}

const bool reco::SkimEvent::isLooseRobustElectron(size_t i ) const {
  if(i < leps_.size()) isLooseRobustElectron(leps_[i]);
  return false;

}
const bool reco::SkimEvent::isLooseRobustElectron(const refToCand &c) const {
  if( isElectron(c) ){
    if(getElectron(c)->isElectronIDAvailable("eidRobustLoose")) return getElectron(c)->electronID("eidRobustLoose");
    else return false;
  }
  else return false;
}

const bool reco::SkimEvent::isTightRobustElectron(size_t i ) const {
  if(i < leps_.size()) isTightRobustElectron(leps_[i]);
  return false;
}

const bool reco::SkimEvent::isTightRobustElectron(const refToCand &c) const {
  if( isElectron(c) ){
    if(getElectron(c)->isElectronIDAvailable("eidRobustTight")) return getElectron(c)->electronID("eidRobustTight");
    else return false;
  }
  else return false;
}

const bool reco::SkimEvent::isRobustHighEnergyElectron(size_t i ) const {
  if(i < leps_.size()) isRobustHighEnergyElectron(leps_[i]);
  return false;
}

const bool reco::SkimEvent::isRobustHighEnergyElectron(const refToCand &c) const {
  if( isElectron(c) ){
    if(getElectron(c)->isElectronIDAvailable("eidRobustHighEnergy")) return getElectron(c)->electronID("eidRobustHighEnergy");
    else return false;
  }
  else return false;
}

// muons and electron isolation                                                                                                                                                     
const float reco::SkimEvent::pfNeutralHadronsIso  (size_t i) const {
  if(i >= leps_.size()) return -9999 ;
  if(isMuon(i)){
    return getMuon(i)->neutralHadronIso();  
  }
  else if(isElectron(i)){
    return getElectron(i)->neutralHadronIso();
  }  
  else  return -9999 ;
}

const float reco::SkimEvent::pfParticleAllIso  (size_t i) const {
  if(i >= leps_.size()) return -9999 ;
  if(isMuon(i)) return getMuon(i)->particleIso();  
  else if(isElectron(i)) return getElectron(i)->particleIso();  
  else  return -9999 ;
}

const float reco::SkimEvent::pfPUChargedHadronIso (size_t i) const {
  if(i >= leps_.size()) return -9999 ;
  if(isMuon(i)) return getMuon(i)->puChargedHadronIso();  
  else if(isElectron(i)) return getElectron(i)->puChargedHadronIso();  
  else  return -9999 ;

}
const float reco::SkimEvent::pfChargedHadronsIso  (size_t i) const {
  if(i >= leps_.size()) return -9999 ;
  if(isMuon(i)) return getMuon(i)->chargedHadronIso();  
  else if(isElectron(i)) return getElectron(i)->chargedHadronIso();  
  else  return -9999 ;

}
const float reco::SkimEvent::pfPhotonsIso         (size_t i) const {
  if(i >= leps_.size()) return -9999 ;
  if(isMuon(i)) return getMuon(i)->photonIso();  
  else if(isElectron(i)) return getElectron(i)->photonIso();  
  else  return -9999 ;

}

///////////

const bool reco::SkimEvent::isMuTriggered(size_t i) const {
    if( isMuon(i) ) {
        const pat::Muon* mu = getMuon(i);
        return ( !mu->triggerObjectMatchesByPath("HLT_Mu9").empty() || !mu->triggerObjectMatchesByPath("HLT_Mu15_v1").empty() );
    } else {
        return false;
    }
}


const int reco::SkimEvent::mitType() const {
    if( abs(pdgIdByPt(0)) == 11 ) {
        if( abs(pdgIdByPt(1)) == 11 ) return 11;
        if( abs(pdgIdByPt(1)) == 13 ) return 12;
    } else if( abs(pdgIdByPt(0)) == 13 ) {
        if( abs(pdgIdByPt(1)) == 11 ) return 13;
        if( abs(pdgIdByPt(1)) == 13 ) return 10;
    } else {
        return -1;
    }
  return -1;
}


const int reco::SkimEvent::nJetVBF(float minPt,float eta,int applyCorrection, int applyJetID, int applyPileUpJetID) const {
    if(jets_.size() < 2) return -9999.;
    float jetEta1 = leadingJetEta(0,minPt,eta,applyCorrection,applyJetID,applyPileUpJetID);
    float jetEta2 = leadingJetEta(1,minPt,eta,applyCorrection,applyJetID,applyPileUpJetID);
    float jetEtaMax = std::max(jetEta1,jetEta2);
    float jetEtaMin = std::min(jetEta1,jetEta2);

    int count = 0;
    for(size_t i=0;i<jets_.size();++i) {
      if(!(passJetID(jets_[i],applyJetID)) ) continue;
      if(!(passPileUpJetID(jets_[i],applyPileUpJetID)) ) continue;
      if( std::fabs(jets_[i]->eta()) >= eta) continue;
      if( jetPt(jets_[i].get(),applyCorrection) <= minPt) continue;

      if(isThisJetALepton(jets_[i])) continue;
      if( jets_[i]->eta() >= jetEtaMax ) continue;
      if( jets_[i]->eta() <= jetEtaMin ) continue;
      count++;
    }
    return count;

}

const int reco::SkimEvent::nPuppiJetVBF(float minPt,float eta,int applyCorrection, int applyJetID, int applyPileUpJetID) const {
    if(puppiJets_.size() < 2) return -9999.;
    float jetEta1 = leadingJetEta(0,minPt,eta,applyCorrection,applyJetID,applyPileUpJetID);
    float jetEta2 = leadingJetEta(1,minPt,eta,applyCorrection,applyJetID,applyPileUpJetID);
    float jetEtaMax = std::max(jetEta1,jetEta2);
    float jetEtaMin = std::min(jetEta1,jetEta2);

    int count = 0;
    for(size_t i=0;i<puppiJets_.size();++i) {
      if(!(passJetID(puppiJets_[i],applyJetID)) ) continue;
      if(!(passPileUpJetID(puppiJets_[i],applyPileUpJetID)) ) continue;
      if( std::fabs(puppiJets_[i]->eta()) >= eta) continue;
      if( jetPt(puppiJets_[i].get(),applyCorrection) <= minPt) continue;

      if(isThisJetALepton(puppiJets_[i])) continue;
      if( puppiJets_[i]->eta() >= jetEtaMax ) continue;
      if( puppiJets_[i]->eta() <= jetEtaMin ) continue;
      count++;
    }
    return count;

}

const float reco::SkimEvent::mjj(float pt ,float eta,int applyCorrection, int applyJetID, int applyPileUpJetID) const {
    if(jets_.size() < 2) return -9999.;
    return (leadingJet(0,pt,eta,applyCorrection,applyJetID,applyPileUpJetID)->p4() + leadingJet(1,pt,eta,applyCorrection,applyJetID,applyPileUpJetID)->p4()).mass();

}

const float reco::SkimEvent::puppiMjj(float pt ,float eta,int applyCorrection, int applyJetID, int applyPileUpJetID) const {
    if(puppiJets_.size() < 2) return -9999.;
    return (leadingPuppiJet(0,pt,eta,applyCorrection,applyJetID,applyPileUpJetID)->p4() + leadingPuppiJet(1,pt,eta,applyCorrection,applyJetID,applyPileUpJetID)->p4()).mass();

}

const float reco::SkimEvent::dEtajj(float pt ,float eta,int applyCorrection, int applyJetID, int applyPileUpJetID) const {
    if(jets_.size() < 2) return -9999.;
    return fabs(leadingJetEta(0,pt,eta,applyCorrection,applyJetID,applyPileUpJetID) -
                leadingJetEta(1,pt,eta,applyCorrection,applyJetID,applyPileUpJetID));

}

const float reco::SkimEvent::puppidEtajj(float pt ,float eta,int applyCorrection, int applyJetID, int applyPileUpJetID) const {
    if(puppiJets_.size() < 2) return -9999.;
    return fabs(leadingPuppiJetEta(0,pt,eta,applyCorrection,applyJetID,applyPileUpJetID) -
                leadingPuppiJetEta(1,pt,eta,applyCorrection,applyJetID,applyPileUpJetID));

}

const float reco::SkimEvent::zeppenfeld(size_t a,float pt ,float eta,int applyCorrection, int applyJetID, int applyPileUpJetID) const {
    if(jets_.size() < 2) return -9999.;
    return -9999.;

}

const float reco::SkimEvent::zeppenfeldPuppi(size_t a,float pt ,float eta,int applyCorrection, int applyJetID, int applyPileUpJetID) const {
    if(puppiJets_.size() < 2) return -9999.;
    return -9999.;

}



void reco::SkimEvent::FindDaughterParticles(const reco::Candidate ** pCurrent, std::vector<const reco::Candidate*>* pFinal) const {
  
  // Variable for decayed current particle
  const reco::Candidate* pCurrentNew = 0;
  
  for(unsigned int i = 0; i < (*pCurrent) -> numberOfDaughters(); ++i) {
    if( (pFinal != 0) && ((*pCurrent) -> daughter(i) -> pdgId() != (*pCurrent) -> pdgId()) ) pFinal -> push_back((*pCurrent) -> daughter(i));
    
    if((*pCurrent) -> daughter(i) -> pdgId() == (*pCurrent) -> pdgId()) pCurrentNew = (*pCurrent) -> daughter(i);
  } 
  // Change the current particle into the decayed one
  if(pCurrentNew) (*pCurrent) = pCurrentNew;
}



//---- DY > ll : DY decay final state (mumu,ee,tautau)
const float reco::SkimEvent::getFinalStateMC() const {

  float finalState = -1;

  const reco::Candidate* mcV = 0;
  const reco::Candidate* mcF1_fromV; // fermion 1 from V
  const reco::Candidate* mcF2_fromV; // fermion 2 from V

  // loop over gen particles
  for(size_t gp=0; gp<genParticles_.size();++gp){

    int pdgId  = genParticles_[gp] -> pdgId();
    int status = genParticles_[gp] -> status();

    if((pdgId == 23) && (status == 3) ) { // take a vector boson of status3 and Z
      mcV = &(*(genParticles_[gp]));
    }
    
  } // loop over gen particles
    
  if (mcV != 0) {

   // find fermions from vector boson decay
   std::vector<const reco::Candidate*> fFromVBuffer;
   FindDaughterParticles(&mcV, &fFromVBuffer);
    
   if (fFromVBuffer.size() == 2) {

    mcF1_fromV = fFromVBuffer.at(0);
    mcF2_fromV = fFromVBuffer.at(1);
  
    // If leptons, see if there is a photon emission
    if(abs(mcF1_fromV -> pdgId()) >= 11) FindDaughterParticles(&mcF1_fromV);
    
    if(abs(mcF2_fromV -> pdgId()) >= 11) FindDaughterParticles(&mcF2_fromV);
    

    // mm from Z decay 
    if ( abs(mcF1_fromV -> pdgId()) == 13 && abs(mcF2_fromV -> pdgId()) == 13 )  finalState = 0;
       
    // ee from Z ddecay 
    if ( abs(mcF1_fromV -> pdgId()) == 11 && abs(mcF2_fromV -> pdgId()) == 11 )  finalState = 1;
    
    // tt
    if ( abs(mcF1_fromV -> pdgId()) == 15 && abs(mcF2_fromV -> pdgId()) == 15 ) finalState = 2;
    
   }
  }

  return finalState;
}



//---- H > WW > lvlv : WW decay final state
const float reco::SkimEvent::getWWdecayMC() const {

  float finalState = -1;
  // 0 = mm
  // 1 = ee
  // 2 = tt
  // 3 = em
  // 4 = et
  // 5 = mt
  // 6 = jj - ev
  // 7 = jj - mv
  // 8 = jj - tv
  // 9 = jj - jj


  const reco::Candidate* mcH = 0;

  // loop over gen particles
  for(size_t gp=0; gp<genParticles_.size();++gp){

    int pdgId  = genParticles_[gp] -> pdgId();
    int status = genParticles_[gp] -> status();
    if( (pdgId == 25) && (status == 3) ) {
      mcH = &(*(genParticles_[gp]));
    }
  } // loop over gen particles
  
    
  // find vector bosons
  if (mcH != 0) { 
   std::vector<const reco::Candidate*> VfromHBuffer;
   FindDaughterParticles(&mcH,&VfromHBuffer);

   // H > VV
   if(VfromHBuffer.size() == 2) { // VV decay
    const reco::Candidate* mcV1;
    const reco::Candidate* mcV2;

    mcV1 = VfromHBuffer.at(0);
    mcV2 = VfromHBuffer.at(1);

    const reco::Candidate* mcF1_fromV1;
    const reco::Candidate* mcF2_fromV1;
    const reco::Candidate* mcF1_fromV2;
    const reco::Candidate* mcF2_fromV2;
  
    bool isHWWok = true;
    std::vector<const reco::Candidate*> fFromV1Buffer;
    FindDaughterParticles(&mcV1,&fFromV1Buffer);
    if(fFromV1Buffer.size() == 2) {
     mcF1_fromV1 = fFromV1Buffer.at(0);
     mcF2_fromV1 = fFromV1Buffer.at(1);

     // If leptons, see if there is a photon emission
     if(abs(mcF1_fromV1 -> pdgId()) >= 11) FindDaughterParticles(&mcF1_fromV1);     
     if(abs(mcF2_fromV1 -> pdgId()) >= 11) FindDaughterParticles(&mcF2_fromV1);
     
    }
    else {
     isHWWok = false;
    }

    std::vector<const reco::Candidate*> fFromV2Buffer;
    FindDaughterParticles(&mcV2,&fFromV2Buffer);
    if(fFromV2Buffer.size() == 2) {
     mcF1_fromV2 = fFromV2Buffer.at(0);
     mcF2_fromV2 = fFromV2Buffer.at(1);
     // If leptons, see if there is a photon emission
     if(abs(mcF1_fromV2 -> pdgId()) >= 11) FindDaughterParticles(&mcF1_fromV2);     
     if(abs(mcF2_fromV2 -> pdgId()) >= 11) FindDaughterParticles(&mcF2_fromV2);     
    }
    else {
     isHWWok = false;
    }
    
    if (isHWWok) {

     // mm
     if ( (abs(mcF1_fromV1 -> pdgId()) == 13 || abs(mcF1_fromV1 -> pdgId()) == 14) && (abs(mcF1_fromV2 -> pdgId()) == 13 || abs(mcF1_fromV2 -> pdgId()) == 14) ) finalState = 0;
     
     // ee
     if ( (abs(mcF1_fromV1 -> pdgId()) == 11 || abs(mcF1_fromV1 -> pdgId()) == 12) && (abs(mcF1_fromV2 -> pdgId()) == 11 || abs(mcF1_fromV2 -> pdgId()) == 12) ) finalState = 1;
     
     // tt
     if ( (abs(mcF1_fromV1 -> pdgId()) == 15 || abs(mcF1_fromV1 -> pdgId()) == 16) && (abs(mcF1_fromV2 -> pdgId()) == 15 || abs(mcF1_fromV2 -> pdgId()) == 16) ) finalState = 2;
     
     // em
     if ( ( (abs(mcF1_fromV1 -> pdgId()) == 11 || abs(mcF1_fromV1 -> pdgId()) == 12) && (abs(mcF1_fromV2 -> pdgId()) == 13 || abs(mcF1_fromV2 -> pdgId()) == 14) )
      || ( (abs(mcF1_fromV1 -> pdgId()) == 13 || abs(mcF1_fromV1 -> pdgId()) == 14) && (abs(mcF1_fromV2 -> pdgId()) == 11 || abs(mcF1_fromV2 -> pdgId()) == 12) ) ) finalState = 3;
     
     // et
     if ( ( (abs(mcF1_fromV1 -> pdgId()) == 11 || abs(mcF1_fromV1 -> pdgId()) == 12) && (abs(mcF1_fromV2 -> pdgId()) == 15 || abs(mcF1_fromV2 -> pdgId()) == 16) )
      || ( (abs(mcF1_fromV1 -> pdgId()) == 15 || abs(mcF1_fromV1 -> pdgId()) == 16) && (abs(mcF1_fromV2 -> pdgId()) == 11 || abs(mcF1_fromV2 -> pdgId()) == 12) ) ) finalState = 4;
     

     // mt
     if ( ( (abs(mcF1_fromV1 -> pdgId()) == 12 || abs(mcF1_fromV1 -> pdgId()) == 13) && (abs(mcF1_fromV2 -> pdgId()) == 15 || abs(mcF1_fromV2 -> pdgId()) == 16) )
      || ( (abs(mcF1_fromV1 -> pdgId()) == 15 || abs(mcF1_fromV1 -> pdgId()) == 16) && (abs(mcF1_fromV2 -> pdgId()) == 12 || abs(mcF1_fromV2 -> pdgId()) == 13) ) ) finalState = 5;
     

     // jj - ev
      if (( (abs(mcF1_fromV1 -> pdgId()) <= 6 ) && (abs(mcF1_fromV2 -> pdgId()) == 11 || abs(mcF1_fromV2 -> pdgId()) == 12) ) ||
          ( (abs(mcF1_fromV2 -> pdgId()) <= 6 ) && (abs(mcF1_fromV1 -> pdgId()) == 11 || abs(mcF1_fromV1 -> pdgId()) == 12) )
         ) finalState = 6;
     

     // jj - mv
     if (( (abs(mcF1_fromV1 -> pdgId()) <= 6 ) && (abs(mcF1_fromV2 -> pdgId()) == 13 || abs(mcF1_fromV2 -> pdgId()) == 14) ) ||
         ( (abs(mcF1_fromV2 -> pdgId()) <= 6 ) && (abs(mcF1_fromV1 -> pdgId()) == 13 || abs(mcF1_fromV1 -> pdgId()) == 14) )
        ) finalState = 7;
     

     // jj - tv
     if (( (abs(mcF1_fromV1 -> pdgId()) <= 6 ) && (abs(mcF1_fromV2 -> pdgId()) == 15 || abs(mcF1_fromV2 -> pdgId()) == 16) ) ||
         ( (abs(mcF1_fromV2 -> pdgId()) <= 6 ) && (abs(mcF1_fromV1 -> pdgId()) == 15 || abs(mcF1_fromV1 -> pdgId()) == 16) )
        ) finalState = 8;
     

     // jj - jj
     if (( (abs(mcF1_fromV1 -> pdgId()) <= 6 ) && (abs(mcF1_fromV2 -> pdgId()) <= 6 ) ) ||
         ( (abs(mcF1_fromV2 -> pdgId()) <= 6 ) && (abs(mcF1_fromV1 -> pdgId()) <= 6 ) )) finalState = 9;
     
    }
    else {
     finalState = -2;
    }
   }
  }
    
  return finalState;
}

//---- HEP MC information
//---- http://cmssdt.cern.ch/SDT/doxygen/CMSSW_3_6_0/doc/html/d5/db1/GenEventInfoProduct_8h-source.html
//---- HEPMC wieght. Used for minlo-powheg reweighting

const float reco::SkimEvent::getHEPMCweight() const {
 float HEPMCweight = -1;
 HEPMCweight = GenInfoHandle_.weight();
 return HEPMCweight;
}


//---- H production mode: ggH, vbf, WH, ZH, ttH

const float reco::SkimEvent::getMCHiggsProd() const {
 
// std::cout << " mcHiggsProd " << std::endl;
  float productionMechanism = -1;
  // pythia coding
  // ## = ggH
  // ## = vbf (qqH)
  // 24 = ZH
  // 26 = WH
  // 121 = ttH

  // std::cout << " event flag = " << GenInfoHandle_.signalProcessID() << std::endl;
  productionMechanism = GenInfoHandle_.signalProcessID() ;
  return productionMechanism;
}


//---- Higgs masses
const float reco::SkimEvent::getHiggsMass() const {

// std::cout << " getSusyMass1 " << std::endl;
 float mass = -1;

 const reco::Candidate* mcH = 0;

  // loop over gen particles
 for(size_t gp=0; gp<genParticles_.size();++gp){

  int pdgId  = genParticles_[gp] -> pdgId();
  int status = genParticles_[gp] -> status();
   
    // Stop {1000006}
  if( (pdgId == 25) && (status == 3) ) {
   mcH = &(*(genParticles_[gp]));
   mass = mcH->mass();
  }
 } // loop over gen particles

 return mass;
}



const float reco::SkimEvent::getHiggsPt() const {

// std::cout << " getSusyMass1 " << std::endl;
 float pt = -1;

 const reco::Candidate* mcH = 0;

  // loop over gen particles
 for(size_t gp=0; gp<genParticles_.size();++gp){

  int pdgId  = genParticles_[gp] -> pdgId();
  int status = genParticles_[gp] -> status();
   
    // Stop {1000006}
  if( (pdgId == 25) && (status == 3) ) {
   mcH = &(*(genParticles_[gp]));
   pt = mcH->pt();
  }
 } // loop over gen particles

 return pt;
}



//---- Susy masses

const float reco::SkimEvent::getSusyStopMass() const {

  float mass = -1;
  const reco::Candidate* mcStop = 0;

  // loop over gen particles
  for(size_t gp=0; gp<genParticles_.size();++gp){

    int pdgId = genParticles_[gp] -> pdgId();
    int status = genParticles_[gp] -> status();

    if( (abs(pdgId) == 1000006 || abs(pdgId) == 2000006) && (status == 3) ) {
      mcStop = &(*(genParticles_[gp]));
      mass = mcStop->mass();
    }
  } // loop over gen particles

  return mass;
}


const float reco::SkimEvent::getSusyLSPMass() const {

 float mass = -1;

 const reco::Candidate* mcChi = 0;

 // loop over gen particles
 for(size_t gp=0; gp<genParticles_.size();++gp){

  int pdgId = genParticles_[gp] -> pdgId();
  int status = genParticles_[gp] -> status();

    // LSP {1000022, 1000023, 1000025, 1000035}
  if( (abs(pdgId) == 1000022 || abs(pdgId) == 1000023 || abs(pdgId) == 1000025 || abs(pdgId) == 1000035) && (status == 3) ) {
   mcChi = &(*(genParticles_[gp]));
   mass = mcChi->mass();
  }
 } // loop over gen particles

 return mass;
}



const float reco::SkimEvent::getPDFscalePDF() const {
 float scale=-9999.9;
 scale= (GenInfoHandle_.pdf())->scalePDF;
 return scale;
}

const float reco::SkimEvent::getPDFx1() const {
 float x=-9999.9;
 x= ((GenInfoHandle_.pdf())->x).first;
 return x;
}

const float reco::SkimEvent::getPDFx2() const {
 float x=-9999.9;
 x= ((GenInfoHandle_.pdf())->x).second;
 return x;
}

const float reco::SkimEvent::getPDFid1() const {
 float id=-9999.9;
 id= ((GenInfoHandle_.pdf())->id).first;
 return id;
}

const float reco::SkimEvent::getPDFid2() const {
 float id=-9999.9;
 id= ((GenInfoHandle_.pdf())->id).second;
 return id;
}

const float reco::SkimEvent::getPDFx1PDF() const {
 float xPDF=-9999.9;
 xPDF= ((GenInfoHandle_.pdf())->xPDF).first;
 return xPDF;
}

const float reco::SkimEvent::getPDFx2PDF() const {
 float xPDF=-9999.9;
 xPDF= ((GenInfoHandle_.pdf())->xPDF).second;
 return xPDF;
}


///---- save LHE information ----
const float reco::SkimEvent::leadingLHEJetPt(size_t index) const {
 std::vector<float> v_jetsLHE_pt ;
 // loop over particles in the event
 for (unsigned int iPart = 0 ; iPart < LHEhepeup_.IDUP.size (); ++iPart) {
  if (LHEhepeup_.ISTUP.at (iPart) != 1) continue ;
  int type = abs (LHEhepeup_.IDUP.at (iPart)) ;
  if ((type < 9 && type > 0) || type == 21) {
   v_jetsLHE_pt.push_back (
     sqrt (LHEhepeup_.PUP.at (iPart) [0] * LHEhepeup_.PUP.at (iPart) [0] + // px
           LHEhepeup_.PUP.at (iPart) [1] * LHEhepeup_.PUP.at (iPart) [1]) // py
    );
  }
 }
 if (v_jetsLHE_pt.size () > 0) {
  std::sort (v_jetsLHE_pt.rbegin (), v_jetsLHE_pt.rend ()) ;
 }
 //---- now return ----
 size_t count = 0;
 for(size_t i=0;i<v_jetsLHE_pt.size();++i) {
  if(++count > index) return v_jetsLHE_pt.at(i);
 }
 return -9999.9;
}

const float reco::SkimEvent::leadingLHEJetPID(size_t index) const {
 float pt_ofIndex = leadingLHEJetPt(index);
 float particleID=-9999.9;
 // loop over particles in the event
 for (unsigned int iPart = 0 ; iPart < LHEhepeup_.IDUP.size (); ++iPart) {
  if (LHEhepeup_.ISTUP.at (iPart) != 1) continue ;
  int type = abs (LHEhepeup_.IDUP.at (iPart)) ;
  if ((type < 9 && type > 0) || type == 21) {
      float iPart_Pt = sqrt (LHEhepeup_.PUP.at (iPart) [0] * LHEhepeup_.PUP.at (iPart) [0] + // px
                               LHEhepeup_.PUP.at (iPart) [1] * LHEhepeup_.PUP.at (iPart) [1]); // py
      if(iPart_Pt!=pt_ofIndex) continue;
      particleID = (float) type;
  }
 }
 //---- now return ----
 return particleID;
}

const float reco::SkimEvent::leadingLHEJetPhi(size_t index) const {
 float pt_ofIndex = leadingLHEJetPt(index);
 float phi=-9999.9;
 // loop over particles in the event
 for (unsigned int iPart = 0 ; iPart < LHEhepeup_.IDUP.size (); ++iPart) {
  if (LHEhepeup_.ISTUP.at (iPart) != 1) continue ;
  int type = abs (LHEhepeup_.IDUP.at (iPart)) ;
  if ((type < 9 && type > 0) || type == 21) {
      float iPart_Pt = sqrt (LHEhepeup_.PUP.at (iPart) [0] * LHEhepeup_.PUP.at (iPart) [0] + // px
                               LHEhepeup_.PUP.at (iPart) [1] * LHEhepeup_.PUP.at (iPart) [1]); // py
      if(iPart_Pt!=pt_ofIndex) continue;
      TVector3 pt_ofIndex(LHEhepeup_.PUP.at (iPart) [0], LHEhepeup_.PUP.at (iPart) [1], LHEhepeup_.PUP.at (iPart) [2] ); // pass px, py, pz
      phi = pt_ofIndex.Phi() ;
  }
 }

 //---- now return ----
 return phi;
}

const float reco::SkimEvent::leadingLHEJetEta(size_t index) const {
 float pt_ofIndex = leadingLHEJetPt(index);
 float eta=-9999.9;
 // loop over particles in the event
 for (unsigned int iPart = 0 ; iPart < LHEhepeup_.IDUP.size (); ++iPart) {
  if (LHEhepeup_.ISTUP.at (iPart) != 1) continue ;
  int type = abs (LHEhepeup_.IDUP.at (iPart)) ;
  if ((type < 9 && type > 0) || type == 21) {
      float iPart_Pt = sqrt (LHEhepeup_.PUP.at (iPart) [0] * LHEhepeup_.PUP.at (iPart) [0] + // px
                               LHEhepeup_.PUP.at (iPart) [1] * LHEhepeup_.PUP.at (iPart) [1]); // py
      if(iPart_Pt!=pt_ofIndex) continue;
      TVector3 pt_ofIndex(LHEhepeup_.PUP.at (iPart) [0], LHEhepeup_.PUP.at (iPart) [1], LHEhepeup_.PUP.at (iPart) [2] ); // pass px, py, pz
      eta = pt_ofIndex.PseudoRapidity() ;
  }
 }
 //---- now return ----
 return eta;
}

const float reco::SkimEvent::leadingLHELeptonPt(size_t index) const {
 std::vector<float> v_jetsLHE_pt ;
 // loop over particles in the event
 for (unsigned int iPart = 0 ; iPart < LHEhepeup_.IDUP.size (); ++iPart) {
  if (LHEhepeup_.ISTUP.at (iPart) != 1) continue ;
  int type = abs (LHEhepeup_.IDUP.at (iPart)) ;
  if (type == 11 || type == 13 || type == 15) {
   v_jetsLHE_pt.push_back (
     sqrt (LHEhepeup_.PUP.at (iPart) [0] * LHEhepeup_.PUP.at (iPart) [0] + // px
           LHEhepeup_.PUP.at (iPart) [1] * LHEhepeup_.PUP.at (iPart) [1]) // py
    );
  }
 }
 if (v_jetsLHE_pt.size () > 0) {
  std::sort (v_jetsLHE_pt.rbegin (), v_jetsLHE_pt.rend ()) ;
 }
 //---- now return ----
 size_t count = 0;
 for(size_t i=0;i<v_jetsLHE_pt.size();++i) {
  if(++count > index) return v_jetsLHE_pt.at(i);
 }

 return -9999.9;
}

const float reco::SkimEvent::leadingLHELeptonPID(size_t index) const {
 float pt_ofIndex = leadingLHELeptonPt(index);
 float particleID=-9999.9;
 // loop over particles in the event
 for (unsigned int iPart = 0 ; iPart < LHEhepeup_.IDUP.size (); ++iPart) {
  if (LHEhepeup_.ISTUP.at (iPart) != 1) continue ; //incoming particle / beam, we only want intermediate or outgoing particles
  int type = abs (LHEhepeup_.IDUP.at (iPart)) ;

  if (type == 11 || type == 13 || type == 15) {
      float iPart_Pt = sqrt (LHEhepeup_.PUP.at (iPart) [0] * LHEhepeup_.PUP.at (iPart) [0] + // px
                               LHEhepeup_.PUP.at (iPart) [1] * LHEhepeup_.PUP.at (iPart) [1]); // py
      if(iPart_Pt!=pt_ofIndex) continue;
      particleID = (float) type;
      //std::cout << "particleID " << particleID << std::endl;
  }
 }
 //---- now return ----
 return particleID;
}

const float reco::SkimEvent::leadingLHELeptonPhi(size_t index) const {
 float pt_ofIndex = leadingLHELeptonPt(index);
 float phi=-9999.9;
 // loop over particles in the event
 for (unsigned int iPart = 0 ; iPart < LHEhepeup_.IDUP.size (); ++iPart) {
  if (LHEhepeup_.ISTUP.at (iPart) != 1) continue ;
  int type = abs (LHEhepeup_.IDUP.at (iPart)) ;
  if (type == 11 || type == 13 || type == 15) { //---- quarks or gluons
      float iPart_Pt = sqrt (LHEhepeup_.PUP.at (iPart) [0] * LHEhepeup_.PUP.at (iPart) [0] + // px
                               LHEhepeup_.PUP.at (iPart) [1] * LHEhepeup_.PUP.at (iPart) [1]); // py
      if(iPart_Pt!=pt_ofIndex) continue;
      TVector3 pt_ofIndex(LHEhepeup_.PUP.at (iPart) [0], LHEhepeup_.PUP.at (iPart) [1], LHEhepeup_.PUP.at (iPart) [2] ); // pass px, py, pz
      phi = pt_ofIndex.Phi() ;
  }
 }

 //---- now return ----
 return phi;
}

const float reco::SkimEvent::leadingLHELeptonEta(size_t index) const {
 float pt_ofIndex = leadingLHELeptonPt(index);
 float eta=-9999.9;
 // loop over particles in the event
 for (unsigned int iPart = 0 ; iPart < LHEhepeup_.IDUP.size (); ++iPart) {
  if (LHEhepeup_.ISTUP.at (iPart) != 1) continue ;
  int type = abs (LHEhepeup_.IDUP.at (iPart)) ;
  if (type == 11 || type == 13 || type == 15) { //---- quarks or gluons
      float iPart_Pt = sqrt (LHEhepeup_.PUP.at (iPart) [0] * LHEhepeup_.PUP.at (iPart) [0] + // px
                               LHEhepeup_.PUP.at (iPart) [1] * LHEhepeup_.PUP.at (iPart) [1]); // py
      if(iPart_Pt!=pt_ofIndex) continue;
      TVector3 pt_ofIndex(LHEhepeup_.PUP.at (iPart) [0], LHEhepeup_.PUP.at (iPart) [1], LHEhepeup_.PUP.at (iPart) [2] ); // pass px, py, pz
      eta = pt_ofIndex.PseudoRapidity() ;
  }
 }
 //---- now return ----
 return eta;
}

const float reco::SkimEvent::leadingLHENeutrinoPt(size_t index) const {
 std::vector<float> v_jetsLHE_pt ;
 // loop over particles in the event
 for (unsigned int iPart = 0 ; iPart < LHEhepeup_.IDUP.size (); ++iPart) {
  if (LHEhepeup_.ISTUP.at (iPart) != 1) continue ;
  int type = abs (LHEhepeup_.IDUP.at (iPart)) ;
  if (type == 12 || type == 14 || type == 16) {
   v_jetsLHE_pt.push_back (
     sqrt (LHEhepeup_.PUP.at (iPart) [0] * LHEhepeup_.PUP.at (iPart) [0] + // px
           LHEhepeup_.PUP.at (iPart) [1] * LHEhepeup_.PUP.at (iPart) [1]) // py
    );
  }
 }
 if (v_jetsLHE_pt.size () > 0) {
  std::sort (v_jetsLHE_pt.rbegin (), v_jetsLHE_pt.rend ()) ;
 }
 //---- now return ----
 size_t count = 0;
 for(size_t i=0;i<v_jetsLHE_pt.size();++i) {
  if(++count > index) return v_jetsLHE_pt.at(i);
 }

 return -9999.9;
}

const float reco::SkimEvent::leadingLHENeutrinoPID(size_t index) const {
 float pt_ofIndex = leadingLHENeutrinoPt(index);
 float particleID=-9999.9;
 // loop over particles in the event
 for (unsigned int iPart = 0 ; iPart < LHEhepeup_.IDUP.size (); ++iPart) {
  if (LHEhepeup_.ISTUP.at (iPart) != 1) continue ; //incoming particle / beam, we only want intermediate or outgoing particles
  int type = abs (LHEhepeup_.IDUP.at (iPart)) ;
  if (type == 12 || type == 14 || type == 16) {
      float iPart_Pt = sqrt (LHEhepeup_.PUP.at (iPart) [0] * LHEhepeup_.PUP.at (iPart) [0] + // px
                               LHEhepeup_.PUP.at (iPart) [1] * LHEhepeup_.PUP.at (iPart) [1]); // py
      if(iPart_Pt!=pt_ofIndex) continue;
      particleID = (float) type;
  }
 }
 //---- now return ----
 return particleID;
}

const float reco::SkimEvent::leadingLHENeutrinoPhi(size_t index) const {
 float pt_ofIndex = leadingLHENeutrinoPt(index);
 float phi=-9999.9;
 // loop over particles in the event
 for (unsigned int iPart = 0 ; iPart < LHEhepeup_.IDUP.size (); ++iPart) {
  if (LHEhepeup_.ISTUP.at (iPart) != 1) continue ;
  int type = abs (LHEhepeup_.IDUP.at (iPart)) ;
  if (type == 12 || type == 14 || type == 16) {
      float iPart_Pt = sqrt (LHEhepeup_.PUP.at (iPart) [0] * LHEhepeup_.PUP.at (iPart) [0] + // px
                               LHEhepeup_.PUP.at (iPart) [1] * LHEhepeup_.PUP.at (iPart) [1]); // py
      if(iPart_Pt!=pt_ofIndex) continue;
      TVector3 pt_ofIndex(LHEhepeup_.PUP.at (iPart) [0], LHEhepeup_.PUP.at (iPart) [1], LHEhepeup_.PUP.at (iPart) [2] ); // pass px, py, pz
      phi = pt_ofIndex.Phi() ;
  }
 }

 //---- now return ----
 return phi;
}

const float reco::SkimEvent::leadingLHENeutrinoEta(size_t index) const {
 float pt_ofIndex = leadingLHENeutrinoPt(index);
 float eta=-9999.9;
 // loop over particles in the event
 for (unsigned int iPart = 0 ; iPart < LHEhepeup_.IDUP.size (); ++iPart) {
  if (LHEhepeup_.ISTUP.at (iPart) != 1) continue ;
  int type = abs (LHEhepeup_.IDUP.at (iPart)) ;
  if (type == 12 || type == 14 || type == 16) {
      float iPart_Pt = sqrt (LHEhepeup_.PUP.at (iPart) [0] * LHEhepeup_.PUP.at (iPart) [0] + // px
                               LHEhepeup_.PUP.at (iPart) [1] * LHEhepeup_.PUP.at (iPart) [1]); // py
      if(iPart_Pt!=pt_ofIndex) continue;
      TVector3 pt_ofIndex(LHEhepeup_.PUP.at (iPart) [0], LHEhepeup_.PUP.at (iPart) [1], LHEhepeup_.PUP.at (iPart) [2] ); // pass px, py, pz
      eta = pt_ofIndex.PseudoRapidity() ;
  }
 }
 //---- now return ----
 return eta;
}

const float reco::SkimEvent::LHEMetPt() const {
 float sum_px=0;
 float sum_py=0;
 float final_pT=-9999.9;
 int number_neutrino=0;
 // loop over particles in the event
 for (unsigned int iPart = 0 ; iPart < LHEhepeup_.IDUP.size (); ++iPart) {
  if (LHEhepeup_.ISTUP.at (iPart) != 1) continue ;
  int type = abs (LHEhepeup_.IDUP.at (iPart)) ;
  if (type == 12 || type == 14 || type == 16) {
      sum_px+=LHEhepeup_.PUP.at (iPart) [0];
      sum_py+=LHEhepeup_.PUP.at (iPart) [1];
      number_neutrino++;
  }
 }
 //---- now return ----
 if(number_neutrino>0) final_pT=sqrt( sum_px * sum_px + sum_py * sum_py );

 return final_pT;
}

const float reco::SkimEvent::LHEMetPhi() const {
 float sum_px=0;
 float sum_py=0;
 float sum_pz=0;
 int number_neutrino=0;
 float phi=-9999.9;
 // loop over particles in the event
 for (unsigned int iPart = 0 ; iPart < LHEhepeup_.IDUP.size (); ++iPart) {
  if (LHEhepeup_.ISTUP.at (iPart) != 1) continue ;
  int type = abs (LHEhepeup_.IDUP.at (iPart)) ;
  if (type == 12 || type == 14 || type == 16) {
      sum_px+=LHEhepeup_.PUP.at (iPart) [0];
      sum_py+=LHEhepeup_.PUP.at (iPart) [1];
      sum_pz+=LHEhepeup_.PUP.at (iPart) [2];
      number_neutrino++;
  }
 }
 //now return
 if(number_neutrino==0) return phi;
 TVector3 pt_ofIndex(sum_px, sum_py, sum_pz ); // pass px, py, pz
 phi = pt_ofIndex.Phi() ;
 return phi;
}

const float reco::SkimEvent::LHEMetEta() const {
 float sum_px=0;
 float sum_py=0;
 float sum_pz=0;
 int number_neutrino=0;
 float eta=-9999.9;
 // loop over particles in the event
 for (unsigned int iPart = 0 ; iPart < LHEhepeup_.IDUP.size (); ++iPart) {
  if (LHEhepeup_.ISTUP.at (iPart) != 1) continue ;
  int type = abs (LHEhepeup_.IDUP.at (iPart)) ;
  if (type == 12 || type == 14 || type == 16) {
      sum_px+=LHEhepeup_.PUP.at (iPart) [0];
      sum_py+=LHEhepeup_.PUP.at (iPart) [1];
      sum_pz+=LHEhepeup_.PUP.at (iPart) [2];
      number_neutrino++;
  }
 }
 //---- now return ----
 if(number_neutrino==0) return eta;
 TVector3 pt_ofIndex(sum_px, sum_py, sum_pz ); // pass px, py, pz
 return eta;
}

const float reco::SkimEvent::higgsLHEPt() const {
 std::vector<float> v_jetsLHE_pt ;
 // loop over particles in the event
 for (unsigned int iPart = 0 ; iPart < LHEhepeup_.IDUP.size (); ++iPart) {
  if (LHEhepeup_.ISTUP.at (iPart) != 1) continue ;
  int type = abs (LHEhepeup_.IDUP.at (iPart)) ;
  if (type ==25) { //---- Higgs
   v_jetsLHE_pt.push_back (
     sqrt (LHEhepeup_.PUP.at (iPart) [0] * LHEhepeup_.PUP.at (iPart) [0] + // px
           LHEhepeup_.PUP.at (iPart) [1] * LHEhepeup_.PUP.at (iPart) [1]) // py
    );
  }
 }
 if (v_jetsLHE_pt.size () > 0) {
  std::sort (v_jetsLHE_pt.rbegin (), v_jetsLHE_pt.rend ()) ;
 }
 //---- now return ----
 if ( 0 < v_jetsLHE_pt.size() ) return v_jetsLHE_pt.at(0);

 return -9999.9; //if no Higgs was found
}

const float reco::SkimEvent::leadingGenJetPartonPt(size_t index) const {
 std::vector<float> v_jets_pt ;

// std::cout << " getSusyMass1 " << std::endl;
 float pt = -9999.9;

 const reco::Candidate* mcH = 0;

  // loop over gen particles
 for(size_t gp=0; gp<genParticles_.size();++gp){
  int type   = abs( genParticles_[gp] -> pdgId() );
  int status = genParticles_[gp] -> status();

   // Stop {1000006}
  if( ((type < 9 && type > 0) || type == 21) && (status == 3) ) {
   mcH = &(*(genParticles_[gp]));
   v_jets_pt.push_back( mcH->pt() );
  }
 } // loop over gen particles

if (v_jets_pt.size () > 0) {
  std::sort (v_jets_pt.rbegin (), v_jets_pt.rend ()) ;
 }
 //---- now return ----
 size_t count = 0;
 for(size_t i=0;i<v_jets_pt.size();++i) {
  if(++count > index) return v_jets_pt.at(i);
 }

 return pt;
}

const float reco::SkimEvent::leadingGenJetPartonPID(size_t index) const {
 float pt_ofIndex = leadingGenJetPartonPt(index);
 float particleID=-9999.9;
 const reco::Candidate* mcH = 0;
  // loop over gen particles
 for(size_t gp=0; gp<genParticles_.size();++gp){
   int type = abs( genParticles_[gp] -> pdgId() );
   int status = genParticles_[gp] -> status();
   if( ((type < 9 && type > 0) || type == 21) && (status == 3) ) {
     mcH = &(*(genParticles_[gp]));
     if( mcH->pt() != pt_ofIndex) continue;
     particleID = (float) type;
   }
 } // loop over gen particles
 return particleID;
}

const float reco::SkimEvent::leadingGenJetPartonEta(size_t index) const {
 float pt_ofIndex = leadingGenJetPartonPt(index);
 float particleEta=-9999.9;
 const reco::Candidate* mcH = 0;
  // loop over gen particles
 for(size_t gp=0; gp<genParticles_.size();++gp){
   int type = abs( genParticles_[gp] -> pdgId() );
   int status = genParticles_[gp] -> status();
   if( ((type < 9 && type > 0) || type == 21) && (status == 3) ) {
     mcH = &(*(genParticles_[gp]));
     if( mcH->pt() != pt_ofIndex) continue;
     particleEta = (float) mcH->eta();
   }
 } // loop over gen particles
 return particleEta;
}

const float reco::SkimEvent::leadingGenJetPartonPhi(size_t index) const {
 float pt_ofIndex = leadingGenJetPartonPt(index);
 float particlePhi=-9999.9;
 const reco::Candidate* mcH = 0;
  // loop over gen particles
 for(size_t gp=0; gp<genParticles_.size();++gp){
   int type = abs( genParticles_[gp] -> pdgId() );
   int status = genParticles_[gp] -> status();
   if( ((type < 9 && type > 0) || type == 21) && (status == 3) ) {
     mcH = &(*(genParticles_[gp]));
     if( mcH->pt() != pt_ofIndex) continue;
     particlePhi = (float) mcH->phi();
   }
 } // loop over gen particles
 return particlePhi;
}


const float reco::SkimEvent::leadingGenLeptonPt(size_t index) const {
 std::vector<float> v_jets_pt ;

// std::cout << " getSusyMass1 " << std::endl;
 float pt = -9999.9;

 const reco::Candidate* mcH = 0;

  // loop over gen particles
 for(size_t gp=0; gp<genParticles_.size();++gp){
  int type = abs( genParticles_[gp] -> pdgId() );
  int status = genParticles_[gp] -> status();

   // Stop {1000006}
  if( (type == 11 || type == 13 || type == 15) && (status == 3) ) {
   mcH = &(*(genParticles_[gp]));
   v_jets_pt.push_back( mcH->pt() );
  }
 } // loop over gen particles

if (v_jets_pt.size () > 0) {
  std::sort (v_jets_pt.rbegin (), v_jets_pt.rend ()) ;
 }
 //---- now return ----
 size_t count = 0;
 for(size_t i=0;i<v_jets_pt.size();++i) {
  if(++count > index) return v_jets_pt.at(i);
 }

 return pt;
}


const float reco::SkimEvent::leadingGenLeptonPID(size_t index) const {
 float pt_ofIndex = leadingGenLeptonPt(index);
 float particleID=-9999.9;
 const reco::Candidate* mcH = 0;
  // loop over gen particles
 for(size_t gp=0; gp<genParticles_.size();++gp){
   int type = abs( genParticles_[gp] -> pdgId() );
   int status = genParticles_[gp] -> status();
   if( (type == 11 || type == 13 || type == 15) && (status == 3) ) {
     mcH = &(*(genParticles_[gp]));
     if( mcH->pt() != pt_ofIndex) continue;
     particleID = (float) type;
   }
 } // loop over gen particles
 return particleID;
}

const float reco::SkimEvent::leadingGenLeptonEta(size_t index) const {
 float pt_ofIndex = leadingGenLeptonPt(index);
 float particleEta=-9999.9;
 const reco::Candidate* mcH = 0;
  // loop over gen particles
 for(size_t gp=0; gp<genParticles_.size();++gp){
   int type = abs( genParticles_[gp] -> pdgId() );
   int status = genParticles_[gp] -> status();
   if( (type == 11 || type == 13 || type == 15) && (status == 3) ) {
     mcH = &(*(genParticles_[gp]));
     if( mcH->pt() != pt_ofIndex) continue;
     particleEta = (float) mcH->eta();
   }
 } // loop over gen particles
 return particleEta;
}

const float reco::SkimEvent::leadingGenLeptonPhi(size_t index) const {
 float pt_ofIndex = leadingGenLeptonPt(index);
 float particlePhi=-9999.9;
 const reco::Candidate* mcH = 0;
  // loop over gen particles
 for(size_t gp=0; gp<genParticles_.size();++gp){
   int type = abs( genParticles_[gp] -> pdgId() );
   int status = genParticles_[gp] -> status();
   if( (type == 11 || type == 13 || type == 15) && (status == 3) ) {
     mcH = &(*(genParticles_[gp]));
     if( mcH->pt() != pt_ofIndex) continue;
     particlePhi = (float) mcH->phi();
   }
 } // loop over gen particles
 return particlePhi;
}

const float reco::SkimEvent::leadingGenNeutrinoPt(size_t index) const {
 std::vector<float> v_jets_pt ;
// std::cout << " getSusyMass1 " << std::endl;
 float pt = -9999.9;
 const reco::Candidate* mcH = 0;

  // loop over gen particles
 for(size_t gp=0; gp<genParticles_.size();++gp){
  int type = abs( genParticles_[gp] -> pdgId() );
  int status = genParticles_[gp] -> status();

   // Stop {1000006}
  if( (type == 12 || type == 14 || type == 16) && (status == 3) ) {
   mcH = &(*(genParticles_[gp]));
   v_jets_pt.push_back( mcH->pt() );
  }
 } // loop over gen particles

if (v_jets_pt.size () > 0) {
  std::sort (v_jets_pt.rbegin (), v_jets_pt.rend ()) ;
 }
 //---- now return ----
 size_t count = 0;
 for(size_t i=0;i<v_jets_pt.size();++i) {
  if(++count > index) return v_jets_pt.at(i);
 }

 return pt;
}


const float reco::SkimEvent::leadingGenNeutrinoPID(size_t index) const {
 float pt_ofIndex = leadingGenNeutrinoPt(index);
 float particleID=-9999.9;
 const reco::Candidate* mcH = 0;
  // loop over gen particles
 for(size_t gp=0; gp<genParticles_.size();++gp){
   int type = abs( genParticles_[gp] -> pdgId() );
   int status = genParticles_[gp] -> status();
   if( (type == 12 || type == 14 || type == 16) && (status == 3) ) {
     mcH = &(*(genParticles_[gp]));
     if( mcH->pt() != pt_ofIndex) continue;
     particleID = (float) type;
   }
 } // loop over gen particles
 return particleID;
}

const float reco::SkimEvent::leadingGenNeutrinoEta(size_t index) const {
 float pt_ofIndex = leadingGenNeutrinoPt(index);
 float particleEta=-9999.9;
 const reco::Candidate* mcH = 0;
  // loop over gen particles
 for(size_t gp=0; gp<genParticles_.size();++gp){
   int type = abs( genParticles_[gp] -> pdgId() );
   int status = genParticles_[gp] -> status();
   if( (type == 12 || type == 14 || type == 16) && (status == 3) ) {
     mcH = &(*(genParticles_[gp]));
     if( mcH->pt() != pt_ofIndex) continue;
     particleEta = (float) mcH->eta();
   }
 } // loop over gen particles
 return particleEta;
}

const float reco::SkimEvent::leadingGenNeutrinoPhi(size_t index) const {
 float pt_ofIndex = leadingGenNeutrinoPt(index);
 float particlePhi=-9999.9;
 const reco::Candidate* mcH = 0;
  // loop over gen particles
 for(size_t gp=0; gp<genParticles_.size();++gp){
   int type = abs( genParticles_[gp] -> pdgId() );
   int status = genParticles_[gp] -> status();
   if( (type == 12 || type == 14 || type == 16) && (status == 3) ) {
     mcH = &(*(genParticles_[gp]));
     if( mcH->pt() != pt_ofIndex) continue;
     particlePhi = (float) mcH->phi();
   }
 } // loop over gen particles
 return particlePhi;
}

const float reco::SkimEvent::genMetPt() const {
  float pT = -9999.9;
  if(genMet_.isNonnull()) pT = genMet_->pt();
  return pT;
}

const float reco::SkimEvent::genMetEta() const {
  float eta = -9999.9;
  if(genMet_.isNonnull()) eta = genMet_->eta();
  return eta;
}

const float reco::SkimEvent::genMetPhi() const {
  float phi = -9999.9;
  if(genMet_.isNonnull()) phi = genMet_->phi();
  return phi;
}

const float reco::SkimEvent::leadingGenJetPt(size_t index) const {
 std::vector<float> v_jets_pt ;
 // loop over particles in the event
 for (unsigned int iPart = 0 ; iPart < genJets_.size (); ++iPart) {
  if ( abs(genJets_[iPart]->eta()) > 4.7) continue ;
   v_jets_pt.push_back ( genJets_[iPart]->pt());
 }
 if (v_jets_pt.size () > 0) {
  std::sort (v_jets_pt.rbegin (), v_jets_pt.rend ()) ;
 }
 //---- now return ----
 size_t count = 0;
 for(size_t i=0;i<v_jets_pt.size();++i) {
  if(++count > index) return v_jets_pt.at(i);
 }

 return -9999.9;
}

const float reco::SkimEvent::leadingGenJetPhi(size_t index) const {
 float pt_ofIndex = leadingGenJetPt(index);
 float phi=-9999.9;
 // loop over particles in the event
 for (unsigned int iPart = 0 ; iPart < genJets_.size (); ++iPart) {
  if ( abs(genJets_[iPart]->eta()) > 4.7) continue ;
      if((genJets_[iPart]->pt())!=pt_ofIndex) continue;
      phi = genJets_[iPart]->phi() ;
 }

 //---- now return ----
 return phi;
}

const float reco::SkimEvent::leadingGenJetEta(size_t index) const {
 float pt_ofIndex = leadingGenJetPt(index);
 float eta=-9999.9;
 // loop over particles in the event
 
 for (unsigned int iPart = 0 ; iPart < genJets_.size (); ++iPart) {
  if ( abs(genJets_[iPart]->eta()) > 4.7) continue ;
      if((genJets_[iPart]->pt())!=pt_ofIndex) continue;
      eta = genJets_[iPart]->eta() ;
 }
 //---- now return ----
 return eta;
}



const int reco::SkimEvent::numberOfbQuarks() const {
 int numb = 0;

 // loop over particles in the event
 for (unsigned int iPart = 0 ; iPart < LHEhepeup_.IDUP.size (); ++iPart) {
   // outgoing particles
  if (LHEhepeup_.ISTUP.at (iPart) == 1) {
   // b quarks
   if (abs (LHEhepeup_.IDUP.at (iPart)) == 5) {
    numb++;
   }
  }
 }
 return numb;
}


const int reco::SkimEvent::numberOftQuarks() const {
 int numt = 0;

 // loop over particles in the event
 for (unsigned int iPart = 0 ; iPart < LHEhepeup_.IDUP.size (); ++iPart) {
   // outgoing particles or intermediate particles
  if (LHEhepeup_.ISTUP.at (iPart) == 1 || LHEhepeup_.ISTUP.at (iPart) == 2) {
   // t quarks
   if (abs (LHEhepeup_.IDUP.at (iPart)) == 6) {
    numt++;
   }
  }
 }
 return numt;
}

