#include "DataFormats/PatCandidates/interface/JetCorrFactors.h"
#include "LatinoTrees/DataFormats/interface/SkimEvent.h"
#include "CommonTools/Utils/interface/StringCutObjectSelector.h"
#include "Math/VectorUtil.h"

#include <TVector3.h>

#include <iostream>
#include <algorithm>
#include <sstream>

// const int reco::SkimEvent::channel() const {
//     switch(hypo_) {
//         case WWMUMU: return 0;
//         case WWELEL: return 1;
//         case WWELMU: return 2;
//         case WWMUEL: return 3;
//     }
// return -1;
// }

const int reco::SkimEvent::channel() const {
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


float reco::SkimEvent::userFloat( const std::string &key ) const
{
 std::vector<std::string>::const_iterator it = std::find(userFloatLabels_.begin(), userFloatLabels_.end(), key);
 if (it != userFloatLabels_.end()) {
  return userFloats_[it - userFloatLabels_.begin()];
 }
 return 0.0;
}

void reco::SkimEvent::addUserFloat( const std::string & label,
                                    float data )
{
 userFloatLabels_.push_back(label);
 userFloats_.push_back( data );
}


int reco::SkimEvent::userInt( const std::string & key ) const
{
 std::vector<std::string>::const_iterator it = std::find(userIntLabels_.begin(), userIntLabels_.end(), key);
 if (it != userIntLabels_.end()) {
  return userInts_[it - userIntLabels_.begin()];
 }
 return 0;
}

void reco::SkimEvent::addUserInt( const std::string &label, int data )
{
 userIntLabels_.push_back(label);
 userInts_.push_back( data );
}



const bool reco::SkimEvent::peaking() const {
 
 if(leps_.size() < 2) return false;
 
 if (getMotherID(0).isNonnull() && getMotherID(1).isNonnull() && getMotherID(0) == getMotherID(1)) return true;
 
 return false;
}

const reco::GenParticle *reco::SkimEvent::genParticle(size_t i) const {
 return isMuon(i) ? getMuon(i)->genLepton() : getElectron(i)->genLepton();
}

const reco::GenParticleRef reco::SkimEvent::getMotherID(size_t i) const {
 
 if(i >= leps_.size()) return reco::GenParticleRef();   
 
 const reco::GenParticle *match = isMuon(i) ? getMuon(i)->genLepton() : getElectron(i)->genLepton();
 
 if( !match ) return reco::GenParticleRef();
 if( !match->numberOfMothers() ) return reco::GenParticleRef();
 
 int pdgId = match->pdgId();
 
 reco::GenParticleRef mother = match->motherRefVector()[0];
 if( !mother.isNonnull() ) return reco::GenParticleRef();
 if( !mother->numberOfMothers() ) return reco::GenParticleRef();
 while(mother->pdgId() == pdgId) {
  mother = mother->motherRefVector()[0];
  if( !mother.isNonnull() ) return reco::GenParticleRef();
  if( !mother->numberOfMothers() ) return reco::GenParticleRef();
 }
 return mother;
}

const bool reco::SkimEvent::isHardMuID(size_t i) const {
 
 if( i >= std::min((uint) 2,(uint) leps_.size()) ) return false;
 
 if( isElectron(i) ) return true;
 if( isMuon(i) ) {
  pat::Muon const * const mu = getMuon(i);
  return (mu->isGlobalMuon() && mu->isTrackerMuon());
 }
 return false;
 
}

const bool reco::SkimEvent::passesSmurfMuonID() const {
 
 //     switch(hypo_) {
 //         case WWELMU: return isHardMuID(indexByPt(1)); break;
 //         case WWMUEL: return isHardMuID(indexByPt(0)); break;
 //         case WWMUMU: return (isHardMuID(indexByPt(0)) && isHardMuID(indexByPt(1))); break;
 //         case WWELEL: return true; break;
 //         default : return true; break;
 //     }
 return true;
 
}

struct indexValueStruct {
 indexValueStruct(const float &v, size_t i) : value(v), index(i) {}
 float value;
 size_t index;
};

bool lowToHigh(const indexValueStruct &a, const indexValueStruct &b) {
 return a.value < b.value;
}

bool highToLow(const indexValueStruct &a, const indexValueStruct &b) {
 return a.value > b.value;
}

// To be kept in synch with the enumerator definitions in SkimEvent.h file
// const std::string reco::SkimEvent::hypoTypeNames[] = { "undefined", "WELNU", "WMUNU", "WWELEL", "WWELMU", "WWMUEL", "WWMUMU"};


// std::string reco::SkimEvent::hypoTypeName(reco::SkimEvent::hypoType a){
//   if(int(a) < int(hypoTypeSize) && int(a)>0) return hypoTypeNames[int(a)];
//   return "undefined";
// }
//
//
//
// reco::SkimEvent::hypoType reco::SkimEvent::hypoTypeByName(const std::string &name){
//   hypoType size = hypoTypeSize;
//   int index = std::find(hypoTypeNames, hypoTypeNames+size, name)-hypoTypeNames;
//   if(index == size) return undefined; // better this or throw() ?
//
//   // cast
//   return hypoType(index);
// }



reco::SkimEvent::SkimEvent() :
//         hypo_(-1),
           sumPts_(0)/*, jec_(0), vtxPoint_(0,0,0) */{
 
 InitEffectiveAreasPhoton();
 InitEffectiveAreasElectrons();
 
 //---- default value
 maxEtaForJets_ = 4.7;
 minPtForJets_ = 0;
 applyCorrectionForJets_ = 1;
 applyIDForJets_ = 0;
 dzCutForBtagJets_ = 999999.9;

 _maxDrSoftMuonJet = 0.3;
 _minPtSoftMuon = 5;
 
}

// reco::SkimEvent::SkimEvent(const reco::SkimEvent::hypoType &h) :
//         hypo_(h), sumPts_(0)/*, jec_(0), vtxPoint_(0,0,0) */{ }



void reco::SkimEvent::setMaxEtaForJets(double value) {
 maxEtaForJets_ = value;
}
void reco::SkimEvent::setMinPtForJets(double value) {
 minPtForJets_ = value;
}
void reco::SkimEvent::setApplyCorrectionForJets(bool flag) {
 applyCorrectionForJets_ = flag;
}
void reco::SkimEvent::setApplyIDForJets(int jetidvalue) {
 applyIDForJets_ = jetidvalue;
 //---- "0" is no ID
}
void reco::SkimEvent::setDzCutForBtagJets(double value) {
 dzCutForBtagJets_ = value;
}

void reco::SkimEvent::setPuJetIdDiscriminantName(std::string pujetiddiscriminant) {
 _name_puJetIdDiscriminant = pujetiddiscriminant;
}


void reco::SkimEvent::setMaxDrSoftMuonJet(double value) {
 _maxDrSoftMuonJet = value;
}
void reco::SkimEvent::setMinPtSoftMuon(double value) {
 _minPtSoftMuon = value;
}



// set GenParticles
void reco::SkimEvent::setGenParticles(const edm::Handle<reco::GenParticleCollection> & h) {
 genParticles_.clear();
 for(size_t i=0; i<h->size(); ++i) {
  genParticles_.push_back( reco::GenParticleRef(h,i) );
 }
}

// set GenJets
void reco::SkimEvent::setGenJets(const edm::Handle<reco::GenJetCollection> & h) {
 genJets_.clear();
 for(size_t i=0; i<h->size(); ++i) {
  genJets_.push_back( reco::GenJetRef(h,i) );
 }
}



const float reco::SkimEvent::LHEMCweight(int i) const {
 
 if (i == -1) return LHEInfoHandle_->originalXWGTUP();
 
 int num_whichWeight = LHEInfoHandle_->weights().size();
 if (i<num_whichWeight) {
  //   return (LHEInfoHandle_->weights()[i].wgt/LHEInfoHandle_->originalXWGTUP());
  return LHEInfoHandle_->weights()[i].wgt;
 }
 else {
  return -999;
 }
}


const float reco::SkimEvent::GENMCweight(int i) const {
 
 if (i == -1) return GenInfoHandle_.weight();
 
 int num_whichWeight = GenInfoHandle_.weights().size();
 if (i<num_whichWeight) {
  return GenInfoHandle_.weights()[i];
 }
 else {
  return -999;
 }
}




const float reco::SkimEvent::HEPMCweightScale(size_t i) const {
 if (i < comments_LHE_weight_.size()) return comments_LHE_weight_.at(i);
 else return -1;
}

const float reco::SkimEvent::HEPMCweightRen(size_t i) const {
 if (i < comments_LHE_rfac_.size()) return comments_LHE_rfac_.at(i);
 else return -1;
}

const float reco::SkimEvent::HEPMCweightFac(size_t i) const {
 if (i < comments_LHE_ffac_.size()) return comments_LHE_ffac_.at(i);
 else return -1;
}


//EDM RefToBase implementation
void reco::SkimEvent::setLepton(const edm::Handle<edm::View<reco::RecoCandidate> > &h,size_t i){
 leps_.push_back( h->ptrAt(i) );
}

void reco::SkimEvent::setExtraLepton(const edm::Handle<edm::View<reco::RecoCandidate> > &h,size_t i){
 extraLeps_.push_back( h->ptrAt(i) );
 //extraLeps_.push_back(refToCand(h,i));
}

void reco::SkimEvent::setSoftMuon(const edm::Handle<edm::View<reco::RecoCandidate> > &h,size_t i){
 softMuons_.push_back( h->ptrAt(i) );
}

// Old implementation
// void reco::SkimEvent::setLepton(const pat::Electron& ele){
// leps_.push_back(ele);
// }
//
// void reco::SkimEvent::setLepton(const pat::Muon& mu){
// leps_.push_back(mu);
// }
//
// void reco::SkimEvent::setExtraLepton(const pat::Electron& ele){
// extraLeps_.push_back(ele);
// }
//
// void reco::SkimEvent::setExtraLepton(const pat::Muon& mu){
// extraLeps_.push_back(mu);
// }
//
// void reco::SkimEvent::setSoftMuon(const pat::Muon& mu){
// softMuons_.push_back(mu);
// }


void reco::SkimEvent::setTaus(const edm::Handle<pat::TauCollection> & jH) {
 
 for(size_t i=0;i<jH->size();++i)
  //   taus_.push_back(pat::TauRef(jH,i));
  taus_.push_back(jH->at(i));
 
 //sortJetsByPt();
 
}



void reco::SkimEvent::setJets(const edm::Handle<pat::JetCollection> & jH) {
 
 jets_.clear();
 
 for(size_t i=0;i<jH->size();++i)
  jets_.push_back(pat::JetRef(jH,i));
 
 //sortJetsByPt();
 
}


void reco::SkimEvent::setFatJets(const edm::Handle<pat::JetCollection> & jH) {
 
 fatJets_.clear();
 
 for(size_t i=0;i<jH->size();++i)
  fatJets_.push_back(pat::JetRef(jH,i));
 
 //sortJetsByPt();
 
}

void reco::SkimEvent::setSecondJets(const edm::Handle<pat::JetCollection> & jH) {
 
 secondJets_.clear();
 
 for(size_t i=0;i<jH->size();++i)
  secondJets_.push_back(pat::JetRef(jH,i));
 
 //sortJetsByPt();
 
}


void reco::SkimEvent::setTagJets(const edm::Handle<pat::JetCollection> & jH) {
 
 tagJets_.clear();
 
 for(size_t i=0;i<jH->size();++i)
  tagJets_.push_back(pat::JetRef(jH,i));
 
 //sortTagJetsByPt();
 
}

void reco::SkimEvent::setTCMet(const edm::Handle<reco::METCollection> & mH) {
 tcMet_ = reco::METRef(mH,0);
}

void reco::SkimEvent::setPFMet(const edm::Handle< std::vector<pat::MET> > & mH) {
 pfMet_ = pat::METRef(mH,0);
}

void reco::SkimEvent::setPUpMet(const edm::Handle< std::vector<pat::MET> > & mH) {
 pupMet_ = pat::METRef(mH,0);
}
void reco::SkimEvent::setTrkMet(const reco::MET & trkMET) {
 trkMet_ = trkMET;
}

void reco::SkimEvent::setChargedMet(const reco::PFMET & chMET) {
 chargedMet_ = chMET;
}

void reco::SkimEvent::setGenMet(const edm::Handle< std::vector<pat::MET> > & mH) { 
 genMet_ = *((mH.product()->front()).genMET());
}

void reco::SkimEvent::setGenMet(const edm::Handle<reco::GenMETCollection> & mH) {
 genMetRef_ = reco::GenMETRef(mH,0);
}


// void reco::SkimEvent::addElectronId(const edm::Handle<edm::ValueMap<bool> > &valueMapEleId, std::string name) {
//  
//  if (_electronIdsMap.find( name ) != _electronIdsMap.end()) {
//   _electronIdsMap[name].clear();
//  }
//  std::vector<bool> eleidvector;
//  for (unsigned int iLep = 0; iLep < leps_.size(); iLep++) {
//   if (isElectron(leps_[iLep])) {
//    eleidvector.push_back( (*valueMapEleId)[leps_[iLep]] );
//   }
//   else { //---- if it is a muon, it's ok!
//    eleidvector.push_back( true );
//   }
//  }
//  _electronIdsMap[name] = eleidvector;
//  
// }


void reco::SkimEvent::addElectronId(const std::vector<bool> &EleId, std::string name) {
 
 if (_electronIdsMap.find( name ) != _electronIdsMap.end()) {
  _electronIdsMap[name].clear();
 }
 _electronIdsMap[name] = EleId;
 
}

void reco::SkimEvent::setVtxSumPts(const edm::Handle<edm::ValueMap<float> > &s) {
 
 for(size_t i=0;i<vtxs_.size();++i) sumPts_.push_back( (*s)[vtxs_[i]] );
}

void reco::SkimEvent::setVtxSumPt2s(const edm::Handle<edm::ValueMap<float> > &s) {
 
 for(size_t i=0;i<vtxs_.size();++i) sumPt2s_.push_back( (*s)[vtxs_[i]] );
}

void reco::SkimEvent::setVertex(const edm::Handle<reco::VertexCollection> & vtxH) {
 
 for(size_t i=0;i<vtxH->size();++i) vtxs_.push_back(reco::VertexRef(vtxH,i));
}


// void reco::SkimEvent::setGenWeight(const edm::Handle<double> &mcGenWeight) {
void reco::SkimEvent::setGenWeight(const edm::Handle<GenFilterInfo> &mcGenWeight) {
 mcGenWeight_ = *(mcGenWeight.product());
}


// set LHEinfo
void reco::SkimEvent::setLHEinfo(const edm::Handle<LHEEventProduct> &h,const edm::Handle<LHERunInfoProduct> &productLHERunInfoHandle) {
 
 setLHEinfo(h);
 
 typedef std::vector<LHERunInfoProduct::Header>::const_iterator headers_const_iterator; 
 LHERunInfoProduct myLHERunInfoProduct = *(productLHERunInfoHandle.product());
 
 for (headers_const_iterator iter=myLHERunInfoProduct.headers_begin(); iter!=myLHERunInfoProduct.headers_end(); iter++){
  std::cout << iter->tag() << std::endl;
  std::vector<std::string> lines = iter->lines();
  for (unsigned int iLine = 0; iLine<lines.size(); iLine++) {
   std::cout << lines.at(iLine);
  }
 }
 
 //  <weight id="1001"> muR=0.10000E+01 muF=0.10000E+01 </weight>
 //  <weight id="2007"> pdfset=292207 </weight>
 
}



void reco::SkimEvent::setLHEinfo(const edm::Handle<LHEEventProduct> &h) {
 
 LHEInfoHandle_ = (h.product());
 
 LHEhepeup_ = (*(h.product())).hepeup();
 
 std::vector<std::string>::const_iterator it_end = (*(h.product())).comments_end();
 std::vector<std::string>::const_iterator it = (*(h.product())).comments_begin();
 for(; it != it_end; it++) {
  comments_LHE_.push_back (*it);
 }
 
 // std::cout << " comments_LHE_.size() = " << comments_LHE_.size() << std::endl;
 for (unsigned int iComm = 0; iComm<comments_LHE_.size(); iComm++) {
  // std::cout << " i=" << iComm << " :: " << comments_LHE_.size() << " ==> " << comments_LHE_.at(iComm) << std::endl;
  /// #new weight,renfact,facfact,pdf1,pdf2 32.2346904790193 1.00000000000000 1.00000000000000 11000 11000 lha
  std::stringstream line( comments_LHE_.at(iComm) );
  std::string dummy;
  line >> dummy; // #new weight,renfact,facfact,pdf1,pdf2
  float dummy_float;
  line >> dummy_float; // 32.2346904790193
  comments_LHE_weight_.push_back(dummy_float);
  // std::cout << dummy_float << std::endl;
  line >> dummy_float; // 1.00000000000000
  comments_LHE_rfac_.push_back(dummy_float);
  // std::cout << dummy_float << std::endl;
  line >> dummy_float; // 1.00000000000000
  comments_LHE_ffac_.push_back(dummy_float);
  // std::cout << dummy_float << std::endl;
 }
 
}


//---- set GEN info
void reco::SkimEvent::setGenInfo(const edm::Handle<GenEventInfoProduct> &GenInfoHandle) {
 GenInfoHandle_ = *(GenInfoHandle.product());
}




//Lepton variables

const int reco::SkimEvent::nLep(float minPt) const {
 int count = 0;
 if(minPt < 0) count = leps_.size();
 else for(size_t i=0;i<leps_.size();++i) if(leps_[i]->pt() > minPt) count++;
 return count;
}

const int reco::SkimEvent::nExtraLep(float minPt) const {
 int count = 0;
 if(minPt < 0) count = extraLeps_.size();
 else for(size_t i=0;i<extraLeps_.size();++i) if(extraLeps_[i]->pt() > minPt) count++;
 // for(size_t i=0;i<extraLeps_.size();++i) if(extraLeps_[i]->pt() > minPt && passesIP(extraLeps_[i]) ) count++;
 return count;
}


// Doesn't veto soft muons around jets by default
// i.e. vetoJets == -1
// if vetoJets != -1 then vetoJets is taken as the jet pt threshold
const int reco::SkimEvent::nSoftMu(float minPt, float vetoJets, float dRCut) const {
 int count = 0;
 if(minPt < 0 && vetoJets < 0) count = softMuons_.size();
 else if(vetoJets < 0) {
  for(size_t i=0;i<softMuons_.size();++i) if(softMuons_[i]->pt() > minPt) count++;
 } else {
  for(size_t i=0;i<softMuons_.size();++i) {
   bool toCount = true;
   for(size_t j=0; j<jets_.size();++j){
    if( jetPt(j,true) <= vetoJets) continue;
    double dR = fabs(ROOT::Math::VectorUtil::DeltaR(softMuons_[i]->p4(),jets_[j]->p4()) );
    if(dR < dRCut) toCount = false;
   }
   if(toCount && softMuons_[i]->pt() > minPt) count++;
  }
 }
 return count;
}



const float reco::SkimEvent::jetSoftMuonPtByPt(size_t i = 0) const {
  return jetSoftMuonPt(i,_minPtSoftMuon, _maxDrSoftMuonJet, minPtForJets_, maxEtaForJets_, applyCorrectionForJets_, applyIDForJets_); 
}

const float reco::SkimEvent::jetSoftMuonPt(size_t index, float minPtMuon, float maxDrMuonJet, float minPt,float eta,int applyCorrection,int applyID) const {
 
 size_t count = 0;
 for(size_t i=0;i<jets_.size();++i) {
  //---- get the correct jet index ...
  if(!(passJetID(jets_[i],applyID)) ) continue;
  if( std::fabs(jets_[i]->eta()) >= eta) continue;
  if( jetPt(i,applyCorrection) <= minPt) continue;  
  if(isThisJetALepton(jets_[i])) continue;
  
  //---- now check for the closest muon
  if(++count > index) {
   float minDR = 9999999.9;
   int nMu = -1;
   for (size_t iMu=0; iMu<softMuons_.size(); iMu++) {
    //---- check if it is really a soft-muon
    //---- see https://twiki.cern.ch/twiki/bin/view/CMS/SWGuideMuonId2015
    if (muon::isSoftMuon(*(static_cast<const reco::Muon*>(softMuons_[iMu].get())), highestPtVtx())) {
//      if (muon::isSoftMuon(static_cast<const pat::Muon*>(softMuons_[iMu].get()), highestPtVtx())) {
     float muonPt = softMuons_[iMu]->pt();
     if (muonPt >= minPtMuon) {
      double dR = fabs(ROOT::Math::VectorUtil::DeltaR(softMuons_[iMu]->p4(),jets_[i]->p4()) );
      if(dR < maxDrMuonJet && dR < minDR) {
       minDR = dR;
       nMu = iMu;
      }
     }
    }
   }
   if (nMu != -1) {
    return softMuons_[nMu]->pt();
   }
  }
  
 }
 return defaultvalues::defaultFloat;
 
}



const float reco::SkimEvent::jetSoftMuonEtaByPt(size_t i = 0) const {
 return jetSoftMuonEta(i,_minPtSoftMuon, _maxDrSoftMuonJet, minPtForJets_, maxEtaForJets_, applyCorrectionForJets_, applyIDForJets_); 
}

const float reco::SkimEvent::jetSoftMuonEta(size_t index, float minPtMuon, float maxDrMuonJet, float minPt,float eta,int applyCorrection,int applyID) const {
 
 size_t count = 0;
 for(size_t i=0;i<jets_.size();++i) {
  //---- get the correct jet index ...
  if(!(passJetID(jets_[i],applyID)) ) continue;
  if( std::fabs(jets_[i]->eta()) >= eta) continue;
  if( jetPt(i,applyCorrection) <= minPt) continue;  
  if(isThisJetALepton(jets_[i])) continue;
  
  //---- now check for the closest muon
  if(++count > index) {
   float minDR = 9999999.9;
   int nMu = -1;
   for (size_t iMu=0; iMu<softMuons_.size(); iMu++) {
    //---- check if it is really a soft-muon
    //---- see https://twiki.cern.ch/twiki/bin/view/CMS/SWGuideMuonId2015
    if (muon::isSoftMuon(*(static_cast<const reco::Muon*>(softMuons_[iMu].get())), highestPtVtx())) {
     //      if (muon::isSoftMuon(static_cast<const pat::Muon*>(softMuons_[iMu].get()), highestPtVtx())) {
     float muonPt = softMuons_[iMu]->pt();
     if (muonPt >= minPtMuon) {
      double dR = fabs(ROOT::Math::VectorUtil::DeltaR(softMuons_[iMu]->p4(),jets_[i]->p4()) );
      if(dR < maxDrMuonJet && dR < minDR) {
       minDR = dR;
       nMu = iMu;
      }
     }
    }
   }
   if (nMu != -1) {
    return softMuons_[nMu]->eta();
   }
  }
  
 }
 return defaultvalues::defaultFloat;
 
}



const float reco::SkimEvent::jetSoftMuonPhiByPt(size_t i = 0) const {
 return jetSoftMuonPhi(i,_minPtSoftMuon, _maxDrSoftMuonJet, minPtForJets_, maxEtaForJets_, applyCorrectionForJets_, applyIDForJets_); 
}

const float reco::SkimEvent::jetSoftMuonPhi(size_t index, float minPtMuon, float maxDrMuonJet, float minPt,float eta,int applyCorrection,int applyID) const {
 
 size_t count = 0;
 for(size_t i=0;i<jets_.size();++i) {
  //---- get the correct jet index ...
  if(!(passJetID(jets_[i],applyID)) ) continue;
  if( std::fabs(jets_[i]->eta()) >= eta) continue;
  if( jetPt(i,applyCorrection) <= minPt) continue;  
  if(isThisJetALepton(jets_[i])) continue;
  
  //---- now check for the closest muon
  if(++count > index) {
   float minDR = 9999999.9;
   int nMu = -1;
   for (size_t iMu=0; iMu<softMuons_.size(); iMu++) {
    //---- check if it is really a soft-muon
    //---- see https://twiki.cern.ch/twiki/bin/view/CMS/SWGuideMuonId2015
    if (muon::isSoftMuon(*(static_cast<const reco::Muon*>(softMuons_[iMu].get())), highestPtVtx())) {
     //      if (muon::isSoftMuon(static_cast<const pat::Muon*>(softMuons_[iMu].get()), highestPtVtx())) {
     float muonPt = softMuons_[iMu]->pt();
     if (muonPt >= minPtMuon) {
      double dR = fabs(ROOT::Math::VectorUtil::DeltaR(softMuons_[iMu]->p4(),jets_[i]->p4()) );
      if(dR < maxDrMuonJet && dR < minDR) {
       minDR = dR;
       nMu = iMu;
      }
     }
    }
   }
   if (nMu != -1) {
    return softMuons_[nMu]->phi();
   }
  }
  
 }
 return defaultvalues::defaultFloat;
 
}



const float reco::SkimEvent::jetSoftMuonIsoByPt(size_t i = 0) const {
 return jetSoftMuonIso(i,_minPtSoftMuon, _maxDrSoftMuonJet, minPtForJets_, maxEtaForJets_, applyCorrectionForJets_, applyIDForJets_); 
}

const float reco::SkimEvent::jetSoftMuonIso(size_t index, float minPtMuon, float maxDrMuonJet, float minPt,float eta,int applyCorrection,int applyID) const {
 
 size_t count = 0;
 for(size_t i=0;i<jets_.size();++i) {
  //---- get the correct jet index ...
  if(!(passJetID(jets_[i],applyID)) ) continue;
  if( std::fabs(jets_[i]->eta()) >= eta) continue;
  if( jetPt(i,applyCorrection) <= minPt) continue;  
  if(isThisJetALepton(jets_[i])) continue;
  
  //---- now check for the closest muon
  if(++count > index) {
   float minDR = 9999999.9;
   int nMu = -1;
   for (size_t iMu=0; iMu<softMuons_.size(); iMu++) {
    //---- check if it is really a soft-muon
    //---- see https://twiki.cern.ch/twiki/bin/view/CMS/SWGuideMuonId2015
    if (muon::isSoftMuon(*(static_cast<const reco::Muon*>(softMuons_[iMu].get())), highestPtVtx())) {
     //      if (muon::isSoftMuon(static_cast<const pat::Muon*>(softMuons_[iMu].get()), highestPtVtx())) {
     float muonPt = softMuons_[iMu]->pt();
     if (muonPt >= minPtMuon) {
      double dR = fabs(ROOT::Math::VectorUtil::DeltaR(softMuons_[iMu]->p4(),jets_[i]->p4()) );
      if(dR < maxDrMuonJet && dR < minDR) {
       minDR = dR;
       nMu = iMu;
      }
     }
    }
   }
   if (nMu != -1) {
    //---- isolation
    return ((getMuon(softMuons_[nMu])->pfIsolationR04().sumChargedHadronPt+std::max(0.,getMuon(softMuons_[nMu])->pfIsolationR04().sumNeutralHadronEt+getMuon(softMuons_[nMu])->pfIsolationR04().sumPhotonEt-0.50*getMuon(softMuons_[nMu])->pfIsolationR04().sumPUPt))/getMuon(softMuons_[nMu])->pt());
   }
  }
  
 }
 return defaultvalues::defaultFloat;
 
}



//---- number of soft muons associated to a jet
//----     it should be at most 1!
const float reco::SkimEvent::jetSoftMuonCountingByPt(size_t i = 0) const {
 return jetSoftMuonCounting(i,_minPtSoftMuon, _maxDrSoftMuonJet, minPtForJets_, maxEtaForJets_, applyCorrectionForJets_, applyIDForJets_); 
}

const float reco::SkimEvent::jetSoftMuonCounting(size_t index, float minPtMuon, float maxDrMuonJet, float minPt,float eta,int applyCorrection,int applyID) const {
 
 size_t count = 0;
 for(size_t i=0;i<jets_.size();++i) {
  //---- get the correct jet index ...
  if(!(passJetID(jets_[i],applyID)) ) continue;
  if( std::fabs(jets_[i]->eta()) >= eta) continue;
  if( jetPt(i,applyCorrection) <= minPt) continue;  
  if(isThisJetALepton(jets_[i])) continue;
  
  //---- now check all the soft muons that are close to the jet
  if(++count > index) {
   int numberOfMuons = 0;
//    int nMu = -1;
   for (size_t iMu=0; iMu<softMuons_.size(); iMu++) {
    //---- check if it is really a soft-muon
    //---- see https://twiki.cern.ch/twiki/bin/view/CMS/SWGuideMuonId2015
    if (muon::isSoftMuon(*(static_cast<const reco::Muon*>(softMuons_[iMu].get())), highestPtVtx())) {
     float muonPt = softMuons_[iMu]->pt();
     if (muonPt >= minPtMuon) {
      double dR = fabs(ROOT::Math::VectorUtil::DeltaR(softMuons_[iMu]->p4(),jets_[i]->p4()) );
      if(dR < maxDrMuonJet) {
       numberOfMuons++;
      }
     }
    }
   }

   return 1. * numberOfMuons;
  }
  
 }
 return defaultvalues::defaultFloat;
 
}



const int reco::SkimEvent::pdgId(size_t i) const {
 if(i < leps_.size()) return leps_[i]->pdgId();
 else return -9999;
}


const bool reco::SkimEvent::isMuon(size_t i) const {
 if(i < leps_.size()) return isMuon(leps_[i]);
 else return false;
}

const bool reco::SkimEvent::isElectron(size_t i) const {
 if(i < leps_.size()) return isElectron(leps_[i]);
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
 return static_cast<const pat::Muon*>(c.get());
 
}

const pat::Electron * reco::SkimEvent::getElectron(const refToCand &c) const {
 return static_cast<const pat::Electron*>(c.get());
}

const float reco::SkimEvent::pt(size_t i) const {
 if(i >= leps_.size()) return defaultvalues::defaultFloat; //---- -9999.0
 return leps_[i]->pt();
}

const math::XYZTLorentzVector reco::SkimEvent::lepton(size_t i) const {
  if (indexByPt (i) >= leps_.size()) return math::XYZTLorentzVector(0,0,0,0);
  return leps_[indexByPt (i)]->p4();
}

const int reco::SkimEvent::passCustom(size_t i, const std::string &muStr, const std::string &elStr) const {
 if(i >= leps_.size()) return 0;
 else if( isElectron(i) ) return StringCutObjectSelector<pat::Electron>(elStr,true)( *getElectron(i) );
 else if( isMuon(i) )     return StringCutObjectSelector<pat::Muon >   (muStr,true)( *getMuon(i) );
 else return 0;
}


// const float reco::SkimEvent::leptId(size_t i, std::string idele, std::string idmu) const {
const float reco::SkimEvent::leptId(std::string idele, std::string idmu, size_t i) const {
 //  std::cout << " >> id-ele = " << idele << std::endl;
 if(i >= leps_.size()) return defaultvalues::defaultFloat;
 if( isElectron(i) ) {
//   std::cout << " >> getElectron(i)->userFloat(" << idele << ") = " << getElectron(i)->userFloat(idele) << std::endl;
//   return getElectron(i)->userFloat(idele); //---- "idele" is the name of the id for electrons
//   std::cout << " _electronIdsMap[" << idele << "].at(" << i << ") = " << (_electronIdsMap.at(idele)).at(i) << std::endl;
  if ((_electronIdsMap.at(idele)).at(i)) return 1;
  else                              return 0;
 }
 else return getMuon(i)->userFloat(idmu);
}

const float reco::SkimEvent::leptBdt(size_t i) const {
 if(i >= leps_.size()) return defaultvalues::defaultFloat;
 if( isElectron(i) ) return getElectron(i)->userFloat("bdttrig"); // changed from "bdt" to "bdttrig"
 else return 999999.;
}

const float reco::SkimEvent::leptLH(size_t i) const {
 if(i >= leps_.size()) return defaultvalues::defaultFloat;
 //   if( isElectron(i) ) return getElectron(i)->electronID("egammaIDLikelihood"); # FIXME
 else return 999999.;
}

const float reco::SkimEvent::eta(size_t i) const {
 if(i < leps_.size()) return leps_[i]->eta();
 else return defaultvalues::defaultFloat;
}

const float reco::SkimEvent::etaSC(size_t i) const {
 if(i >= leps_.size()) return defaultvalues::defaultFloat;
 if( isElectron(i) ) return getElectron(i)->superCluster()->eta();
 else return leps_[i]->eta();
}

const float reco::SkimEvent::nBrem(size_t i) const {
  if (i >= leps_.size()) return defaultvalues::defaultFloat;
  if (isElectron(i))     return getElectron(i)->numberOfBrems();
  else return 0;
}

const float reco::SkimEvent::phi(size_t i) const {
  if (i < leps_.size()) return leps_[i]->phi();
  else return defaultvalues::defaultFloat;
}

const int reco::SkimEvent::q(size_t i) const{
  if (i < leps_.size()) return leps_[i]->charge();
  else return defaultvalues::defaultFloat;
}


// Jet variables
void reco::SkimEvent::setupJEC(const std::string &l2File, const std::string &l3File, const std::string &residualFile) {
 
 jecFiles_.clear();
 jecFiles_.push_back(l2File);
 jecFiles_.push_back(l3File);
 if(residualFile.compare("")) jecFiles_.push_back(residualFile);
 
}





const float reco::SkimEvent::leadingTauPt(size_t index) const {
 
 size_t count = 0;
 for(size_t i=0;i<taus_.size();++i) {
  if(++count > index) return taus_[i].pt();
 }
 return -9999.9;
 
}



const float reco::SkimEvent::leadingTauEta(size_t index) const {
 
 size_t count = 0;
 for(size_t i=0;i<taus_.size();++i) {
  if(++count > index) return taus_[i].eta();
 }
 return -9999.9;
 
}



const float reco::SkimEvent::leadingTauPhi(size_t index) const {
 
 size_t count = 0;
 for(size_t i=0;i<taus_.size();++i) {
  if(++count > index) return taus_[i].phi();
 }
 return -9999.9;
 
}






// void reco::SkimEvent::setupJEC(const JetCorrector *c) {
// jec_ =

const int reco::SkimEvent::nJets(float minPt, int applyCorrection,int applyID) const {
 return nCentralJets(minPt,99.9, applyCorrection,applyID);
}

const bool reco::SkimEvent::isThisJetALepton(pat::JetRef jet, float drCut, float minLeptonPt) const {
 bool thisJetIsLepton(false);
//  for(size_t j=0; j<std::min((uint) 2,(uint) leps_.size());++j){ //---- check only with the first 2 leptons -> miniAOD/PAT should already have filtered?
//   double dR = fabs(ROOT::Math::VectorUtil::DeltaR(jet->p4(),leps_[j]->p4()) );
//   if(dR < drCut){
//    thisJetIsLepton = true;
//    break;
//   }

 for(size_t j=0; j<leps_.size();j++){ //---- check all leptons up to "minimum pt" (default 10 GeV, minLeptonPt)
  if (leps_[indexByPt(j)]->pt() > minLeptonPt) {
   double dR = fabs(ROOT::Math::VectorUtil::DeltaR(jet->p4(),leps_[indexByPt(j)]->p4()) );
   if(dR < drCut){
    thisJetIsLepton = true;
    break;
   }
  }
  else {
   break;
  }
 }
 
 
 return thisJetIsLepton;
}

const bool reco::SkimEvent::passJetID(pat::JetRef jet, int applyID) const{
 // no ID
 if(applyID == 0) return true;
 
 // old ID
 else if(applyID == 1) {
  unsigned int multiplicity = jet->neutralMultiplicity () + jet->chargedMultiplicity ();
  if(jet->neutralEmEnergyFraction() >=0.99 ||
   jet->neutralHadronEnergyFraction() >=0.99 ||
   multiplicity==0 ) return false;
  if(fabs(jet->eta())<2.4){
   if(jet->chargedEmEnergyFraction() >=0.99 ||
    jet->chargedHadronEnergyFraction() == 0 ||
    jet->chargedMultiplicity()==0 ) return false;
  }
  return true;
 }
 
 // MVA ID loose
 else if(applyID == 4) {
  if(jet->userInt("jetId") >= 4) return true;
  else return false;
 }
 
 // MVA ID medium
 else if(applyID == 5) {
  if(jet->userInt("jetId") >= 6) return true;
  else return false;
 }
 
 // MVA ID tight
 else if(applyID == 6) {
  if(jet->userInt("jetId") >= 7) return true;
  else return false;
 }
 
 else if(applyID == 7) { //---- first Run II jet ID: LOOSE
  //---- see https://twiki.cern.ch/twiki/bin/view/CMS/JetID
  if (jet->neutralHadronEnergyFraction() >=0.99) return false;
  if (jet->neutralEmEnergyFraction() >=0.99) return false;
  
  unsigned int multiplicity = jet->chargedMultiplicity() + jet->neutralMultiplicity();
  if ( multiplicity <= 1) return false;
  if ( jet->muonEnergyFraction() >= 0.8) return false;
  
  if(fabs(jet->eta())<=2.4) {
   if ( jet->chargedHadronEnergyFraction() <= 0 ) return false;
   if ( jet->chargedMultiplicity() <= 0 ) return false;
   if ( jet->chargedEmEnergyFraction() >= 0.99 ) return false;
  }
  
  return true;  
 }
 
 else if(applyID == 8) { //---- first Run II jet ID: TIGHT
  //---- see https://twiki.cern.ch/twiki/bin/view/CMS/JetID
  if (jet->neutralHadronEnergyFraction() >=0.90) return false;
  if (jet->neutralEmEnergyFraction() >=0.90) return false;
  
  unsigned int multiplicity = jet->chargedMultiplicity() + jet->neutralMultiplicity();
  if ( multiplicity <= 1) return false;
  if ( jet->muonEnergyFraction() >= 0.8) return false;
  
  if(fabs(jet->eta())<=2.4) {
   if ( jet->chargedHadronEnergyFraction() <= 0 ) return false;
   if ( jet->chargedMultiplicity() <= 0 ) return false;
   if ( jet->chargedEmEnergyFraction() >= 0.90 ) return false;
  }
  
  return true;  
 }
 
 
 return false;
}


const bool reco::SkimEvent::passFatJetID(pat::JetRef jet, int applyID) const{
 // no ID
 if(applyID == 0) return true;
 
 // old ID
 else if(applyID == 1) { ///----> AN 2013/139 Sec 3.1.3
  // muon energy fraction < 0.99
  // photon energy fraction < 0.99,
  // electromagnetic energy fraction < 0.99
  // number of jet constituent grater than one,
  // neutral and charged hadron energy fraction respectively < 0.99 and > 0
  unsigned int multiplicity = jet->neutralMultiplicity () + jet->chargedMultiplicity ();
  
  // if(
  // jet -> muonEnergyFraction() >= 0.99 ||
  // jet -> photonEnergyFraction() >= 0.99 ||
  // (jet -> chargedEmEnergy() + jet -> neutralEmEnergy() ) / jet -> energy() * jet -> userFloat("JEC") >= 0.99 || // energy is uncorrected!
  // multiplicity<=1 ||
  // jet->neutralHadronEnergyFraction() >=0.99 ||
  // jet->chargedHadronEnergyFraction() <=0.0
  // ) {
  // return false;
  // }
  // else {
  // return true;
  // }
  
  ///---- Loose from https://twiki.cern.ch/twiki/bin/view/CMS/JetID
  if(
   jet->neutralEmEnergyFraction() >=0.99 ||
   jet->neutralHadronEnergyFraction() >=0.99 ||
   multiplicity<=1
  ) return false;
  
  if(fabs(jet->eta())<2.4){
   if(
    jet->chargedEmEnergyFraction() >=0.99 ||
    jet->chargedHadronEnergyFraction() == 0 ||
    jet->chargedMultiplicity()==0
   ) return false;
  }
  return true;
 }
 return false;
}


const float reco::SkimEvent::dPhiJetllInDegrees(size_t leadingIndex,float minPt,float eta,int applyCorrection,int applyID) const {
 return dPhiJetll(leadingIndex,minPt,eta,applyCorrection,applyID)/M_PI * 180.;
}

const float reco::SkimEvent::dPhiJetll(size_t leadingIndex,float minPt,float eta,int applyCorrection,int applyID) const {
 
 if(leps_.size() < 2) return defaultvalues::defaultFloat;
 
 size_t count = 0, newIndex = 0;
 for(size_t i=0;i<jets_.size();++i) {
  if(!(passJetID(jets_[i],applyID)) ) continue;
  if( std::fabs(jets_[i]->eta()) >= eta) continue;
  if( jetPt(i,applyCorrection) <= minPt) continue;
  
  if(isThisJetALepton(jets_[i])) continue;
  // When count becomes higher than leadingIndex, we've found the leadingIndexth jet that passes all the cuts
  if(++count > leadingIndex) {
   newIndex = i;
   break;
  }
 }
 
 return fabs(ROOT::Math::VectorUtil::DeltaPhi(leps_[indexByPt(0)]->p4() + leps_[indexByPt(1)]->p4(), jets_[newIndex]->p4()) );
 
}



const math::XYZTLorentzVector reco::SkimEvent::jet(size_t index, float minPt,float eta,int applyCorrection,int applyID) const {
  unsigned int index_jet_ordered = indexJetByPt(index, minPt, eta, applyCorrection, applyID);
  if (index_jet_ordered >= jets_.size()) return math::XYZTLorentzVector(0,0,0,0);
  return jets_[index_jet_ordered]->p4();
}





const pat::Jet* reco::SkimEvent::leadingJet(size_t index, float minPt,float eta,int applyCorrection,int applyID) const {
 
 size_t count = 0;
 for(size_t i=0;i<jets_.size();++i) {
  if(!(passJetID(jets_[i],applyID)) ) continue;
  if( std::fabs(jets_[i]->eta()) >= eta) continue;
  if( jetPt(i,applyCorrection) <= minPt) continue;
  
  if(isThisJetALepton(jets_[i])) continue;
  if(++count > index) return jets_[i].get();
 }
 return new pat::Jet();
}



const float reco::SkimEvent::leadingJetPUid(size_t index, float minPt,float eta,int applyCorrection,int applyID, float dzCut) const {
 
 size_t count = 0;
 for(size_t i=0;i<jets_.size();++i) {
  if(!(passJetID(jets_[i],applyID)) ) continue;
  if( std::fabs(jets_[i]->eta()) >= eta) continue;
  if( jetPt(i,applyCorrection) <= minPt) continue;
  
  if(isThisJetALepton(jets_[i])) continue;
  if(jets_[i]->hasUserFloat("dz") && fabs(jets_[i]->userFloat("dz")) > dzCut) continue;
  if(++count > index) {
//    std::cout << " jets_[" << i << "]->userFloat(pileupJetId:fullDiscriminant) = " << jets_[i]->userFloat("pileupJetId:fullDiscriminant") << std::endl;
//    if (jets_[i]->userFloat("pileupJetId:fullDiscriminant") ) return jets_[i]->userFloat("pileupJetId:fullDiscriminant");
//    if (jets_[i]->userFloat("pileupJetIdEvaluator:fullDiscriminant") ) return jets_[i]->userFloat("pileupJetIdEvaluator:fullDiscriminant");  
//    if (jets_[i]->userFloat("AK4PFCHSpileupJetIdEvaluator:fullDiscriminant") ) return jets_[i]->userFloat("AK4PFCHSpileupJetIdEvaluator:fullDiscriminant");  
   if (jets_[i]->userFloat(_name_puJetIdDiscriminant) ) return jets_[i]->userFloat(_name_puJetIdDiscriminant);  
   
   else return defaultvalues::defaultFloat;
  }
 }
 return defaultvalues::defaultFloat;
 
}


const float reco::SkimEvent::leadingJetPUid(size_t index) const { 
 return leadingJetPUid(index,minPtForJets_,maxEtaForJets_,applyCorrectionForJets_,applyIDForJets_);
}


const float reco::SkimEvent::leadingJetPartonFlavour(size_t index, float minPt,float eta,int applyCorrection,int applyID, float dzCut) const {
 
 size_t count = 0;
 for(size_t i=0;i<jets_.size();++i) {
  if(!(passJetID(jets_[i],applyID)) ) continue;
  if( std::fabs(jets_[i]->eta()) >= eta) continue;
  if( jetPt(i,applyCorrection) <= minPt) continue;
  
  if(isThisJetALepton(jets_[i])) continue;
  if(jets_[i]->hasUserFloat("dz") && fabs(jets_[i]->userFloat("dz")) > dzCut) continue;
  if(++count > index) return jets_[i]->partonFlavour();
 }
 return -9999.9;
 
}


const float reco::SkimEvent::leadingJetPartonFlavour(size_t index) const { 
 return leadingJetPartonFlavour(index,minPtForJets_,maxEtaForJets_,applyCorrectionForJets_,applyIDForJets_);
}


const float reco::SkimEvent::leadingJetHadronFlavour(size_t index, float minPt,float eta,int applyCorrection,int applyID, float dzCut) const {
 
 size_t count = 0;
 for(size_t i=0;i<jets_.size();++i) {
  if(!(passJetID(jets_[i],applyID)) ) continue;
  if( std::fabs(jets_[i]->eta()) >= eta) continue;
  if( jetPt(i,applyCorrection) <= minPt) continue;
  
  if(isThisJetALepton(jets_[i])) continue;
  if(jets_[i]->hasUserFloat("dz") && fabs(jets_[i]->userFloat("dz")) > dzCut) continue;
  if(++count > index) return jets_[i]->hadronFlavour();
 }
 return -9999.9;
 
}


const float reco::SkimEvent::leadingJetHadronFlavour(size_t index) const { 
 return leadingJetHadronFlavour(index,minPtForJets_,maxEtaForJets_,applyCorrectionForJets_,applyIDForJets_);
}



const float reco::SkimEvent::leadingJetBtag(size_t index, std::string discriminator, float minPt,float eta,int applyCorrection,int applyID, float dzCut) const {

 size_t count = 0;
 for(size_t i=0;i<jets_.size();++i) {
  if(!(passJetID(jets_[i],applyID)) ) continue;
  if( std::fabs(jets_[i]->eta()) >= eta) continue;
  if( jetPt(i,applyCorrection) <= minPt) continue;
  
  if(isThisJetALepton(jets_[i])) continue;
  if(jets_[i]->hasUserFloat("dz") && fabs(jets_[i]->userFloat("dz")) > dzCut) continue;
  if(++count > index) return jets_[i]->bDiscriminator(discriminator);
  
 }
 return -9999.9;
 
}


const float reco::SkimEvent::leadingJetMass(size_t index, float minPt,float eta,int applyCorrection,int applyID) const {
 
 size_t count = 0;
 for(size_t i=0;i<jets_.size();++i) {
  if(!(passJetID(jets_[i],applyID)) ) continue;
  if( std::fabs(jets_[i]->eta()) >= eta) continue;
  if( jetPt(i,applyCorrection) <= minPt) continue;
  
  if(isThisJetALepton(jets_[i])) continue;
  if(++count > index) return jets_[i]->mass();
 }
 return -9999.9;
 
}


const float reco::SkimEvent::leadingJetPhi(size_t index, float minPt,float eta,int applyCorrection,int applyID) const {
 
 size_t count = 0;
 for(size_t i=0;i<jets_.size();++i) {
  if(!(passJetID(jets_[i],applyID)) ) continue;
  if( std::fabs(jets_[i]->eta()) >= eta) continue;
  if( jetPt(i,applyCorrection) <= minPt) continue;
  
  if(isThisJetALepton(jets_[i])) continue;
  if(++count > index) return jets_[i]->phi();
 }
 return -9999.9;
 
}

const float reco::SkimEvent::leadingJetEta(size_t index, float minPt,float eta,int applyCorrection,int applyID) const {
 
 size_t count = 0;
 for(size_t i=0;i<jets_.size();++i) {
  if(!(passJetID(jets_[i],applyID)) ) continue;
  if( std::fabs(jets_[i]->eta()) >= eta) continue;
  if( jetPt(i,applyCorrection) <= minPt) continue;
  
  if(isThisJetALepton(jets_[i])) continue;
  if(++count > index) return jets_[i]->eta();
 }
 return -9999.9;
 
}

const int reco::SkimEvent::leadingJetId(size_t index, float minPt,float eta,int applyCorrection,int applyID) const {
 
 size_t count = 0;
 for(size_t i=0;i<jets_.size();++i) {
  if(!(passJetID(jets_[i],applyID)) ) continue;
  if( std::fabs(jets_[i]->eta()) >= eta) continue;
  if( jetPt(i,applyCorrection) <= minPt) continue;
  
  if(isThisJetALepton(jets_[i])) continue;
  if(++count > index) return jets_[i]->userInt("jetId");
 }
 return -9999.9;
}

const float reco::SkimEvent::leadingJetMva(size_t index, float minPt,float eta,int applyCorrection,int applyID) const {
 
 size_t count = 0;
 for(size_t i=0;i<jets_.size();++i) {
  if(!(passJetID(jets_[i],applyID)) ) continue;
  if( std::fabs(jets_[i]->eta()) >= eta) continue;
  if( jetPt(i,applyCorrection) <= minPt) continue;
  
  if(isThisJetALepton(jets_[i])) continue;
  if(++count > index) return jets_[i]->userFloat("jetMva");
 }
 return -9999.9;
}



const float reco::SkimEvent::leadingSecondJetPt(size_t index, float minPt,float eta,int applyCorrection,int applyID) const {
 
 size_t count = 0;
 for(size_t i=0;i<secondJets_.size();++i) {
  //   if(!(passJetID(secondJets_[i],applyID)) ) continue;
  if( std::fabs(secondJets_[i]->eta()) >= eta) continue;
  if( secondJetPt(i,applyCorrection) <= minPt) continue;
  
  if(isThisJetALepton(secondJets_[i])) continue;
  if(++count > index) return secondJetPt(i,applyCorrection);
 }
 return -9999.9;
}



const float reco::SkimEvent::leadingSecondJetEta(size_t index, float minPt,float eta,int applyCorrection,int applyID) const {
 
 size_t count = 0;
 for(size_t i=0;i<secondJets_.size();++i) {
  //   if(!(passJetID(secondJets_[i],applyID)) ) continue;
  if( std::fabs(secondJets_[i]->eta()) >= eta) continue;
  if( secondJetPt(i,applyCorrection) <= minPt) continue;
  
  if(isThisJetALepton(secondJets_[i])) continue;
  if(++count > index) return secondJets_[i]->eta();
 }
 return -9999.9;
}


const float reco::SkimEvent::leadingSecondJetPhi(size_t index, float minPt,float eta,int applyCorrection,int applyID) const {
 
 size_t count = 0;
 for(size_t i=0;i<secondJets_.size();++i) {
  //   if(!(passJetID(secondJets_[i],applyID)) ) continue;
  if( std::fabs(secondJets_[i]->eta()) >= eta) continue;
  if( secondJetPt(i,applyCorrection) <= minPt) continue;
  
  if(isThisJetALepton(secondJets_[i])) continue;
  if(++count > index) return secondJets_[i]->phi();
 }
 return -9999.9;
}




const float reco::SkimEvent::leadingSecondJetPt(size_t index) const {
 return leadingSecondJetPt(index,minPtForJets_,maxEtaForJets_,applyCorrectionForJets_,applyIDForJets_); //---- FIXME check default values
}

const float reco::SkimEvent::leadingSecondJetEta(size_t index) const {
 return leadingSecondJetEta(index,minPtForJets_,maxEtaForJets_,applyCorrectionForJets_,applyIDForJets_); //---- FIXME check default values
}

const float reco::SkimEvent::leadingSecondJetPhi(size_t index) const {
 return leadingSecondJetPhi(index,minPtForJets_,maxEtaForJets_,applyCorrectionForJets_,applyIDForJets_); //---- FIXME check default values
}



const float reco::SkimEvent::leadingJetPt(size_t index) const { 
 return leadingJetPt(index,minPtForJets_,maxEtaForJets_,applyCorrectionForJets_,applyIDForJets_);
}

const float reco::SkimEvent::leadingJetEta(size_t index) const { 
 return leadingJetEta(index,minPtForJets_,maxEtaForJets_,applyCorrectionForJets_,applyIDForJets_);
}

const float reco::SkimEvent::leadingJetPhi(size_t index) const { 
 return leadingJetPhi(index,minPtForJets_,maxEtaForJets_,applyCorrectionForJets_,applyIDForJets_);
}


const float reco::SkimEvent::leadingJetPt(size_t index, float minPt,float eta,int applyCorrection,int applyID) const {
 size_t count = 0;
 for(size_t i=0;i<jets_.size();++i) {
//   std::cout << " >> jets_[" << i << "]->pt() = " << jets_[i]->pt() << " > " << minPt << std::endl;
  if(!(passJetID(jets_[i],applyID)) ) continue;
//   std::cout << " >> jets_[" << i << "]->eta() = " << jets_[i]->eta() << " < " << eta << std::endl;
  if( std::fabs(jets_[i]->eta()) >= eta) continue;
  if( jetPt(i,applyCorrection) <= minPt) continue;
//   std::cout << " >> isThisJetALepton(jets_[i]) = " << isThisJetALepton(jets_[i]) << std::endl;
  if(isThisJetALepton(jets_[i])) continue;
  if(++count > index) return jetPt(i,applyCorrection);
 }
 return -9999.9;
}


const float reco::SkimEvent::leadingJetPtd(size_t index, float minPt,float eta,int applyCorrection,int applyID) const {
 
 size_t count = 0;
 for(size_t i=0;i<jets_.size();++i) {
  if(!(passJetID(jets_[i],applyID)) ) continue;
  if( std::fabs(jets_[i]->eta()) >= eta) continue;
  if( jetPt(i,applyCorrection) <= minPt) continue;
  
  if(isThisJetALepton(jets_[i])) continue;
  if(++count > index) {
   return jets_[i]->userFloat("ptd");
  }
 }
 return -9999.9;
 
}



const float reco::SkimEvent::leadingJetNChgQC(size_t index, float minPt,float eta,int applyCorrection,int applyID) const {
 
 size_t count = 0;
 for(size_t i=0;i<jets_.size();++i) {
  if(!(passJetID(jets_[i],applyID)) ) continue;
  if( std::fabs(jets_[i]->eta()) >= eta) continue;
  if( jetPt(i,applyCorrection) <= minPt) continue;
  
  if(isThisJetALepton(jets_[i])) continue;
  if(++count > index) {
   return jets_[i]->userFloat("nChgQC");
  }
 }
 return -9999.9;
 
}


const float reco::SkimEvent::leadingJetNChgptCut(size_t index, float minPt,float eta,int applyCorrection,int applyID) const {
 
 size_t count = 0;
 for(size_t i=0;i<jets_.size();++i) {
  if(!(passJetID(jets_[i],applyID)) ) continue;
  if( std::fabs(jets_[i]->eta()) >= eta) continue;
  if( jetPt(i,applyCorrection) <= minPt) continue;
  
  if(isThisJetALepton(jets_[i])) continue;
  if(++count > index) {
   return jets_[i]->userFloat("nChgptCut");
  }
 }
 return -9999.9;
 
}


const float reco::SkimEvent::leadingJetNNeutralptCut(size_t index, float minPt,float eta,int applyCorrection,int applyID) const {
 
 size_t count = 0;
 for(size_t i=0;i<jets_.size();++i) {
  if(!(passJetID(jets_[i],applyID)) ) continue;
  if( std::fabs(jets_[i]->eta()) >= eta) continue;
  if( jetPt(i,applyCorrection) <= minPt) continue;
  
  if(isThisJetALepton(jets_[i])) continue;
  if(++count > index) {
   return jets_[i]->userFloat("nNeutralptCut");
  }
 }
 return -9999.9;
 
}


const float reco::SkimEvent::leadingJetPtD(size_t index, float minPt,float eta,int applyCorrection,int applyID, int QualityCut) const {
 
 size_t count = 0;
 for(size_t i=0;i<jets_.size();++i) {
  if(!(passJetID(jets_[i],applyID)) ) continue;
  if( std::fabs(jets_[i]->eta()) >= eta) continue;
  if( jetPt(i,applyCorrection) <= minPt) continue;
  
  if(isThisJetALepton(jets_[i])) continue;
  if(++count > index) {
   if (QualityCut == 1) return jets_[i]->userFloat("QCptD");
   else return jets_[i]->userFloat("ptD");
  }
 }
 return -9999.9;
 
}





const float reco::SkimEvent::leadingJetQGaxis1(size_t index) const {
 return leadingJetQGaxis1(index,minPtForJets_,maxEtaForJets_,applyCorrectionForJets_,applyIDForJets_,1);
 //  FIXME QualityCut == 1 hardcoded
}

const float reco::SkimEvent::leadingJetQGaxis2(size_t index) const {
 return leadingJetQGaxis2(index,minPtForJets_,maxEtaForJets_,applyCorrectionForJets_,applyIDForJets_,1);
 //  FIXME QualityCut == 1 hardcoded 
}

const float reco::SkimEvent::leadingJetQGRMScand(size_t index) const {
 return leadingJetQGRMScand(index,minPtForJets_,maxEtaForJets_,applyCorrectionForJets_,applyIDForJets_,1);
 //  FIXME QualityCut == 1 hardcoded
}

const float reco::SkimEvent::leadingJetQGRmax(size_t index) const {
 return leadingJetQGRmax(index,minPtForJets_,maxEtaForJets_,applyCorrectionForJets_,applyIDForJets_,1);
 //  FIXME QualityCut == 1 hardcoded
}

const float reco::SkimEvent::leadingJetQGlikelihood(size_t index) const {
 return leadingJetQGlikelihood(index,minPtForJets_,maxEtaForJets_,applyCorrectionForJets_,applyIDForJets_);
}






const float reco::SkimEvent::leadingJetQGRmax(size_t index, float minPt,float eta,int applyCorrection,int applyID, int QualityCut) const {
 
 size_t count = 0;
 for(size_t i=0;i<jets_.size();++i) {
  if(!(passJetID(jets_[i],applyID)) ) continue;
  if( std::fabs(jets_[i]->eta()) >= eta) continue;
  if( jetPt(i,applyCorrection) <= minPt) continue;
  
  if(isThisJetALepton(jets_[i])) continue;
  if(++count > index) {
   if (QualityCut == 1) return jets_[i]->userFloat("QCRmax");
   else return jets_[i]->userFloat("Rmax");
  }
 }
 return -9999.9;
 
}


const float reco::SkimEvent::leadingJetQGRMScand(size_t index, float minPt,float eta,int applyCorrection,int applyID, int QualityCut) const {
 
 size_t count = 0;
 for(size_t i=0;i<jets_.size();++i) {
  if(!(passJetID(jets_[i],applyID)) ) continue;
  if( std::fabs(jets_[i]->eta()) >= eta) continue;
  if( jetPt(i,applyCorrection) <= minPt) continue;
  
  if(isThisJetALepton(jets_[i])) continue;
  if(++count > index) {
   if (QualityCut == 1) return jets_[i]->userFloat("QCRMScand");
   else return jets_[i]->userFloat("RMScand");
  }
 }
 return -9999.9;
 
}


const float reco::SkimEvent::leadingJetQGaxis1(size_t index, float minPt,float eta,int applyCorrection,int applyID, int QualityCut) const {
 
 size_t count = 0;
 for(size_t i=0;i<jets_.size();++i) {
  if(!(passJetID(jets_[i],applyID)) ) continue;
  if( std::fabs(jets_[i]->eta()) >= eta) continue;
  if( jetPt(i,applyCorrection) <= minPt) continue;
  
  if(isThisJetALepton(jets_[i])) continue;
  if(++count > index) {
   if (QualityCut == 1) return jets_[i]->userFloat("QCaxis1");
   else return jets_[i]->userFloat("axis1");
  }
 }
 return -9999.9;
 
}


const float reco::SkimEvent::leadingJetQGaxis2(size_t index, float minPt,float eta,int applyCorrection,int applyID, int QualityCut) const {
 
 size_t count = 0;
 for(size_t i=0;i<jets_.size();++i) {
  if(!(passJetID(jets_[i],applyID)) ) continue;
  if( std::fabs(jets_[i]->eta()) >= eta) continue;
  if( jetPt(i,applyCorrection) <= minPt) continue;
  
  if(isThisJetALepton(jets_[i])) continue;
  if(++count > index) {
   if (QualityCut == 1) return jets_[i]->userFloat("QCaxis2");
   else return jets_[i]->userFloat("axis2");
  }
 }
 return -9999.9;
 
}



const float reco::SkimEvent::leadingJetQGlikelihood(size_t index, float minPt,float eta,int applyCorrection,int applyID) const {
 
 size_t count = 0;
 for(size_t i=0;i<jets_.size();++i) {
  if(!(passJetID(jets_[i],applyID)) ) continue;
  if( std::fabs(jets_[i]->eta()) >= eta) continue;
  if( jetPt(i,applyCorrection) <= minPt) continue;
  
  if(isThisJetALepton(jets_[i])) continue;
  if(++count > index) {
   return jets_[i]->userFloat("QGTagger:qgLikelihood");
  }
 }
 return -9999.9;
 
}






const float reco::SkimEvent::leadingJetChargedHadronMultiplicity(size_t index, float minPt,float eta,int applyCorrection,int applyID) const {
 
 size_t count = 0;
 for(size_t i=0;i<jets_.size();++i) {
  if(!(passJetID(jets_[i],applyID)) ) continue;
  if( std::fabs(jets_[i]->eta()) >= eta) continue;
  if( jetPt(i,applyCorrection) <= minPt) continue;
  
  if(isThisJetALepton(jets_[i])) continue;
  if(++count > index) return jets_[i]->chargedHadronMultiplicity();
 }
 return -9999.9;
 
}

const float reco::SkimEvent::leadingJetNeutralHadronMultiplicity(size_t index, float minPt,float eta,int applyCorrection,int applyID) const {
 
 size_t count = 0;
 for(size_t i=0;i<jets_.size();++i) {
  if(!(passJetID(jets_[i],applyID)) ) continue;
  if( std::fabs(jets_[i]->eta()) >= eta) continue;
  if( jetPt(i,applyCorrection) <= minPt) continue;
  
  if(isThisJetALepton(jets_[i])) continue;
  if(++count > index) return jets_[i]->neutralHadronMultiplicity();
 }
 return -9999.9;
 
}

const float reco::SkimEvent::leadingJetPhotonMultiplicity(size_t index, float minPt,float eta,int applyCorrection,int applyID) const {
 
 size_t count = 0;
 for(size_t i=0;i<jets_.size();++i) {
  if(!(passJetID(jets_[i],applyID)) ) continue;
  if( std::fabs(jets_[i]->eta()) >= eta) continue;
  if( jetPt(i,applyCorrection) <= minPt) continue;
  
  if(isThisJetALepton(jets_[i])) continue;
  if(++count > index) return jets_[i]->photonMultiplicity();
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
//


const float reco::SkimEvent::leadingVBFJetPhi(size_t index, float minPt,float eta,int applyCorrection,int applyID) const {
 if(jets_.size() < 2) return -9999.;
 float jetEta1 = leadingJetEta(0,minPt,eta,applyCorrection,applyID);
 float jetEta2 = leadingJetEta(1,minPt,eta,applyCorrection,applyID);
 float jetEtaMax = std::max(jetEta1,jetEta2);
 float jetEtaMin = std::min(jetEta1,jetEta2);
 
 size_t count = 0;
 for(size_t i=0;i<jets_.size();++i) {
  if(!(passJetID(jets_[i],applyID)) ) continue;
  if( std::fabs(jets_[i]->eta()) >= eta) continue;
  if( jetPt(i,applyCorrection) <= minPt) continue;
  
  if(isThisJetALepton(jets_[i])) continue;
  if( jets_[i]->eta() >= jetEtaMax ) continue;
  if( jets_[i]->eta() <= jetEtaMin ) continue;
  if(++count > index) return jets_[i]->phi();
 }
 return -9999.9;
}

const float reco::SkimEvent::leadingVBFJetEta(size_t index, float minPt,float eta,int applyCorrection,int applyID) const {
 if(jets_.size() < 2) return -9999.;
 float jetEta1 = leadingJetEta(0,minPt,eta,applyCorrection,applyID);
 float jetEta2 = leadingJetEta(1,minPt,eta,applyCorrection,applyID);
 float jetEtaMax = std::max(jetEta1,jetEta2);
 float jetEtaMin = std::min(jetEta1,jetEta2);
 
 size_t count = 0;
 for(size_t i=0;i<jets_.size();++i) {
  if(!(passJetID(jets_[i],applyID)) ) continue;
  if( std::fabs(jets_[i]->eta()) >= eta) continue;
  if( jetPt(i,applyCorrection) <= minPt) continue;
  
  if(isThisJetALepton(jets_[i])) continue;
  if( jets_[i]->eta() >= jetEtaMax ) continue;
  if( jets_[i]->eta() <= jetEtaMin ) continue;
  if(++count > index) return jets_[i]->eta();
 }
 return -9999.9;
 
}

const int reco::SkimEvent::leadingVBFJetId(size_t index, float minPt,float eta,int applyCorrection,int applyID) const {
 if(jets_.size() < 2) return -9999.;
 float jetEta1 = leadingJetEta(0,minPt,eta,applyCorrection,applyID);
 float jetEta2 = leadingJetEta(1,minPt,eta,applyCorrection,applyID);
 float jetEtaMax = std::max(jetEta1,jetEta2);
 float jetEtaMin = std::min(jetEta1,jetEta2);
 
 size_t count = 0;
 for(size_t i=0;i<jets_.size();++i) {
  if(!(passJetID(jets_[i],applyID)) ) continue;
  if( std::fabs(jets_[i]->eta()) >= eta) continue;
  if( jetPt(i,applyCorrection) <= minPt) continue;
  
  if(isThisJetALepton(jets_[i])) continue;
  if( jets_[i]->eta() >= jetEtaMax ) continue;
  if( jets_[i]->eta() <= jetEtaMin ) continue;
  if(++count > index) return jets_[i]->userInt("jetId");
 }
 return -9999.9;
}

const float reco::SkimEvent::leadingVBFJetMva(size_t index, float minPt,float eta,int applyCorrection,int applyID) const {
 if(jets_.size() < 2) return -9999.;
 float jetEta1 = leadingJetEta(0,minPt,eta,applyCorrection,applyID);
 float jetEta2 = leadingJetEta(1,minPt,eta,applyCorrection,applyID);
 float jetEtaMax = std::max(jetEta1,jetEta2);
 float jetEtaMin = std::min(jetEta1,jetEta2);
 
 size_t count = 0;
 for(size_t i=0;i<jets_.size();++i) {
  if(!(passJetID(jets_[i],applyID)) ) continue;
  if( std::fabs(jets_[i]->eta()) >= eta) continue;
  if( jetPt(i,applyCorrection) <= minPt) continue;
  
  if(isThisJetALepton(jets_[i])) continue;
  if( jets_[i]->eta() >= jetEtaMax ) continue;
  if( jets_[i]->eta() <= jetEtaMin ) continue;
  if(++count > index) return jets_[i]->userInt("jetMva");
 }
 return -9999.9;
}

const float reco::SkimEvent::leadingVBFJetPt(size_t index, float minPt,float eta,int applyCorrection,int applyID) const {
 if(jets_.size() < 2) return -9999.;
 float jetEta1 = leadingJetEta(0,minPt,eta,applyCorrection,applyID);
 float jetEta2 = leadingJetEta(1,minPt,eta,applyCorrection,applyID);
 float jetEtaMax = std::max(jetEta1,jetEta2);
 float jetEtaMin = std::min(jetEta1,jetEta2);
 
 size_t count = 0;
 for(size_t i=0;i<jets_.size();++i) {
  if(!(passJetID(jets_[i],applyID)) ) continue;
  if( std::fabs(jets_[i]->eta()) >= eta) continue;
  if( jetPt(i,applyCorrection) <= minPt) continue;
  
  if(isThisJetALepton(jets_[i])) continue;
  if( jets_[i]->eta() >= jetEtaMax ) continue;
  if( jets_[i]->eta() <= jetEtaMin ) continue;
  if(++count > index) return jetPt(i,applyCorrection);
 }
 return -9999.9;
}

const int reco::SkimEvent::nCentralJets(float minPt,float eta,int applyCorrection,int applyID) const {
 
 int count = 0;
 for(size_t i=0;i<jets_.size();++i) {
  if(!(passJetID(jets_[i],applyID)) ) continue;
  if( std::fabs(jets_[i]->eta()) >= eta) continue;
  if( jetPt(i,applyCorrection) <= minPt) continue;
  
  if(isThisJetALepton(jets_[i])) continue;
  count++;
 }
 return count;
}

const bool reco::SkimEvent::passesDPhillJet(float ptMin, float eta,int applyCorrection,int applyID) const {
 if(leps_.size() < 2) return defaultvalues::defaultFloat;   
 float dphi = 0, ptMax = 0;
 for(size_t i=0;i<jets_.size();++i) {
  if(!(passJetID(jets_[i],applyID)) ) continue;
  if( std::fabs(jets_[i]->eta()) >= eta) continue;
  
  if(isThisJetALepton(jets_[i])) continue;
  float pt = jetPt(i,applyCorrection);
  if (pt > ptMax) {
   ptMax = pt;
   dphi = fabs(ROOT::Math::VectorUtil::DeltaPhi(leps_[indexByPt(0)]->p4()+leps_[indexByPt(1)]->p4(), jets_[i]->p4()) );
  }
 }
 return (ptMax <= ptMin || dphi / M_PI * 180. < 165.0);
}

const float reco::SkimEvent::dPhillLeadingJet(float eta,int applyCorrection,int applyID) const {
 if(leps_.size() < 2) return defaultvalues::defaultFloat;
 float dphi = 0, ptMax = 0;
 for(size_t i=0;i<jets_.size();++i) {
  if(!(passJetID(jets_[i],applyID)) ) continue;
  if( std::fabs(jets_[i]->eta()) >= eta) continue;
  
  if(isThisJetALepton(jets_[i])) continue;
  float pt = jetPt(i,applyCorrection);
  if (pt > ptMax) {
   ptMax = pt;
   dphi = fabs(ROOT::Math::VectorUtil::DeltaPhi(leps_[indexByPt(0)]->p4()+leps_[indexByPt(1)]->p4(), jets_[i]->p4()) );
  }
 }
 return dphi;
}





//---- jet closest to lepton
//----     like normal jets 


const float reco::SkimEvent::leadingJetCloseLeptonPt(size_t ilepton = 0) const {
 return leadingJetCloseLeptonPt(ilepton, minPtForJets_, maxEtaForJets_, applyCorrectionForJets_, applyIDForJets_); 
}

const float reco::SkimEvent::leadingJetCloseLeptonPt(size_t ilepton, float ptminjet ,float etamaxjet,int applyCorrection, int applyID) const { 
 
 if (ilepton < leps_.size()) {
  float minDR = 9999999.9;
  int numJet = -1;
  
 for(size_t i=0;i<jets_.size();++i) {
  if(!(passJetID(jets_[i],applyID)) ) continue;
  if( std::fabs(jets_[i]->eta()) >= etamaxjet) continue;
  if( jetPt(i,applyCorrection) <= ptminjet) continue;
  
  //---- save the first jet closest to the lepton
  double dR = fabs(ROOT::Math::VectorUtil::DeltaR(leps_[ilepton]->p4(),jets_[i]->p4()) );
  if (dR < minDR) {
    minDR = dR;
    numJet = i;
   }
  }
  if (numJet != -1) {
   return jetPt(numJet,applyCorrection);
  }
 }
 
 return defaultvalues::defaultFloat;
}



const float reco::SkimEvent::leadingJetCloseLeptonEta(size_t ilepton = 0) const {
 return leadingJetCloseLeptonEta(ilepton, minPtForJets_, maxEtaForJets_, applyCorrectionForJets_, applyIDForJets_); 
}

const float reco::SkimEvent::leadingJetCloseLeptonEta(size_t ilepton, float ptminjet ,float etamaxjet,int applyCorrection, int applyID) const { 
 
 if (ilepton < leps_.size()) {
  float minDR = 9999999.9;
  int numJet = -1;
  
  for(size_t i=0;i<jets_.size();++i) {
   if(!(passJetID(jets_[i],applyID)) ) continue;
   if( std::fabs(jets_[i]->eta()) >= etamaxjet) continue;
   if( jetPt(i,applyCorrection) <= ptminjet) continue;
   
   //---- save the first jet closest to the lepton
   double dR = fabs(ROOT::Math::VectorUtil::DeltaR(leps_[ilepton]->p4(),jets_[i]->p4()) );
   if (dR < minDR) {
    minDR = dR;
    numJet = i;
   }
  }
  if (numJet != -1) {
   return jets_[numJet]->eta();
  }
 }
 
 return defaultvalues::defaultFloat;
}



const float reco::SkimEvent::leadingJetCloseLeptonPhi(size_t ilepton = 0) const {
 return leadingJetCloseLeptonPhi(ilepton, minPtForJets_, maxEtaForJets_, applyCorrectionForJets_, applyIDForJets_); 
}

const float reco::SkimEvent::leadingJetCloseLeptonPhi(size_t ilepton, float ptminjet ,float etamaxjet,int applyCorrection, int applyID) const { 
 
 if (ilepton < leps_.size()) {
  float minDR = 9999999.9;
  int numJet = -1;
  
  for(size_t i=0;i<jets_.size();++i) {
   if(!(passJetID(jets_[i],applyID)) ) continue;
   if( std::fabs(jets_[i]->eta()) >= etamaxjet) continue;
   if( jetPt(i,applyCorrection) <= ptminjet) continue;
   
   //---- save the first jet closest to the lepton
   double dR = fabs(ROOT::Math::VectorUtil::DeltaR(leps_[ilepton]->p4(),jets_[i]->p4()) );
   if (dR < minDR) {
    minDR = dR;
    numJet = i;
   }
  }
  if (numJet != -1) {
   return jets_[numJet]->phi();
  }
 }
 
 return defaultvalues::defaultFloat;
}




const float reco::SkimEvent::leadingJetCloseLeptonFlavour(size_t ilepton = 0) const {
 return leadingJetCloseLeptonFlavour(ilepton, minPtForJets_, maxEtaForJets_, applyCorrectionForJets_, applyIDForJets_); 
}

const float reco::SkimEvent::leadingJetCloseLeptonFlavour(size_t ilepton, float ptminjet ,float etamaxjet,int applyCorrection, int applyID) const { 
 
 if (ilepton < leps_.size()) {
  float minDR = 9999999.9;
  int numJet = -1;
  
  for(size_t i=0;i<jets_.size();++i) {
   if(!(passJetID(jets_[i],applyID)) ) continue;
   if( std::fabs(jets_[i]->eta()) >= etamaxjet) continue;
   if( jetPt(i,applyCorrection) <= ptminjet) continue;
   
   //---- save the first jet closest to the lepton
   double dR = fabs(ROOT::Math::VectorUtil::DeltaR(leps_[ilepton]->p4(),jets_[i]->p4()) );
   if (dR < minDR) {
    minDR = dR;
    numJet = i;
   }
  }
  if (numJet != -1) {
   return jets_[numJet]->partonFlavour();
  }
 }
 
 return defaultvalues::defaultFloat;
}







// const int reco::SkimEvent::leadingJetIndex(size_t index, float minPt,float eta,int applyCorrection,int applyID) const {
//   
//     size_t count = 0;
//     for(size_t i=0;i<jets_.size();++i) {
//       if(!(passJetID(jets_[i],applyID)) ) continue;
//       if( std::fabs(jets_[i]->eta()) >= eta) continue;
//       if( jetPt(i,applyCorrection) <= minPt) continue;
// 
//       if(isThisJetALepton(jets_[i])) continue;
//       if(++count > index) return i;
//     }
//     return -1;
// }

const float reco::SkimEvent::dPhilljetjet(float eta,int applyCorrection,int applyID) const {
 if(leps_.size() < 2) return defaultvalues::defaultFloat;
 float dphi = -1;
 int jet1 = indexJetByPt(0,0,eta,applyCorrection,applyID);
 int jet2 = indexJetByPt(1,0,eta,applyCorrection,applyID);
 
 if (jet1 != 9999 && jet2 != 9999) dphi = fabs(ROOT::Math::VectorUtil::DeltaPhi(leps_[indexByPt(0)]->p4()+leps_[indexByPt(1)]->p4(), jets_[jet1]->p4()+jets_[jet2]->p4()) );
 
 return dphi;
}



const float reco::SkimEvent::fatJetPt(size_t i, int applyCorrection) const {
 return jetPt(fatJets_[i].get(),applyCorrection);
}

const float reco::SkimEvent::jetPt(size_t i, int applyCorrection) const {
 return jetPt(jets_[i].get(),applyCorrection);
}

const float reco::SkimEvent::secondJetPt(size_t i, int applyCorrection) const {
 return jetPt(secondJets_[i].get(),applyCorrection);
}


const float reco::SkimEvent::jetPt(const pat::Jet *j, int applyCorrection) const {
 // if(applyCorrection) return jets_[i]->correctedJet("L3Absolute","none").pt();
 if (applyCorrection) return j->pt();
 else {
  if (j->currentJECLevel() != "ERROR") {
   return j->correctedJet("Uncorrected","none").pt();
  }
  else {
   return j->pt(); //---- the jet is already "uncorrected"
  }
 }
}

const float reco::SkimEvent::tagJetPt(size_t i, int applyCorrection) const {
 // if(applyCorrection) return tagJets_[i]->correctedJet("L3Absolute","none").pt();
 if (applyCorrection) return tagJets_[i]->pt();
 else {
  //    std::cout << " level = " ;
  //    std::cout << tagJets_[i]->currentJECLevel() << std::endl;
  //    std::cout << " lenght is = ";
  //    std::cout << tagJets_[i]->availableJECLevels().size() << std::endl;
  //    if (tagJets_[i]->availableJECLevels().size() != 0) {
  if (tagJets_[i]->currentJECLevel() != "ERROR") {
   return tagJets_[i]->correctedJet("Uncorrected","none").pt();
  }
  else {
   return tagJets_[i]->pt(); //---- the jet is already "uncorrected"
  }
 }
}














// Fat jet variables

const float reco::SkimEvent::leadingFatJetPt(size_t index, float minPt,float eta,int applyCorrection,int applyID) const {
 
 size_t count = 0;
 for(size_t i=0;i<fatJets_.size();++i) {
  if(!(passFatJetID(fatJets_[i],applyID)) ) continue;
  if( std::fabs(fatJets_[i]->eta()) >= eta) continue;
  if( fatJetPt(i,applyCorrection) <= minPt) continue;
  if(isThisJetALepton(fatJets_[i])) continue;
  if(++count > index) return fatJetPt(i,applyCorrection);
 }
 return -9999.9;
}

const float reco::SkimEvent::leadingFatJetEta(size_t index, float minPt,float eta,int applyCorrection,int applyID) const {
 
 size_t count = 0;
 for(size_t i=0;i<fatJets_.size();++i) {
  if(!(passFatJetID(fatJets_[i],applyID)) ) continue;
  if( std::fabs(fatJets_[i]->eta()) >= eta) continue;
  if( fatJetPt(i,applyCorrection) <= minPt) continue;
  if(isThisJetALepton(fatJets_[i])) continue;
  if(++count > index) return fatJets_[i]->eta();
 }
 return -9999.9;
}

const float reco::SkimEvent::leadingFatJetPhi(size_t index, float minPt,float eta,int applyCorrection,int applyID) const {
 
 size_t count = 0;
 for(size_t i=0;i<fatJets_.size();++i) {
  if(!(passFatJetID(fatJets_[i],applyID)) ) continue;
  if( std::fabs(fatJets_[i]->eta()) >= eta) continue;
  if( fatJetPt(i,applyCorrection) <= minPt) continue;
  if(isThisJetALepton(fatJets_[i])) continue;
  if(++count > index) return fatJets_[i]->phi();
 }
 return -9999.9;
}

const float reco::SkimEvent::leadingFatJetTrimmedMass(size_t index, float minPt,float eta,int applyCorrection,int applyID) const {
 
 size_t count = 0;
 for(size_t i=0;i<fatJets_.size();++i) {
  if(!(passFatJetID(fatJets_[i],applyID)) ) continue;
  if( std::fabs(fatJets_[i]->eta()) >= eta) continue;
  if( fatJetPt(i,applyCorrection) <= minPt) continue;
  if(isThisJetALepton(fatJets_[i])) continue;
  if(++count > index) return fatJets_[i]->userFloat("TrimmedMass");
 }
 return -9999.9;
}


const float reco::SkimEvent::leadingFatJetFilteredMass(size_t index, float minPt,float eta,int applyCorrection,int applyID) const {
 
 size_t count = 0;
 for(size_t i=0;i<fatJets_.size();++i) {
  if(!(passJetID(fatJets_[i],applyID)) ) continue;
  if( std::fabs(fatJets_[i]->eta()) >= eta) continue;
  if( fatJetPt(i,applyCorrection) <= minPt) continue;
  if(isThisJetALepton(fatJets_[i])) continue;
  if(++count > index) return fatJets_[i]->userFloat("FilteredMass");
 }
 return -9999.9;
}


const float reco::SkimEvent::leadingFatJetPrunedMass(size_t index, float minPt,float eta,int applyCorrection,int applyID) const {
 
 size_t count = 0;
 for(size_t i=0;i<fatJets_.size();++i) {
  if(!(passJetID(fatJets_[i],applyID)) ) continue;
  if( std::fabs(fatJets_[i]->eta()) >= eta) continue;
  if( fatJetPt(i,applyCorrection) <= minPt) continue;
  if(isThisJetALepton(fatJets_[i])) continue;
  if(++count > index) return fatJets_[i]->userFloat("PrunedMass");
 }
 return -9999.9;
}




const float reco::SkimEvent::leadingFatJetMassDrop(size_t index, float minPt,float eta,int applyCorrection,int applyID) const {
 
 size_t count = 0;
 for(size_t i=0;i<fatJets_.size();++i) {
  if(!(passJetID(fatJets_[i],applyID)) ) continue;
  if( std::fabs(fatJets_[i]->eta()) >= eta) continue;
  if( fatJetPt(i,applyCorrection) <= minPt) continue;
  if(isThisJetALepton(fatJets_[i])) continue;
  if(++count > index) return fatJets_[i]->userFloat("massDrop");
 }
 return -9999.9;
}


const float reco::SkimEvent::leadingFatJetPrunedTau2Tau1(size_t index, float minPt,float eta,int applyCorrection,int applyID) const {
 
 size_t count = 0;
 for(size_t i=0;i<fatJets_.size();++i) {
  if(!(passJetID(fatJets_[i],applyID)) ) continue;
  if( std::fabs(fatJets_[i]->eta()) >= eta) continue;
  if( fatJetPt(i,applyCorrection) <= minPt) continue;
  if(isThisJetALepton(fatJets_[i])) continue;
  if(++count > index) return fatJets_[i]->userFloat("Prunedtau2tau1");
 }
 return -9999.9;
}


const float reco::SkimEvent::leadingFatJetPrunedTau1(size_t index, float minPt,float eta,int applyCorrection,int applyID) const {
 
 size_t count = 0;
 for(size_t i=0;i<fatJets_.size();++i) {
  if(!(passJetID(fatJets_[i],applyID)) ) continue;
  if( std::fabs(fatJets_[i]->eta()) >= eta) continue;
  if( fatJetPt(i,applyCorrection) <= minPt) continue;
  if(isThisJetALepton(fatJets_[i])) continue;
  if(++count > index) return fatJets_[i]->userFloat("Prunedtau1");
 }
 return -9999.9;
}


const float reco::SkimEvent::leadingFatJetPrunedTau2(size_t index, float minPt,float eta,int applyCorrection,int applyID) const {
 
 size_t count = 0;
 for(size_t i=0;i<fatJets_.size();++i) {
  if(!(passJetID(fatJets_[i],applyID)) ) continue;
  if( std::fabs(fatJets_[i]->eta()) >= eta) continue;
  if( fatJetPt(i,applyCorrection) <= minPt) continue;
  if(isThisJetALepton(fatJets_[i])) continue;
  if(++count > index) return fatJets_[i]->userFloat("Prunedtau2");
 }
 return -9999.9;
}


const float reco::SkimEvent::leadingFatJetPrunedTau3(size_t index, float minPt,float eta,int applyCorrection,int applyID) const {
 
 size_t count = 0;
 for(size_t i=0;i<fatJets_.size();++i) {
  if(!(passJetID(fatJets_[i],applyID)) ) continue;
  if( std::fabs(fatJets_[i]->eta()) >= eta) continue;
  if( fatJetPt(i,applyCorrection) <= minPt) continue;
  if(isThisJetALepton(fatJets_[i])) continue;
  if(++count > index) return fatJets_[i]->userFloat("Prunedtau3");
 }
 return -9999.9;
}


const float reco::SkimEvent::leadingFatJetPrunedTau4(size_t index, float minPt,float eta,int applyCorrection,int applyID) const {
 
 size_t count = 0;
 for(size_t i=0;i<fatJets_.size();++i) {
  if(!(passJetID(fatJets_[i],applyID)) ) continue;
  if( std::fabs(fatJets_[i]->eta()) >= eta) continue;
  if( fatJetPt(i,applyCorrection) <= minPt) continue;
  if(isThisJetALepton(fatJets_[i])) continue;
  if(++count > index) return fatJets_[i]->userFloat("Prunedtau4");
 }
 return -9999.9;
}



//Event variables

const float reco::SkimEvent::pfType1Met() const {
 
 if(pfMet_.isNonnull()) return pfMet_->pt();
 else return defaultvalues::defaultFloat;
}
const float reco::SkimEvent::pfType1SumEt() const {
 
 if(pfMet_.isNonnull()) return pfMet_->sumEt();
 else return defaultvalues::defaultFloat;
}
const float reco::SkimEvent::pfType1MetUp() const {
 
 if(pfMet_.isNonnull()) return pfMet_->shiftedPt(pat::MET::JetEnUp);
 else return defaultvalues::defaultFloat;
}
const float reco::SkimEvent::pfType1MetDn() const {
 
 if(pfMet_.isNonnull()) return pfMet_->shiftedPt(pat::MET::JetEnDown);
 else return defaultvalues::defaultFloat;
}
const float reco::SkimEvent::pfType1MetPhi() const {
 
 if(pfMet_.isNonnull()) return pfMet_->phi();
 else return defaultvalues::defaultFloat;
}

const float reco::SkimEvent::pfRawSumEt() const {
 
 if(pfMet_.isNonnull()) return pfMet_->shiftedSumEt(pat::MET::NoShift, pat::MET::Raw);
 else return defaultvalues::defaultFloat;
}

const float reco::SkimEvent::pfRawMet() const {
 
 if(pfMet_.isNonnull()) return pfMet_->shiftedPt(pat::MET::NoShift, pat::MET::Raw);
 else return defaultvalues::defaultFloat;
}
const float reco::SkimEvent::pfRawMetPhi() const {
 
 if(pfMet_.isNonnull()) return pfMet_->shiftedPhi(pat::MET::NoShift, pat::MET::Raw);
 else return defaultvalues::defaultFloat;
}

const float reco::SkimEvent::pupMet() const {
 
 if(pupMet_.isNonnull()) return pupMet_->pt();
 else return defaultvalues::defaultFloat;
}
const float reco::SkimEvent::trkMet() const {
 return trkMet_.pt();
}



const float reco::SkimEvent::tcSumEt() const {
 
 if(pfMet_.isNonnull()) return tcMet_->sumEt();
 else return defaultvalues::defaultFloat;
}

const float reco::SkimEvent::tcMet() const {
 
 if(tcMet_.isNonnull()) return tcMet_->pt();
 else return defaultvalues::defaultFloat;
}

const float reco::SkimEvent::tcMetPhi() const {
 
 if(tcMet_.isNonnull()) return tcMet_->phi();
 else return defaultvalues::defaultFloat;
}


const float reco::SkimEvent::chargedSumEt() const {
 
 return chargedMet_.sumEt();
}

const float reco::SkimEvent::chargedMet() const {
 return chargedMet_.pt();
}

const float reco::SkimEvent::pfMetSignificance() const {
 
 if(pfMet_.isNonnull()) return pfMet_->significance();
 else return defaultvalues::defaultFloat;
}

const float reco::SkimEvent::pfMetMEtSig() const {
 
 if(pfMet_.isNonnull()) return pfMet_->mEtSig();
 else return defaultvalues::defaultFloat;
}



const float reco::SkimEvent::chargedMetSmurfSumEt() const {
 
 return chargedMetSmurf_.sumEt();
}


/*
 * const float reco::SkimEvent::minMet() const {
 * return ((chargedMet() < pfMet()) ? chargedMet() : pfMet()) ;
 * }
 * 
 * const math::XYZTLorentzVector reco::SkimEvent::minMetP4() const {
 * return ((chargedMet() < pfMet()) ? chargedMet_.p4() : pfMet_->p4()) ;
 * }
 */


const float reco::SkimEvent::tcMetX() const {
 
 if(tcMet_.isNonnull()) return tcMet_->px();
 else return defaultvalues::defaultFloat;
}

const float reco::SkimEvent::tcMetY() const {
 
 if(tcMet_.isNonnull()) return tcMet_->py();
 else return defaultvalues::defaultFloat;
}

const float reco::SkimEvent::mll() const {
 if(leps_.size() < 2) return defaultvalues::defaultFloat;
 return (leps_[indexByPt(0)]->p4() + leps_[indexByPt(1)]->p4()).mass();
}

const float reco::SkimEvent::pTll() const {
 if(leps_.size() < 2) return defaultvalues::defaultFloat;
 return (leps_[indexByPt(0)]->p4() + leps_[indexByPt(1)]->p4()).pt();
}

const float reco::SkimEvent::dPhill() const {
 if(leps_.size() < 2) return defaultvalues::defaultFloat;
 return fabs(ROOT::Math::VectorUtil::DeltaPhi(leps_[indexByPt(0)]->p4(),leps_[indexByPt(1)]->p4()) );
}

const float reco::SkimEvent::dRll() const {
 if(leps_.size() < 2) return defaultvalues::defaultFloat;
 return ROOT::Math::VectorUtil::DeltaR(leps_[indexByPt(0)]->p4(),leps_[indexByPt(1)]->p4());
}

const float reco::SkimEvent::dEtall() const {
 if(leps_.size() < 2) return defaultvalues::defaultFloat;
 return fabs(leps_[indexByPt(0)]->eta() - leps_[indexByPt(1)]->eta());
}

const float reco::SkimEvent::etall() const {
 if(leps_.size() < 2) return defaultvalues::defaultFloat;
 return (leps_[indexByPt(0)]->p4() + leps_[indexByPt(1)]->p4()).eta();
}

const float reco::SkimEvent::yll() const {
 if(leps_.size() < 2) return defaultvalues::defaultFloat;
 return (leps_[indexByPt(0)]->p4() + leps_[indexByPt(1)]->p4()).Rapidity();
}

const float reco::SkimEvent::dPhillMet(metType metToUse) const {
 switch (metToUse) {
  case TCMET: return dPhillTcMet();
  case PFMET: return dPhillPfMet();
  case CHMET: return dPhillChargedMet();
  //case MINMET: return dPhillMinMet();
 }
 return 0;
}

const float reco::SkimEvent::dPhillPfMet() const {
 if(leps_.size() < 2 || pfMet_.isNull()) return defaultvalues::defaultFloat;
 return fabs(ROOT::Math::VectorUtil::DeltaPhi(leps_[indexByPt(0)]->p4()+leps_[indexByPt(1)]->p4(),pfMet_->p4()) );
}

const float reco::SkimEvent::dPhillTcMet() const {
 if(leps_.size() < 2 || tcMet_.isNull()) return defaultvalues::defaultFloat;
 return fabs(ROOT::Math::VectorUtil::DeltaPhi(leps_[indexByPt(0)]->p4()+leps_[indexByPt(1)]->p4(),tcMet_->p4()) );
}

const float reco::SkimEvent::dPhillChargedMet() const {
 if(leps_.size() < 2) return defaultvalues::defaultFloat;
 return fabs(ROOT::Math::VectorUtil::DeltaPhi(leps_[indexByPt(0)]->p4()+leps_[indexByPt(1)]->p4(),chargedMet_.p4()) );
}

/*
 * const float reco::SkimEvent::dPhillMinMet() const {
 * if(leps_.size()!=2) return defaultvalues::defaultFloat;
 * return fabs(ROOT::Math::VectorUtil::DeltaPhi(leps_[indexByPt(0)]->p4()+leps_[indexByPt(1)]->p4(),minMetP4()) );
 * }
 */

const float reco::SkimEvent::mTHiggs(metType metToUse) const {
 // AN 2011/155, v2, p19
 return sqrt( 2 * pTll() * met(metToUse) * ( 1 - cos(dPhillMet(metToUse)) ) );
 //version 2 from guillelmo's talk
 /*
  * return sqrt( mll()*mll() +
  * 2*( sqrt(pTll()*pTll()+mTll()*mTll()) * tcMet() -
  * (pXll()+tcMetX())*(pXll()+tcMetX()) -
  * (pYll()+tcMetY())*(pYll()+tcMetY()) ) );
  */
}

const float reco::SkimEvent::pXll() const {
 if(leps_.size() < 2) return defaultvalues::defaultFloat;
 return (leps_[indexByPt(0)]->p4() + leps_[indexByPt(1)]->p4()).px();
}

const float reco::SkimEvent::pYll() const {
 if(leps_.size() < 2) return defaultvalues::defaultFloat;
 return (leps_[indexByPt(0)]->p4() + leps_[indexByPt(1)]->p4()).py();
}

const float reco::SkimEvent::mTll() const {
 if(leps_.size() < 2) return defaultvalues::defaultFloat;
 return (leps_[indexByPt(0)]->p4() + leps_[indexByPt(1)]->p4()).mt();
}

const float reco::SkimEvent::mT(size_t i, metType metToUse) const {
 if(i>=leps_.size()) return defaultvalues::defaultFloat;
 return sqrt(2*pt(i)*met(metToUse)*(1 - cos(dPhilMet(i, metToUse))));
}

const float reco::SkimEvent::met(metType metToUse) const {
 switch (metToUse) {
  case TCMET: return tcMet();
  case PFMET: return pfRawMet();
  case CHMET: return chargedMet();
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
 if(dphi < M_PI/2.) return pfRawMet()*sin(dphi);
 else return pfRawMet();
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

/*
 * const float reco::SkimEvent::projMinMet() const {
 * float dphi = dPhilMinMet();
 * if(dphi < M_PI/2.) return minMet()*sin(dphi);
 * else return minMet();
 * }
 */

const float reco::SkimEvent::dPhilMet(metType metToUse) const {
 switch (metToUse) {
  case TCMET: return dPhilTcMet();
  case PFMET: return dPhilPfMet();
  case CHMET: return dPhilChargedMet();
  //case MINMET: return dPhilMinMet();
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

/*
 * const float reco::SkimEvent::dPhilMinMet() const {
 * float smallestDphi = 9999.;
 * for(size_t l=0; l<leps_.size();++l){
 * float dphi = dPhilMinMet(l);
 * if( dphi < smallestDphi) smallestDphi = dphi;
 * }
 * return smallestDphi;
 * }
 */

const float reco::SkimEvent::dPhilMet(size_t i, metType metToUse) const {
 if( i >= std::min((uint) 2,(uint) leps_.size()) ) return defaultvalues::defaultFloat;
 switch (metToUse) {
  case TCMET: return dPhilTcMet(i);
  case PFMET: return dPhilPfMet(i);
  case CHMET: return dPhilChargedMet(i);
  //case MINMET: return dPhilMinMet(i);
 }
 return defaultvalues::defaultFloat;
}

const float reco::SkimEvent::dPhilTcMet(size_t i) const {
 if( i >= std::min((uint) 2,(uint) leps_.size()) ) return defaultvalues::defaultFloat;
 return fabs(ROOT::Math::VectorUtil::DeltaPhi(tcMet_->p4(),leps_[i]->p4()) );
}

const float reco::SkimEvent::dPhilPfMet(size_t i) const {
 if( i >= std::min((uint) 2,(uint) leps_.size()) ) return defaultvalues::defaultFloat;
 return fabs(ROOT::Math::VectorUtil::DeltaPhi(pfMet_->shiftedP4(pat::MET::NoShift, pat::MET::Raw),leps_[i]->p4()) );
}

const float reco::SkimEvent::dPhilMvaMet(size_t i) const {
 if( i >= std::min((uint) 2,(uint) leps_.size()) ) return defaultvalues::defaultFloat;
 return fabs(ROOT::Math::VectorUtil::DeltaPhi(mvaMet_.p4(),leps_[i]->p4()) );
}

const float reco::SkimEvent::dPhilChargedMet(size_t i) const {
 if( i >= std::min((uint) 2,(uint) leps_.size()) ) return defaultvalues::defaultFloat;
 return fabs(ROOT::Math::VectorUtil::DeltaPhi(chargedMet_.p4(),leps_[i]->p4()) );
}

const float reco::SkimEvent::dPhilChargedMetSmurf(size_t i) const {
 if( i >= std::min((uint) 2,(uint) leps_.size()) ) return defaultvalues::defaultFloat;
 return fabs(ROOT::Math::VectorUtil::DeltaPhi(chargedMetSmurf_.p4(),leps_[i]->p4()) );
}

/*
 * const float reco::SkimEvent::dPhilMinMet(size_t i) const {
 * if( i >= leps_.size() ) return defaultvalues::defaultFloat;
 * return fabs(ROOT::Math::VectorUtil::DeltaPhi(minMetP4(),leps_[i]->p4()) );
 * }
 */


const float reco::SkimEvent::nTracks() const {
 
 return 0;
}

/*
 * const float reco::SkimEvent::dPhiJetPfMet() const {
 * 
 * return 0;
 * }
 * 
 * const float reco::SkimEvent::dPhiJetTcMet() const {
 * 
 * return 0;
 * }
 */

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



void reco::SkimEvent::setElectronIds( const std::vector<std::string> &idnames) {
 _electronIds.clear();
 for (unsigned int iname = 0; iname < idnames.size(); iname++) {
  _electronIds.push_back(idnames.at(iname)); 
 }
}
 


// ... in spite my egregious programming
void reco::SkimEvent::setTriggerBits( const std::vector<bool> &bits) {
 
 passesSingleMuData_ = bits[0];
 passesSingleElData_ = bits[1];
 passesDoubleMuData_ = bits[2];
 passesDoubleElData_ = bits[3];
 passesMuEGData_ = bits[4];
 passesSingleMuMC_ = bits[5];
 passesSingleElMC_ = bits[6];
 passesDoubleMuMC_ = bits[7];
 passesDoubleElMC_ = bits[8];
 passesMuEGMC_ = bits[9];
 passesAllEmbed_ = bits[10];
 
}

const bool reco::SkimEvent::triggerBitsCut( SkimEvent::primaryDatasetType pdType) const{
 
 if (pdType == MC) return true;
 
 //     if( hypo() == WWMUMU ) {
 //         if ( pdType == DoubleMuon ) return ( passesDoubleMuData_ );
 //         else if ( pdType == SingleMuon ) return ( !passesDoubleMuData_ && passesSingleMuData_ );
 //         //else if ( pdType == MC ) return ( passesDoubleMuMC_ || passesSingleMuMC_ );
 //     } else if( hypo() == WWMUEL || hypo() == WWELMU ) {
 //         if ( pdType == SingleMuon ) return ( passesSingleMuData_ );
 //         else if ( pdType == MuEG ) return ( !passesSingleMuData_ && passesMuEGData_ );
 //         //else if ( pdType == MC ) return ( passesSingleMuMC_ || passesMuEGMC_ );
 //     } else if( hypo() == WWELEL ) {
 //         if ( pdType == DoubleElectron ) return ( passesDoubleElData_ );
 //         //else if ( pdType == MC ) return ( passesDoubleElMC_ );
 //     }
 
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
 pat::Muon const * const mu = getMuon(i);
 
 const pat::TriggerObjectStandAlone * match = mu->triggerObjectMatchByCollection("hltL3MuonCandidates");
 if(isData){
  if(match) result= (match->hasPathName("HLT_Mu24_v*",false) || match->hasPathName("HLT_IsoMu17_v*",false) );}
  else{
   if(match) result=(match->hasPathName("HLT_Mu21_v*",false) && match->pt()>24.0);
  }
  
  return result;
}

bool reco::SkimEvent::passTriggerDoubleMu(size_t i, bool isData) const{
 using namespace std;
 bool result(false);
 
 if( !isMuon(i) ) return false;
 pat::Muon const * const mu = getMuon(i);
 
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
 pat::Electron const * const el = getElectron(i);
 
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
 /*
  * vector< std::string > pathNames = match->pathNames(false);
  * for(unsigned int i=0; i<pathNames.size();++i){
  * cout << "match path name: " << pathNames[i] << endl;
}
*/
 bool result(false);
 using namespace std;
 if( isMuon(i) ) {
  pat::Muon const * const mu = getMuon(i);
  const pat::TriggerObjectStandAlone * match = mu->triggerObjectMatchByCollection("hltL3MuonCandidates");
  
  if(match){
   if(isData){
    bool res1 = match->hasPathName("HLT_Mu8_Ele17_CaloIdL_v*",false);
    bool res2 = match->hasPathName("HLT_Mu17_Ele8_CaloIdL_v*",false);
    result=( res1 || res2);}
    else{
     bool res1 = (match->hasPathName("HLT_Mu5_Ele17_v*",false) && match->pt()>8.0);
     bool res2 = (match->hasPathName("HLT_Mu11_Ele8_v*",false) && match->pt()>17.0);
     result=( res1 || res2);
    }
  }
  return result;
 }
 
 if( isElectron(i) ) {
  pat::Electron const * const el = getElectron(i);
  if(isData){
   if(el->triggerObjectMatchesByPath("HLT_Mu8_Ele17_CaloIdL*").size() ||
    el->triggerObjectMatchesByPath("HLT_Mu17_Ele8_CaloIdL*").size() )
    result=true;}
    else{
     const pat::TriggerObjectStandAlone * match1=
     el->triggerObjectMatchByPath("HLT_Mu5_Ele17_v*",true);
     const pat::TriggerObjectStandAlone * match2=
     el->triggerObjectMatchByPath("HLT_Mu11_Ele8_v*",true);
     result=( match1 || match2 );
    }
 }
 
 return result;
 
}


// TO BE FIXED: This guy should take the list of trigger from some
// sort of configuration file.
const bool reco::SkimEvent::triggerMatchingCut(SkimEvent::primaryDatasetType pdType) const{
 if( pdType == MC ) return true;
 
 //   using namespace std;
 bool result(false);
 
 //   if(hypo()==WWMUMU){
 //     if(pdType==DoubleMuon){ //configuration (1)
 //       result=(passTriggerDoubleMu(0) && passTriggerDoubleMu(1));}
 //     if(pdType==SingleMuon) {//configuration (2)
 //       result=( (passTriggerSingleMu(0) || passTriggerSingleMu(1)) &&
 // !(passTriggerDoubleMu(0) && passTriggerDoubleMu(1)) );
 //     }
 //     if(pdType==MC){
 //       result=( (passTriggerDoubleMu(0,false) && passTriggerDoubleMu(1,false)) ||
 // (passTriggerSingleMu(0,false) || passTriggerSingleMu(1,false)) );
 //     }
 //   }
 // 
 //   if(hypo()==WWMUEL || hypo()==WWELMU){
 //     if(pdType==SingleMuon) //configuration (3)
 //       result=( passTriggerSingleMu(0) || passTriggerSingleMu(1) );
 //     if(pdType==MuEG){ //configuration (4)
 //       result=( (passTriggerElMu(0) && passTriggerElMu(1)) &&
 // !(passTriggerSingleMu(0) || passTriggerSingleMu(1)) );
 //     }
 //     if(pdType==MC){
 //       result=( (passTriggerSingleMu(0,false) || passTriggerSingleMu(1,false)) ||
 // (passTriggerElMu(0,false) && passTriggerElMu(1,false)) );   
 //     }
 //   }
 // 
 // 
 //   if(hypo()==WWELEL){
 //     if(pdType==DoubleElectron)//configuration (5)
 //       result= (passTriggerDoubleEl(0) && passTriggerDoubleEl(1));
 //     if(pdType==MC)
 //       result= (passTriggerDoubleEl(0,false) && passTriggerDoubleEl(1,false));
 //   }
 
 return result;
}


//Iso Functions
const float reco::SkimEvent::tkIso(size_t i) const {
 
 if( i >= leps_.size() ) return defaultvalues::defaultFloat;
 
 if( isElectron(i) ) {
  return getElectron(i)->dr03TkSumPt();
 } else if ( isMuon(i) ) {
  return getMuon(i)->isolationR03().sumPt;
 } else {
  return defaultvalues::defaultFloat;
 }
}

const float reco::SkimEvent::ecalIso(size_t i) const {
 
 if( i >= leps_.size() ) return defaultvalues::defaultFloat;
 
 if( isElectron(i) ) {
  return getElectron(i)->dr03EcalRecHitSumEt();
 } else if ( isMuon(i) ) {
  return getMuon(i)->isolationR03().emEt;
 } else {
  return defaultvalues::defaultFloat;
 }
}

const float reco::SkimEvent::hcalIso(size_t i) const {
 
 if( i >= leps_.size() ) return defaultvalues::defaultFloat;
 
 if( isElectron(i) ) {
  return getElectron(i)->userFloat("hcalFull");
 } else if ( isMuon(i) ) {
  return getMuon(i)->isolationR03().hadEt;
 } else {
  return defaultvalues::defaultFloat;
 }
}

const float reco::SkimEvent::getRho(size_t i) const {
 
 if( i >= leps_.size() ) return defaultvalues::defaultFloat;
 
 if( isElectron(i) ) {
  return getElectron(i)->userFloat("rhoEl");
 } else if ( isMuon(i) ) {
  return getMuon(i)->userFloat("rhoMu");
 } else {
  return defaultvalues::defaultFloat;
 }
}

const float reco::SkimEvent::allIso(size_t i) const {
 
 if( i >= leps_.size() ) return defaultvalues::defaultFloat;
 
 if( isElectron(i) ) {
  //FIXME check definition for electron. Now just copied from muon
  return ((getElectron(i)->pfIsolationVariables().sumChargedHadronPt+std::max(0.,getElectron(i)->pfIsolationVariables().sumNeutralHadronEt+getElectron(i)->pfIsolationVariables().sumPhotonEt-0.50*getElectron(i)->pfIsolationVariables().sumPUPt))/getElectron(i)->pt());
 } else if( isMuon(i) ) {
  return ((getMuon(i)->pfIsolationR04().sumChargedHadronPt+std::max(0.,getMuon(i)->pfIsolationR04().sumNeutralHadronEt+getMuon(i)->pfIsolationR04().sumPhotonEt-0.50*getMuon(i)->pfIsolationR04().sumPUPt))/getMuon(i)->pt());
 } else {
  std::cout << " Do I ever friggin get here?" << std::endl;
  return 9999.0;
 }
 
 
 
//  if( isElectron(i) ) {
//   return getElectron(i)->userFloat("eleSmurfPF04");
//  } else if( isMuon(i) ) {
//   return getMuon(i)->userFloat("muSmurfPF");
//  } else {
//   std::cout << " Do I ever friggin get here?" << std::endl;
//   return 9999.0;
//  }
 // if( isElectron(i) && isEB(i) ) {
 // return tkIso(i) + std::max((float)0,ecalIso(i)-1) + hcalIso(i) - getRho(i) * 3.14159265 * 0.3 * 0.3;
 // } else if( (isElectron(i) && !isEB(i)) || isMuon(i) ) {
 // return tkIso(i) + ecalIso(i) + hcalIso(i) - getRho(i) * 3.14159265 * 0.3 * 0.3;
 // } else {
 // std::cout << " Do I ever friggin get here?" << std::endl;
 // return 9999.0;
 // }
}

const float reco::SkimEvent::mvaIso(size_t i) const {
 
 if( i >= leps_.size() ) return defaultvalues::defaultFloat;
 if ( isElectron(i) ) return getElectron(i)->userFloat("pfCombRelIso04EACorr");
 else if ( isMuon(i) ) return getMuon(i)->userFloat("bdtisonontrigDZ");
 else return defaultvalues::defaultFloat;
}


const float reco::SkimEvent::tkVeto(size_t i) const {
 
 if( i >= leps_.size() ) return defaultvalues::defaultFloat;
 
 if( isElectron(i) ) {
  return 0;
 } else if ( isMuon(i) ) {
  return getMuon(i)->isolationR03().trackerVetoPt;
 } else {
  return defaultvalues::defaultFloat;
 }
}

const float reco::SkimEvent::ecalVeto(size_t i) const {
 
 if( i >= leps_.size() ) return defaultvalues::defaultFloat;
 
 if( isElectron(i) ) {
  return 0;
 } else if ( isMuon(i) ) {
  return getMuon(i)->isolationR03().emVetoEt;
 } else {
  return defaultvalues::defaultFloat;
 }
}

const float reco::SkimEvent::hcalVeto(size_t i) const {
 
 if( i >= leps_.size() ) return defaultvalues::defaultFloat;
 
 if( isElectron(i) ) {
  return 0;
 } else if ( isMuon(i) ) {
  return getMuon(i)->isolationR03().hadVetoEt;
 } else {
  return defaultvalues::defaultFloat;
 }
}

const float reco::SkimEvent::allVeto(size_t i) const {
 
 if( i >= leps_.size() ) return defaultvalues::defaultFloat;
 
 if( isElectron(i) ) {
  return 0;
 } else if( isMuon(i) ) {
  return tkVeto(i) + ecalVeto(i) + hcalVeto(i);
 } else {
  return defaultvalues::defaultFloat;
 }
}


const size_t reco::SkimEvent::indexJetByPt(size_t i, float minPt,float eta,int applyCorrection,int applyID) const {
 
 if( i >= jets_.size() ) return 9999; //--> big number then it will fail other tests later! good!
 std::vector<indexValueStruct> a;
 
 for(size_t j=0;j<jets_.size();++j) {
  if(!(passJetID(jets_[j],applyID)) ) continue;
  if( std::fabs(jets_[j]->eta()) >= eta) continue;
  if( jetPt(j,applyCorrection) <= minPt) continue;
  if(isThisJetALepton(jets_[j])) continue;
  a.push_back(indexValueStruct(jets_[j]->pt(),j));
 }
 
 std::sort(a.begin(),a.end(),highToLow);
 
 if( i < a.size() ) return a[i].index;
 else  return 9999;
}



const size_t reco::SkimEvent::indexByPt(size_t i) const {
 
 if( i >= leps_.size() ) return 9999; //--> big number then it will fail other tests later! good!
 std::vector<indexValueStruct> a;
 
 for(size_t j=0;j<leps_.size();++j) a.push_back(indexValueStruct(pt(j),j));
 std::sort(a.begin(),a.end(),highToLow);
 
 return a[i].index;
}

const size_t reco::SkimEvent::indexByIso(size_t i) const {
 
 if( i >= leps_.size() ) return 9999;
 std::vector<indexValueStruct> a;
 
 for(size_t j=0;j<leps_.size();++j) a.push_back(indexValueStruct(allIso(j),j));
 std::sort(a.begin(),a.end(),lowToHigh);
 
 return a[i].index;
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
  if (i >= vtxs_.size()) return false;
  else {
    
    // https://github.com/ikrav/EgammaWork/blob/ntupler_and_VID_demos/ElectronNtupler/plugins/SimpleElectronNtupler.cc#L321
//     bool isFake_miniaod = (vtxs_[i]->chi2() == 0 && vtxs_[i]->ndof() == 0);
//     std::cout << " [Jonatan debug] vtxs_[i]->isFake() = " << vtxs_[i]->isFake() << " isFake_miniaod = " << isFake_miniaod << std::endl;;

    return (vtxs_[i]->isValid() &&
	    !vtxs_[i]->isFake() &&
	    vtxs_[i]->ndof() >= 4.0 &&
	    fabs(vtxs_[i]->position().Rho()) < 2.0 &&
	    fabs(vtxs_[i]->z()) < 24.0);
  }
  
  return false;
}

const int reco::SkimEvent::nGoodVertices() const {
  int count = 0;

  for(size_t i=0; i<vtxs_.size(); ++i)
    if (passesVtxSel(i)) count++;
  
  return count;
}

const bool reco::SkimEvent::hasGoodVertex() const {
 
 return (nGoodVertices() > 0);
}

const reco::Vertex reco::SkimEvent::highestPtVtx() const {
  if (vtxs_.size()   == 0) return reco::Vertex();
//   if (sumPts_.size() == 0) return *vtxs_[0];
  // [Jonatan, 2015-06-12] All the events that I have tested have sumPts_.size() = 0
  
  
  //---- vertices should be ordered in sumpt
  //----   - here we just take the vertex with maximum sumpt but that is also a "good" vertex
  for (size_t i=0;i<vtxs_.size();++i) {
   if (passesVtxSel(i)) {
    return *vtxs_[i];
   }
  }
  
  //---- if none are good ... return the default?!?
  return reco::Vertex();
  
//   double sum  = 0;
//   size_t high = 0;
// 
//   for (size_t i=0;i<vtxs_.size();++i) {
//     if (sumPts_[i] > sum && passesVtxSel(i)) {
//       high = i;
//       sum  = sumPts_[i];
//     }
//   }
// 
//   return *vtxs_[high];
}

const bool reco::SkimEvent::passesIP() const {
 if(leps_.size() < 2) return defaultvalues::defaultFloat;
 return (passesIP(leps_[indexByPt(0)]) && passesIP(leps_[indexByPt(1)]));
}

const bool reco::SkimEvent::passesIP(const refToCand &c) const {
 
 if( isElectron(c) ) {
  
  if( fabs(getElectron(c)->userFloat("ip2")) >= 0.03) return false;
  
 } else if( isMuon(c) && !isSTA(c) ) {
  
  if( fabs(getMuon(c)->userFloat("tip2")) >= 0.01) return false;
  if( fabs(getMuon(c)->userFloat("dzPV")) >= 0.05) return false;
  
 }
 
 return true;
 
}

const double reco::SkimEvent::d0Reco(size_t i) const {
 double dxyPV = 9999;
 if( isElectron(i) ) {
  dxyPV = getElectron(i)->userFloat("dxyPV");
 } else if( isMuon(i) && !isSTA(i) ) {
  dxyPV = getMuon(i)->userFloat("dxyPV");
 }
 return dxyPV;
}

const double reco::SkimEvent::dZReco(size_t i) const {
 double dzPV = 9999;
 if( isElectron(i) ) {
  dzPV = getElectron(i)->userFloat("dzPV");
 } else if( isMuon(i) && !isSTA(i) ) {
  dzPV = getMuon(i)->userFloat("dzPV");
 }
 return dzPV;
}

const bool reco::SkimEvent::isSTA(size_t i) const {
 if(i < leps_.size()) return isSTA(leps_[i]);
 return false;
}

const bool reco::SkimEvent::isSTA(const refToCand &c) const {
 if( isMuon(c) ) {
  return (getMuon(c)->type() == 8);
 } else {
  return false;
 }
}

const float reco::SkimEvent::highestHardBDisc(const float& maxPt, std::string discriminator, int applyID, float dzCut) const {
 
 float disc=-9999.9;
 
 for(size_t i=0;i<tagJets_.size();++i) {
  if( tagJetPt(i,true) < maxPt ) continue;
  if(!(passJetID(tagJets_[i],applyID)) ) continue;
  if(isThisJetALepton(tagJets_[i])) continue;
  if(jets_[i]->hasUserFloat("dz") && fabs(jets_[i]->userFloat("dz")) > dzCut) continue;
  if( tagJets_[i]->bDiscriminator(discriminator) > disc ) disc = tagJets_[i]->bDiscriminator(discriminator);
 }
 
 return disc;
 
}

const float reco::SkimEvent::highestBDiscRange(const float& minPt, const float& maxPt, std::string discriminator, int applyID, float dzCut, int minPtApplyCorrection) const {
 
 float disc=-9999.9;
 
 for(size_t i=0;i<tagJets_.size();++i) {
  if( tagJetPt(i,true) > maxPt ) continue;
  if( tagJetPt(i,minPtApplyCorrection) <= minPt ) continue;
  if(!(passJetID(tagJets_[i],applyID)) ) continue;
  if(isThisJetALepton(tagJets_[i])) continue;
  //         std::cout << " HELLO! " << std::endl;
  //         std::cout << " has float = " << jets_[i]->hasUserFloat("dz") << std::endl;        
  if(jets_[i]->hasUserFloat("dz") && fabs(jets_[i]->userFloat("dz")) > dzCut) continue;
  if( tagJets_[i]->bDiscriminator(discriminator) > disc ) disc = tagJets_[i]->bDiscriminator(discriminator);
 }
 
 return disc;
 
}

const float reco::SkimEvent::highestSoftBDisc(const float& maxPt, std::string discriminator, int applyID, float dzCut) const {
 
 float disc=-9999.9;
 
 for(size_t i=0;i<tagJets_.size();++i) {
  if( tagJetPt(i,true) > maxPt ) continue;
  if(!(passJetID(tagJets_[i],applyID)) ) continue;
  if(isThisJetALepton(tagJets_[i])) continue;
  if(jets_[i]->hasUserFloat("dz") && fabs(jets_[i]->userFloat("dz")) > dzCut) continue;
  if( tagJets_[i]->bDiscriminator(discriminator) > disc ) disc = tagJets_[i]->bDiscriminator(discriminator);
 }
 
 return disc;
 
}

const int reco::SkimEvent::bTaggedJetsBetween(const float& minPt, const float& maxPt, const float& cut, std::string discriminator, int applyID , float dzCut) const {
 
 int count=0;
 
 for(size_t i=0;i<tagJets_.size();++i) {
  if( tagJetPt(i,true) > maxPt ) continue;
  if( tagJetPt(i,false) <= minPt ) continue;
  if(!(passJetID(tagJets_[i],applyID)) ) continue;
  if( tagJets_[i]->bDiscriminator(discriminator) <= cut ) continue;   
  if(jets_[i]->hasUserFloat("dz") && fabs(jets_[i]->userFloat("dz")) > dzCut) continue;
  if(isThisJetALepton(tagJets_[i])) continue;
  count++;
 }
 
 return count;
}

const int reco::SkimEvent::bTaggedJetsUnder(const float& maxPt, const float& cut, std::string discriminator, int applyID, float dzCut) const {
 
 int count=0;
 
 for(size_t i=0;i<tagJets_.size();++i) {
  if( tagJetPt(i,true) > maxPt ) continue;
  if(!(passJetID(tagJets_[i],applyID)) ) continue;
  if( tagJets_[i]->bDiscriminator(discriminator) <= cut ) continue;   
  if(jets_[i]->hasUserFloat("dz") && fabs(jets_[i]->userFloat("dz")) > dzCut) continue;
  if(isThisJetALepton(tagJets_[i])) continue;
  count++;
 }
 
 return count;
}

const int reco::SkimEvent::bTaggedJetsOver(const float& maxPt, const float& cut, std::string discriminator, int applyID, float dzCut) const {
 
 int count=0;
 
 for(size_t i=0;i<tagJets_.size();++i) {
  if( tagJetPt(i,true) <= maxPt ) continue;
  if(!(passJetID(tagJets_[i],applyID)) ) continue;
  if( tagJets_[i]->bDiscriminator(discriminator) <= cut ) continue;
  if(jets_[i]->hasUserFloat("dz") && fabs(jets_[i]->userFloat("dz")) > dzCut) continue;
  if(isThisJetALepton(tagJets_[i])) continue;
  count++;
 }
 
 return count;
}

const bool reco::SkimEvent::isMuTriggered(size_t i) const {
 
 if( isMuon(i) ) {
  pat::Muon const * const mu = getMuon(i);
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


const float reco::SkimEvent::nearestJet(int i,float minPt, float eta, bool applyCorrection , int applyID ) const {
 
 if (i >= (int)std::min((uint) 2,(uint) leps_.size()) || i < -1) return -9999.9;
 
 float dR = 9999;
 for(size_t j=0;j<jets_.size();++j) {
  if(!(passJetID(jets_[i],applyID)) ) continue;
  if( std::fabs(jets_[j]->eta()) >= eta) continue;
  if( jetPt(j,applyCorrection) <= minPt) continue;
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

const float reco::SkimEvent::matchedJetPt(size_t i, float minDr, bool applyCorrection) const {
 if (i >= std::min((uint) 2,(uint) leps_.size())) return -9999.9;
 
 float dR = minDr, pt = 0;
 for(size_t j=0;j<jets_.size();++j) {
  float tempdR = fabs(ROOT::Math::VectorUtil::DeltaR(jets_[j]->p4(),leps_[i]->p4()) );
  if( tempdR < dR ) {
   dR = tempdR;
   pt = jetPt(j,applyCorrection);
  }
 }
 return pt;
}


/**
 * ---- muon  id ----
 */


const int reco::SkimEvent::muNValidHitsInTrk(size_t i) const {
 if(i >= leps_.size()) return defaultvalues::defaultFloat;
 if( isMuon(i) ) {
  reco::TrackRef tr_innertrack = getMuon(i)->innerTrack(); 
  if (!tr_innertrack.isNull()) {
   return tr_innertrack->hitPattern().numberOfValidTrackerHits();
  } else return -999.0;
 } else return -999.0; 
}

const float reco::SkimEvent::muNValidFractInTrk(size_t i) const {
 if(i >= leps_.size()) return defaultvalues::defaultFloat;
 if( isMuon(i) ) {
  reco::TrackRef tr_innertrack = getMuon(i)->innerTrack(); 
  if (!tr_innertrack.isNull()) {
   return tr_innertrack->validFraction();
  } else return -999.0;
 } else return -999.0; 
}



const float reco::SkimEvent::muNormChi2GTrk(size_t i) const {
 if(i >= leps_.size()) return defaultvalues::defaultFloat;
 if( isMuon(i) ) {
  reco::TrackRef tr_globaltrack =getMuon(i)->globalTrack(); 
  if (!tr_globaltrack.isNull()) {
   return tr_globaltrack->normalizedChi2();
  } else return -999.0;
 } else return -999.0;
}

const int reco::SkimEvent::muNValidHitsSATrk(size_t i) const {
 if(i >= leps_.size()) return defaultvalues::defaultFloat;
 if( isMuon(i) ) {
  reco::TrackRef tr_outtrack = getMuon(i)->standAloneMuon(); 
  if (!tr_outtrack.isNull()) {
   return tr_outtrack->hitPattern().numberOfValidMuonHits();
  } else return -999.0;
 } else return -999.0;
}



const int  reco::SkimEvent::muNumOfMatchedStations(size_t i) const {
 if(i >= leps_.size()) return defaultvalues::defaultFloat;
 if( isMuon(i) ) {
  return getMuon(i)->numberOfMatchedStations();
 } else return -999.0;
}


const float reco::SkimEvent::muBestTrackdz(size_t i) const {
 if(i >= leps_.size()) return defaultvalues::defaultFloat;
 const reco::Vertex primaryVtx = highestPtVtx();
 if( isMuon(i) ) {
  return getMuon(i)->muonBestTrack()->dz(primaryVtx.position());
 } else return -999.0;
}


const float reco::SkimEvent::muBestTrackdxy(size_t i) const {
 if(i >= leps_.size()) return defaultvalues::defaultFloat;
 const reco::Vertex primaryVtx = highestPtVtx();
 if( isMuon(i) ) {
  return getMuon(i)->muonBestTrack()->dxy(primaryVtx.position());
 } else return -999.0;
}


const int  reco::SkimEvent::muNValidPixelHitsInTrk(size_t i) const {
 if(i >= leps_.size()) return defaultvalues::defaultFloat;
 if( isMuon(i) ) { 
  reco::TrackRef tr_innertrack = getMuon(i)->innerTrack();     
  if (!tr_innertrack.isNull()) {
   return tr_innertrack->hitPattern().numberOfValidPixelHits(); 
  } else return -999.0;
 } else return -999.0;
}


const int  reco::SkimEvent::muNTkLayers(size_t i) const {
 if(i >= leps_.size()) return defaultvalues::defaultFloat;
 if( isMuon(i) ) { 
  reco::TrackRef tr_innertrack = getMuon(i)->innerTrack(); 
  if (!tr_innertrack.isNull()) {
   return tr_innertrack->hitPattern().trackerLayersWithMeasurement();
  } else return -999.0;
 } else return -999.0;
}


const float  reco::SkimEvent::muTrkKink(size_t  i) const {
 if(i >= leps_.size()) return defaultvalues::defaultFloat;  
 if( isMuon(i) ) {
  return  getMuon(i)->combinedQuality().trkKink;
 } else return -999.0;
}

const float reco::SkimEvent::muChi2LocalPos(size_t i) const  {
 if(i >= leps_.size()) return defaultvalues::defaultFloat;  
 if( isMuon(i) ) {
  return getMuon(i)->combinedQuality().chi2LocalPosition;
 } else return -999.0;
}

const float reco::SkimEvent::muSegCompatibilty(size_t i) const  {
 if(i >= leps_.size()) return defaultvalues::defaultFloat;  
 if( isMuon(i) ) {
  return getMuon(i)->segmentCompatibility();
 } else return -999.0;
}


const bool reco::SkimEvent::isTightMuon(size_t i) const {
 if(i >= leps_.size()) return defaultvalues::defaultFloat;  
 if( isMuon(i) ) {
  return (muon::isTightMuon(*getMuon(i), highestPtVtx()));
 } else {
  return false;
 }
}


const bool reco::SkimEvent::isMediumMuon(size_t i) const
{
  if (i >= leps_.size()) return defaultvalues::defaultFloat;  

  if (isMuon(i)) {
    return (muon::isMediumMuon(*getMuon(i)));
  } else {
    return false;
  }
}


const float reco::SkimEvent::muSIP3D(size_t i) const {
 if(i >= leps_.size()) return defaultvalues::defaultFloat;  
 if( isMuon(i) ) {
  double ip = fabs(getMuon(i)->dB(pat::Muon::PV3D));
  double ipError = getMuon(i)->edB(pat::Muon::PV3D);
  double sip = ip/ipError;
  return sip;
 } else {
  return defaultvalues::defaultFloat;
 }
}


const float reco::SkimEvent::elSIP3D(size_t i) const {
 if(i >= leps_.size()) return defaultvalues::defaultFloat;  
 if( isElectron(i) ) {
  double ip = fabs(getElectron(i)->dB(pat::Electron::PV3D));
  double ipError = getElectron(i)->edB(pat::Electron::PV3D);
  double sip = ip/ipError;
  return sip;
 } else {
  return defaultvalues::defaultFloat;
 }
}


// Electron cut based ID
const float reco::SkimEvent::deltaEtaSuperClusterTrackAtVtx(size_t i) const {
  if (i >= leps_.size())  return defaultvalues::defaultFloat;
  else if (isElectron(i)) return getElectron(i)->deltaEtaSuperClusterTrackAtVtx();
  else return -999.0;
}

const float reco::SkimEvent::deltaPhiSuperClusterTrackAtVtx(size_t i) const {
  if (i >= leps_.size())  return defaultvalues::defaultFloat;
  else if (isElectron(i)) return getElectron(i)->deltaPhiSuperClusterTrackAtVtx();
  else return -999.0;
}

const float reco::SkimEvent::full5x5_sigmaIetaIeta(size_t i) const {
  if (i >= leps_.size())  return defaultvalues::defaultFloat;
  else if (isElectron(i)) return getElectron(i)->full5x5_sigmaIetaIeta();
  else return -999.0;
}

const float reco::SkimEvent::hcalOverEcal(size_t i) const {
  if (i >= leps_.size())  return defaultvalues::defaultFloat;
  else if (isElectron(i)) return getElectron(i)->hcalOverEcal();
  else return -999.0;
}

const float reco::SkimEvent::numberOfHits(size_t i) const {
  if (i >= leps_.size())  return defaultvalues::defaultFloat;
  else if (isElectron(i)) return getElectron(i)->gsfTrack()->hitPattern().numberOfTrackerHits(reco::HitPattern::TRACK_HITS);
  else return -999.0;
}

const float reco::SkimEvent::ooEmooP(size_t i) const {
  if(i >= leps_.size() || getElectron(i)->ecalEnergy() == 0 || !std::isfinite(getElectron(i)->ecalEnergy())) return defaultvalues::defaultFloat;
  else if (isElectron(i)) return (1.0/getElectron(i)->ecalEnergy() - getElectron(i)->eSuperClusterOverP()/getElectron(i)->ecalEnergy());
  else return -999.0;
}

const float reco::SkimEvent::d0(size_t i) const {
  const reco::Vertex primaryVtx = highestPtVtx();
  if (i >= leps_.size())  return defaultvalues::defaultFloat;
  else if (isElectron(i)) return getElectron(i)->gsfTrack()->dxy(primaryVtx.position()); 
  else return -999.0;
}

const float reco::SkimEvent::dz(size_t i) const {
  const reco::Vertex primaryVtx = highestPtVtx();
  if (i >= leps_.size())  return defaultvalues::defaultFloat;
  else if (isElectron(i)) return getElectron(i)->gsfTrack()->dz(primaryVtx.position());
  else return -999.0;
}

const float reco::SkimEvent::expectedMissingInnerHits(size_t i) const {
 if (i >= leps_.size())  return defaultvalues::defaultFloat;
 else if (isElectron(i)) return getElectron(i)->gsfTrack()->hitPattern().numberOfHits(reco::HitPattern::MISSING_INNER_HITS);
 else return -999.9;
}

const bool reco::SkimEvent::passConversionVeto(size_t i) const {
  if (i >= leps_.size())  return false;
  else if (isElectron(i)) return getElectron(i)->passConversionVeto();
  else if (isMuon(i))     return true;
  else                    return false;
}


// Muon and electron isolation
const float reco::SkimEvent::chargedHadronIso(size_t i) const {
 if      (i >= leps_.size()) return defaultvalues::defaultFloat;
 else if (isElectron(i))     return getElectron(i)->pfIsolationVariables().sumChargedHadronPt;
 else if (isMuon(i))         return getMuon(i)->pfIsolationR04().sumChargedHadronPt;
 else                        return -999.0;
}

const float reco::SkimEvent::chargedParticleIso(size_t i) const {
 if      (i >= leps_.size()) return defaultvalues::defaultFloat;
 else if (isElectron(i))     return -99.0;
 else if (isMuon(i))         return getMuon(i)->pfIsolationR04().sumChargedParticlePt;
 else                        return -999.0;
}

const float reco::SkimEvent::neutralHadronIso(size_t i) const {
 if      (i >= leps_.size()) return defaultvalues::defaultFloat;
 else if (isElectron(i))     return getElectron(i)->pfIsolationVariables().sumNeutralHadronEt;
 else if (isMuon(i))         return getMuon(i)->pfIsolationR04().sumNeutralHadronEt;
 else                        return -999.0;
}

const float reco::SkimEvent::photonIso(size_t i) const {
 if      (i >= leps_.size()) return defaultvalues::defaultFloat;
 else if (isElectron(i))     return getElectron(i)->pfIsolationVariables().sumPhotonEt;
 else if (isMuon(i))         return getMuon(i)->pfIsolationR04().sumPhotonEt;
 else                        return -999.0;
}

const float reco::SkimEvent::sumPUPt(size_t i) const {
 if      (i >= leps_.size()) return defaultvalues::defaultFloat;
 else if (isElectron(i))     return getElectron(i)->pfIsolationVariables().sumPUPt;
 else if (isMuon(i))         return getMuon(i)->pfIsolationR04().sumPUPt;
 else                        return -999.0;
}


// New emanuele gamma mr star thingy
const float reco::SkimEvent::mRStar() const {
 if(leps_.size() < 2) return -9999.;
 float A = leps_[indexByPt(0)]->p();
 float B = leps_[indexByPt(1)]->p();
 float az = leps_[indexByPt(0)]->pz();
 float bz = leps_[indexByPt(1)]->pz();
 TVector3 jaT, jbT;
 jaT.SetXYZ(leps_[indexByPt(0)]->px(),leps_[indexByPt(0)]->py(),0.0);
 jbT.SetXYZ(leps_[indexByPt(1)]->px(),leps_[indexByPt(1)]->py(),0.0);
 
 float temp = sqrt((A+B)*(A+B)-(az+bz)*(az+bz)-
 (jbT.Dot(jbT)-jaT.Dot(jaT))*(jbT.Dot(jbT)-jaT.Dot(jaT))/(jaT+jbT).Mag2());
 
 return temp;
}

const float reco::SkimEvent::gamma() const {
 if(leps_.size() < 2) return -9999.;
 float A = leps_[indexByPt(0)]->p();
 float B = leps_[indexByPt(1)]->p();
 float az = leps_[indexByPt(0)]->pz();
 float bz = leps_[indexByPt(1)]->pz();
 TVector3 jaT, jbT;
 jaT.SetXYZ(leps_[indexByPt(0)]->px(),leps_[indexByPt(0)]->py(),0.0);
 jbT.SetXYZ(leps_[indexByPt(1)]->px(),leps_[indexByPt(1)]->py(),0.0);
 
 float ATBT = (jaT+jbT).Mag2();
 double mybeta = (jbT.Dot(jbT)-jaT.Dot(jaT))/
 sqrt(ATBT*((A+B)*(A+B)-(az+bz)*(az+bz)));
 
 return 1./sqrt(1.-mybeta*mybeta);
}

const float reco::SkimEvent::gammaMRStar() const {
 
 return gamma() * mRStar();
 
}

const int reco::SkimEvent::nJetVBF(float minPt,float eta,int applyCorrection, int applyID) const {
 if(jets_.size() < 2) return -9999.;
 float jetEta1 = leadingJetEta(0,minPt,eta,applyCorrection,applyID);
 float jetEta2 = leadingJetEta(1,minPt,eta,applyCorrection,applyID);
 float jetEtaMax = std::max(jetEta1,jetEta2);
 float jetEtaMin = std::min(jetEta1,jetEta2);
 
 int count = 0;
 for(size_t i=0;i<jets_.size();++i) {
  if(!(passJetID(jets_[i],applyID)) ) continue;
  if( std::fabs(jets_[i]->eta()) >= eta) continue;
  if( jetPt(i,applyCorrection) <= minPt) continue;
  
  if(isThisJetALepton(jets_[i])) continue;
  if( jets_[i]->eta() >= jetEtaMax ) continue;
  if( jets_[i]->eta() <= jetEtaMin ) continue;
  count++;
 }
 return count;
 
}

const float reco::SkimEvent::mjj(float pt ,float eta,int applyCorrection, int applyID) const {
 if(jets_.size() < 2) return -9999.;
 return (leadingJet(0,pt,eta,applyCorrection,applyID)->p4() +
 leadingJet(1,pt,eta,applyCorrection,applyID)->p4()).mass();
 
}

const float reco::SkimEvent::dEtajj(float pt ,float eta,int applyCorrection, int applyID) const {
 if(jets_.size() < 2) return -9999.;
 return fabs(leadingJetEta(0,pt,eta,applyCorrection,applyID) -
 leadingJetEta(1,pt,eta,applyCorrection,applyID));
 
}

const float reco::SkimEvent::zeppenfeld(size_t a,float pt ,float eta,int applyCorrection, int applyID) const {
 if(jets_.size() < 2) return -9999.;
 return -9999.;
 
}



void reco::SkimEvent::FindDaughterParticles(const reco::Candidate** pCurrent, std::vector<const reco::Candidate*>* pFinal) const {
 
 // Variable for decayed current particle
 const reco::Candidate* pCurrentNew = 0;
 
 for(unsigned int i = 0; i < (*pCurrent) -> numberOfDaughters(); ++i) {
  if( (pFinal != 0) && ((*pCurrent) -> daughter(i) -> pdgId() != (*pCurrent) -> pdgId()) )
   pFinal -> push_back((*pCurrent) -> daughter(i));
  
  if((*pCurrent) -> daughter(i) -> pdgId() == (*pCurrent) -> pdgId())
   pCurrentNew = (*pCurrent) -> daughter(i);
 }
 
 // Change the current particle into the decayed one
 if(pCurrentNew)
  (*pCurrent) = pCurrentNew;
}




//---- DY > ll : DY decay final state (mumu,ee,tautau)

const float reco::SkimEvent::getFinalStateMC() const {
 
 float finalState = -1;
 // 0 = mm
 // 1 = ee
 // 2 = tt
 
 const reco::Candidate* mcV = 0;
 const reco::Candidate* mcF1_fromV;
 const reco::Candidate* mcF2_fromV;
 // loop over gen particles
 for(size_t gp=0; gp<genParticles_.size();++gp){
  // const reco::Candidate* pMother = 0;
  // if(genParticles_[gp] -> mother()) {
  // pMother = genParticles_[gp] -> mother();
  // }
  
  int pdgId = genParticles_[gp] -> pdgId();
  int status = genParticles_[gp] -> status();
  // int charge = genParticles_[gp] -> charge();
  // int motherPdgId = 0;
  // if(genParticles_[gp] -> mother()) {
  // motherPdgId = pMother -> pdgId();
  // }
  
  // Z {23}
  if( (pdgId == 23) && (status == 3) ) {
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
   if(abs(mcF1_fromV -> pdgId()) >= 11) {
    FindDaughterParticles(&mcF1_fromV);
   }
   if(abs(mcF2_fromV -> pdgId()) >= 11) {
    FindDaughterParticles(&mcF2_fromV);
   }
   
   // mm
   if ( abs(mcF1_fromV -> pdgId()) == 13 && abs(mcF2_fromV -> pdgId()) == 13 ) {
    finalState = 0;
   }
   
   // ee
   if ( abs(mcF1_fromV -> pdgId()) == 11 && abs(mcF2_fromV -> pdgId()) == 11 ) {
    finalState = 1;
   }
   
   // tt
   if ( abs(mcF1_fromV -> pdgId()) == 15 && abs(mcF2_fromV -> pdgId()) == 15 ) {
    finalState = 2;
   }
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
 
 // const reco::Candidate* mcV1 = 0;
 // const reco::Candidate* mcF1_fromV1;
 // const reco::Candidate* mcF2_fromV1;
 // const reco::Candidate* mcV2 = 0;
 // const reco::Candidate* mcF1_fromV2;
 // const reco::Candidate* mcF2_fromV2;
 
 // loop over gen particles
 for(size_t gp=0; gp<genParticles_.size();++gp){
  // const reco::Candidate* pMother = 0;
  // if(genParticles_[gp] -> mother()) {
  // pMother = genParticles_[gp] -> mother();
  // }
  
  int pdgId = genParticles_[gp] -> pdgId();
  int status = genParticles_[gp] -> status();
  // int charge = genParticles_[gp] -> charge();
  // int motherPdgId = 0;
  // if(genParticles_[gp] -> mother()) {
  // motherPdgId = pMother -> pdgId();
  // }
  
  // std::cout << " genParticles_[" << gp << "::" << genParticles_.size() << "] = " << pdgId << " @ " << status << std::endl;
  
  // H {25}
  if( (pdgId == 25) && (status == 3) ) {
   mcH = &(*(genParticles_[gp]));
  }
 } // loop over gen particles
 
 
 // find vector bosons
 if (mcH != 0) {
  std::vector<const reco::Candidate*> VfromHBuffer;
  FindDaughterParticles(&mcH,&VfromHBuffer);
  
  // H > VV
  if(VfromHBuffer.size() == 2) {
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
    if(abs(mcF1_fromV1 -> pdgId()) >= 11) {
     FindDaughterParticles(&mcF1_fromV1);
    }
    if(abs(mcF2_fromV1 -> pdgId()) >= 11) {
     FindDaughterParticles(&mcF2_fromV1);
    }
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
    if(abs(mcF1_fromV2 -> pdgId()) >= 11) {
     FindDaughterParticles(&mcF1_fromV2);
    }
    if(abs(mcF2_fromV2 -> pdgId()) >= 11) {
     FindDaughterParticles(&mcF2_fromV2);
    }
   }
   else {
    isHWWok = false;
   }
   
   
   if (isHWWok) {
    
    // mm
    if ( (abs(mcF1_fromV1 -> pdgId()) == 13 || abs(mcF1_fromV1 -> pdgId()) == 14) && (abs(mcF1_fromV2 -> pdgId()) == 13 || abs(mcF1_fromV2 -> pdgId()) == 14) ) {
     finalState = 0;
    }
    
    // ee
    if ( (abs(mcF1_fromV1 -> pdgId()) == 11 || abs(mcF1_fromV1 -> pdgId()) == 12) && (abs(mcF1_fromV2 -> pdgId()) == 11 || abs(mcF1_fromV2 -> pdgId()) == 12) ) {
     finalState = 1;
    }
    
    // tt
    if ( (abs(mcF1_fromV1 -> pdgId()) == 15 || abs(mcF1_fromV1 -> pdgId()) == 16) && (abs(mcF1_fromV2 -> pdgId()) == 15 || abs(mcF1_fromV2 -> pdgId()) == 16) ) {
     finalState = 2;
    }
    
    // em
    if ( ( (abs(mcF1_fromV1 -> pdgId()) == 11 || abs(mcF1_fromV1 -> pdgId()) == 12) && (abs(mcF1_fromV2 -> pdgId()) == 13 || abs(mcF1_fromV2 -> pdgId()) == 14) )
     || ( (abs(mcF1_fromV1 -> pdgId()) == 13 || abs(mcF1_fromV1 -> pdgId()) == 14) && (abs(mcF1_fromV2 -> pdgId()) == 11 || abs(mcF1_fromV2 -> pdgId()) == 12) ) ) {
     finalState = 3;
     }
     
     // et
     if ( ( (abs(mcF1_fromV1 -> pdgId()) == 11 || abs(mcF1_fromV1 -> pdgId()) == 12) && (abs(mcF1_fromV2 -> pdgId()) == 15 || abs(mcF1_fromV2 -> pdgId()) == 16) )
      || ( (abs(mcF1_fromV1 -> pdgId()) == 15 || abs(mcF1_fromV1 -> pdgId()) == 16) && (abs(mcF1_fromV2 -> pdgId()) == 11 || abs(mcF1_fromV2 -> pdgId()) == 12) ) ) {
      finalState = 4;
      }
      
      // mt
      if ( ( (abs(mcF1_fromV1 -> pdgId()) == 12 || abs(mcF1_fromV1 -> pdgId()) == 13) && (abs(mcF1_fromV2 -> pdgId()) == 15 || abs(mcF1_fromV2 -> pdgId()) == 16) )
       || ( (abs(mcF1_fromV1 -> pdgId()) == 15 || abs(mcF1_fromV1 -> pdgId()) == 16) && (abs(mcF1_fromV2 -> pdgId()) == 12 || abs(mcF1_fromV2 -> pdgId()) == 13) ) ) {
       finalState = 5;
       }
       
       // jj - ev
       if (( (abs(mcF1_fromV1 -> pdgId()) <= 6 ) && (abs(mcF1_fromV2 -> pdgId()) == 11 || abs(mcF1_fromV2 -> pdgId()) == 12) ) ||
        ( (abs(mcF1_fromV2 -> pdgId()) <= 6 ) && (abs(mcF1_fromV1 -> pdgId()) == 11 || abs(mcF1_fromV1 -> pdgId()) == 12) )
       ){
        finalState = 6;
       }
       
       // jj - mv
       if (( (abs(mcF1_fromV1 -> pdgId()) <= 6 ) && (abs(mcF1_fromV2 -> pdgId()) == 13 || abs(mcF1_fromV2 -> pdgId()) == 14) ) ||
        ( (abs(mcF1_fromV2 -> pdgId()) <= 6 ) && (abs(mcF1_fromV1 -> pdgId()) == 13 || abs(mcF1_fromV1 -> pdgId()) == 14) )
       ){
        finalState = 7;
       }
       
       // jj - tv
       if (( (abs(mcF1_fromV1 -> pdgId()) <= 6 ) && (abs(mcF1_fromV2 -> pdgId()) == 15 || abs(mcF1_fromV2 -> pdgId()) == 16) ) ||
        ( (abs(mcF1_fromV2 -> pdgId()) <= 6 ) && (abs(mcF1_fromV1 -> pdgId()) == 15 || abs(mcF1_fromV1 -> pdgId()) == 16) )
       ){
        finalState = 8;
       }
       
       // jj - jj
       if (( (abs(mcF1_fromV1 -> pdgId()) <= 6 ) && (abs(mcF1_fromV2 -> pdgId()) <= 6 ) ) ||
        ( (abs(mcF1_fromV2 -> pdgId()) <= 6 ) && (abs(mcF1_fromV1 -> pdgId()) <= 6 ) )
       ){
        finalState = 9;
       }
       
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

const float reco::SkimEvent::HEPMCweight() const {
 float HEPMCweight = -1;
 HEPMCweight = GenInfoHandle_.weight();
 return HEPMCweight;
}















//---- H production mode: ggH, vbf, WH, ZH, ttH

const float reco::SkimEvent::mcHiggsProd() const {
 
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
 
 /*
  * 
  * 
  * const reco::Candidate* mcH = 0;
  * 
  * // loop over gen particles
  * for(size_t gp=0; gp<genParticles_.size();++gp){
  * 
  * int pdgId = genParticles_[gp] -> pdgId();
  * int status = genParticles_[gp] -> status();
  * // int charge = genParticles_[gp] -> charge();
  * // int motherPdgId = 0;
  * // if(genParticles_[gp] -> mother()) {
  * // motherPdgId = pMother -> pdgId();
  * // }
  * 
  * // std::cout << " [" << gp << "::" << genParticles_.size() << "] id = " << pdgId << std::endl;
  * 
  * // H {25}
  * if( (pdgId == 25) ) {
  * std::cout << "Higgs mother not status 3 : " << genParticles_[gp] -> mother() -> pdgId() << std::endl;
}
if( (pdgId == 25) && (status == 3) ) {
 mcH = &(*(genParticles_[gp]));
 std::cout << "Higgs mother : " << mcH -> mother() -> pdgId() << std::endl;
}
} // loop over gen particles

*/
 
 return productionMechanism;
}








//---- Higgs masses

const float reco::SkimEvent::getHiggsMass() const {
 
 float mass = -1;
 
 const reco::Candidate* mcH = 0;
 
 // loop over gen particles
 for(size_t gp=0; gp<genParticles_.size();++gp){
  
  int pdgId = genParticles_[gp] -> pdgId();
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
 
 float pt = -1;
 
 const reco::Candidate* mcH = 0;
 
 // loop over gen particles
 for(size_t gp=0; gp<genParticles_.size();++gp){
  
  int pdgId = genParticles_[gp] -> pdgId();
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
 
 // std::cout << " genParticles_.size() = " << genParticles_.size() << std::endl;
 
 // loop over gen particles
 for(size_t gp=0; gp<genParticles_.size();++gp){
  
  int pdgId = genParticles_[gp] -> pdgId();
  int status = genParticles_[gp] -> status();
  // std::cout << " pdgId = " << pdgId << " ~~ status = " << status << std::endl;
  
  // Stop1 {1000006} Stop2 {2000006}
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
  
  /* for general debugging purposes
   * float iPart_Pt1 = sqrt (LHEhepeup_.PUP.at (iPart) [0] * LHEhepeup_.PUP.at (iPart) [0] + // px
   * LHEhepeup_.PUP.at (iPart) [1] * LHEhepeup_.PUP.at (iPart) [1]); // py
   * TVector3 pt_ofIndex1(LHEhepeup_.PUP.at (iPart) [0], LHEhepeup_.PUP.at (iPart) [1], LHEhepeup_.PUP.at (iPart) [2] ); // pass px, py, pz
   * float phi1 = pt_ofIndex1.Phi() ;
   * float eta1 = pt_ofIndex1.PseudoRapidity() ;
   * float mass1 = LHEhepeup_.PUP.at (iPart) [4] ;
   * float energy1 = LHEhepeup_.PUP.at (iPart) [3] ;
   * std::cout << iPart << " ISTUP " << LHEhepeup_.ISTUP.at (iPart)
   * << " ID " << LHEhepeup_.IDUP.at (iPart) << " MO1 "
   * << LHEhepeup_.MOTHUP.at (iPart).first << " MO2 " << LHEhepeup_.MOTHUP.at(iPart).second
   * << " pT " << iPart_Pt1 << " eta " << eta1 << " phi " << phi1 << " mass " << mass1 << " energy " << energy1
   * << " px " << LHEhepeup_.PUP.at (iPart) [0] << " py " << LHEhepeup_.PUP.at (iPart) [1] << " pz " << LHEhepeup_.PUP.at (iPart) [2]
   * << std::endl;
   */
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


//---- FIXME if there is more than one Higgs, you need to fix this to create an order
//----       possibility: add "pt ordered" Higgs, as done for leptons
const float reco::SkimEvent::higgsLHEEta() const {
 std::vector<float> v_particleLHE_eta ;
 // loop over particles in the event
 for (unsigned int iPart = 0 ; iPart < LHEhepeup_.IDUP.size (); ++iPart) {
  if (LHEhepeup_.ISTUP.at (iPart) != 1) continue ;
  int type = abs (LHEhepeup_.IDUP.at (iPart)) ;
  if (type ==25) { //---- Higgs
   TVector3 temp_vector(LHEhepeup_.PUP.at (iPart) [0], LHEhepeup_.PUP.at (iPart) [1], LHEhepeup_.PUP.at (iPart) [2] ); // pass px, py, pz
   v_particleLHE_eta.push_back(temp_vector.Eta());
  }
 }
 //---- now return ----
 if ( 0 < v_particleLHE_eta.size() ) return v_particleLHE_eta.at(0);
 return -9999.9; //if no Higgs was found
}


const float reco::SkimEvent::higgsLHEPhi() const {
 std::vector<float> v_particleLHE_phi ;
 // loop over particles in the event
 for (unsigned int iPart = 0 ; iPart < LHEhepeup_.IDUP.size (); ++iPart) {
  if (LHEhepeup_.ISTUP.at (iPart) != 1) continue ;
  int type = abs (LHEhepeup_.IDUP.at (iPart)) ;
  if (type ==25) { //---- Higgs
   TVector3 temp_vector(LHEhepeup_.PUP.at (iPart) [0], LHEhepeup_.PUP.at (iPart) [1], LHEhepeup_.PUP.at (iPart) [2] ); // pass px, py, pz
   v_particleLHE_phi.push_back(temp_vector.Phi());
  }
 }
 //---- now return ----
 if ( 0 < v_particleLHE_phi.size() ) return v_particleLHE_phi.at(0);
 return -9999.9; //if no Higgs was found
}


const float reco::SkimEvent::higgsLHEmass() const {
 std::vector<float> v_particleLHE_mass ;
 // loop over particles in the event
 for (unsigned int iPart = 0 ; iPart < LHEhepeup_.IDUP.size (); ++iPart) {
  if (LHEhepeup_.ISTUP.at (iPart) != 1) continue ;
  int type = abs (LHEhepeup_.IDUP.at (iPart)) ;
  if (type ==25) { //---- Higgs
   v_particleLHE_mass.push_back(LHEhepeup_.PUP.at (iPart) [4]); //---- mass
   //---- see http://home.thep.lu.se/~leif/LHEF/classLHEF_1_1HEPEUP.html
  }
 }
 //---- now return ----
 if ( 0 < v_particleLHE_mass.size() ) return v_particleLHE_mass.at(0);
 return -9999.9; //if no Higgs was found
}



const float reco::SkimEvent::leadingGenJetPartonPt(size_t index) const {
 std::vector<float> v_jets_pt ;
 
 float pt = -9999.9;
 
 const reco::Candidate* mcH = 0;
 
 // loop over gen particles
 for(size_t gp=0; gp<genParticles_.size();++gp){
  int type = abs( genParticles_[gp] -> pdgId() );
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





//---- Vector Bosons

const float reco::SkimEvent::genVBosonPt(size_t index) const {
 
 std::vector<float> v_bosons_pt;
 
 float particlePt = defaultvalues::defaultFloat;
 
 const reco::Candidate* mcH = 0;
 
 // Loop over gen particles
 for (size_t gp=0; gp<genParticles_.size(); ++gp) {
  int type = abs(genParticles_[gp]->pdgId());
  
  if (type != 23 && type != 24) continue;
  
  mcH = &(*(genParticles_[gp]));
  v_bosons_pt.push_back(mcH->pt());
 }
 
 if (v_bosons_pt.size () > 0) {
  std::sort(v_bosons_pt.rbegin(), v_bosons_pt.rend());
 }
 
 size_t count = 0;
 for (size_t i=0; i<v_bosons_pt.size(); ++i) {
  if (++count > index) return v_bosons_pt.at(i);
 }
 
 return particlePt;
}


// Compatible with PYTHIA8
const float reco::SkimEvent::genVBosonStatus(size_t index) const {
 
 float pt_ofIndex     = genVBosonPt(index);
 float particleStatus = defaultvalues::defaultFloat;
 
 const reco::Candidate* mcH = 0;
 
 // Loop over gen particles
 for (size_t gp=0; gp<genParticles_.size(); ++gp) {
  int type = abs(genParticles_[gp]->pdgId());
  
  if (type != 23 && type != 24) continue;
  
  mcH = &(*(genParticles_[gp]));
  if (mcH->pt() != pt_ofIndex) continue;
  particleStatus = (float) genParticles_[gp]->status();
 }
 
 return particleStatus;
}



// Compatible with PYTHIA8
const float reco::SkimEvent::genVBosonPID(size_t index) const {
 
 float pt_ofIndex = genVBosonPt(index);
 float particleID = defaultvalues::defaultFloat;
 
 const reco::Candidate* mcH = 0;
 
 // Loop over gen particles
 for (size_t gp=0; gp<genParticles_.size(); ++gp) {
  int type = genParticles_[gp]->pdgId();
  
  if (abs(type) != 23 && abs(type) != 24) continue;
  
  mcH = &(*(genParticles_[gp]));
  if (mcH->pt() != pt_ofIndex) continue;
  
  particleID = (float) type;
 }
 
 return particleID;
}


// Compatible with PYTHIA8
const float reco::SkimEvent::genVBosonEta(size_t index) const {
 
 float pt_ofIndex  = genVBosonPt(index);
 float particleEta = defaultvalues::defaultFloat;
 
 const reco::Candidate* mcH = 0;
 
 // Loop over gen particles
 for (size_t gp=0; gp<genParticles_.size(); ++gp) {
  int type = abs(genParticles_[gp]->pdgId());
  
  if (type != 23 && type != 24) continue;
  
  mcH = &(*(genParticles_[gp]));
  if (mcH->pt() != pt_ofIndex) continue;
  particleEta = (float) mcH->eta();
 }
 
 return particleEta;
}


// Compatible with PYTHIA8
const float reco::SkimEvent::genVBosonPhi(size_t index) const {
 
 float pt_ofIndex  = genVBosonPt(index);
 float particlePhi = defaultvalues::defaultFloat;
 
 const reco::Candidate* mcH = 0;
 
 // Loop over gen particles
 for (size_t gp=0; gp<genParticles_.size(); ++gp) {
  int type = abs(genParticles_[gp]->pdgId());
  
  if (type != 23 && type != 24) continue;
  
  mcH = &(*(genParticles_[gp]));
  if (mcH->pt() != pt_ofIndex) continue;
  particlePhi = (float) mcH->phi();
 }
 
 return particlePhi;
}



const float reco::SkimEvent::genVBosonIsHardProcess(size_t index) const {
 float pt_ofIndex   = genVBosonPt(index);
 const reco::Candidate* mcH = 0;
 
 // Loop over gen particles
 for (size_t gp=0; gp<genParticles_.size(); gp++) {
  int type = abs(genParticles_[gp]->pdgId());
  if (type != 23 && type != 24) continue; //--- Z = 23, W = 24
  
  mcH = &(*(genParticles_[gp]));
  if (mcH->pt() != pt_ofIndex) continue;
  
  return genParticles_[gp]-> isHardProcess();
 } 
 
 return defaultvalues::defaultFloat;
}


const float reco::SkimEvent::genVBosonFromHardProcessBeforeFSR(size_t index) const {
 float pt_ofIndex   = genVBosonPt(index);
 const reco::Candidate* mcH = 0;
 
 // Loop over gen particles
 for (size_t gp=0; gp<genParticles_.size(); gp++) {
  int type = abs(genParticles_[gp]->pdgId());
  if (type != 23 && type != 24) continue; //--- Z = 23, W = 24
  
  mcH = &(*(genParticles_[gp]));
  if (mcH->pt() != pt_ofIndex) continue;
  
  return genParticles_[gp]-> fromHardProcessBeforeFSR();
 } 
 
 return defaultvalues::defaultFloat;
}







// This method traces carbon copies of the particle up to its top mother. If
// there are no such carbon copies, the status of the particle itself will be
// returned. A carbon copy is when the "same" particle appears several times in
// the event record, but with changed momentum owing to recoil effects.
const int reco::SkimEvent::originalStatus(const reco::Candidate* p) const
{
  const reco::Candidate* pMother = 0;

  if (p->mother()) {
    pMother = p->mother();

    if (pMother->pdgId() == p->pdgId())
      return originalStatus(pMother);
    else
      return p->status();
  }
  else
    return p->status();
}


// Compatible with PYTHIA8
const float reco::SkimEvent::genLeptonPt(size_t index) const {

  std::vector<float> v_leptons_pt;

  float particlePt = defaultvalues::defaultFloat;
 
  const reco::Candidate* mcH = 0;
 
  // Loop over gen particles
  for (size_t gp=0; gp<genParticles_.size(); ++gp) {
    int type = abs(genParticles_[gp]->pdgId());

    if (type != 11 && type != 13 && type != 15) continue;

    mcH = &(*(genParticles_[gp]));
    v_leptons_pt.push_back(mcH->pt());
  }
 
  if (v_leptons_pt.size () > 0) {
    std::sort(v_leptons_pt.rbegin(), v_leptons_pt.rend());
  }

  size_t count = 0;
  for (size_t i=0; i<v_leptons_pt.size(); ++i) {
    if (++count > index) return v_leptons_pt.at(i);
  }
 
  return particlePt;
}


// Compatible with PYTHIA8
const float reco::SkimEvent::genLeptonStatus(size_t index) const {

  float pt_ofIndex     = genLeptonPt(index);
  float particleStatus = defaultvalues::defaultFloat;

  const reco::Candidate* mcH = 0;

  // Loop over gen particles
  for (size_t gp=0; gp<genParticles_.size(); ++gp) {
    int type = abs(genParticles_[gp]->pdgId());

    if (type != 11 && type != 13 && type != 15) continue;

    mcH = &(*(genParticles_[gp]));
    if (mcH->pt() != pt_ofIndex) continue;
    particleStatus = (float) genParticles_[gp]->status();
 }

 return particleStatus;
}


// Compatible with PYTHIA8
const float reco::SkimEvent::genLeptonIndex(size_t index) const {

  float pt_ofIndex    = genLeptonPt(index);
  float particleIndex = defaultvalues::defaultFloat;

  const reco::Candidate* mcH = 0;

  // Loop over gen particles
  for (size_t gp=0; gp<genParticles_.size(); ++gp) {
    int type = abs(genParticles_[gp]->pdgId());

    if (type != 11 && type != 13 && type != 15) continue;

    mcH = &(*(genParticles_[gp]));
    if (mcH->pt() != pt_ofIndex) continue;
    particleIndex = (float) gp;
 }

 return particleIndex;
}


// Compatible with PYTHIA8
const float reco::SkimEvent::genLeptonPID(size_t index) const {

  float pt_ofIndex = genLeptonPt(index);
  float particleID = defaultvalues::defaultFloat;

  const reco::Candidate* mcH = 0;

  // Loop over gen particles
  for (size_t gp=0; gp<genParticles_.size(); ++gp) {
    int type = genParticles_[gp]->pdgId();

    if (abs(type) != 11 && abs(type) != 13 && abs(type) != 15) continue;

    mcH = &(*(genParticles_[gp]));
    if (mcH->pt() != pt_ofIndex) continue;

    particleID = (float) type;
 }

 return particleID;
}


// Compatible with PYTHIA8
const float reco::SkimEvent::genLeptonEta(size_t index) const {

  float pt_ofIndex  = genLeptonPt(index);
  float particleEta = defaultvalues::defaultFloat;

  const reco::Candidate* mcH = 0;

  // Loop over gen particles
  for (size_t gp=0; gp<genParticles_.size(); ++gp) {
    int type = abs(genParticles_[gp]->pdgId());

    if (type != 11 && type != 13 && type != 15) continue;

    mcH = &(*(genParticles_[gp]));
    if (mcH->pt() != pt_ofIndex) continue;
    particleEta = (float) mcH->eta();
  }

  return particleEta;
}


// Compatible with PYTHIA8
const float reco::SkimEvent::genLeptonPhi(size_t index) const {

  float pt_ofIndex  = genLeptonPt(index);
  float particlePhi = defaultvalues::defaultFloat;

  const reco::Candidate* mcH = 0;

  // Loop over gen particles
  for (size_t gp=0; gp<genParticles_.size(); ++gp) {
    int type = abs(genParticles_[gp]->pdgId());

    if (type != 11 && type != 13 && type != 15) continue;

    mcH = &(*(genParticles_[gp]));
    if (mcH->pt() != pt_ofIndex) continue;
    particlePhi = (float) mcH->phi();
  }

  return particlePhi;
}


// Compatible with PYTHIA8
const float reco::SkimEvent::genLeptonMotherPID(size_t index) const {

  float pt_ofIndex = genLeptonPt(index);
  float motherPID  = defaultvalues::defaultFloat;

  const reco::Candidate* mcH = 0;

  // Loop over gen particles
  for (size_t gp=0; gp<genParticles_.size(); ++gp) {

    int type = abs(genParticles_[gp]->pdgId());
    if (type != 11 && type != 13 && type != 15) continue;

    int motherPdgId = 0;
    const reco::Candidate* pMother = 0;
    if (genParticles_[gp] -> mother()) {
      pMother = genParticles_[gp]->mother();
      motherPdgId = pMother->pdgId();
    }
  
    mcH = &(*(genParticles_[gp]));
    if (mcH->pt() != pt_ofIndex) continue;
    motherPID = (float) motherPdgId;
  }
  
  return motherPID;
}


const float reco::SkimEvent::genLeptonIsHardProcess(size_t index) const {
 float pt_ofIndex   = genLeptonPt(index);
 const reco::Candidate* mcH = 0;
 
 // Loop over gen particles
 for (size_t gp=0; gp<genParticles_.size(); gp++) {
  int type = abs(genParticles_[gp]->pdgId());
  if (type != 11 && type != 13 && type != 15) continue; //--- e = 11, mu = 13, tau = 15
  
  mcH = &(*(genParticles_[gp]));
  if (mcH->pt() != pt_ofIndex) continue;
  
  return genParticles_[gp]-> isHardProcess();
 } 
 
 return defaultvalues::defaultFloat;
}

const float reco::SkimEvent::genLeptonFromHardProcessDecayed(size_t index) const {
 float pt_ofIndex   = genLeptonPt(index);
 const reco::Candidate* mcH = 0;
 
 // Loop over gen particles
 for (size_t gp=0; gp<genParticles_.size(); gp++) {
  int type = abs(genParticles_[gp]->pdgId());
  if (type != 11 && type != 13 && type != 15) continue; //--- e = 11, mu = 13, tau = 15
  
  mcH = &(*(genParticles_[gp]));
  if (mcH->pt() != pt_ofIndex) continue;
  
  return genParticles_[gp]-> fromHardProcessDecayed();
 } 
 
 return defaultvalues::defaultFloat;
}


const float reco::SkimEvent::genLeptonFromHardProcessBeforeFSR(size_t index) const {
 float pt_ofIndex   = genLeptonPt(index);
 const reco::Candidate* mcH = 0;
 
 // Loop over gen particles
 for (size_t gp=0; gp<genParticles_.size(); gp++) {
  int type = abs(genParticles_[gp]->pdgId());
  if (type != 11 && type != 13 && type != 15) continue; //--- e = 11, mu = 13, tau = 15
  
  mcH = &(*(genParticles_[gp]));
  if (mcH->pt() != pt_ofIndex) continue;
  
  return genParticles_[gp]-> fromHardProcessBeforeFSR();
 } 
 
 return defaultvalues::defaultFloat;
}




// Compatible with PYTHIA8
const float reco::SkimEvent::genLeptonMotherStatus(size_t index) const {

  float pt_ofIndex   = genLeptonPt(index);
  float motherStatus = defaultvalues::defaultFloat;

  const reco::Candidate* mcH = 0;

  // Loop over gen particles
  for (size_t gp=0; gp<genParticles_.size(); ++gp) {

    int type = abs(genParticles_[gp]->pdgId());
    if (type != 11 && type != 13 && type != 15) continue;

    int motherOriginalStatus = 0;
    const reco::Candidate* pMother = 0;
    if (genParticles_[gp] -> mother()) {
      pMother = genParticles_[gp]->mother();
      motherOriginalStatus = originalStatus(pMother);
    }
  
    mcH = &(*(genParticles_[gp]));
    if (mcH->pt() != pt_ofIndex) continue;
    motherStatus = (float) motherOriginalStatus;
  }

  return motherStatus;
}



//---- neutrinos

const float reco::SkimEvent::leadingGenNeutrinoIsHardProcess(size_t index) const {
 float pt_ofIndex   = leadingGenNeutrinoPt(index);
 const reco::Candidate* mcH = 0;
 
 // Loop over gen particles
 for (size_t gp=0; gp<genParticles_.size(); gp++) {
  int type = abs(genParticles_[gp]->pdgId());
  if( (type == 12 || type == 14 || type == 16) ) {
   
   mcH = &(*(genParticles_[gp]));
   if (mcH->pt() != pt_ofIndex) continue;
   
   return genParticles_[gp]-> isHardProcess();
  }
 } 
 
 return defaultvalues::defaultFloat;
}


const float reco::SkimEvent::leadingGenNeutrinoFromHardProcessBeforeFSR(size_t index) const {
 float pt_ofIndex   = leadingGenNeutrinoPt(index);
 const reco::Candidate* mcH = 0;
 
 // Loop over gen particles
 for (size_t gp=0; gp<genParticles_.size(); gp++) {
  int type = abs(genParticles_[gp]->pdgId());
  if( (type == 12 || type == 14 || type == 16) ) {
   
   mcH = &(*(genParticles_[gp]));
   if (mcH->pt() != pt_ofIndex) continue;
   
   return genParticles_[gp]-> fromHardProcessBeforeFSR();
  }
 } 
 
 return defaultvalues::defaultFloat;
}



const float reco::SkimEvent::leadingGenNeutrinoPt(size_t index) const {
 std::vector<float> v_jets_pt ;
 // std::cout << " getSusyMass1 " << std::endl;
 float pt = -9999.9;
 const reco::Candidate* mcH = 0;
 
 // loop over gen particles
 for(size_t gp=0; gp<genParticles_.size();++gp){
  int type = abs( genParticles_[gp] -> pdgId() );
//   int status = genParticles_[gp] -> status();
  
  // Stop {1000006}
  if( (type == 12 || type == 14 || type == 16) ) {
//   if( (type == 12 || type == 14 || type == 16) && (status == 3) ) {
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
//   int status = genParticles_[gp] -> status();
  if( (type == 12 || type == 14 || type == 16) ) {
//   if( (type == 12 || type == 14 || type == 16) && (status == 3) ) {
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
//   int status = genParticles_[gp] -> status();
  if( (type == 12 || type == 14 || type == 16) ) {
//   if( (type == 12 || type == 14 || type == 16) && (status == 3) ) {
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
//   int status = genParticles_[gp] -> status();
  if( (type == 12 || type == 14 || type == 16) ) {
//   if( (type == 12 || type == 14 || type == 16) && (status == 3) ) {
   mcH = &(*(genParticles_[gp]));
   if( mcH->pt() != pt_ofIndex) continue;
   particlePhi = (float) mcH->phi();
  }
 } // loop over gen particles
 return particlePhi;
}

const float reco::SkimEvent::genMetPt() const {
 float pT = -9999.9;
 if(genMetRef_.isNonnull()) pT = genMetRef_->pt();
 else pT = genMet_.pt(); //---- FIXME
 return pT;
}

const float reco::SkimEvent::genMetEta() const {
 float eta = -9999.9;
 if(genMetRef_.isNonnull()) eta = genMetRef_->eta();
 else eta = genMet_.eta(); //---- FIXME
 return eta;
}

const float reco::SkimEvent::genMetPhi() const {
 float phi = -9999.9;
 if(genMetRef_.isNonnull()) phi = genMetRef_->phi();
 else phi = genMet_.phi(); //---- FIXME
 return phi;
}

const float reco::SkimEvent::leadingGenJetPt(size_t index) const {
 std::vector<float> v_jets_pt ;
 // loop over particles in the event
 for (unsigned int iPart = 0 ; iPart < genJets_.size (); ++iPart) {
  if ( abs(genJets_[iPart]->eta()) > maxEtaForJets_) continue ;
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
  if ( abs(genJets_[iPart]->eta()) > maxEtaForJets_) continue ;
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
  if ( abs(genJets_[iPart]->eta()) > maxEtaForJets_) continue ;
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

//BTAGGING ################################################################
const float reco::SkimEvent::jettcheByPt(size_t i = 0) const {
 return leadingJetBtag(i,"pfTrackCountingHighEffBJetTags",minPtForJets_,maxEtaForJets_,applyCorrectionForJets_,applyIDForJets_,dzCutForBtagJets_);
}
const float reco::SkimEvent::jettchpByPt(size_t i = 0) const {
 return leadingJetBtag(i,"pfTrackCountingHighPurBJetTags",minPtForJets_,maxEtaForJets_,applyCorrectionForJets_,applyIDForJets_,dzCutForBtagJets_);
}
const float reco::SkimEvent::jetbjpbByPt(size_t i = 0) const {
 return leadingJetBtag(i,"pfJetBProbabilityBJetTags",minPtForJets_,maxEtaForJets_,applyCorrectionForJets_,applyIDForJets_,dzCutForBtagJets_);
}
const float reco::SkimEvent::jetcsvv2ivfByPt(size_t i = 0) const {
 return leadingJetBtag(i,"pfCombinedInclusiveSecondaryVertexV2BJetTags",minPtForJets_,maxEtaForJets_,applyCorrectionForJets_,applyIDForJets_,dzCutForBtagJets_);
}
const float reco::SkimEvent::jetssvheByPt(size_t i = 0) const {
 return leadingJetBtag(i,"pfSimpleSecondaryVertexHighEffBJetTags",minPtForJets_,maxEtaForJets_,applyCorrectionForJets_,applyIDForJets_,dzCutForBtagJets_);
}
const float reco::SkimEvent::jetssvhbByPt(size_t i = 0) const {
 return leadingJetBtag(i,"pfSimpleSecondaryVertexHighPurBJetTags",minPtForJets_,maxEtaForJets_,applyCorrectionForJets_,applyIDForJets_,dzCutForBtagJets_);
}
const float reco::SkimEvent::jetpfcsvByPt(size_t i = 0) const {
 return leadingJetBtag(i,"pfCombinedSecondaryVertexBJetTags",minPtForJets_,maxEtaForJets_,applyCorrectionForJets_,applyIDForJets_,dzCutForBtagJets_);
}
const float reco::SkimEvent::jetcmvaByPt(size_t i = 0) const {
 return leadingJetBtag(i,"combinedMVABJetTags",minPtForJets_,maxEtaForJets_,applyCorrectionForJets_,applyIDForJets_,dzCutForBtagJets_); 
}

//---- Photon
void reco::SkimEvent::setPhoton(const edm::Handle<edm::View<reco::RecoCandidate> > &h,size_t i){
 //std::cout << "setting lepton with collection ID: " << h->ptrAt(i).id() << std::endl;
 phos_.push_back( h->ptrAt(i) );
}
const pat::Photon * reco::SkimEvent::getPhoton(size_t i) const {
 return getPhoton(phos_[i]);
}
const pat::Photon * reco::SkimEvent::getPhoton(const refToCand &c) const {
 return static_cast<const pat::Photon*>(c.get());
}
const math::XYZTLorentzVector reco::SkimEvent::photon(size_t i) const {
 //  std::cout << " reco::SkimEvent::lepton :: accessing i = " << i << std::endl;
 if(indexByPtPho (i) >= phos_.size()) return math::XYZTLorentzVector(0,0,0,0);
 return phos_[indexByPtPho (i)]->p4();
}
const size_t reco::SkimEvent::indexByPtPho(size_t i) const {
 
 if( i >= phos_.size() ) return 9999; //--> big number then it will fail other tests later! good!
 std::vector<indexValueStruct> a;
 
 for(size_t j=0;j<phos_.size();++j) a.push_back(indexValueStruct(pt(j),j));
 std::sort(a.begin(),a.end(),highToLow);
 
 return a[i].index;
}
const int reco::SkimEvent::nPhos() const{
 return (const int) phos_.size();
}

const float reco::SkimEvent::mllg() const{
 if( phos_.size() < 1 || leps_.size() < 2 ) return 0;
 else{
  math::XYZTLorentzVector pho = phos_[indexByPtPho (0)]->p4();
  math::XYZTLorentzVector lep1 = leps_[indexByPt (0)]->p4();
  math::XYZTLorentzVector lep2 = leps_[indexByPt (1)]->p4();
  math::XYZTLorentzVector HCand = pho + lep1 + lep2;
  return HCand.M();
 }
}

const float reco::SkimEvent::mllgid(int WP) const{
 if( Pho_n_ID(WP) < 1 || leps_.size() < 2 ) return 0;
 else{
  math::XYZTLorentzVector pho = photon_id(0, WP);
  math::XYZTLorentzVector lep1 = leps_[indexByPt (0)]->p4();
  math::XYZTLorentzVector lep2 = leps_[indexByPt (1)]->p4();
  math::XYZTLorentzVector HCand = pho + lep1 + lep2;
  return HCand.M();
 }
}


//--- electrons

void reco::SkimEvent::InitEffectiveAreasElectrons(){
 //---- see https://github.com/ikrav/cmssw/blob/work_in_progress_v2/EgammaAnalysis/ElectronTools/data/PHYS14/effAreaElectrons_cone03_pfNeuHadronsAndPhotons.txt
 
//  # |eta| min   |eta| max   effective area
//  0.0000         0.8000        0.1013
//  0.8000         1.3000        0.0988
//  1.3000         2.0000        0.0572
//  2.0000         2.2000        0.0842
//  2.2000         5.0000        0.1530
 
 //double _eaElectronIso[eta range]
 //
 _eaElectronIso[0] = 0.1013;
 _eaElectronIso[1] = 0.0988;
 _eaElectronIso[2] = 0.0572;
 _eaElectronIso[3] = 0.0842;
 _eaElectronIso[4] = 0.1530;
 
} 


const float reco::SkimEvent::GetElectronEffectiveArea(size_t i) const {
 if(i >= leps_.size()) return defaultvalues::defaultFloat;
 if( isElectron(i) ) {
  float eta = leps_.at(i)->eta();
  if( fabs(eta) < 0.8000 ) return _eaElectronIso[0];
  else if( fabs(eta) > 0.8000 && fabs(eta) < 1.3000 ) return _eaElectronIso[1];
  else if( fabs(eta) > 1.3000 && fabs(eta) < 2.0000 ) return _eaElectronIso[2];
  else if( fabs(eta) > 2.0000 && fabs(eta) < 2.2000 ) return _eaElectronIso[3];
  else if( fabs(eta) > 2.2000)                        return _eaElectronIso[4];
  //  else if( fabs(eta) > 2.2000 && fabs(eta) < 5.0000 ) return _eaElectronIso[1]; 
  else return defaultvalues::defaultFloat; 
 }
 else return defaultvalues::defaultFloat; 
}




//--- Photon ID variables

const float reco::SkimEvent::Pho_sigmaIetaIeta(size_t i) const {
 if(i >= phos_.size()) return defaultvalues::defaultFloat;
 return getPhoton(i)->sigmaIetaIeta();
 //return -999.0;
}

const float reco::SkimEvent::Pho_hadronicOverEm(size_t i) const {
 if(i >= phos_.size()) return defaultvalues::defaultFloat;
 return getPhoton(i)->hadronicOverEm();
 //else return -999.0;
}

const float reco::SkimEvent::Pho_ChargedHadronIso(size_t i) const {
 if(i >= phos_.size()) return defaultvalues::defaultFloat;
 return getPhoton(i)->chargedHadronIso();
 //else return -999.0;
}

const float reco::SkimEvent::Pho_NeutralHadronIso(size_t i) const {
 if(i >= phos_.size()) return defaultvalues::defaultFloat;
 return getPhoton(i)->neutralHadronIso();
 //else return -999.0;
}

const float reco::SkimEvent::Pho_PhotonIso(size_t i) const {
 if(i >= phos_.size()) return defaultvalues::defaultFloat;
 return getPhoton(i)->photonIso();
 //else return -999.0;
}

const int reco::SkimEvent::Pho_PassElectronVeto(size_t i) const {
 if(i >= phos_.size()) return defaultvalues::defaultFloat;
 return getPhoton(i)->passElectronVeto();
 //else return -999.0;
}

const int reco::SkimEvent::Pho_HasPixelSeed(size_t i) const {
 if(i >= phos_.size()) return defaultvalues::defaultFloat;
 return getPhoton(i)->hasPixelSeed();
 //else return -999.0;
}

double reco::SkimEvent::ChoosePhotonEffectiveArea(int type, double phoEta) const {
 if(type > 2) {
  std::cout << "You chose the wrong type. Types are 0 (charged), 1 (neutral) and 2 (photon)" << std::endl;
  return -1;
 }
 if( fabs(phoEta) < 1.0 ) return eaPhotonIso_[0][type];
 else if( fabs(phoEta) > 1.000 && fabs(phoEta) < 1.479 ) return eaPhotonIso_[1][type];
 else if( fabs(phoEta) > 1.479 && fabs(phoEta) < 2.000 ) return eaPhotonIso_[2][type];
 else if( fabs(phoEta) > 2.000 && fabs(phoEta) < 2.200 ) return eaPhotonIso_[3][type];
 else if( fabs(phoEta) > 2.200 && fabs(phoEta) < 2.300 ) return eaPhotonIso_[4][type];
 else if( fabs(phoEta) > 2.300 && fabs(phoEta) < 2.400 ) return eaPhotonIso_[5][type];
 else if( fabs(phoEta) > 2.400 ) return eaPhotonIso_[5][type];
 std::cout << "Something weird has happened and I couldn't find the correct rho for you... :(" << std::endl;
 return -1;
 
}

void reco::SkimEvent::InitEffectiveAreasPhoton(){
 //double eaPhotonIso_[eta range][0: charged, 1: neutral, 2: photon] 
 //
 //abs(eta) < 1.0
 eaPhotonIso_[0][0] = 0.0080; //ch
 eaPhotonIso_[0][1] = 0.0126; //nh
 eaPhotonIso_[0][2] = 0.0982; //ph
 
 //1.0 < abs(eta) < 1.479
 eaPhotonIso_[1][0] = 0.0079; //ch
 eaPhotonIso_[1][1] = 0.0237; //nh
 eaPhotonIso_[1][2] = 0.0857; //ph
 
 //1.479 < abs(eta) < 2.0
 eaPhotonIso_[2][0] = 0.0080; //ch
 eaPhotonIso_[2][1] = 0.0000; //nh
 eaPhotonIso_[2][2] = 0.0484; //ph
 
 //2.0 < abs(eta) < 2.2
 eaPhotonIso_[3][0] = 0.0048; //ch
 eaPhotonIso_[3][1] = 0.0000; //nh
 eaPhotonIso_[3][2] = 0.0668; //ph
 
 //2.2 < abs(eta) < 2.3
 eaPhotonIso_[4][0] = 0.0029; //ch
 eaPhotonIso_[4][1] = 0.0000; //nh
 eaPhotonIso_[4][2] = 0.0868; //ph
 
 //2.3 < abs(eta) < 2.4
 eaPhotonIso_[5][0] = 0.0036; //ch
 eaPhotonIso_[5][1] = 0.0000; //nh
 eaPhotonIso_[5][2] = 0.0982; //ph
 
 //2.4 < abs(eta)
 eaPhotonIso_[6][0] = 0.0016; //ch
 eaPhotonIso_[6][1] = 0.0769; //nh
 eaPhotonIso_[6][2] = 0.1337; //ph
 
} 

const float reco::SkimEvent::Pho_rhoChargedHadronIso(size_t i) const {
 if(i >= phos_.size()) return defaultvalues::defaultFloat;
 math::XYZTLorentzVector pho = phos_[indexByPtPho (i)]->p4();
 double thisEA = ChoosePhotonEffectiveArea(0, pho.Eta());
 double thisRho = getJetRhoIso(); 
 double ch = std::max(getPhoton(i)->chargedHadronIso() - thisRho*thisEA, 0.);
 return ch;
 //else return -999.0;
}

const float reco::SkimEvent::Pho_rhoNeutralHadronIso(size_t i) const {
 if(i >= phos_.size()) return defaultvalues::defaultFloat;
 math::XYZTLorentzVector pho = phos_[indexByPtPho (i)]->p4();
 double thisEA = ChoosePhotonEffectiveArea(1, pho.Eta());
 double thisRho = getJetRhoIso(); 
 double nh = std::max(getPhoton(i)->neutralHadronIso() - thisRho*thisEA, 0.);
 return nh;
 //else return -999.0;
}

const float reco::SkimEvent::Pho_rhoPhotonIso(size_t i) const {
 if(i >= phos_.size()) return defaultvalues::defaultFloat;
 math::XYZTLorentzVector pho = phos_[indexByPtPho (i)]->p4();
 double thisEA = ChoosePhotonEffectiveArea(2, pho.Eta());
 double thisRho = getJetRhoIso(); 
 double ph = std::max(getPhoton(i)->photonIso()- thisRho*thisEA, 0.);
 return ph;
 //else return -999.0;
}


void reco::SkimEvent::InitIDPhoton(){
 
 //Loose
 ////Barrel
 /*hoe_cut*/             PhotonIDparams_[0][0][0] = 0.553;
 /*sietaieta_cut*/       PhotonIDparams_[0][0][1] = 0.0099;
 /*chiso_cut*/           PhotonIDparams_[0][0][2] = 2.49;
 ///*nhiso_cut*/         PhotonIDparams_[0][0][3] = 15.43 + 0.007*pho.Pt();
 ///*phiso_cut*/         PhotonIDparams_[0][0][4] = 9.42 + 0.0033*pho.Pt();
 ////Endcap
 /*hoe_cut*/             PhotonIDparams_[0][1][0] = 0.062;
 /*sietaieta_cut*/       PhotonIDparams_[0][1][1] = 0.0284;
 /*chiso_cut*/           PhotonIDparams_[0][1][2] = 1.04;
 ///*nhiso_cut*/         PhotonIDparams_[0][1][3] = 19.71 + 0.0129*pho.Pt();
 ///*phiso_cut*/         PhotonIDparams_[0][1][4] = 11.88 + 0.0108*pho.Pt();
 
 //Medium
 ////Barrel
 /*hoe_cut*/             PhotonIDparams_[1][0][0] = 0.058;
 /*sietaieta_cut*/       PhotonIDparams_[1][0][1] = 0.0099;
 /*chiso_cut*/           PhotonIDparams_[1][0][2] = 1.91;
 ///*nhiso_cut*/         PhotonIDparams_[1][0][3] = 4.66 + 0.007*pho.Pt();
 ///*phiso_cut*/         PhotonIDparams_[1][0][4] = 4.29 + 0.0033*pho.Pt();
 ////Endcap
 /*hoe_cut*/             PhotonIDparams_[1][1][0] = 0.020;
 /*sietaieta_cut*/       PhotonIDparams_[1][1][1] = 0.0268;
 /*chiso_cut*/           PhotonIDparams_[1][1][2] = 0.82;
 ///*nhiso_cut*/         PhotonIDparams_[1][1][3] = 14.65 + 0.0129*pho.Pt();
 ///*phiso_cut*/         PhotonIDparams_[1][1][4] = 4.06 + 0.0108*pho.Pt();
 
 //Tight
 ////Barrel
 /*hoe_cut*/             PhotonIDparams_[2][0][0] = 0.019;
 /*sietaieta_cut*/       PhotonIDparams_[2][0][1] = 0.0099;
 /*chiso_cut*/           PhotonIDparams_[2][0][2] = 1.61;
 ///*nhiso_cut*/         PhotonIDparams_[2][0][3] = 3.98 + 0.007*pho.Pt();
 ///*phiso_cut*/         PhotonIDparams_[2][0][4] = 3.01 + 0.0033*pho.Pt();
 ////Endcap
 /*hoe_cut*/             PhotonIDparams_[2][1][0] = 0.016;
 /*sietaieta_cut*/       PhotonIDparams_[2][1][1] = 0.0263;
 /*chiso_cut*/           PhotonIDparams_[2][1][2] = 0.69;
 ///*nhiso_cut*/         PhotonIDparams_[2][1][3] = 4.52 + 0.0129*pho.Pt();
 ///*phiso_cut*/         PhotonIDparams_[2][1][4] = 3.61 + 0.0108*pho.Pt();
 
}


const math::XYZTLorentzVector reco::SkimEvent::photon_id(size_t i, int WP) const {
 //  std::cout << " reco::SkimEvent::lepton :: accessing i = " << i << std::endl;
 if(indexByPtPho (i) >= phos_.size()) return math::XYZTLorentzVector(0,0,0,0);
 if( Pho_n_ID(WP) < 1) return math::XYZTLorentzVector(0,0,0,0);
 
 for( unsigned int a = 0; a < phos_.size(); a++){
  if( Pho_IsIdIso(indexByPtPho(a), WP)) return phos_[indexByPtPho(a)]->p4();
 }
 return phos_[indexByPtPho (i)]->p4();
}

const int reco::SkimEvent::Pho_n_ID(int WP) const {
 if( phos_.size() < 1) return 0;
 int nphoid = 0;
 for( unsigned int i = 0; i < phos_.size(); i++){
  if( Pho_IsIdIso( i, WP) ) nphoid++;
 }
 return nphoid;
}

const bool reco::SkimEvent::Pho_IsIdIso(size_t i, int wp) const { //wp = 0 (loose), 1 (medium), 2 (tight)
 if(i >= phos_.size()) return defaultvalues::defaultFloat;
 math::XYZTLorentzVector pho = phos_[indexByPtPho (i)]->p4();
 double hoe_cut[2];
 double sietaieta_cut[2];
 double chiso_cut[2];
 double nhiso_cut[2];
 double phiso_cut[2];
 switch(wp){
  case 0:
   //Barrel
   hoe_cut[0] = 0.553;
   sietaieta_cut[0] = 0.0099;
   chiso_cut[0] = 2.49;
   nhiso_cut[0] = 15.43 + 0.007*pho.Pt();
   phiso_cut[0] = 9.42 + 0.0033*pho.Pt();
   //Endcap
   hoe_cut[1] = 0.062;
   sietaieta_cut[1] = 0.0284;
   chiso_cut[1] = 1.04;
   nhiso_cut[1] = 19.71 + 0.0129*pho.Pt();
   phiso_cut[1] = 11.88 + 0.0108*pho.Pt();
  case 1:
   //Barrel
   hoe_cut[0] = 0.058;
   sietaieta_cut[0] = 0.0099;
   chiso_cut[0] = 1.91;
   nhiso_cut[0] = 4.66 + 0.007*pho.Pt();
   phiso_cut[0] = 4.29 + 0.0033*pho.Pt();
   //Endcap
   hoe_cut[1] = 0.020;
   sietaieta_cut[1] = 0.0268;
   chiso_cut[1] = 0.82;
   nhiso_cut[1] = 14.65 + 0.0129*pho.Pt();
   phiso_cut[1] = 4.06 + 0.0108*pho.Pt();
  case 2:
   //Barrel
   hoe_cut[0] = 0.019;
   sietaieta_cut[0] = 0.0099;
   chiso_cut[0] = 1.61;
   nhiso_cut[0] = 3.98 + 0.007*pho.Pt();
   phiso_cut[0] = 3.01 + 0.0033*pho.Pt();
   //Endcap
   hoe_cut[1] = 0.016;
   sietaieta_cut[1] = 0.0263;
   chiso_cut[1] = 0.69;
   nhiso_cut[1] = 4.52 + 0.0129*pho.Pt();
   phiso_cut[1] = 3.61 + 0.0108*pho.Pt();
 }
 
 int isEndCap = ( fabs(pho.Eta()) > 1.479 ) ? 1 : 0;
 
 if( Pho_hadronicOverEm(i) > hoe_cut[isEndCap] ) return false;
 if( Pho_sigmaIetaIeta(i) > sietaieta_cut[isEndCap] ) return false;
 if( Pho_rhoChargedHadronIso(i) > chiso_cut[isEndCap] ) return false;
 if( Pho_rhoNeutralHadronIso(i) > nhiso_cut[isEndCap] ) return false;
 if( Pho_rhoPhotonIso(i) > phiso_cut[isEndCap] ) return false;
 // if( Pho_PassElectronVeto(i) == 0 ) return false;
 
 return true;
}
//END photon -----------
