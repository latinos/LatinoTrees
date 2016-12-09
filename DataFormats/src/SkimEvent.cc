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
  
  if (leps_.size() < 2) return false;
  
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
  apply50nsValues_ = 0;
  applyIDForJets_ = 0;
  dzCutForBtagJets_ = 999999.9;
  
  applyJetCleaning_ = 1;
  
  _maxDrSoftMuonJet = 0.3;
  _minPtSoftMuon = 5;
  
}

// reco::SkimEvent::SkimEvent(const reco::SkimEvent::hypoType &h) :
//         hypo_(h), sumPts_(0)/*, jec_(0), vtxPoint_(0,0,0) */{ }



void reco::SkimEvent::setApplyJetCleaning(int applyJetCleaning) {
  applyJetCleaning_ = applyJetCleaning;
  //---- "1" is apply jet cleaning (default in constructor)
  //---- "0" is DON'T apply jet cleaning
}


void reco::SkimEvent::setMaxEtaForJets(double value) {
  maxEtaForJets_ = value;
}
void reco::SkimEvent::setMinPtForJets(double value) {
  minPtForJets_ = value;
}
void reco::SkimEvent::setApply50nsValues(bool flag) {
  apply50nsValues_ = flag;
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

void reco::SkimEvent::setGenDiLeptFromZGstar(edm::Handle<reco::GenParticleCollection> genParticles)
{
  // Definition
  bool isMuon=false;
  const reco::Candidate* pLeptMom(0);
  const reco::Candidate* pLeptMomInit(0);
  const reco::Candidate* pDaught[4]={0};
  const reco::Candidate* pLeptState1[4]={0};
  int nLeptFromW;
  int nLeptFromZ;
  int nLeptFromQ;
  int nWFromQ;
  int IdxDaughtLept[4];
  TLorentzVector lept1_4V;
  TLorentzVector lept2_4V;
  TLorentzVector tmp4V_1;
  TLorentzVector tmp4V_2;
  float tmpInvM;
  
  muon1FromGstar.pt  = defaultvalues::defaultFloat;
  muon1FromGstar.eta = defaultvalues::defaultFloat;
  muon1FromGstar.phi = defaultvalues::defaultFloat;
  muon2FromGstar.pt  = defaultvalues::defaultFloat;
  muon2FromGstar.eta = defaultvalues::defaultFloat;
  muon2FromGstar.phi = defaultvalues::defaultFloat;
  
  elec1FromGstar.pt  = defaultvalues::defaultFloat;
  elec1FromGstar.eta = defaultvalues::defaultFloat;
  elec1FromGstar.phi = defaultvalues::defaultFloat;
  elec2FromGstar.pt  = defaultvalues::defaultFloat;
  elec2FromGstar.eta = defaultvalues::defaultFloat;
  elec2FromGstar.phi = defaultvalues::defaultFloat;
  
  genDiLeptMassZGstar = defaultvalues::defaultFloat;
  _ZGstarDiLept_DelaR = defaultvalues::defaultFloat;
  
  MomInfo.id         = defaultvalues::defaultInt;
  MomInfo.status     = defaultvalues::defaultInt;
  MomInfo.initStatus = defaultvalues::defaultInt;
  MomInfo.nDaughters = defaultvalues::defaultInt;
  MomInfo.mass       = defaultvalues::defaultFloat;
  
  for (reco::GenParticleCollection::const_iterator genPart = genParticles->begin(); genPart != genParticles->end(); genPart++){
    // Initialization
    pLeptMom=0;
    isMuon = false;
    MomInfo.id         = defaultvalues::defaultInt;
    MomInfo.status     = defaultvalues::defaultInt;
    MomInfo.initStatus = defaultvalues::defaultInt;
    MomInfo.nDaughters = defaultvalues::defaultInt;
    MomInfo.mass       = defaultvalues::defaultFloat;
    
    // Check Mom of Muon
    int id = abs(genPart->pdgId());
    if (id == 11 && genPart->status()==1) { //---- electron
      if(genPart->numberOfMothers() < 1) continue;
      pLeptMom = genPart->mother();
      while(abs(pLeptMom->pdgId()) == 11){
        if(pLeptMom->numberOfMothers() < 1) break;
        pLeptMom     = pLeptMom->mother();
        //pLeptMomCopy = pLeptMom;
      };
      isMuon = false;
      MomInfo.id = pLeptMom->pdgId();
      MomInfo.status = pLeptMom->status();
      MomInfo.initStatus = pLeptMom->status();
      MomInfo.nDaughters = pLeptMom->numberOfDaughters();
      if(pLeptMom->numberOfMothers() >= 1){
        pLeptMomInit = pLeptMom->mother();
        while(pLeptMomInit->pdgId() == MomInfo.id){
          MomInfo.initStatus = pLeptMomInit->status();
          if(pLeptMomInit->numberOfMothers() < 1) break;
          pLeptMomInit = pLeptMomInit->mother();
        }
      }
      //while(pLeptMomCopy->pdgId() == MomInfo.id){
      //  MomInfo.initStatus = pLeptMomCopy->status();
      //  if(pLeptMomCopy->numberOfMothers() < 1) break;
      //  pLeptMomCopy = pLeptMomCopy->mother();
      //}
      //if(_debug) std::cout<<"muon mother pid "<<MomInfo.id<<"\t"<<"nDaughers: "<<MomInfo.nDaughters<<std::endl;
    }else if (id == 13 && genPart->status()==1) { //---- mu
      if(genPart->numberOfMothers() < 1) continue;
      pLeptMom = genPart->mother();
      while(abs(pLeptMom->pdgId()) == 13){
        if(pLeptMom->numberOfMothers() < 1) break;
        pLeptMom     = pLeptMom->mother();
        //pLeptMomCopy = pLeptMom;
      };
      isMuon=true;
      MomInfo.id = pLeptMom->pdgId();
      MomInfo.status = pLeptMom->status();
      MomInfo.initStatus = pLeptMom->status();
      MomInfo.nDaughters = pLeptMom->numberOfDaughters();
      if(pLeptMom->numberOfMothers() >= 1){
        pLeptMomInit = pLeptMom->mother();
        while(pLeptMomInit->pdgId() == MomInfo.id){
          MomInfo.initStatus = pLeptMomInit->status();
          if(pLeptMomInit->numberOfMothers() < 1) break;
          pLeptMomInit = pLeptMomInit->mother();
        }
      }
      
      //while(pLeptMomCopy->pdgId() == MomInfo.id){
      //  MomInfo.initStatus = pLeptMomCopy->status();
      //  if(pLeptMomCopy->numberOfMothers() < 1) break;
      //  pLeptMomCopy = pLeptMomCopy->mother();
      //}
      //if(_debug) std::cout<<"muon mother pid "<<MomInfo.id<<"\t"<<"nDaughers: "<<MomInfo.nDaughters<<std::endl;
    }else continue;
    
    //Muon Mom is a W boson case
    if(abs(MomInfo.id) == 24){
      if(MomInfo.nDaughters != 4) continue;
      nLeptFromW=0;
      for( int i(0); i<4;i++)
      {
        pDaught[i] = pLeptMom->daughter(i);
        //if(_debug) std::cout<<"W boson daughter "<<i<<"\t"<<"id is "<<pDaught[i]->pdgId()<<std::endl;
        if(isMuon)if(abs(pDaught[i]->pdgId()) == 13)
        {
          IdxDaughtLept[nLeptFromW] = i;
          nLeptFromW++;
        }
        if(!isMuon)if(abs(pDaught[i]->pdgId()) == 11)
        {
          IdxDaughtLept[nLeptFromW] = i;
          nLeptFromW++;
        }
        
      }
      if(nLeptFromW == 2)
      {
        //if(_debug){
        //  std::cout<<"nLeptFromW is 2"<<std::endl;
        //  std::cout<<pDaught[IdxDaughtLept[0]]->pdgId()<<std::endl;
        //  std::cout<<pDaught[IdxDaughtLept[1]]->pdgId()<<std::endl;
        //}
        if(pDaught[IdxDaughtLept[0]]->pdgId() * pDaught[IdxDaughtLept[1]]->pdgId() > 0) continue;
        //Looking for stat 1 daughter
        pLeptState1[0] = pDaught[IdxDaughtLept[0]];
        pLeptState1[1] = pDaught[IdxDaughtLept[1]];
        //std::cout<<pLeptState1[0]->pdgId()<<std::endl;
        //std::cout<<pLeptState1[1]->pdgId()<<std::endl;
        
        while(pLeptState1[0]->status() != 1)
        {
          if(pLeptState1[0]->numberOfDaughters() <= 0)
          {
            return;
          }
          for(unsigned int i(0);i<pLeptState1[0]->numberOfDaughters();i++)
          {
            if(isMuon)if( abs(pLeptState1[0]->daughter(i)->pdgId()) == 13)
            {
              pLeptState1[0] = pLeptState1[0]->daughter(i);
              break;
            }
            if(!isMuon)if( abs(pLeptState1[0]->daughter(i)->pdgId()) == 11)
            {
              pLeptState1[0] = pLeptState1[0]->daughter(i);
              break;
            }
          }
        }
        while(pLeptState1[1]->status() != 1)
        {
          if(pLeptState1[1]->numberOfDaughters() <= 0)
          {
            return;
          }
          for(unsigned int i(0);i<pLeptState1[1]->numberOfDaughters();i++)
          {
            if(isMuon)if( abs(pLeptState1[1]->daughter(i)->pdgId()) == 13)
            {
              pLeptState1[1]= pLeptState1[1]->daughter(i);
              break;
            }
            if(!isMuon)if( abs(pLeptState1[1]->daughter(i)->pdgId()) == 11)
            {
              pLeptState1[1]= pLeptState1[1]->daughter(i);
              break;
            }
          }
        }
        
        //if(_debug){
        //  std::cout<<"Checking two staus 1 muons from a W"<<std::endl;
        //  std::cout<<"id: "<<pLeptState1[0]->pdgId()<<"\t"<<"status: "<<pLeptState1[0]->status()<<"\t"<<"pt: "<<pLeptState1[0]->pt()<<std::endl;
        //  std::cout<<"id: "<<pLeptState1[1]->pdgId()<<"\t"<<"status: "<<pLeptState1[1]->status()<<"\t"<<"pt: "<<pLeptState1[1]->pt()<<std::endl;
        //}
        
        if(isMuon){
          muon1FromGstar.pt  = pLeptState1[0]->pt();
          muon1FromGstar.eta = pLeptState1[0]->eta();
          muon1FromGstar.phi = pLeptState1[0]->phi();
          
          muon2FromGstar.pt  = pLeptState1[1]->pt();
          muon2FromGstar.eta = pLeptState1[1]->eta();
          muon2FromGstar.phi = pLeptState1[1]->phi();
          lept1_4V.SetPtEtaPhiM(pLeptState1[0]->pt(),
                                pLeptState1[0]->eta(),
                                pLeptState1[0]->phi(),
                                0.106);
          lept2_4V.SetPtEtaPhiM(pLeptState1[1]->pt(),
                                pLeptState1[1]->eta(),
                                pLeptState1[1]->phi(),
                                0.106);
        }else{
          elec1FromGstar.pt  = pLeptState1[0]->pt();
          elec1FromGstar.eta = pLeptState1[0]->eta();
          elec1FromGstar.phi = pLeptState1[0]->phi();
          
          elec2FromGstar.pt  = pLeptState1[1]->pt();
          elec2FromGstar.eta = pLeptState1[1]->eta();
          elec2FromGstar.phi = pLeptState1[1]->phi();
          
          lept1_4V.SetPtEtaPhiM(pLeptState1[0]->pt(),
                                pLeptState1[0]->eta(),
                                pLeptState1[0]->phi(),
                                0.0005);
          lept2_4V.SetPtEtaPhiM(pLeptState1[1]->pt(),
                                pLeptState1[1]->eta(),
                                pLeptState1[1]->phi(),
                                0.0005);
        }
        
        _ZGstarDiLept_DelaR=
        sqrt(
          reco::deltaPhi(pLeptState1[0]->phi(),pLeptState1[1]->phi()) * reco::deltaPhi(pLeptState1[0]->phi(),pLeptState1[1]->phi())
          +
          (pLeptState1[0]->eta() - pLeptState1[1]->eta()) * (pLeptState1[0]->eta() - pLeptState1[1]->eta())
        );
        genDiLeptMassZGstar = (lept1_4V + lept2_4V).M();
        
        
        
        return;
        
      }else if( nLeptFromW == 3)
      {
        //if(_debug){
        //  std::cout<<"nLeptFromW is 3"<<std::endl;
        //  std::cout<<pDaught[IdxDaughtLept[0]]->pdgId()<<std::endl;
        //  std::cout<<pDaught[IdxDaughtLept[1]]->pdgId()<<std::endl;
        //  std::cout<<pDaught[IdxDaughtLept[2]]->pdgId()<<std::endl;
        //}
        
        //Looking for stat 1 daughter
        pLeptState1[0] = pDaught[IdxDaughtLept[0]];
        pLeptState1[1] = pDaught[IdxDaughtLept[1]];
        pLeptState1[2] = pDaught[IdxDaughtLept[2]];
        //std::cout<<pLeptState1[0]->pdgId()<<std::endl;
        //std::cout<<pLeptState1[1]->pdgId()<<std::endl;
        while(pLeptState1[0]->status() != 1)
        {
          if(pLeptState1[0]->numberOfDaughters() <= 0)
          {
            return;
          }
          for(unsigned int i(0);i<pLeptState1[0]->numberOfDaughters();i++)
          {
            if(isMuon)if( abs(pLeptState1[0]->daughter(i)->pdgId()) == 13)
            {
              pLeptState1[0] = pLeptState1[0]->daughter(i);
              break;
            }
            if(!isMuon)if( abs(pLeptState1[0]->daughter(i)->pdgId()) == 11)
            {
              pLeptState1[0] = pLeptState1[0]->daughter(i);
              break;
            }
          }
        }
        while(pLeptState1[1]->status() != 1)
        {
          if(pLeptState1[1]->numberOfDaughters() <= 0)
          {
            return;
          }
          for(unsigned int i(0);i<pLeptState1[1]->numberOfDaughters();i++)
          {
            if(isMuon)if( abs(pLeptState1[1]->daughter(i)->pdgId()) == 13)
            {
              pLeptState1[1]= pLeptState1[1]->daughter(i);
              break;
            }
            if(!isMuon)if( abs(pLeptState1[1]->daughter(i)->pdgId()) == 11)
            {
              pLeptState1[1]= pLeptState1[1]->daughter(i);
              break;
            }
          }
        }
        while(pLeptState1[2]->status() != 1)
        {
          if(pLeptState1[2]->numberOfDaughters() <= 0)
          {
            return;
          }
          for(unsigned int i(0);i<pLeptState1[2]->numberOfDaughters();i++)
          {
            if(isMuon)if( abs(pLeptState1[2]->daughter(i)->pdgId()) == 13)
            {
              pLeptState1[2]= pLeptState1[2]->daughter(i);
              break;
            }
            if(!isMuon)if( abs(pLeptState1[2]->daughter(i)->pdgId()) == 11)
            {
              pLeptState1[2]= pLeptState1[2]->daughter(i);
              break;
            }
          }
        }
        //if(_debug){
        //  std::cout<<"Checking three staus 1 muons from a W"<<std::endl;
        //  std::cout<<"id: "<<pLeptState1[0]->pdgId()<<"\t"<<"status: "<<pLeptState1[0]->status()<<"\t"<<"pt: "<<pLeptState1[0]->pt()<<std::endl;
        //  std::cout<<"id: "<<pLeptState1[1]->pdgId()<<"\t"<<"status: "<<pLeptState1[1]->status()<<"\t"<<"pt: "<<pLeptState1[1]->pt()<<std::endl;
        //  std::cout<<"id: "<<pLeptState1[2]->pdgId()<<"\t"<<"status: "<<pLeptState1[2]->status()<<"\t"<<"pt: "<<pLeptState1[2]->pt()<<std::endl;
        //}
        
        genDiLeptMassZGstar = 100000000.0;
        for(int i(0); i<3; i++)
        {
          for(int j(0); j<3; j++)
          {
            if(i >= j) continue;
            if(pLeptState1[i]->pdgId()*pLeptState1[j]->pdgId() > 0)continue;
            if(isMuon){
              tmp4V_1.SetPtEtaPhiM(pLeptState1[i]->pt(),
                                   pLeptState1[i]->eta(),
                                   pLeptState1[i]->phi(),
                                   0.106);
              tmp4V_2.SetPtEtaPhiM(pLeptState1[j]->pt(),
                                   pLeptState1[j]->eta(),
                                   pLeptState1[j]->phi(),
                                   0.106);
            }else{
              tmp4V_1.SetPtEtaPhiM(pLeptState1[i]->pt(),
                                   pLeptState1[i]->eta(),
                                   pLeptState1[i]->phi(),
                                   0.0005);
              tmp4V_2.SetPtEtaPhiM(pLeptState1[j]->pt(),
                                   pLeptState1[j]->eta(),
                                   pLeptState1[j]->phi(),
                                   0.0005);
            }
            
            tmpInvM = (tmp4V_1 + tmp4V_2).M();
            if(tmpInvM < genDiLeptMassZGstar )
            {
              genDiLeptMassZGstar = tmpInvM;
              
              if(isMuon){
                muon1FromGstar.pt  = pLeptState1[i]->pt();
                muon1FromGstar.eta = pLeptState1[i]->eta();
                muon1FromGstar.phi = pLeptState1[i]->phi();
                
                muon2FromGstar.pt  = pLeptState1[j]->pt();
                muon2FromGstar.eta = pLeptState1[j]->eta();
                muon2FromGstar.phi = pLeptState1[j]->phi();
              }else{
                elec1FromGstar.pt  = pLeptState1[i]->pt();
                elec1FromGstar.eta = pLeptState1[i]->eta();
                elec1FromGstar.phi = pLeptState1[i]->phi();
                
                elec2FromGstar.pt  = pLeptState1[j]->pt();
                elec2FromGstar.eta = pLeptState1[j]->eta();
                elec2FromGstar.phi = pLeptState1[j]->phi();
              }
              
              _ZGstarDiLept_DelaR=
              sqrt(
                reco::deltaPhi(pLeptState1[i]->phi(),pLeptState1[j]->phi()) * reco::deltaPhi(pLeptState1[i]->phi(),pLeptState1[j]->phi())
                +
                (pLeptState1[i]->eta() - pLeptState1[j]->eta())             * (pLeptState1[i]->eta() - pLeptState1[j]->eta())
              );
              
              //lept1_4V = tmp4V_1;
              //lept2_4V = tmp4V_2;
            }
          }
        }
        //if(_debug)
        //{
        //  tmpInvM = (lept1_4V + lept2_4V).M();
        //  std::cout<<"3 Muon from W case: InvMas from final muon candidates is "<<tmpInvM<<"\t"<<"InvMass from roof: "<<genDiLeptMassZGstar<<std::endl;
        //}
        
        return;
      }else continue;
    }
    else if(abs(MomInfo.id) == 23 || abs(MomInfo.id) == 22){// Z0 or Gamma
      if(MomInfo.nDaughters != 2) continue;
      nLeptFromZ=0;
      for( int i(0); i<2;i++)
      {
        pDaught[i] = pLeptMom->daughter(i);
        //if(_debug) std::cout<<"Z boson daughter "<<i<<"\t"<<"id is "<<pDaught[i]->pdgId()<<std::endl;
        if(isMuon)if(abs(pDaught[i]->pdgId()) == 13)
        {
          //IdxDaughtLept[nLeptFromW] = i;
          nLeptFromZ++;
        }
        if(!isMuon)if(abs(pDaught[i]->pdgId()) == 11)
        {
          //IdxDaughtLept[nLeptFromW] = i;
          nLeptFromZ++;
        }
        
      }
      if(nLeptFromZ == 2)
      {
        if(pDaught[0]->pdgId() * pDaught[1]->pdgId() > 0) continue;
        //Looking for stat 1 daughter
        pLeptState1[0] = pDaught[0];
        pLeptState1[1] = pDaught[1];
        //std::cout<<pLeptState1[0]->pdgId()<<std::endl;
        //std::cout<<pLeptState1[1]->pdgId()<<std::endl;
        
        while(pLeptState1[0]->status() != 1)
        {
          if(pLeptState1[0]->numberOfDaughters() <= 0)
          {
            return;
          }
          for(unsigned int i(0);i<pLeptState1[0]->numberOfDaughters();i++)
          {
            if(isMuon)if( abs(pLeptState1[0]->daughter(i)->pdgId()) == 13)
            {
              pLeptState1[0] = pLeptState1[0]->daughter(i);
              break;
            }
            if(!isMuon)if( abs(pLeptState1[0]->daughter(i)->pdgId()) == 11)
            {
              pLeptState1[0] = pLeptState1[0]->daughter(i);
              break;
            }
          }
        }
        while(pLeptState1[1]->status() != 1)
        {
          if(pLeptState1[1]->numberOfDaughters() <= 0)
          {
            return;
          }
          for(unsigned int i(0);i<pLeptState1[1]->numberOfDaughters();i++)
          {
            if(isMuon)if( abs(pLeptState1[1]->daughter(i)->pdgId()) == 13)
            {
              pLeptState1[1]= pLeptState1[1]->daughter(i);
              break;
            }
            if(!isMuon)if( abs(pLeptState1[1]->daughter(i)->pdgId()) == 11)
            {
              pLeptState1[1]= pLeptState1[1]->daughter(i);
              break;
            }
          }
        }
        
        //if(_debug){
        //  std::cout<<"Checking two staus 1 muons from a Z"<<std::endl;
        //  std::cout<<"id: "<<pLeptState1[0]->pdgId()<<"\t"<<"status: "<<pLeptState1[0]->status()<<"\t"<<"pt: "<<pLeptState1[0]->pt()<<std::endl;
        //  std::cout<<"id: "<<pLeptState1[1]->pdgId()<<"\t"<<"status: "<<pLeptState1[1]->status()<<"\t"<<"pt: "<<pLeptState1[1]->pt()<<std::endl;
        //}
        
        if(isMuon)
        {
          muon1FromGstar.pt  = pLeptState1[0]->pt();
          muon1FromGstar.eta = pLeptState1[0]->eta();
          muon1FromGstar.phi = pLeptState1[0]->phi();
          
          muon2FromGstar.pt  = pLeptState1[1]->pt();
          muon2FromGstar.eta = pLeptState1[1]->eta();
          muon2FromGstar.phi = pLeptState1[1]->phi();
          
          lept1_4V.SetPtEtaPhiM(pLeptState1[0]->pt(),
                                pLeptState1[0]->eta(),
                                pLeptState1[0]->phi(),
                                0.106);
          lept2_4V.SetPtEtaPhiM(pLeptState1[1]->pt(),
                                pLeptState1[1]->eta(),
                                pLeptState1[1]->phi(),
                                0.106);
        }else{
          elec1FromGstar.pt  = pLeptState1[0]->pt();
          elec1FromGstar.eta = pLeptState1[0]->eta();
          elec1FromGstar.phi = pLeptState1[0]->phi();
          
          elec2FromGstar.pt  = pLeptState1[1]->pt();
          elec2FromGstar.eta = pLeptState1[1]->eta();
          elec2FromGstar.phi = pLeptState1[1]->phi();
          
          lept1_4V.SetPtEtaPhiM(pLeptState1[0]->pt(),
                                pLeptState1[0]->eta(),
                                pLeptState1[0]->phi(),
                                0.0005);
          lept2_4V.SetPtEtaPhiM(pLeptState1[1]->pt(),
                                pLeptState1[1]->eta(),
                                pLeptState1[1]->phi(),
                                0.0005);
        }
        
        genDiLeptMassZGstar = (lept1_4V + lept2_4V).M();
        
        _ZGstarDiLept_DelaR=
        sqrt(
          reco::deltaPhi(pLeptState1[0]->phi(),pLeptState1[1]->phi()) * reco::deltaPhi(pLeptState1[0]->phi(),pLeptState1[1]->phi())
          +
          (pLeptState1[0]->eta() - pLeptState1[1]->eta()) * (pLeptState1[0]->eta() - pLeptState1[1]->eta())
        );
        
        //if(_debug)
        //{
        //  std::cout<<"2 Muon from Z case: InvMas from Status1 muon candidates is "<<genDiLeptMassZGstar<<std::endl;
        //}
        
        return;
        
      }else continue; // nMuon from Z0 should be 2
    }else if( (abs(MomInfo.id) <= 6 && abs(MomInfo.id) >= 1) || abs(MomInfo.id)== 21)
    {// Quark or gluon
      nLeptFromQ = 0;
      nWFromQ = 0;
      for( int i(0); i<MomInfo.nDaughters ; i++)
      {
        if(abs(pLeptMom->daughter(i)->pdgId()) == 24) nWFromQ++;
        if(isMuon)if(abs(pLeptMom->daughter(i)->pdgId()) == 13)
        {
          pDaught[nLeptFromQ] = pLeptMom->daughter(i);
          nLeptFromQ++;
        }
        if(!isMuon)if(abs(pLeptMom->daughter(i)->pdgId()) == 11)
        {
          pDaught[nLeptFromQ] = pLeptMom->daughter(i);
          nLeptFromQ++;
        }
      }
      //if(_debug){
      //  std::cout<<"nLeptFromQ: "<<nLeptFromQ<<"\t"<<"nWFromQ: "<<nWFromQ<<std::endl;
      //}
      if(nLeptFromQ == 2 && nWFromQ == 1)
      {
        //if(_debug){
        //  std::cout<<"nLeptFromQ is 2"<<std::endl;
        //  std::cout<<pDaught[0]->pdgId()<<std::endl;
        //  std::cout<<pDaught[1]->pdgId()<<std::endl;
        //}
        if(pDaught[0]->pdgId() * pDaught[1]->pdgId() > 0) continue;
        //Looking for stat 1 daughter
        pLeptState1[0] = pDaught[0];
        pLeptState1[1] = pDaught[1];
        //std::cout<<pLeptState1[0]->pdgId()<<std::endl;
        //std::cout<<pLeptState1[1]->pdgId()<<std::endl;
        
        while(pLeptState1[0]->status() != 1)
        {
          if(pLeptState1[0]->numberOfDaughters() <= 0)
          {
            return;
          }
          for(unsigned int i(0);i<pLeptState1[0]->numberOfDaughters();i++)
          {
            if(isMuon)if( abs(pLeptState1[0]->daughter(i)->pdgId()) == 13)
            {
              pLeptState1[0] = pLeptState1[0]->daughter(i);
              break;
            }
            if(!isMuon)if( abs(pLeptState1[0]->daughter(i)->pdgId()) == 11)
            {
              pLeptState1[0] = pLeptState1[0]->daughter(i);
              break;
            }
          }
        }
        while(pLeptState1[1]->status() != 1)
        {
          if(pLeptState1[1]->numberOfDaughters() <= 0)
          {
            return;
          }
          for(unsigned int i(0);i<pLeptState1[1]->numberOfDaughters();i++)
          {
            if(isMuon)if(abs(pLeptState1[1]->daughter(i)->pdgId()) == 13)
            {
              pLeptState1[1]= pLeptState1[1]->daughter(i);
              break;
            }
            if(!isMuon)if(abs(pLeptState1[1]->daughter(i)->pdgId()) == 11)
            {
              pLeptState1[1]= pLeptState1[1]->daughter(i);
              break;
            }
          }
        }
        
        //if(_debug){
        //  std::cout<<"Checking two staus 1 muons from a Q"<<std::endl;
        //  std::cout<<"id: "<<pLeptState1[0]->pdgId()<<"\t"<<"status: "<<pLeptState1[0]->status()<<"\t"<<"pt: "<<pLeptState1[0]->pt()<<std::endl;
        //  std::cout<<"id: "<<pLeptState1[1]->pdgId()<<"\t"<<"status: "<<pLeptState1[1]->status()<<"\t"<<"pt: "<<pLeptState1[1]->pt()<<std::endl;
        //}
        
        if(isMuon)
        {
          muon1FromGstar.pt  = pLeptState1[0]->pt();
          muon1FromGstar.eta = pLeptState1[0]->eta();
          muon1FromGstar.phi = pLeptState1[0]->phi();
          
          muon2FromGstar.pt  = pLeptState1[1]->pt();
          muon2FromGstar.eta = pLeptState1[1]->eta();
          muon2FromGstar.phi = pLeptState1[1]->phi();
          
          lept1_4V.SetPtEtaPhiM(pLeptState1[0]->pt(),
                                pLeptState1[0]->eta(),
                                pLeptState1[0]->phi(),
                                0.106);
          lept2_4V.SetPtEtaPhiM(pLeptState1[1]->pt(),
                                pLeptState1[1]->eta(),
                                pLeptState1[1]->phi(),
                                0.106);
        }else{
          elec1FromGstar.pt  = pLeptState1[0]->pt();
          elec1FromGstar.eta = pLeptState1[0]->eta();
          elec1FromGstar.phi = pLeptState1[0]->phi();
          
          elec2FromGstar.pt  = pLeptState1[1]->pt();
          elec2FromGstar.eta = pLeptState1[1]->eta();
          elec2FromGstar.phi = pLeptState1[1]->phi();
          
          lept1_4V.SetPtEtaPhiM(pLeptState1[0]->pt(),
                                pLeptState1[0]->eta(),
                                pLeptState1[0]->phi(),
                                0.0005);
          lept2_4V.SetPtEtaPhiM(pLeptState1[1]->pt(),
                                pLeptState1[1]->eta(),
                                pLeptState1[1]->phi(),
                                0.0005);
        }
        
        genDiLeptMassZGstar = (lept1_4V + lept2_4V).M();
        
        _ZGstarDiLept_DelaR=
        sqrt(
          reco::deltaPhi(pLeptState1[0]->phi(),pLeptState1[1]->phi()) * reco::deltaPhi(pLeptState1[0]->phi(),pLeptState1[1]->phi())
          +
          (pLeptState1[0]->eta() - pLeptState1[1]->eta()) * (pLeptState1[0]->eta() - pLeptState1[1]->eta())
        );
        
        
        //if(_debug)
        //{
        //  std::cout<<"InvM of dimuon from Q: "<<genDiLeptMassZGstar<<std::endl;
        //}
        
        return; // Strop here to aboid doube checking from two muons
        
      }else continue;
      
    }else continue;
  }
  return;
}

void reco::SkimEvent::setGenLeptonIndices() {
  leptonIndices.clear();
  std::vector<std::pair<size_t, float>> genLeptonPair;
  
  const reco::Candidate* mcH = 0;
  for (size_t gp=0; gp<genParticles_.size(); ++gp) {
    int type = abs(genParticles_[gp]->pdgId());
    if( !((type == 11 || type == 13) && genParticles_[gp]->status()==1 ) && !(type == 15 && genParticles_[gp]->isPromptDecayed() ) )
      continue;
    
    mcH = &(*(genParticles_[gp]));
    genLeptonPair.push_back(std::make_pair(gp, mcH->pt()));
  }
  
  if (genLeptonPair.size () > 0) {
    std::sort(genLeptonPair.begin(), genLeptonPair.end(), [](std::pair<size_t, float> const& a, std::pair<size_t, float> const& b) { return a.second > b.second;});
    
    for( const auto& iPair : genLeptonPair )
      leptonIndices.push_back(iPair.first);
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


const float reco::SkimEvent::LHEMCweightID(int i) const {
  
  if (i == -1) return LHEInfoHandle_->originalXWGTUP();
  
  int num_whichWeight = LHEInfoHandle_->weights().size();
  if (i<num_whichWeight) {
    return std::stod(LHEInfoHandle_->weights()[i].id);
  }
  else {
    return LHEInfoHandle_->originalXWGTUP();
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
void reco::SkimEvent::setPFCollection(const edm::Handle<pat::PackedCandidateCollection> & h) {
  
  for(size_t i=0;i<h->size();++i)
    pfcollection_.push_back(h->at(i));
  
}


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

void reco::SkimEvent::setTrackJets(const edm::Handle<pat::JetCollection> & jH) {
  
  trackJets_.clear();
  
  for(size_t i=0;i<jH->size();++i)
    trackJets_.push_back(pat::JetRef(jH,i));
  
  //sortTagJetsByPt();
  
}

void reco::SkimEvent::setTCMet(const edm::Handle<reco::METCollection> & mH) {
  tcMet_ = reco::METRef(mH,0);
}

void reco::SkimEvent::setPFMetNoHf(const edm::Handle< std::vector<pat::MET> > & mH) {
  pfMetNoHf_ = pat::METRef(mH,0);
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

void reco::SkimEvent::set_XYshift_counts(
  int hEtaPlus_counts, int hEtaMinus_counts, int h0Barrel_counts,
  int h0EndcapPlus_counts, int h0EndcapMinus_counts, int gammaBarrel_counts,
  int gammaEndcapPlus_counts, int gammaEndcapMinus_counts, int hHFPlus_counts,
  int hHFMinus_counts, int egammaHFPlus_counts, int egammaHFMinus_counts )
{
  hEtaPlus_counts_ = hEtaPlus_counts; hEtaMinus_counts_ = hEtaMinus_counts; h0Barrel_counts_ = h0Barrel_counts;
  h0EndcapPlus_counts_ = h0EndcapPlus_counts; h0EndcapMinus_counts_ = h0EndcapMinus_counts; gammaBarrel_counts_ = gammaBarrel_counts;
  gammaEndcapPlus_counts_ = gammaEndcapPlus_counts; gammaEndcapMinus_counts_ = gammaEndcapMinus_counts; hHFPlus_counts_ = hHFPlus_counts;
  hHFMinus_counts_ = hHFMinus_counts; egammaHFPlus_counts_ = egammaHFPlus_counts; egammaHFMinus_counts_ = egammaHFMinus_counts;
}
void reco::SkimEvent::set_XYshift_sumPt(
  double hEtaPlus_sumPt, double hEtaMinus_sumPt, double h0Barrel_sumPt,
  double h0EndcapPlus_sumPt, double h0EndcapMinus_sumPt, double gammaBarrel_sumPt,
  double gammaEndcapPlus_sumPt, double gammaEndcapMinus_sumPt, double hHFPlus_sumPt,
  double hHFMinus_sumPt, double egammaHFPlus_sumPt, double egammaHFMinus_sumPt )
{
  hEtaPlus_sumPt_ = hEtaPlus_sumPt; hEtaMinus_sumPt_ = hEtaMinus_sumPt; h0Barrel_sumPt_ = h0Barrel_sumPt;
  h0EndcapPlus_sumPt_ = h0EndcapPlus_sumPt; h0EndcapMinus_sumPt_ = h0EndcapMinus_sumPt; gammaBarrel_sumPt_ = gammaBarrel_sumPt;
  gammaEndcapPlus_sumPt_ = gammaEndcapPlus_sumPt; gammaEndcapMinus_sumPt_ = gammaEndcapMinus_sumPt; hHFPlus_sumPt_ = hHFPlus_sumPt;
  hHFMinus_sumPt_ = hHFMinus_sumPt; egammaHFPlus_sumPt_ = egammaHFPlus_sumPt; egammaHFMinus_sumPt_ = egammaHFMinus_sumPt;
}

const int reco::SkimEvent::hEtaPlus_counts() const {
  return hEtaPlus_counts_;
}
const int reco::SkimEvent::hEtaMinus_counts() const {
  return hEtaMinus_counts_;
}
const int reco::SkimEvent::h0Barrel_counts() const {
  return h0Barrel_counts_;
}
const int reco::SkimEvent::h0EndcapPlus_counts() const {
  return h0EndcapPlus_counts_;
}
const int reco::SkimEvent::h0EndcapMinus_counts() const {
  return h0EndcapMinus_counts_;
}
const int reco::SkimEvent::gammaBarrel_counts() const {
  return gammaBarrel_counts_;
}
const int reco::SkimEvent::gammaEndcapPlus_counts() const {
  return gammaEndcapPlus_counts_;
}
const int reco::SkimEvent::gammaEndcapMinus_counts() const {
  return gammaEndcapMinus_counts_;
}
const int reco::SkimEvent::hHFPlus_counts() const {
  return hHFPlus_counts_;
}
const int reco::SkimEvent::hHFMinus_counts() const {
  return hHFMinus_counts_;
}
const int reco::SkimEvent::egammaHFPlus_counts() const {
  return egammaHFPlus_counts_;
}
const int reco::SkimEvent::egammaHFMinus_counts() const {
  return egammaHFMinus_counts_;
}

const double reco::SkimEvent::hEtaPlus_sumPt() const {
  return hEtaPlus_sumPt_;
}
const double reco::SkimEvent::hEtaMinus_sumPt() const {
  return hEtaMinus_sumPt_;
}
const double reco::SkimEvent::h0Barrel_sumPt() const {
  return h0Barrel_sumPt_;
}
const double reco::SkimEvent::h0EndcapPlus_sumPt() const {
  return h0EndcapPlus_sumPt_;
}
const double reco::SkimEvent::h0EndcapMinus_sumPt() const {
  return h0EndcapMinus_sumPt_;
}
const double reco::SkimEvent::gammaBarrel_sumPt() const {
  return gammaBarrel_sumPt_;
}
const double reco::SkimEvent::gammaEndcapPlus_sumPt() const {
  return gammaEndcapPlus_sumPt_;
}
const double reco::SkimEvent::gammaEndcapMinus_sumPt() const {
  return gammaEndcapMinus_sumPt_;
}
const double reco::SkimEvent::hHFPlus_sumPt() const {
  return hHFPlus_sumPt_;
}
const double reco::SkimEvent::hHFMinus_sumPt() const {
  return hHFMinus_sumPt_;
}
const double reco::SkimEvent::egammaHFPlus_sumPt() const {
  return egammaHFPlus_sumPt_;
}
const double reco::SkimEvent::egammaHFMinus_sumPt() const {
  return egammaHFMinus_sumPt_;
}

const float reco::SkimEvent::dmZllReco() const {
  float dmll = abs(defaultvalues::defaultFloat);
  unsigned int i1=0, i2=0;
  for( unsigned int iLep1=0; iLep1 < leps_.size(); ++iLep1 )
  {
      i1 = indexByPt(iLep1);
      for( unsigned int iLep2=iLep1+1; iLep2 < leps_.size(); ++iLep2 )
      {  
          i2 = indexByPt(iLep2);
          if( leps_[i2]->pt() < 10. )
              break;
            
          // check opposite charge and same flavour
          if( leps_[i1]->pdgId() == -1 * leps_[i2]->pdgId() )
          {
              float dmll_temp = abs( (leps_[i1]->p4() + leps_[i2]->p4()).mass() - 91.1876 );
              if( dmll_temp < dmll )
                  dmll = dmll_temp;
          }
      }
  }
  return dmll;
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




/** Indexing: leptons, jets, second jet collection, ...
 */


bool lowToHigh(const indexValueStruct &a, const indexValueStruct &b) {
  return a.value < b.value;
}

bool highToLow(const indexValueStruct &a, const indexValueStruct &b) {
  return a.value > b.value;
}

const size_t reco::SkimEvent::indexJetByPt(size_t i, float minPt,float eta,int applyCorrection,int applyID) const {
  
  if( i >= jets_.size() ) return 9999; //--> big number then it will fail other tests later! good! -> we will ever have 9999 jets in an event !?!?!
  //----                 NB: "9999" is used in other places, DO NOT change this number unless you propagate the changes everywhere
  std::vector<indexValueStruct> a;
  
  std::string nameIndex = "" + std::to_string(minPt) + "_" + std::to_string(eta) + "_" + std::to_string(applyCorrection) + "_" + std::to_string(applyID);
  
  //---- save information in a map ... it should save time!
  std::map<std::string, std::vector<indexValueStruct> >::const_iterator it = _index_jet.find(nameIndex);
  
  if(it != _index_jet.end()) {
    if( i < (it->second).size() ) return (it->second)[i].index;
    else  return 9999;
  }
  else {
    for(size_t j=0;j<jets_.size();++j) {
      if(!(passJetID(jets_[j],applyID)) ) continue;
      if( std::fabs(jets_[j]->eta()) >= eta) continue;
      if( jetPt(j,applyCorrection) <= minPt) continue;
      if(isThisJetALepton(jets_[j])) continue;
      a.push_back(indexValueStruct(jets_[j]->pt(),j));
    }
    
    std::sort(a.begin(),a.end(),highToLow);
    
    _index_jet.insert(std::pair<std::string, std::vector<indexValueStruct> >(nameIndex, a));
    //   _index_jet[nameIndex] = a;
    
    if ( i < a.size() ) return a[i].index;
    else                return 9999;
  }
  
}


const size_t reco::SkimEvent::indexJetByPt(size_t i, float minPt,float eta,int applyCorrection,int applyID, float dzCut) const {
  
  if( i >= jets_.size() ) return 9999; //--> big number then it will fail other tests later! good!
  std::vector<indexValueStruct> a;
  
  std::string nameIndex = "" + std::to_string(minPt) + "_" + std::to_string(eta) + "_" + std::to_string(applyCorrection) + "_" + std::to_string(applyID) + "_" + std::to_string(dzCut);
  
  //---- save information in a map ... it should save time!
  std::map<std::string, std::vector<indexValueStruct> >::const_iterator it = _index_jet.find(nameIndex);
  
  if(it != _index_jet.end()) {
    if( i < (it->second).size() ) return (it->second)[i].index;
    else  return 9999;
  }
  else {
    
    for(size_t j=0;j<jets_.size();++j) {
      if(!(passJetID(jets_[j],applyID)) ) continue;
      if( std::fabs(jets_[j]->eta()) >= eta) continue;
      if( jetPt(j,applyCorrection) <= minPt) continue;
      if(isThisJetALepton(jets_[j])) continue;
      if(jets_[j]->hasUserFloat("dz") && fabs(jets_[j]->userFloat("dz")) > dzCut) continue;
      a.push_back(indexValueStruct(jets_[j]->pt(),j));
    }
    
    std::sort(a.begin(),a.end(),highToLow);
    
    _index_jet.insert(std::pair<std::string, std::vector<indexValueStruct> >(nameIndex, a));
    
    if ( i < a.size() ) return a[i].index;
    else                return 9999;
  }
  
}




const size_t reco::SkimEvent::indexSecondJetByPt(size_t i, float minPt,float eta,int applyCorrection,int applyID) const {
  
  if( i >= secondJets_.size() ) return 9999; //--> big number then it will fail other tests later! good!
  std::vector<indexValueStruct> a;
  
  std::string nameIndex = "" + std::to_string(minPt) + "_" + std::to_string(eta) + "_" + std::to_string(applyCorrection) + "_" + std::to_string(applyID);
  
  //---- save information in a map ... it should save time!
  std::map<std::string, std::vector<indexValueStruct> >::const_iterator it = _index_secondjet.find(nameIndex);
  
  if(it != _index_secondjet.end()) {
    if( i < (it->second).size() ) return (it->second)[i].index;
    else  return 9999;
  }
  else {
    
    for(size_t j=0;j<secondJets_.size();++j) {
      //   if(!(passJetID(secondJets_[j],applyID)) ) continue;  Jet ID is broken in 76 for Puppi jets in the forward region
      if( std::fabs(secondJets_[j]->eta()) >= eta) continue;
      if( secondJetPt(j,applyCorrection) <= minPt) continue;
      if(isThisJetALepton(secondJets_[j])) continue;
      a.push_back(indexValueStruct(secondJets_[j]->pt(),j));
    }
    
    std::sort(a.begin(),a.end(),highToLow);
    
    _index_secondjet.insert(std::pair<std::string, std::vector<indexValueStruct> >(nameIndex, a));
    
    if ( i < a.size() ) return a[i].index;
    else                return 9999;
  }
  
}



const size_t reco::SkimEvent::indexByPt(size_t i) const {
  
  if( i >= leps_.size() ) return 9999; //--> big number then it will fail other tests later! good!
  std::vector<indexValueStruct> a;
  
  std::string nameIndex = "lepton";
  
  //---- save information in a map ... it should save time!
  std::map<std::string, std::vector<indexValueStruct> >::const_iterator it = _index_lep.find(nameIndex);
  
  if(it != _index_lep.end()) {
    if( i < (it->second).size() ) return (it->second)[i].index;
    else  return 9999;
  }
  else {
    
    for(size_t j=0;j<leps_.size();++j) a.push_back(indexValueStruct(pt(j),j));
    std::sort(a.begin(),a.end(),highToLow);
    
    _index_lep.insert(std::pair<std::string, std::vector<indexValueStruct> >(nameIndex, a));
    
    if ( i < a.size() ) return a[i].index;
    else                return 9999;
  }
  
}


const size_t reco::SkimEvent::indexByPtSoftMuon(size_t i) const {
  
  if( i >= softMuons_.size() ) return 9999; //--> big number then it will fail other tests later! good!
  std::vector<indexValueStruct> a;
  
  std::string nameIndex = "softMuon";
  
  //---- save information in a map ... it should save time!
  std::map<std::string, std::vector<indexValueStruct> >::const_iterator it = _index_softMuon.find(nameIndex);
  
  if(it != _index_softMuon.end()) {
    if( i < (it->second).size() ) return (it->second)[i].index;
    else  return 9999;
  }
  else {
    
    for(size_t j=0;j<softMuons_.size();++j){ 
      if( SoftMuonPt(j) < _minPtSoftMuon) continue;
      a.push_back(indexValueStruct(SoftMuonPt(j),j));
    }
    
    std::sort(a.begin(),a.end(),highToLow);
    
    _index_softMuon.insert(std::pair<std::string, std::vector<indexValueStruct> >(nameIndex, a));
    
    if ( i < a.size() ) return a[i].index;
    else                return 9999;
  }
  
}
/*
 f or(size_t j=0;j<softM*uons_.size();++j) 
 a.push_back(indexValueStruct(SoftMuonPt(j),j));
 std::sort(a.begin(),a.end(),highToLow);
 
 return a[i].index;
 */




const size_t reco::SkimEvent::indexByIso(size_t i) const {
  
  if( i >= leps_.size() ) return 9999;
  std::vector<indexValueStruct> a;
  
  for(size_t j=0;j<leps_.size();++j) a.push_back(indexValueStruct(allIso(j),j));
  std::sort(a.begin(),a.end(),lowToHigh);
  
  return a[i].index;
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
  
  unsigned int index_jet_ordered = indexJetByPt(index, minPt, eta, applyCorrection, applyID);
  if (index_jet_ordered >= jets_.size()) {
    return defaultvalues::defaultFloat;
  }
  else { 
    //---- now check for the closest muon
    float minDR = 9999999.9;
    int nMu = -1;
    for (size_t iMu=0; iMu<softMuons_.size(); iMu++) {
      //---- check if it is really a soft-muon
      //---- see https://twiki.cern.ch/twiki/bin/view/CMS/SWGuideMuonId2015
      if (muon::isSoftMuon(*(static_cast<const reco::Muon*>(softMuons_[iMu].get())), highestPtVtx())) {
        //      if (muon::isSoftMuon(static_cast<const pat::Muon*>(softMuons_[iMu].get()), highestPtVtx())) {
        float muonPt = softMuons_[iMu]->pt();
        if (muonPt >= minPtMuon) {
          double dR = fabs(ROOT::Math::VectorUtil::DeltaR(softMuons_[iMu]->p4(),jets_[index_jet_ordered]->p4()) );
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
    else return defaultvalues::defaultFloat;
  }
  
}


const float reco::SkimEvent::jetSoftMuonEtaByPt(size_t i = 0) const {
  return jetSoftMuonEta(i,_minPtSoftMuon, _maxDrSoftMuonJet, minPtForJets_, maxEtaForJets_, applyCorrectionForJets_, applyIDForJets_); 
}

const float reco::SkimEvent::jetSoftMuonEta(size_t index, float minPtMuon, float maxDrMuonJet, float minPt,float eta,int applyCorrection,int applyID) const {
  
  unsigned int index_jet_ordered = indexJetByPt(index, minPt, eta, applyCorrection, applyID);
  if (index_jet_ordered >= jets_.size()) {
    return defaultvalues::defaultFloat;
  }
  else { 
    //---- now check for the closest muon
    float minDR = 9999999.9;
    int nMu = -1;
    for (size_t iMu=0; iMu<softMuons_.size(); iMu++) {
      //---- check if it is really a soft-muon
      //---- see https://twiki.cern.ch/twiki/bin/view/CMS/SWGuideMuonId2015
      if (muon::isSoftMuon(*(static_cast<const reco::Muon*>(softMuons_[iMu].get())), highestPtVtx())) {
        //      if (muon::isSoftMuon(static_cast<const pat::Muon*>(softMuons_[iMu].get()), highestPtVtx())) {
        float muonPt = softMuons_[iMu]->pt();
        if (muonPt >= minPtMuon) {
          double dR = fabs(ROOT::Math::VectorUtil::DeltaR(softMuons_[iMu]->p4(),jets_[index_jet_ordered]->p4()) );
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
    else return defaultvalues::defaultFloat;
  }
  
}


const float reco::SkimEvent::jetSoftMuonPhiByPt(size_t i = 0) const {
  return jetSoftMuonPhi(i,_minPtSoftMuon, _maxDrSoftMuonJet, minPtForJets_, maxEtaForJets_, applyCorrectionForJets_, applyIDForJets_); 
}

const float reco::SkimEvent::jetSoftMuonPhi(size_t index, float minPtMuon, float maxDrMuonJet, float minPt,float eta,int applyCorrection,int applyID) const {
  
  unsigned int index_jet_ordered = indexJetByPt(index, minPt, eta, applyCorrection, applyID);
  if (index_jet_ordered >= jets_.size()) {
    return defaultvalues::defaultFloat;
  }
  else { 
    //---- now check for the closest muon
    float minDR = 9999999.9;
    int nMu = -1;
    for (size_t iMu=0; iMu<softMuons_.size(); iMu++) {
      //---- check if it is really a soft-muon
      //---- see https://twiki.cern.ch/twiki/bin/view/CMS/SWGuideMuonId2015
      if (muon::isSoftMuon(*(static_cast<const reco::Muon*>(softMuons_[iMu].get())), highestPtVtx())) {
        //      if (muon::isSoftMuon(static_cast<const pat::Muon*>(softMuons_[iMu].get()), highestPtVtx())) {
        float muonPt = softMuons_[iMu]->pt();
        if (muonPt >= minPtMuon) {
          double dR = fabs(ROOT::Math::VectorUtil::DeltaR(softMuons_[iMu]->p4(),jets_[index_jet_ordered]->p4()) );
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
    else return defaultvalues::defaultFloat;
  }
  
}


const float reco::SkimEvent::jetSoftMuonIsoByPt(size_t i = 0) const {
  return jetSoftMuonIso(i, _minPtSoftMuon, _maxDrSoftMuonJet, minPtForJets_, maxEtaForJets_, applyCorrectionForJets_, applyIDForJets_); 
}

const float reco::SkimEvent::jetSoftMuonIso(size_t index, float minPtMuon, float maxDrMuonJet, float minPt,float eta,int applyCorrection,int applyID) const {
  
  unsigned int index_jet_ordered = indexJetByPt(index, minPt, eta, applyCorrection, applyID);
  if (index_jet_ordered >= jets_.size()) {
    return defaultvalues::defaultFloat;
  }
  else { 
    //---- now check for the closest muon
    float minDR = 9999999.9;
    int nMu = -1;
    for (size_t iMu=0; iMu<softMuons_.size(); iMu++) {
      //---- check if it is really a soft-muon
      //---- see https://twiki.cern.ch/twiki/bin/view/CMS/SWGuideMuonId2015
      if (muon::isSoftMuon(*(static_cast<const reco::Muon*>(softMuons_[iMu].get())), highestPtVtx())) {
        //      if (muon::isSoftMuon(static_cast<const pat::Muon*>(softMuons_[iMu].get()), highestPtVtx())) {
        float muonPt = softMuons_[iMu]->pt();
        if (muonPt >= minPtMuon) {
          double dR = fabs(ROOT::Math::VectorUtil::DeltaR(softMuons_[iMu]->p4(),jets_[index_jet_ordered]->p4()) );
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
    else return defaultvalues::defaultFloat;
  }
  
}


const float reco::SkimEvent::jetSoftMuonD0ByPt(size_t i = 0) const {
  return jetSoftMuonD0(i, _minPtSoftMuon, _maxDrSoftMuonJet, minPtForJets_, maxEtaForJets_, applyCorrectionForJets_, applyIDForJets_); 
}

const float reco::SkimEvent::jetSoftMuonD0(size_t index, float minPtMuon, float maxDrMuonJet, float minPt,float eta,int applyCorrection,int applyID) const {
  
  unsigned int index_jet_ordered = indexJetByPt(index, minPt, eta, applyCorrection, applyID);
  if (index_jet_ordered >= jets_.size()) {
    return defaultvalues::defaultFloat;
  }
  else { 
    //---- now check for the closest muon
    float minDR = 9999999.9;
    int nMu = -1;
    for (size_t iMu=0; iMu<softMuons_.size(); iMu++) {
      //---- check if it is really a soft-muon
      //---- see https://twiki.cern.ch/twiki/bin/view/CMS/SWGuideMuonId2015
      if (muon::isSoftMuon(*(static_cast<const reco::Muon*>(softMuons_[iMu].get())), highestPtVtx())) {
        //      if (muon::isSoftMuon(static_cast<const pat::Muon*>(softMuons_[iMu].get()), highestPtVtx())) {
        float muonPt = softMuons_[iMu]->pt();
        if (muonPt >= minPtMuon) {
          double dR = fabs(ROOT::Math::VectorUtil::DeltaR(softMuons_[iMu]->p4(),jets_[index_jet_ordered]->p4()) );
          if(dR < maxDrMuonJet && dR < minDR) {
            minDR = dR;
            nMu = iMu;
          }
        }
      }
    }
    if (nMu != -1) {
      //---- d0
      const reco::Vertex primaryVtx = highestPtVtx();
      return (getMuon(softMuons_[nMu]))->muonBestTrack()->dxy(primaryVtx.position()); 
    }
    else return defaultvalues::defaultFloat;
  }
  
}


const float reco::SkimEvent::jetSoftMuonDzByPt(size_t i = 0) const {
  return jetSoftMuonDz(i, _minPtSoftMuon, _maxDrSoftMuonJet, minPtForJets_, maxEtaForJets_, applyCorrectionForJets_, applyIDForJets_); 
}

const float reco::SkimEvent::jetSoftMuonDz(size_t index, float minPtMuon, float maxDrMuonJet, float minPt,float eta,int applyCorrection,int applyID) const {
  
  unsigned int index_jet_ordered = indexJetByPt(index, minPt, eta, applyCorrection, applyID);
  if (index_jet_ordered >= jets_.size()) {
    return defaultvalues::defaultFloat;
  }
  else { 
    //---- now check for the closest muon
    float minDR = 9999999.9;
    int nMu = -1;
    for (size_t iMu=0; iMu<softMuons_.size(); iMu++) {
      //---- check if it is really a soft-muon
      //---- see https://twiki.cern.ch/twiki/bin/view/CMS/SWGuideMuonId2015
      if (muon::isSoftMuon(*(static_cast<const reco::Muon*>(softMuons_[iMu].get())), highestPtVtx())) {
        //      if (muon::isSoftMuon(static_cast<const pat::Muon*>(softMuons_[iMu].get()), highestPtVtx())) {
        float muonPt = softMuons_[iMu]->pt();
        if (muonPt >= minPtMuon) {
          double dR = fabs(ROOT::Math::VectorUtil::DeltaR(softMuons_[iMu]->p4(),jets_[index_jet_ordered]->p4()) );
          if(dR < maxDrMuonJet && dR < minDR) {
            minDR = dR;
            nMu = iMu;
          }
        }
      }
    }
    if (nMu != -1) {
      //---- dz
      const reco::Vertex primaryVtx = highestPtVtx();
      return (getMuon(softMuons_[nMu]))->muonBestTrack()->dz(primaryVtx.position()); 
    }
    else return defaultvalues::defaultFloat;
  }
  
}


//---- number of soft muons associated to a jet
//----     it should be at most 1!
const float reco::SkimEvent::jetSoftMuonCountingByPt(size_t i = 0) const {
  return jetSoftMuonCounting(i,_minPtSoftMuon, _maxDrSoftMuonJet, minPtForJets_, maxEtaForJets_, applyCorrectionForJets_, applyIDForJets_); 
}

const float reco::SkimEvent::jetSoftMuonCounting(size_t index, float minPtMuon, float maxDrMuonJet, float minPt,float eta,int applyCorrection,int applyID) const {
  
  unsigned int index_jet_ordered = indexJetByPt(index, minPt, eta, applyCorrection, applyID);
  if (index_jet_ordered >= jets_.size()) {
    return defaultvalues::defaultFloat;
  }
  else { 
    int numberOfMuons = 0;
    //    int nMu = -1;
    for (size_t iMu=0; iMu<softMuons_.size(); iMu++) {
      //---- check if it is really a soft-muon
      //---- see https://twiki.cern.ch/twiki/bin/view/CMS/SWGuideMuonId2015
      if (muon::isSoftMuon(*(static_cast<const reco::Muon*>(softMuons_[iMu].get())), highestPtVtx())) {
        float muonPt = softMuons_[iMu]->pt();
        if (muonPt >= minPtMuon) {
          double dR = fabs(ROOT::Math::VectorUtil::DeltaR(softMuons_[iMu]->p4(),jets_[index_jet_ordered]->p4()) );
          if (dR < maxDrMuonJet) {
            numberOfMuons++;
          }
        }
      }
    }
    return 1. * numberOfMuons;
  }
  
}


const float reco::SkimEvent::SoftMuonPt(size_t i) const {
  
  if (i >= softMuons_.size()) return defaultvalues::defaultFloat;
  if (muon::isSoftMuon(*(static_cast<const reco::Muon*>(softMuons_[i].get())), highestPtVtx())) {
    float muonPt = softMuons_[i]->pt();
    if (muonPt >= _minPtSoftMuon) {
      return muonPt;  
    }
    else 
      return defaultvalues::defaultFloat;
  }
  else 
    return defaultvalues::defaultFloat;
}


const float reco::SkimEvent::SoftMuonEta(size_t i) const {
  
  if (i >= softMuons_.size()) return defaultvalues::defaultFloat;
  if (muon::isSoftMuon(*(static_cast<const reco::Muon*>(softMuons_[i].get())), highestPtVtx())) {
    float muonPt = softMuons_[i]->pt();
    if (muonPt >= _minPtSoftMuon) {
      return softMuons_[i]->eta();  
    }
    else 
      return defaultvalues::defaultFloat;
  }
  else 
    return defaultvalues::defaultFloat;
}


const float reco::SkimEvent::SoftMuonPhi(size_t i) const {
  if (i >= softMuons_.size()) return defaultvalues::defaultFloat;
  if (muon::isSoftMuon(*(static_cast<const reco::Muon*>(softMuons_[i].get())), highestPtVtx())) {
    float muonPt = softMuons_[i]->pt();
    if (muonPt >= _minPtSoftMuon) {
      return softMuons_[i]->phi();  
    }
    else 
      return defaultvalues::defaultFloat;
  }
  else 
    return defaultvalues::defaultFloat;
}


const float reco::SkimEvent::SoftMuonIso(size_t i) const {
  if (i >= softMuons_.size()) return defaultvalues::defaultFloat;
  if (muon::isSoftMuon(*(static_cast<const reco::Muon*>(softMuons_[i].get())), highestPtVtx())) {
    float muonPt = softMuons_[i]->pt();
    if (muonPt >= _minPtSoftMuon) {
      return ((getMuon(softMuons_[i])->pfIsolationR04().sumChargedHadronPt+std::max(0.,getMuon(softMuons_[i])->pfIsolationR04().sumNeutralHadronEt+getMuon(softMuons_[i])->pfIsolationR04().sumPhotonEt-0.50*getMuon(softMuons_[i])->pfIsolationR04().sumPUPt))/getMuon(softMuons_[i])->pt());
    }
    else 
      return defaultvalues::defaultFloat;
  }
  else 
    return defaultvalues::defaultFloat;
}


const float reco::SkimEvent::SoftMuonDz(size_t i) const {
  if (i >= softMuons_.size()) return defaultvalues::defaultFloat;
  if (muon::isSoftMuon(*(static_cast<const reco::Muon*>(softMuons_[i].get())), highestPtVtx())) {
    float muonPt = softMuons_[i]->pt();
    if (muonPt >= _minPtSoftMuon) {
      const reco::Vertex primaryVtx = highestPtVtx();
      return (getMuon(softMuons_[i]))->muonBestTrack()->dz(primaryVtx.position()); 
    }
    else 
      return defaultvalues::defaultFloat;
  }
  else 
    return defaultvalues::defaultFloat;
}


const float reco::SkimEvent::SoftMuonDxy(size_t i) const {
  if (i >= softMuons_.size()) return defaultvalues::defaultFloat;
  if (muon::isSoftMuon(*(static_cast<const reco::Muon*>(softMuons_[i].get())), highestPtVtx())) {
    float muonPt = softMuons_[i]->pt();
    if (muonPt >= _minPtSoftMuon) {
      const reco::Vertex primaryVtx = highestPtVtx();
      return (getMuon(softMuons_[i]))->muonBestTrack()->dxy(primaryVtx.position()); 
    }
    else 
      return defaultvalues::defaultFloat;
  }
  else 
    return defaultvalues::defaultFloat;
}


const float reco::SkimEvent::SoftMuonIsTrackerMuon(size_t i) const {
  if (i >= softMuons_.size()) return defaultvalues::defaultFloat;
  if (muon::isSoftMuon(*(static_cast<const reco::Muon*>(softMuons_[i].get())), highestPtVtx())) {
    float muonPt = softMuons_[i]->pt();
    if (muonPt >= _minPtSoftMuon) {
      return softMuons_[i] -> isTrackerMuon();
    } 
    else return defaultvalues::defaultFloat;
  }
  else return defaultvalues::defaultFloat;
}



const float reco::SkimEvent::TMLastStationAngTight(size_t i) const {
  if (i >= softMuons_.size()) return defaultvalues::defaultFloat;
  if (muon::isSoftMuon(*(static_cast<const reco::Muon*>(softMuons_[i].get())), highestPtVtx())) {
    float muonPt = softMuons_[i]->pt();
    if (muonPt >= _minPtSoftMuon) {
      pat::Muon const * const mu = getMuon(softMuons_[i]);
      return (mu->isGood("TMLastStationAngTight"));
    } 
    else return defaultvalues::defaultFloat;
  }
  else return defaultvalues::defaultFloat;
}



const int reco::SkimEvent::flavour(size_t i) const {
  if(i < leps_.size()) {
    if (isMuon(i)) {
      return 13*q(i);
    }
    else {
      return 11*q(i);  
    }
  }
  else return -9999;
}


const float reco::SkimEvent::GetElectronEffectiveAreaByPt(size_t i = 0) const {
  return GetElectronEffectiveArea(indexByPt(i), apply50nsValues_);
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
    else                                   return 0;
  }
  else {
    if (idmu != "") {
      return getMuon(i)->userFloat(idmu);
    }
    else {
      return 1.;  //---- if no muon id is defined, then return "1" 
    }
  }
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
  if(i < leps_.size() && isElectron(i) && getElectron(i)->superCluster().isNonnull()) 
	return getElectron(i)->superCluster()->eta();
  else 
	return defaultvalues::defaultFloat;
}

const float reco::SkimEvent::etaSCSeed(size_t i) const {
  if(i < leps_.size() && isElectron(i) && getElectron(i)->superCluster()->seed().isNonnull())
	return getElectron(i)->superCluster()->seed()->eta();
  else 
	return defaultvalues::defaultFloat;
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




// Tau variables

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


const float reco::SkimEvent::leadingTauVLooseIsoMvaNew(size_t index) const {
  
  size_t count = 0;
  for(size_t i=0;i<taus_.size();++i) {
    if(++count > index) return taus_[i].tauID("byVLooseIsolationMVArun2v1DBnewDMwLT");
  }
  return -9999.9;  
}


const float reco::SkimEvent::leadingTauVLooseIsoMvaOld(size_t index) const {
  
  size_t count = 0;
  for(size_t i=0;i<taus_.size();++i) {
    if(++count > index) return taus_[i].tauID("byVLooseIsolationMVArun2v1DBoldDMwLT");
  }
  return -9999.9;
}


const float reco::SkimEvent::leadingTauLooseIsoDbeta(size_t index) const {
  
  size_t count = 0;
  for(size_t i=0;i<taus_.size();++i) {
    if(++count > index) return taus_[i].tauID("byLooseCombinedIsolationDeltaBetaCorr3Hits");
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
  
  
  if (applyJetCleaning_) {
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
  
  else if(applyID == 7) { //---- recommended Run II jet ID: LOOSE
    //---- see https://twiki.cern.ch/twiki/bin/view/CMS/JetID
    if( abs(jet->eta()) <= 2.7 ) {
      if (jet->neutralHadronEnergyFraction() >=0.99) return false;
      if (jet->neutralEmEnergyFraction() >=0.99) return false;
      
      unsigned int multiplicity = jet->chargedMultiplicity() + jet->neutralMultiplicity();
      if ( multiplicity <= 1) return false;
      //    if ( jet->muonEnergyFraction() >= 0.8) return false;
      
      if(fabs(jet->eta())<=2.4) {
        if ( jet->chargedHadronEnergyFraction() <= 0 ) return false;
        if ( jet->chargedMultiplicity() <= 0 ) return false;
        if ( jet->chargedEmEnergyFraction() >= 0.99 ) return false;
      }
    }
    else if( abs(jet->eta()) <= 3.0 ){
      if( jet->neutralEmEnergyFraction() >= 0.90 ) return false;
      if( jet->neutralMultiplicity() <= 2 ) return false;
    }
    else {
      if( jet->neutralEmEnergyFraction() >= 0.90 ) return false;
      if( jet->neutralMultiplicity() <= 10 ) return false;
    }
    
    return true;  
  }
  
  else if(applyID == 8) { //---- recommended Run II jet ID: TIGHT
    //---- see https://twiki.cern.ch/twiki/bin/view/CMS/JetID
    if( abs(jet->eta()) <= 2.7 ) {
      if (jet->neutralHadronEnergyFraction() >=0.90) return false;
      if (jet->neutralEmEnergyFraction() >=0.90) return false;
      
      unsigned int multiplicity = jet->chargedMultiplicity() + jet->neutralMultiplicity();
      if ( multiplicity <= 1) return false;
      //  if ( jet->muonEnergyFraction() >= 0.8) return false;
      
      if(fabs(jet->eta())<=2.4) {
        if ( jet->chargedHadronEnergyFraction() <= 0 ) return false;
        if ( jet->chargedMultiplicity() <= 0 ) return false;
        if ( jet->chargedEmEnergyFraction() >= 0.99 ) return false;
      }
    }
    else if( abs(jet->eta()) <= 3.0 ){
      if( jet->neutralEmEnergyFraction() >= 0.90 ) return false;
      if( jet->neutralMultiplicity() <= 2 ) return false;
    }
    else {
      if( jet->neutralEmEnergyFraction() >= 0.90 ) return false;
      if( jet->neutralMultiplicity() <= 10 ) return false;
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
  
  unsigned int index_jet_ordered = indexJetByPt(index, minPt, eta, applyCorrection, applyID);
  if (index_jet_ordered >= jets_.size()) {
    return new pat::Jet();
  }
  else {
    return jets_[index_jet_ordered].get();
  }
  
}



const float reco::SkimEvent::leadingJetPUid(size_t index, float minPt,float eta,int applyCorrection,int applyID, float dzCut) const {
  
  unsigned int index_jet_ordered = indexJetByPt(index, minPt, eta, applyCorrection, applyID, dzCut);
  if (index_jet_ordered >= jets_.size()) {
    return defaultvalues::defaultFloat;
  }
  else {
    if (jets_[index_jet_ordered]->userFloat(_name_puJetIdDiscriminant) ) return jets_[index_jet_ordered]->userFloat(_name_puJetIdDiscriminant); 
    else return defaultvalues::defaultFloat;  
  }
  
}


const float reco::SkimEvent::leadingJetPUid(size_t index) const { 
  return leadingJetPUid(index,minPtForJets_,maxEtaForJets_,applyCorrectionForJets_,applyIDForJets_);
}


const float reco::SkimEvent::leadingJetPartonFlavour(size_t index, float minPt,float eta,int applyCorrection,int applyID, float dzCut) const {
  
  unsigned int index_jet_ordered = indexJetByPt(index, minPt, eta, applyCorrection, applyID, dzCut);
  if (index_jet_ordered >= jets_.size()) {
    return defaultvalues::defaultFloat;
  }
  else {
    return jets_[index_jet_ordered]->partonFlavour();
  }
  
}


const float reco::SkimEvent::leadingJetPartonFlavour(size_t index) const { 
  return leadingJetPartonFlavour(index,minPtForJets_,maxEtaForJets_,applyCorrectionForJets_,applyIDForJets_);
}


const float reco::SkimEvent::leadingJetHadronFlavour(size_t index, float minPt,float eta,int applyCorrection,int applyID, float dzCut) const {
  
  unsigned int index_jet_ordered = indexJetByPt(index, minPt, eta, applyCorrection, applyID, dzCut);
  if (index_jet_ordered >= jets_.size()) {
    return defaultvalues::defaultFloat;
  }
  else {
    return jets_[index_jet_ordered]->hadronFlavour();
  }
  
}

const float reco::SkimEvent::leadingJetHadronFlavour(size_t index) const { 
  return leadingJetHadronFlavour(index,minPtForJets_,maxEtaForJets_,applyCorrectionForJets_,applyIDForJets_);
}



const float reco::SkimEvent::leadingJetBtag(size_t index, std::string discriminator, float minPt,float eta,int applyCorrection,int applyID, float dzCut) const {
  
  unsigned int index_jet_ordered = indexJetByPt(index, minPt, eta, applyCorrection, applyID, dzCut);
  if (index_jet_ordered >= jets_.size()) {
    return defaultvalues::defaultFloat;
  }
  else {
    return jets_[index_jet_ordered]->bDiscriminator(discriminator);
  }
  
}



const int reco::SkimEvent::leadingJetId(size_t index, float minPt,float eta,int applyCorrection,int applyID) const {
  
  unsigned int index_jet_ordered = indexJetByPt(index, minPt, eta, applyCorrection, applyID);
  if (index_jet_ordered >= jets_.size()) {
    return defaultvalues::defaultFloat;
  }
  else {
    return jets_[index_jet_ordered]->userInt("jetId");
  }
  
}

const float reco::SkimEvent::leadingJetMva(size_t index, float minPt,float eta,int applyCorrection,int applyID) const {
  
  unsigned int index_jet_ordered = indexJetByPt(index, minPt, eta, applyCorrection, applyID);
  if (index_jet_ordered >= jets_.size()) {
    return defaultvalues::defaultFloat;
  }
  else {
    return jets_[index_jet_ordered]->userFloat("jetMva");
  }
  
}


const float reco::SkimEvent::leadingSecondJetPt(size_t index, float minPt,float eta,int applyCorrection,int applyID) const {
  
  unsigned int index_jet_ordered = indexSecondJetByPt(index, minPt, eta, applyCorrection, applyID);
  if (index_jet_ordered >= jets_.size()) {
    return defaultvalues::defaultFloat;
  }
  else {
    return secondJetPt(index_jet_ordered,applyCorrection);
  }
  
}



const float reco::SkimEvent::leadingSecondJetEta(size_t index, float minPt,float eta,int applyCorrection,int applyID) const {
  
  unsigned int index_jet_ordered = indexSecondJetByPt(index, minPt, eta, applyCorrection, applyID);
  if (index_jet_ordered >= jets_.size()) {
    return defaultvalues::defaultFloat;
  }
  else {
    return secondJets_[index_jet_ordered]->eta();
  }
  
}


const float reco::SkimEvent::leadingSecondJetPhi(size_t index, float minPt,float eta,int applyCorrection,int applyID) const {
  
  unsigned int index_jet_ordered = indexSecondJetByPt(index, minPt, eta, applyCorrection, applyID);
  if (index_jet_ordered >= jets_.size()) {
    return defaultvalues::defaultFloat;
  }
  else {
    return secondJets_[index_jet_ordered]->phi();
  }
  
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
  return leadingJetPt(minPtForJets_,maxEtaForJets_,applyCorrectionForJets_,applyIDForJets_,index);
}

const float reco::SkimEvent::leadingJetEta(size_t index) const { 
  return leadingJetEta(minPtForJets_,maxEtaForJets_,applyCorrectionForJets_,applyIDForJets_,index);
}

const float reco::SkimEvent::leadingJetPhi(size_t index) const { 
  return leadingJetPhi(minPtForJets_,maxEtaForJets_,applyCorrectionForJets_,applyIDForJets_,index);
}

const float reco::SkimEvent::leadingJetMass(size_t index) const { 
  return leadingJetMass(minPtForJets_,maxEtaForJets_,applyCorrectionForJets_,applyIDForJets_,index);
}

const float reco::SkimEvent::leadingJetPtRaw(size_t index) const { 
  unsigned int index_jet_ordered = indexJetByPt(index, minPtForJets_,maxEtaForJets_,applyCorrectionForJets_,applyIDForJets_);
  if (index_jet_ordered >= jets_.size()) {
    return defaultvalues::defaultFloat;
  } else if (jets_[index_jet_ordered]->currentJECLevel()!="ERROR") {
    return jets_[index_jet_ordered]->correctedJet("Uncorrected").pt();
  } else {
    return jets_[index_jet_ordered]->pt(); //---- the jet is already "uncorrected"
  }
}

const float reco::SkimEvent::leadingJetPtL1(size_t index) const { 
  unsigned int index_jet_ordered = indexJetByPt(index, minPtForJets_,maxEtaForJets_,applyCorrectionForJets_,applyIDForJets_);
  if (index_jet_ordered >= jets_.size()) {
    return defaultvalues::defaultFloat;
  } else if (jets_[index_jet_ordered]->currentJECLevel()!="ERROR") {
    return jets_[index_jet_ordered]->correctedJet("L1FastJet").pt();
  } else {
    return -999.;
  }
}

const float reco::SkimEvent::leadingJetPtL3Absolute(size_t index) const { 
  unsigned int index_jet_ordered = indexJetByPt(index, minPtForJets_,maxEtaForJets_,applyCorrectionForJets_,applyIDForJets_);
  if (index_jet_ordered >= jets_.size()) {
    return defaultvalues::defaultFloat;
  } else if (jets_[index_jet_ordered]->currentJECLevel()!="ERROR") {
    return jets_[index_jet_ordered]->correctedJet("L3Absolute").pt();
  } else {
    return -999.;
  }
}


const float reco::SkimEvent::leadingJetPt(float minPt,float eta,int applyCorrection,int applyID, size_t index) const {
  
  unsigned int index_jet_ordered = indexJetByPt(index, minPt, eta, applyCorrection, applyID);
  if (index_jet_ordered >= jets_.size()) {
    return defaultvalues::defaultFloat;
  }
  else {
    return jetPt(index_jet_ordered,applyCorrection);
  }
  
}


const float reco::SkimEvent::leadingJetMass(float minPt,float eta,int applyCorrection,int applyID, size_t index) const {
  
  unsigned int index_jet_ordered = indexJetByPt(index, minPt, eta, applyCorrection, applyID);
  if (index_jet_ordered >= jets_.size()) {
    return defaultvalues::defaultFloat;
  }
  else {
    return jets_[index_jet_ordered]->mass();
  }
  
}


const float reco::SkimEvent::leadingJetPhi(float minPt,float eta,int applyCorrection,int applyID, size_t index) const {
  
  unsigned int index_jet_ordered = indexJetByPt(index, minPt, eta, applyCorrection, applyID);
  if (index_jet_ordered >= jets_.size()) {
    return defaultvalues::defaultFloat;
  }
  else {
    return jets_[index_jet_ordered]->phi();
  }
  
}

const float reco::SkimEvent::leadingJetEta(float minPt,float eta,int applyCorrection,int applyID, size_t index) const {
  
  unsigned int index_jet_ordered = indexJetByPt(index, minPt, eta, applyCorrection, applyID);
  if (index_jet_ordered >= jets_.size()) {
    return defaultvalues::defaultFloat;
  }
  else {
    return jets_[index_jet_ordered]->eta();
  }
  
}


const float reco::SkimEvent::leadingJetPtd(size_t index, float minPt,float eta,int applyCorrection,int applyID) const {
  
  unsigned int index_jet_ordered = indexJetByPt(index, minPt, eta, applyCorrection, applyID);
  if (index_jet_ordered >= jets_.size()) {
    return defaultvalues::defaultFloat;
  }
  else {
    return jets_[index_jet_ordered]->userFloat("ptd");
  }
  
}



const float reco::SkimEvent::leadingJetNChgQC(size_t index, float minPt,float eta,int applyCorrection,int applyID) const {
  
  unsigned int index_jet_ordered = indexJetByPt(index, minPt, eta, applyCorrection, applyID);
  if (index_jet_ordered >= jets_.size()) {
    return defaultvalues::defaultFloat;
  }
  else {
    return jets_[index_jet_ordered]->userFloat("nChgQC");
  }
  
}


const float reco::SkimEvent::leadingJetNChgptCut(size_t index, float minPt,float eta,int applyCorrection,int applyID) const {
  
  unsigned int index_jet_ordered = indexJetByPt(index, minPt, eta, applyCorrection, applyID);
  if (index_jet_ordered >= jets_.size()) {
    return defaultvalues::defaultFloat;
  }
  else {
    return jets_[index_jet_ordered]->userFloat("nChgptCut");
  }
  
}


const float reco::SkimEvent::leadingJetNNeutralptCut(size_t index, float minPt,float eta,int applyCorrection,int applyID) const {
  
  unsigned int index_jet_ordered = indexJetByPt(index, minPt, eta, applyCorrection, applyID);
  if (index_jet_ordered >= jets_.size()) {
    return defaultvalues::defaultFloat;
  }
  else {
    return jets_[index_jet_ordered]->userFloat("nNeutralptCut");
  }
  
}


const float reco::SkimEvent::leadingJetPtD(size_t index, float minPt,float eta,int applyCorrection,int applyID, int QualityCut) const {
  
  unsigned int index_jet_ordered = indexJetByPt(index, minPt, eta, applyCorrection, applyID);
  if (index_jet_ordered >= jets_.size()) {
    return defaultvalues::defaultFloat;
  }
  else {
    if (QualityCut == 1) return jets_[index_jet_ordered]->userFloat("QCptD");
    else                 return jets_[index_jet_ordered]->userFloat("ptD"); 
  }
  
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
  
  unsigned int index_jet_ordered = indexJetByPt(index, minPt, eta, applyCorrection, applyID);
  if (index_jet_ordered >= jets_.size()) {
    return defaultvalues::defaultFloat;
  }
  else {
    if (QualityCut == 1) return jets_[index_jet_ordered]->userFloat("QGTaggerAK4PFCHS:QCRmax");
    else                 return jets_[index_jet_ordered]->userFloat("QGTaggerAK4PFCHS:Rmax"); 
  }
  
}


const float reco::SkimEvent::leadingJetQGRMScand(size_t index, float minPt,float eta,int applyCorrection,int applyID, int QualityCut) const {
  
  unsigned int index_jet_ordered = indexJetByPt(index, minPt, eta, applyCorrection, applyID);
  if (index_jet_ordered >= jets_.size()) {
    return defaultvalues::defaultFloat;
  }
  else {
    if (QualityCut == 1) return jets_[index_jet_ordered]->userFloat("QGTaggerAK4PFCHS:QCRMScand");
    else                 return jets_[index_jet_ordered]->userFloat("QGTaggerAK4PFCHS:RMScand"); 
  }
  
}


const float reco::SkimEvent::leadingJetQGaxis1(size_t index, float minPt,float eta,int applyCorrection,int applyID, int QualityCut) const {
  
  unsigned int index_jet_ordered = indexJetByPt(index, minPt, eta, applyCorrection, applyID);
  if (index_jet_ordered >= jets_.size()) {
    return defaultvalues::defaultFloat;
  }
  else {
    if (QualityCut == 1) return jets_[index_jet_ordered]->userFloat("QGTaggerAK4PFCHS:QCaxis1");
    else                 return jets_[index_jet_ordered]->userFloat("QGTaggerAK4PFCHS:axis1"); 
  }
  
}


const float reco::SkimEvent::leadingJetQGaxis2(size_t index, float minPt,float eta,int applyCorrection,int applyID, int QualityCut) const {
  
  unsigned int index_jet_ordered = indexJetByPt(index, minPt, eta, applyCorrection, applyID);
  if (index_jet_ordered >= jets_.size()) {
    return defaultvalues::defaultFloat;
  }
  else {
    if (QualityCut == 1) return jets_[index_jet_ordered]->userFloat("QGTaggerAK4PFCHS:QCaxis2");
    else                 return jets_[index_jet_ordered]->userFloat("QGTaggerAK4PFCHS:axis2"); 
  }
  
}



const float reco::SkimEvent::leadingJetQGlikelihood(size_t index, float minPt,float eta,int applyCorrection,int applyID) const {
  
  unsigned int index_jet_ordered = indexJetByPt(index, minPt, eta, applyCorrection, applyID);
  if (index_jet_ordered >= jets_.size()) {
    return defaultvalues::defaultFloat;
  }
  else {
    return jets_[index_jet_ordered]->userFloat("QGTaggerAK4PFCHS:qgLikelihood");
  }
  
}






const float reco::SkimEvent::leadingJetChargedHadronMultiplicity(size_t index, float minPt,float eta,int applyCorrection,int applyID) const {
  
  unsigned int index_jet_ordered = indexJetByPt(index, minPt, eta, applyCorrection, applyID);
  if (index_jet_ordered >= jets_.size()) {
    return defaultvalues::defaultFloat;
  }
  else {
    return jets_[index_jet_ordered]->chargedHadronMultiplicity();
  }
  
}

const float reco::SkimEvent::leadingJetNeutralHadronMultiplicity(size_t index, float minPt,float eta,int applyCorrection,int applyID) const {
  
  unsigned int index_jet_ordered = indexJetByPt(index, minPt, eta, applyCorrection, applyID);
  if (index_jet_ordered >= jets_.size()) {
    return defaultvalues::defaultFloat;
  }
  else {
    return jets_[index_jet_ordered]->neutralHadronMultiplicity();
  }
  
}

const float reco::SkimEvent::leadingJetPhotonMultiplicity(size_t index, float minPt,float eta,int applyCorrection,int applyID) const {
  
  unsigned int index_jet_ordered = indexJetByPt(index, minPt, eta, applyCorrection, applyID);
  if (index_jet_ordered >= jets_.size()) {
    return defaultvalues::defaultFloat;
  }
  else {
    return jets_[index_jet_ordered]->photonMultiplicity();
  }
  
}


// set and get Rho for Jet
void reco::SkimEvent::setJetRhoIso(const edm::Handle<double> & h) {
  rhoJetIso_ = (double) (*h);
}

void reco::SkimEvent::setJetRhoCaloIso(const edm::Handle<double> & h) {
  rhoCaloJetIso_ = (double) (*h);
}

const float reco::SkimEvent::getJetRhoIso() const {
  return rhoJetIso_;
}

const float reco::SkimEvent::getJetRhoCaloIso() const {
  return rhoCaloJetIso_;
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
  if (leps_.size() < 2) return defaultvalues::defaultFloat;   
  float dphi = 0, ptMax = 0;
  for (size_t i=0;i<jets_.size();++i) {
    if (!(passJetID(jets_[i],applyID)) ) continue;
    if ( std::fabs(jets_[i]->eta()) >= eta) continue;
    
    if (isThisJetALepton(jets_[i])) continue;
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
    
    for (size_t i=0;i<jets_.size();++i) {
      if (!(passJetID(jets_[i],applyID)) ) continue;
      if ( std::fabs(jets_[i]->eta()) >= etamaxjet) continue;
      if ( jetPt(i,applyCorrection) <= ptminjet) continue;
      
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


const float reco::SkimEvent::leadingJetCloseLeptonDR(size_t ilepton = 0) const {
  return leadingJetCloseLeptonDR(ilepton, minPtForJets_, maxEtaForJets_, applyCorrectionForJets_, applyIDForJets_); 
}

const float reco::SkimEvent::leadingJetCloseLeptonDR(size_t ilepton, float ptminjet ,float etamaxjet,int applyCorrection, int applyID) const { 
  
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
      return minDR;
    }
  }
  
  return defaultvalues::defaultFloat;
}



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

const float reco::SkimEvent::leadingFatJetPt(float minPt,float eta,int applyCorrection,int applyID, size_t index) const {
  
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

const float reco::SkimEvent::leadingFatJetEta(float minPt,float eta,int applyCorrection,int applyID, size_t index) const {
  
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

const float reco::SkimEvent::leadingFatJetPhi(float minPt,float eta,int applyCorrection,int applyID, size_t index) const {
  
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

const float reco::SkimEvent::leadingFatJetTrimmedMass(float minPt,float eta,int applyCorrection,int applyID, size_t index) const {
  
  size_t count = 0;
  for(size_t i=0;i<fatJets_.size();++i) {
    if(!(passFatJetID(fatJets_[i],applyID)) ) continue;
    if( std::fabs(fatJets_[i]->eta()) >= eta) continue;
    if( fatJetPt(i,applyCorrection) <= minPt) continue;
    if(isThisJetALepton(fatJets_[i])) continue;
    if(++count > index) return fatJets_[i]->userFloat("ak8PFJetsCHSTrimmedMass");
  }
  return -9999.9;
}


const float reco::SkimEvent::leadingFatJetFilteredMass(float minPt,float eta,int applyCorrection,int applyID, size_t index) const {
  
  size_t count = 0;
  for(size_t i=0;i<fatJets_.size();++i) {
    if (!(passJetID(fatJets_[i],applyID)) ) continue;
    if ( std::fabs(fatJets_[i]->eta()) >= eta) continue;
    if ( fatJetPt(i,applyCorrection) <= minPt) continue;
    if (isThisJetALepton(fatJets_[i])) continue;
    if (++count > index) return fatJets_[i]->userFloat("ak8PFJetsCHSFilteredMass");
  }
  return -9999.9;
}


const float reco::SkimEvent::leadingFatJetPrunedMass(float minPt,float eta,int applyCorrection,int applyID, size_t index) const {
  
  size_t count = 0;
  for(size_t i=0;i<fatJets_.size();++i) {
    if(!(passJetID(fatJets_[i],applyID)) ) continue;
    if( std::fabs(fatJets_[i]->eta()) >= eta) continue;
    if( fatJetPt(i,applyCorrection) <= minPt) continue;
    if(isThisJetALepton(fatJets_[i])) continue;
    if(++count > index) return fatJets_[i]->userFloat("ak8PFJetsCHSPrunedMass");
  }
  return -9999.9;
}




const float reco::SkimEvent::leadingFatJetMassDrop(float minPt,float eta,int applyCorrection,int applyID, size_t index) const {
  
  size_t count = 0;
  for(size_t i=0;i<fatJets_.size();++i) {
    if(!(passJetID(fatJets_[i],applyID)) ) continue;
    if( std::fabs(fatJets_[i]->eta()) >= eta) continue;
    if( fatJetPt(i,applyCorrection) <= minPt) continue;
    if(isThisJetALepton(fatJets_[i])) continue;
    if(++count > index) return fatJets_[i]->userFloat("ak8PFJetsCHSSoftDropMass");
  }
  return -9999.9;
}


const float reco::SkimEvent::leadingFatJetPrunedTau2Tau1(float minPt,float eta,int applyCorrection,int applyID, size_t index) const {
  
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


const float reco::SkimEvent::leadingFatJetPrunedTau1(float minPt,float eta,int applyCorrection,int applyID, size_t index) const {
  
  size_t count = 0;
  for(size_t i=0;i<fatJets_.size();++i) {
    if(!(passJetID(fatJets_[i],applyID)) ) continue;
    if( std::fabs(fatJets_[i]->eta()) >= eta) continue;
    if( fatJetPt(i,applyCorrection) <= minPt) continue;
    if(isThisJetALepton(fatJets_[i])) continue;
    if(++count > index) return fatJets_[i]->userFloat("NjettinessAK8:tau1");
  }
  return -9999.9;
}


const float reco::SkimEvent::leadingFatJetPrunedTau2(float minPt,float eta,int applyCorrection,int applyID, size_t index) const {
  
  size_t count = 0;
  for(size_t i=0;i<fatJets_.size();++i) {
    if(!(passJetID(fatJets_[i],applyID)) ) continue;
    if( std::fabs(fatJets_[i]->eta()) >= eta) continue;
    if( fatJetPt(i,applyCorrection) <= minPt) continue;
    if(isThisJetALepton(fatJets_[i])) continue;
    if(++count > index) return fatJets_[i]->userFloat("NjettinessAK8:tau2");
  }
  return -9999.9;
}


const float reco::SkimEvent::leadingFatJetPrunedTau3(float minPt,float eta,int applyCorrection,int applyID, size_t index) const {
  
  size_t count = 0;
  for(size_t i=0;i<fatJets_.size();++i) {
    if(!(passJetID(fatJets_[i],applyID)) ) continue;
    if( std::fabs(fatJets_[i]->eta()) >= eta) continue;
    if( fatJetPt(i,applyCorrection) <= minPt) continue;
    if(isThisJetALepton(fatJets_[i])) continue;
    if(++count > index) return fatJets_[i]->userFloat("NjettinessAK8:tau3");
  }
  return -9999.9;
}


const float reco::SkimEvent::leadingFatJetPrunedTau4(float minPt,float eta,int applyCorrection,int applyID, size_t index) const {
  
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


// Track jet variables
float reco::SkimEvent::sumHtTrackJets() const {
  float sumHt=0;
  float minEta = leadingJetEta(0,minPtForJets_,maxEtaForJets_,applyCorrectionForJets_,applyIDForJets_);
  float maxEta = leadingJetEta(1,minPtForJets_,maxEtaForJets_,applyCorrectionForJets_,applyIDForJets_);
  if( minEta > maxEta ) {
    float temp = minEta;
    minEta = maxEta;
    maxEta = temp;
  }
  for(size_t i=0;i<trackJets_.size();++i) {
    if( trackJets_[i]->eta() < (maxEta-0.4) && trackJets_[i]->eta() > (minEta+0.4) ) {
      bool thisJetIsLepton = false;
      for(size_t j=0; j<leps_.size();j++){ //---- check all leptons up to "minimum pt" (default 10 GeV, minLeptonPt)
        double dR = fabs(ROOT::Math::VectorUtil::DeltaR(trackJets_[i]->p4(),leps_[indexByPt(j)]->p4()));
        if(dR < 0.3){
          thisJetIsLepton = true;
          break;
        }
      }
      if( !thisJetIsLepton ) sumHt += abs(trackJets_[i]->pt());
    }
  }
  return sumHt;
}

float reco::SkimEvent::sumHtTrackJetsDensity() const {
  float etaRange = abs(leadingJetEta(0,minPtForJets_,maxEtaForJets_,applyCorrectionForJets_,applyIDForJets_) - leadingJetEta(1,minPtForJets_,maxEtaForJets_,applyCorrectionForJets_,applyIDForJets_)) -0.8;
  if( etaRange > 0 ) {
    return (sumHtTrackJets()/etaRange);
  }
  else {
    return 0;
  }
}

float reco::SkimEvent::multiplicityTrackJets() const {
  float multiplicity=0;
  float minEta = leadingJetEta(0,minPtForJets_,maxEtaForJets_,applyCorrectionForJets_,applyIDForJets_);
  float maxEta = leadingJetEta(1,minPtForJets_,maxEtaForJets_,applyCorrectionForJets_,applyIDForJets_);
  if( minEta > maxEta ) {
    float temp = minEta;
    minEta = maxEta;
    maxEta = temp;
  }
  for(size_t i=0;i<trackJets_.size();++i) {
    if( trackJets_[i]->eta() < (maxEta-0.4) && trackJets_[i]->eta() > (minEta+0.4) ) {
      bool thisJetIsLepton = false;
      for(size_t j=0; j<leps_.size();j++){ //---- check all leptons up to "minimum pt" (default 10 GeV, minLeptonPt)
        double dR = fabs(ROOT::Math::VectorUtil::DeltaR(trackJets_[i]->p4(),leps_[indexByPt(j)]->p4()));
        if(dR < 0.3){
          thisJetIsLepton = true;
          break;
        }
      }
      if( !thisJetIsLepton ) multiplicity++;
    }
  }
  return multiplicity;
}

float reco::SkimEvent::multiplicityTrackJetsDensity() const {
  float etaRange = abs(leadingJetEta(0,minPtForJets_,maxEtaForJets_,applyCorrectionForJets_,applyIDForJets_) - leadingJetEta(1,minPtForJets_,maxEtaForJets_,applyCorrectionForJets_,applyIDForJets_)) -0.8;
  if( etaRange > 0 ) {
    return (multiplicityTrackJets()/etaRange);
  }
  else {
    return 0;
  }
}

float reco::SkimEvent::trackJetPt(size_t i) const {
  if( i < trackJets_.size() )
    return trackJets_[i]->pt();
  else 
    return defaultvalues::defaultFloat;
}

float reco::SkimEvent::trackJetEta(size_t i) const {
  if( i < trackJets_.size() )
    return trackJets_[i]->eta();
  else 
    return defaultvalues::defaultFloat;
}

float reco::SkimEvent::trackJetPhi(size_t i) const {
  if( i < trackJets_.size() )
    return trackJets_[i]->phi();
  else 
    return defaultvalues::defaultFloat;
}

float reco::SkimEvent::trackJetProbabilityB(size_t i) const {
  if( i < trackJets_.size() )
    return trackJets_[i]->bDiscriminator("pfJetProbabilityBJetTags");
  else 
    return defaultvalues::defaultFloat;
}

//Event variables

const float reco::SkimEvent::metPfNoHf() const {
  
  if(pfMetNoHf_.isNonnull()) return pfMetNoHf_->pt();
  else return defaultvalues::defaultFloat;
}

const float reco::SkimEvent::metPfType1SumEt() const {
  
  if(pfMet_.isNonnull()) return pfMet_->sumEt();
  else return defaultvalues::defaultFloat;
}
const float reco::SkimEvent::metPfType1Phi() const {
  
  if(pfMet_.isNonnull()) return pfMet_->phi();
  else return defaultvalues::defaultFloat;
}
// metPfType1
const float reco::SkimEvent::metPfType1() const {
  
  if(pfMet_.isNonnull()) return pfMet_->pt();
  else return defaultvalues::defaultFloat;
}
const float reco::SkimEvent::metPfType1JetEnUp() const {
  
  //if(pfMet_.isNonnull()) return pfMet_->shiftedPt(pat::MET::METUncertainty::JetEnUp,pat::MET::METCorrectionLevel::Type1);
  if(pfMet_.isNonnull()) return pfMet_->shiftedPt(pat::MET::JetEnUp);
  else return defaultvalues::defaultFloat;
}
const float reco::SkimEvent::metPfType1JetEnDn() const {
  
  if(pfMet_.isNonnull()) return pfMet_->shiftedPt(pat::MET::JetEnDown);
  else return defaultvalues::defaultFloat;
}
const float reco::SkimEvent::metPfType1JetResUp() const {
  
  if(pfMet_.isNonnull()) return pfMet_->shiftedPt(pat::MET::JetResUp);
  else return defaultvalues::defaultFloat;
}
const float reco::SkimEvent::metPfType1JetResDn() const {
  
  if(pfMet_.isNonnull()) return pfMet_->shiftedPt(pat::MET::JetResDown);
  else return defaultvalues::defaultFloat;
}
const float reco::SkimEvent::metPfType1MuonEnUp() const {
  
  if(pfMet_.isNonnull()) return pfMet_->shiftedPt(pat::MET::MuonEnUp);
  else return defaultvalues::defaultFloat;
}
const float reco::SkimEvent::metPfType1MuonEnDn() const {
  
  if(pfMet_.isNonnull()) return pfMet_->shiftedPt(pat::MET::MuonEnDown);
  else return defaultvalues::defaultFloat;
}
const float reco::SkimEvent::metPfType1ElecEnUp() const {
  
  if(pfMet_.isNonnull()) return pfMet_->shiftedPt(pat::MET::ElectronEnUp);
  else return defaultvalues::defaultFloat;
}
const float reco::SkimEvent::metPfType1ElecEnDn() const {
  
  if(pfMet_.isNonnull()) return pfMet_->shiftedPt(pat::MET::ElectronEnDown);
  else return defaultvalues::defaultFloat;
}
const float reco::SkimEvent::metPfType1UnclEnUp() const {
  
  if(pfMet_.isNonnull()) return pfMet_->shiftedPt(pat::MET::UnclusteredEnUp);
  else return defaultvalues::defaultFloat;
}
const float reco::SkimEvent::metPfType1UnclEnDn() const {
  
  if(pfMet_.isNonnull()) return pfMet_->shiftedPt(pat::MET::UnclusteredEnDown);
  else return defaultvalues::defaultFloat;
}
// metPfRaw
const float reco::SkimEvent::metPfRaw() const {
  
  if(pfMet_.isNonnull()) return pfMet_->shiftedPt(pat::MET::NoShift, pat::MET::Raw);
  else return defaultvalues::defaultFloat;
}
const float reco::SkimEvent::metPfRawJetEnUp() const {
  
  if(pfMet_.isNonnull()) return pfMet_->shiftedPt(pat::MET::JetEnUp, pat::MET::Raw);
  else return defaultvalues::defaultFloat;
}
const float reco::SkimEvent::metPfRawJetEnDn() const {
  
  if(pfMet_.isNonnull()) return pfMet_->shiftedPt(pat::MET::JetEnDown, pat::MET::Raw);
  else return defaultvalues::defaultFloat;
}
const float reco::SkimEvent::metPfRawJetResUp() const {
  
  if(pfMet_.isNonnull()) return pfMet_->shiftedPt(pat::MET::JetResUp, pat::MET::Raw);
  else return defaultvalues::defaultFloat;
}
const float reco::SkimEvent::metPfRawJetResDn() const {
  
  if(pfMet_.isNonnull()) return pfMet_->shiftedPt(pat::MET::JetResDown, pat::MET::Raw);
  else return defaultvalues::defaultFloat;
}
const float reco::SkimEvent::metPfRawMuonEnUp() const {
  
  if(pfMet_.isNonnull()) return pfMet_->shiftedPt(pat::MET::MuonEnUp, pat::MET::Raw);
  else return defaultvalues::defaultFloat;
}
const float reco::SkimEvent::metPfRawMuonEnDn() const {
  
  if(pfMet_.isNonnull()) return pfMet_->shiftedPt(pat::MET::MuonEnDown, pat::MET::Raw);
  else return defaultvalues::defaultFloat;
}
const float reco::SkimEvent::metPfRawElecEnUp() const {
  
  if(pfMet_.isNonnull()) return pfMet_->shiftedPt(pat::MET::ElectronEnUp, pat::MET::Raw);
  else return defaultvalues::defaultFloat;
}
const float reco::SkimEvent::metPfRawElecEnDn() const {
  
  if(pfMet_.isNonnull()) return pfMet_->shiftedPt(pat::MET::ElectronEnDown, pat::MET::Raw);
  else return defaultvalues::defaultFloat;
}
const float reco::SkimEvent::metPfRawUnclEnUp() const {
  
  if(pfMet_.isNonnull()) return pfMet_->shiftedPt(pat::MET::UnclusteredEnUp, pat::MET::Raw);
  else return defaultvalues::defaultFloat;
}
const float reco::SkimEvent::metPfRawUnclEnDn() const {
  
  if(pfMet_.isNonnull()) return pfMet_->shiftedPt(pat::MET::UnclusteredEnDown, pat::MET::Raw);
  else return defaultvalues::defaultFloat;
}

//

const float reco::SkimEvent::metPfRawSumEt() const {
  
  if(pfMet_.isNonnull()) return pfMet_->shiftedSumEt(pat::MET::NoShift, pat::MET::Raw);
  else return defaultvalues::defaultFloat;
}

const float reco::SkimEvent::metPfRawPhi() const {
  
  if(pfMet_.isNonnull()) return pfMet_->shiftedPhi(pat::MET::NoShift, pat::MET::Raw);
  else return defaultvalues::defaultFloat;
}
const float reco::SkimEvent::metPfRawPhiJetEnUp() const {
  
  if(pfMet_.isNonnull()) return pfMet_->shiftedPhi(pat::MET::JetEnUp, pat::MET::Raw);
  else return defaultvalues::defaultFloat;
}
const float reco::SkimEvent::metPfRawPhiJetEnDn() const {
  
  if(pfMet_.isNonnull()) return pfMet_->shiftedPhi(pat::MET::JetEnDown, pat::MET::Raw);
  else return defaultvalues::defaultFloat;
}
const float reco::SkimEvent::metPfRawPhiJetResUp() const {
  
  if(pfMet_.isNonnull()) return pfMet_->shiftedPhi(pat::MET::JetResUp, pat::MET::Raw);
  else return defaultvalues::defaultFloat;
}
const float reco::SkimEvent::metPfRawPhiJetResDn() const {
  
  if(pfMet_.isNonnull()) return pfMet_->shiftedPhi(pat::MET::JetResDown, pat::MET::Raw);
  else return defaultvalues::defaultFloat;
}
const float reco::SkimEvent::metPfRawPhiMuonEnUp() const {
  
  if(pfMet_.isNonnull()) return pfMet_->shiftedPhi(pat::MET::MuonEnUp, pat::MET::Raw);
  else return defaultvalues::defaultFloat;
}
const float reco::SkimEvent::metPfRawPhiMuonEnDn() const {
  
  if(pfMet_.isNonnull()) return pfMet_->shiftedPhi(pat::MET::MuonEnDown, pat::MET::Raw);
  else return defaultvalues::defaultFloat;
}
const float reco::SkimEvent::metPfRawPhiElecEnUp() const {
  
  if(pfMet_.isNonnull()) return pfMet_->shiftedPhi(pat::MET::ElectronEnUp, pat::MET::Raw);
  else return defaultvalues::defaultFloat;
}
const float reco::SkimEvent::metPfRawPhiElecEnDn() const {
  
  if(pfMet_.isNonnull()) return pfMet_->shiftedPhi(pat::MET::ElectronEnDown, pat::MET::Raw);
  else return defaultvalues::defaultFloat;
}
const float reco::SkimEvent::metPfRawPhiUnclEnUp() const {
  
  if(pfMet_.isNonnull()) return pfMet_->shiftedPhi(pat::MET::UnclusteredEnUp, pat::MET::Raw);
  else return defaultvalues::defaultFloat;
}
const float reco::SkimEvent::metPfRawPhiUnclEnDn() const {
  
  if(pfMet_.isNonnull()) return pfMet_->shiftedPhi(pat::MET::UnclusteredEnDown, pat::MET::Raw);
  else return defaultvalues::defaultFloat;
}

const float reco::SkimEvent::pupMet() const {
  
  if(pupMet_.isNonnull()) return pupMet_->pt();
  else return defaultvalues::defaultFloat;
}
const float reco::SkimEvent::trkMet() const {
  return trkMet_.pt();
}
const float reco::SkimEvent::trkMetphi() const {
  return trkMet_.phi();
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
    case PFMET: return metPfRaw();
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

const float reco::SkimEvent::metPfProj() const {
  float dphi = dPhilPfMet();
  if(dphi < M_PI/2.) return metPfRaw()*sin(dphi);
  else return metPfRaw();
}
const float reco::SkimEvent::metPfProjJetEnUp() const {
  float dphi = dPhilPfMetJetEnUp();
  if(dphi < M_PI/2.) return metPfRawJetEnUp()*sin(dphi);
  else return metPfRawJetEnUp();
}
const float reco::SkimEvent::metPfProjJetEnDn() const {
  float dphi = dPhilPfMetJetEnDn();
  if(dphi < M_PI/2.) return metPfRawJetEnDn()*sin(dphi);
  else return metPfRawJetEnDn();
}
const float reco::SkimEvent::metPfProjJetResUp() const {
  float dphi = dPhilPfMetJetResUp();
  if(dphi < M_PI/2.) return metPfRawJetResUp()*sin(dphi);
  else return metPfRawJetResUp();
}
const float reco::SkimEvent::metPfProjJetResDn() const {
  float dphi = dPhilPfMetJetResDn();
  if(dphi < M_PI/2.) return metPfRawJetResDn()*sin(dphi);
  else return metPfRawJetResDn();
}
const float reco::SkimEvent::metPfProjMuonEnUp() const {
  float dphi = dPhilPfMetMuonEnUp();
  if(dphi < M_PI/2.) return metPfRawMuonEnUp()*sin(dphi);
  else return metPfRawMuonEnUp();
}
const float reco::SkimEvent::metPfProjMuonEnDn() const {
  float dphi = dPhilPfMetMuonEnDn();
  if(dphi < M_PI/2.) return metPfRawMuonEnDn()*sin(dphi);
  else return metPfRawMuonEnDn();
}
const float reco::SkimEvent::metPfProjElecEnUp() const {
  float dphi = dPhilPfMetElecEnUp();
  if(dphi < M_PI/2.) return metPfRawElecEnUp()*sin(dphi);
  else return metPfRawElecEnUp();
}
const float reco::SkimEvent::metPfProjElecEnDn() const {
  float dphi = dPhilPfMetElecEnDn();
  if(dphi < M_PI/2.) return metPfRawElecEnDn()*sin(dphi);
  else return metPfRawElecEnDn();
}
const float reco::SkimEvent::metPfProjUnclEnUp() const {
  float dphi = dPhilPfMetUnclEnUp();
  if(dphi < M_PI/2.) return metPfRawUnclEnUp()*sin(dphi);
  else return metPfRawUnclEnUp();
}
const float reco::SkimEvent::metPfProjUnclEnDn() const {
  float dphi = dPhilPfMetUnclEnDn();
  if(dphi < M_PI/2.) return metPfRawUnclEnDn()*sin(dphi);
  else return metPfRawUnclEnDn();
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
const float reco::SkimEvent::dPhilPfMetJetEnUp() const {
  float smallestDphi = 9999.;
  for(size_t l=0; l<std::min((uint) 2,(uint) leps_.size());++l){
    float dphi = dPhilPfMetJetEnUp(l);
    if( dphi < smallestDphi) smallestDphi = dphi;
  }
  return smallestDphi;
}
const float reco::SkimEvent::dPhilPfMetJetEnDn() const {
  float smallestDphi = 9999.;
  for(size_t l=0; l<std::min((uint) 2,(uint) leps_.size());++l){
    float dphi = dPhilPfMetJetEnDn(l);
    if( dphi < smallestDphi) smallestDphi = dphi;
  }
  return smallestDphi;
}
const float reco::SkimEvent::dPhilPfMetJetResUp() const {
  float smallestDphi = 9999.;
  for(size_t l=0; l<std::min((uint) 2,(uint) leps_.size());++l){
    float dphi = dPhilPfMetJetResUp(l);
    if( dphi < smallestDphi) smallestDphi = dphi;
  }
  return smallestDphi;
}
const float reco::SkimEvent::dPhilPfMetJetResDn() const {
  float smallestDphi = 9999.;
  for(size_t l=0; l<std::min((uint) 2,(uint) leps_.size());++l){
    float dphi = dPhilPfMetJetResDn(l);
    if( dphi < smallestDphi) smallestDphi = dphi;
  }
  return smallestDphi;
}
const float reco::SkimEvent::dPhilPfMetMuonEnUp() const {
  float smallestDphi = 9999.;
  for(size_t l=0; l<std::min((uint) 2,(uint) leps_.size());++l){
    float dphi = dPhilPfMetMuonEnUp(l);
    if( dphi < smallestDphi) smallestDphi = dphi;
  }
  return smallestDphi;
}
const float reco::SkimEvent::dPhilPfMetMuonEnDn() const {
  float smallestDphi = 9999.;
  for(size_t l=0; l<std::min((uint) 2,(uint) leps_.size());++l){
    float dphi = dPhilPfMetMuonEnDn(l);
    if( dphi < smallestDphi) smallestDphi = dphi;
  }
  return smallestDphi;
}
const float reco::SkimEvent::dPhilPfMetElecEnUp() const {
  float smallestDphi = 9999.;
  for(size_t l=0; l<std::min((uint) 2,(uint) leps_.size());++l){
    float dphi = dPhilPfMetElecEnUp(l);
    if( dphi < smallestDphi) smallestDphi = dphi;
  }
  return smallestDphi;
}
const float reco::SkimEvent::dPhilPfMetElecEnDn() const {
  float smallestDphi = 9999.;
  for(size_t l=0; l<std::min((uint) 2,(uint) leps_.size());++l){
    float dphi = dPhilPfMetElecEnDn(l);
    if( dphi < smallestDphi) smallestDphi = dphi;
  }
  return smallestDphi;
}
const float reco::SkimEvent::dPhilPfMetUnclEnUp() const {
  float smallestDphi = 9999.;
  for(size_t l=0; l<std::min((uint) 2,(uint) leps_.size());++l){
    float dphi = dPhilPfMetUnclEnUp(l);
    if( dphi < smallestDphi) smallestDphi = dphi;
  }
  return smallestDphi;
}
const float reco::SkimEvent::dPhilPfMetUnclEnDn() const {
  float smallestDphi = 9999.;
  for(size_t l=0; l<std::min((uint) 2,(uint) leps_.size());++l){
    float dphi = dPhilPfMetUnclEnDn(l);
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
const float reco::SkimEvent::dPhilPfMetJetEnUp(size_t i) const {
  if( i >= std::min((uint) 2,(uint) leps_.size()) ) return defaultvalues::defaultFloat;
  return fabs(ROOT::Math::VectorUtil::DeltaPhi(pfMet_->shiftedP4(pat::MET::JetEnUp, pat::MET::Raw),leps_[i]->p4()) );
}
const float reco::SkimEvent::dPhilPfMetJetEnDn(size_t i) const {
  if( i >= std::min((uint) 2,(uint) leps_.size()) ) return defaultvalues::defaultFloat;
  return fabs(ROOT::Math::VectorUtil::DeltaPhi(pfMet_->shiftedP4(pat::MET::JetEnDown, pat::MET::Raw),leps_[i]->p4()) );
}
const float reco::SkimEvent::dPhilPfMetJetResUp(size_t i) const {
  if( i >= std::min((uint) 2,(uint) leps_.size()) ) return defaultvalues::defaultFloat;
  return fabs(ROOT::Math::VectorUtil::DeltaPhi(pfMet_->shiftedP4(pat::MET::JetResUp, pat::MET::Raw),leps_[i]->p4()) );
}
const float reco::SkimEvent::dPhilPfMetJetResDn(size_t i) const {
  if( i >= std::min((uint) 2,(uint) leps_.size()) ) return defaultvalues::defaultFloat;
  return fabs(ROOT::Math::VectorUtil::DeltaPhi(pfMet_->shiftedP4(pat::MET::JetResDown, pat::MET::Raw),leps_[i]->p4()) );
}
const float reco::SkimEvent::dPhilPfMetMuonEnUp(size_t i) const {
  if( i >= std::min((uint) 2,(uint) leps_.size()) ) return defaultvalues::defaultFloat;
  return fabs(ROOT::Math::VectorUtil::DeltaPhi(pfMet_->shiftedP4(pat::MET::MuonEnUp, pat::MET::Raw),leps_[i]->p4()) );
}
const float reco::SkimEvent::dPhilPfMetMuonEnDn(size_t i) const {
  if( i >= std::min((uint) 2,(uint) leps_.size()) ) return defaultvalues::defaultFloat;
  return fabs(ROOT::Math::VectorUtil::DeltaPhi(pfMet_->shiftedP4(pat::MET::MuonEnDown, pat::MET::Raw),leps_[i]->p4()) );
}
const float reco::SkimEvent::dPhilPfMetElecEnUp(size_t i) const {
  if( i >= std::min((uint) 2,(uint) leps_.size()) ) return defaultvalues::defaultFloat;
  return fabs(ROOT::Math::VectorUtil::DeltaPhi(pfMet_->shiftedP4(pat::MET::ElectronEnUp, pat::MET::Raw),leps_[i]->p4()) );
}
const float reco::SkimEvent::dPhilPfMetElecEnDn(size_t i) const {
  if( i >= std::min((uint) 2,(uint) leps_.size()) ) return defaultvalues::defaultFloat;
  return fabs(ROOT::Math::VectorUtil::DeltaPhi(pfMet_->shiftedP4(pat::MET::ElectronEnDown, pat::MET::Raw),leps_[i]->p4()) );
}
const float reco::SkimEvent::dPhilPfMetUnclEnUp(size_t i) const {
  if( i >= std::min((uint) 2,(uint) leps_.size()) ) return defaultvalues::defaultFloat;
  return fabs(ROOT::Math::VectorUtil::DeltaPhi(pfMet_->shiftedP4(pat::MET::UnclusteredEnUp, pat::MET::Raw),leps_[i]->p4()) );
}
const float reco::SkimEvent::dPhilPfMetUnclEnDn(size_t i) const {
  if( i >= std::min((uint) 2,(uint) leps_.size()) ) return defaultvalues::defaultFloat;
  return fabs(ROOT::Math::VectorUtil::DeltaPhi(pfMet_->shiftedP4(pat::MET::UnclusteredEnDown, pat::MET::Raw),leps_[i]->p4()) );
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
  const reco::Vertex primaryVtx = highestPtVtx();
  return 1.0 * primaryVtx.nTracks();
  
  //  return 0;
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


//---- selected trigger bits

void reco::SkimEvent::setSelectedTriggerBits( const std::vector<float> &bits) {
  for (unsigned int i=0; i<bits.size(); i++) {
    _bits.push_back(bits.at(i));  
  }
}

void reco::SkimEvent::setSelectedTriggerBitsPrescales( const std::vector<float> &bitsprescales) {
  for (unsigned int i=0; i<bitsprescales.size(); i++) {
    _bitsPrescales.push_back(bitsprescales.at(i));  
  }
}

void reco::SkimEvent::setSelectedTriggerL1minBitsPrescales( const std::vector<float> &bitsprescales) {
  for (unsigned int i=0; i<bitsprescales.size(); i++) {
    _bitsL1minPrescales.push_back(bitsprescales.at(i));  
  }
}

void reco::SkimEvent::setSelectedTriggerL1maxBitsPrescales( const std::vector<float> &bitsprescales) {
  for (unsigned int i=0; i<bitsprescales.size(); i++) {
    _bitsL1maxPrescales.push_back(bitsprescales.at(i));  
  }
}


const float reco::SkimEvent::selectedRateTrigger( size_t i ) const {
  //  std::cout << " i = " << i << " :: " <<  _bits.size() << std::endl;
  if (i < _bits.size()) {
    return 1.0 * _bits.at(i);
  }
  else {
    return -2.0;
  }
}

const float reco::SkimEvent::selectedRateTriggerPrescale( size_t i ) const {
  if (i < _bitsPrescales.size()) {
    return 1.0 * _bitsPrescales.at(i);
  }
  else {
    return -2.0;
  }
}

const float reco::SkimEvent::selectedRateTriggerL1minPrescale( size_t i ) const {
  if (i < _bitsL1minPrescales.size()) {
    return 1.0 * _bitsL1minPrescales.at(i);
  }
  else {
    return -2.0;
  } 
}

const float reco::SkimEvent::selectedRateTriggerL1maxPrescale( size_t i ) const {
  if (i < _bitsL1maxPrescales.size()) {
    return 1.0 * _bitsL1maxPrescales.at(i);
  }
  else {
    return -2.0;
  } 
}




//---- special trigger bits. E.g. metFilters

void reco::SkimEvent::setSpecialTriggerBits( const std::vector<float> &bits) {
  for (unsigned int i=0; i<bits.size(); i++) {
    _specialBits.push_back(bits.at(i));  
  }
}

const float reco::SkimEvent::specialRateTrigger( size_t i ) const {
  if (i < _specialBits.size()) {
    return 1.0 * _specialBits.at(i);
  }
  else {
    return -2.0;
  }
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
  passesFakeRateEl_   = bits[11];
  passesFakeRateMu_   = bits[12];
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
  } else if(pdType == DoubleEG  ) { return ( !passesMuEGData_ && !passesDoubleMuData_ && !passesSingleMuData_ && passesDoubleElData_ );
  } else if(pdType == SingleElectron) { return ( !passesMuEGData_ && !passesDoubleMuData_ && !passesSingleMuData_ && !passesDoubleElData_ && passesSingleElData_ );
  } else if(pdType == AllEmbed) { return ( passesMuEGData_ || passesDoubleMuData_ || passesSingleMuData_ || passesDoubleElData_ || passesSingleElData_ );
  }
  
  return false;
}



const bool reco::SkimEvent::fakeRateTrigger( SkimEvent::primaryDatasetType pdType ) const {
  
  //Guillelmo's Implementation:
  if(pdType == MC) return true;
  
  if (pdType == DoubleMuon) { return ( passesFakeRateMu_ );
  } else if(pdType == DoubleEG  ) { return ( !passesFakeRateMu_ && passesFakeRateEl_ );
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




/** Functions
 */

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


// const float reco::SkimEvent::muBestTrackdz(size_t i) const {
//  if(i >= leps_.size()) return defaultvalues::defaultFloat;
//  const reco::Vertex primaryVtx = highestPtVtx();
//  if( isMuon(i) ) {
//   return getMuon(i)->muonBestTrack()->dz(primaryVtx.position());
//  } else return -999.0;
// }
// 
// 
// const float reco::SkimEvent::muBestTrackdxy(size_t i) const {
//  if(i >= leps_.size()) return defaultvalues::defaultFloat;
//  const reco::Vertex primaryVtx = highestPtVtx();
//  if( isMuon(i) ) {
//   return getMuon(i)->muonBestTrack()->dxy(primaryVtx.position());
//  } else return -999.0;
// }


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


const float reco::SkimEvent::SIP3D(size_t i) const {
  if(i >= leps_.size()) return defaultvalues::defaultFloat;  
  if( isMuon(i) ) {
    double ip = fabs(getMuon(i)->dB(pat::Muon::PV3D));
    double ipError = getMuon(i)->edB(pat::Muon::PV3D);
    double sip = ip/ipError;
    return sip;
  } 
  else if( isElectron(i) ) {
    double ip = fabs(getElectron(i)->dB(pat::Electron::PV3D));
    double ipError = getElectron(i)->edB(pat::Electron::PV3D);
    double sip = ip/ipError;
    return sip;
  } 
  else {
    return defaultvalues::defaultFloat;
  }
}


// Electron cut based ID
const float reco::SkimEvent::deltaEtaEleClusterTrackAtCalo(size_t i) const {
  if (i >= leps_.size())  return defaultvalues::defaultFloat;
  else if (isElectron(i)) return getElectron(i)->deltaEtaEleClusterTrackAtCalo();
  else return -999.0;
}

const float reco::SkimEvent::deltaEtaSeedClusterTrackAtCalo(size_t i) const {
  if (i >= leps_.size())  return defaultvalues::defaultFloat;
  else if (isElectron(i)) return getElectron(i)->deltaEtaSeedClusterTrackAtCalo();
  else return -999.0;
}

const float reco::SkimEvent::deltaEtaSuperClusterTrackAtVtx(size_t i) const {
  if (i >= leps_.size())  return defaultvalues::defaultFloat;
  else if (isElectron(i)) return getElectron(i)->deltaEtaSuperClusterTrackAtVtx();
  else return -999.0;
}

const float reco::SkimEvent::deltaPhiSeedClusterTrackAtCalo(size_t i) const {
  if (i >= leps_.size())  return defaultvalues::defaultFloat;
  else if (isElectron(i)) return getElectron(i)->deltaPhiSeedClusterTrackAtCalo();
  else return -999.0;
}

const float reco::SkimEvent::deltaPhiEleClusterTrackAtCalo(size_t i) const {
  if (i >= leps_.size())  return defaultvalues::defaultFloat;
  else if (isElectron(i)) return getElectron(i)->deltaPhiEleClusterTrackAtCalo();
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
  else if (isMuon(i))     return getMuon(i)->muonBestTrack()->dxy(primaryVtx.position()); 
  else return -999.0;
}

const float reco::SkimEvent::dz(size_t i) const {
  const reco::Vertex primaryVtx = highestPtVtx();
  if (i >= leps_.size())  return defaultvalues::defaultFloat;
  else if (isElectron(i)) return getElectron(i)->gsfTrack()->dz(primaryVtx.position());
  else if (isMuon(i))     return getMuon(i)->muonBestTrack()->dz(primaryVtx.position()); 
  else return -999.0;
}

const float reco::SkimEvent::expectedMissingInnerHits(size_t i) const {
  if (i >= leps_.size())  return defaultvalues::defaultFloat;
  else if (isElectron(i)) return getElectron(i)->gsfTrack()->hitPattern().numberOfHits(reco::HitPattern::MISSING_INNER_HITS);
  else return -999.9;
}

const float reco::SkimEvent::expectedMissingOuterHits(size_t i) const {
  if (i >= leps_.size())  return defaultvalues::defaultFloat;
  else if (isElectron(i)) return getElectron(i)->gsfTrack()->hitPattern().numberOfHits(reco::HitPattern::MISSING_OUTER_HITS);
  else return -999.9;
}

const float reco::SkimEvent::expectedMissingTrackHits(size_t i) const {
  if (i >= leps_.size())  return defaultvalues::defaultFloat;
  else if (isElectron(i)) return getElectron(i)->gsfTrack()->hitPattern().numberOfHits(reco::HitPattern::TRACK_HITS);
  else return -999.9;
}



const bool reco::SkimEvent::passConversionVeto(size_t i) const {
  if (i >= leps_.size())  return false;
  else if (isElectron(i)) return getElectron(i)->passConversionVeto();
  else if (isMuon(i))     return true;
  else                    return false;
}


const float reco::SkimEvent::ecalPFClusterIso(size_t i) const {
  if (i >= leps_.size())  return defaultvalues::defaultFloat;
  else if (isElectron(i)) return getElectron(i)->ecalPFClusterIso();
  else return -999.0;
}

const float reco::SkimEvent::hcalPFClusterIso(size_t i) const {
  if (i >= leps_.size())  return defaultvalues::defaultFloat;
  else if (isElectron(i)) return getElectron(i)->hcalPFClusterIso();
  else return -999.0;
}


const float reco::SkimEvent::R9(size_t i) const {
  if (i >= leps_.size())  return defaultvalues::defaultFloat;
  else if (isElectron(i)) return getElectron(i)->r9();
  else return -999.0;
}

const float reco::SkimEvent::full5x5R9(size_t i) const {
  if (i >= leps_.size())  return defaultvalues::defaultFloat;
  else if (isElectron(i)) return getElectron(i)->full5x5_r9();
  else return -999.0;
}



const float reco::SkimEvent::Fbrem(size_t i) const {
  if (i >= leps_.size())  return defaultvalues::defaultFloat;
  else if (isElectron(i)) return getElectron(i)->fbrem();
  else return -999.0;
}

const float reco::SkimEvent::SeedEnergy(size_t i) const {
  if (i >= leps_.size())  return defaultvalues::defaultFloat;
  else if (isElectron(i)) return getElectron(i)->seed()->energy();
  else return -999.0;
}

const float reco::SkimEvent::Energy5x5(size_t i) const {
  if (i >= leps_.size())  return defaultvalues::defaultFloat;
  else if (isElectron(i)) return getElectron(i)->e5x5();
  else return -999.0;
}

const float reco::SkimEvent::tripleChargeAgreement(size_t i) const {
  if (i >= leps_.size())  return defaultvalues::defaultFloat;
  else if (isElectron(i)) return getElectron(i)->chargeInfo().isGsfCtfScPixConsistent;
  else return -999.0;
}



const float reco::SkimEvent::gsfchi2(size_t i) const {
  if (i >= leps_.size())  return defaultvalues::defaultFloat;
  else if (isElectron(i)) return getElectron(i)->gsfTrack()->chi2();
  else return -999.0;
}


const float reco::SkimEvent::gsfndof(size_t i) const {
  if (i >= leps_.size())  return defaultvalues::defaultFloat;
  else if (isElectron(i)) return getElectron(i)->gsfTrack()->ndof();
  else return -999.0;
}


const float reco::SkimEvent::gsfnormalizedchi2(size_t i) const {
  if (i >= leps_.size())  return defaultvalues::defaultFloat;
  else if (isElectron(i)) return getElectron(i)->gsfTrack()->normalizedChi2();
  else return -999.0;
}



// Muon and electron isolation

const float reco::SkimEvent::trackIso(size_t i) const  {
  if(i >= leps_.size()) return defaultvalues::defaultFloat;  
  else if (isElectron(i))     return getElectron(i)->trackIso();
  else if (isMuon(i))         return getMuon(i)->isolationR03().sumPt;
  else                        return -999.0;
}

const float reco::SkimEvent::trackIso03(size_t i) const  {
  if(i >= leps_.size()) return defaultvalues::defaultFloat;  
  else if (isElectron(i))     return getElectron(i)->dr03TkSumPt();
  else if (isMuon(i))         return getMuon(i)->isolationR03().sumPt;
  else                        return -999.0;
}


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

// https://github.com/cms-analysis/MuonAnalysis-TagAndProbe/blob/80X/python/common_modules_cff.py
// https://github.com/cms-analysis/MuonAnalysis-TagAndProbe/blob/80X/plugins/MuonMiniIso.cc
// https://github.com/susy2015/SusyAnaTools/blob/master/SkimsAUX/plugins/common.cc
const float reco::SkimEvent::chargedHadronMiniIso(size_t i) const {
  
  if      (i >= leps_.size()) return defaultvalues::defaultFloat;
  else if (!isElectron(i) && !isMuon(i)) return -999.0;
  else {

    float CandPtThreshold = 0.0, dRCandProbeVeto = 0.0;
    if (isMuon(i)) {
      CandPtThreshold = 0.0; // 0.5? 0.0 reproduces the results of  getMuon(i)->pfIsolationR04().sumChargedHadronPt
      dRCandProbeVeto = 0.0001;
    } else {
      CandPtThreshold = 0.0;
      if (fabs(leps_[i]->eta())>1.479) dRCandProbeVeto = 0.015;
    }

    double RadiusIso = std::max(0.05, std::min(0.2, 10./leps_[i]->pt()));

    float sumChargedHadronMiniIso = 0.;

    for(size_t pfc = 0; pfc<pfcollection_.size() ; ++pfc) {

      // https://twiki.cern.ch/twiki/bin/view/CMSPublic/WorkBookMiniAOD2016
      if (pfcollection_.at(pfc).fromPV()>1) {

	const reco::Candidate& c = pfcollection_.at(pfc);
     
	if (abs(c.pdgId())==211) {
	
	  // check pf candidate threshold
	  if(c.pt() < CandPtThreshold) continue; 
	  
	  double dr = fabs(ROOT::Math::VectorUtil::DeltaR(leps_[i]->p4(), c.p4()));
	  if (dr < dRCandProbeVeto) continue; 
	  
	  if (dr<RadiusIso) 
	    sumChargedHadronMiniIso += c.pt();
	  
	}
	
      }
      
    }
 
    return sumChargedHadronMiniIso;
    
  }
  
}

const float reco::SkimEvent::chargedPileUpMiniIso(size_t i) const {

  if      (i >= leps_.size()) return defaultvalues::defaultFloat;
  else if (!isElectron(i) && !isMuon(i)) return -999.0;
  else {

    float CandPtThreshold = 0.0, dRCandProbeVeto = 0.0;
    if (isMuon(i)) {
      CandPtThreshold = 0.0; // as in muon T&P code. 0.5 reproduces the results of getMuon(i)->pfIsolationR04().sumPUPt
      dRCandProbeVeto = 0.0001; // as in muon T&P code; 0.01 in standard 0.4 iso
    } else {
      CandPtThreshold = 0.0;
      if (fabs(leps_[i]->eta())>1.479) dRCandProbeVeto = 0.015;
    }

    double RadiusIso = std::max(0.05, std::min(0.2, 10./leps_[i]->pt()));

    float sumChargedPileUpMiniIso = 0.;

    for(size_t pfc = 0; pfc<pfcollection_.size() ; ++pfc) {

      if (pfcollection_.at(pfc).fromPV()<=1) {

	const reco::Candidate& c = pfcollection_.at(pfc);
     
	if (c.charge()!=0) {
	
	  // check pf candidate threshold
	  if(c.pt() < CandPtThreshold) continue; 
	  
	  double dr = fabs(ROOT::Math::VectorUtil::DeltaR(leps_[i]->p4(), c.p4()));
	  if (dr < dRCandProbeVeto) continue; 
	  
	  if (dr<RadiusIso) 
	    sumChargedPileUpMiniIso += c.pt();
	  
	}
	
      }

    }
    
    return sumChargedPileUpMiniIso;
  
  }
  
}

const float reco::SkimEvent::neutralHadronMiniIso(size_t i) const {
  
  if      (i >= leps_.size()) return defaultvalues::defaultFloat;
  else if (!isElectron(i) && !isMuon(i)) return -999.0;
  else {
  
    float CandPtThreshold = 0.0, dRCandProbeVeto = 0.0;
    if (isMuon(i)) {
      CandPtThreshold = 1.0; // as in muon T&P code. 0.5 reproduces the results of getMuon(i)->pfIsolationR04().sumNeutralHadronEt
      dRCandProbeVeto = 0.01;
    } else {
      CandPtThreshold = 0.0;
    }

    double RadiusIso = std::max(0.05, std::min(0.2, 10./leps_[i]->pt()));
    
    float sumNeutralHadronMiniIso = 0.;

    for(size_t pfc = 0; pfc<pfcollection_.size() ; ++pfc) { 
      
      const reco::Candidate& c = pfcollection_.at(pfc);
      
      if (abs(c.pdgId())==130) {

	// check pf candidate threshold
	if(c.pt() < CandPtThreshold) continue;

	double dr = fabs(ROOT::Math::VectorUtil::DeltaR(leps_[i]->p4(), c.p4()));
	if (dr < dRCandProbeVeto) continue;
	
	if (dr<RadiusIso)
	  sumNeutralHadronMiniIso += c.pt();
	
      }

    }
    
    return sumNeutralHadronMiniIso;
    
  }
  
}

const float reco::SkimEvent::photonMiniIso(size_t i) const {

  if      (i >= leps_.size()) return defaultvalues::defaultFloat;
  else if (!isElectron(i) && !isMuon(i)) return -999.0;
  else {

    float CandPtThreshold = 0.0, dRCandProbeVeto = 0.0;
    if (isMuon(i)) {
      CandPtThreshold = 0.5;
      dRCandProbeVeto = 0.01;
    } else {
      CandPtThreshold = 0.0;
      if (fabs(leps_[i]->eta())>1.479) dRCandProbeVeto = 0.08;
    }

    double RadiusIso = std::max(0.05, std::min(0.2, 10./leps_[i]->pt()));

    float sumPhotonMiniIso = 0.;
    
    for(size_t pfc = 0; pfc<pfcollection_.size() ; ++pfc) {
      
      const reco::Candidate& c = pfcollection_.at(pfc);

      if (abs(c.pdgId())==22) {

	// check pf candidate threshold
	if(c.pt() < CandPtThreshold) continue; 
      
	double dr = fabs(ROOT::Math::VectorUtil::DeltaR(leps_[i]->p4(), c.p4()));
	if (dr < dRCandProbeVeto) continue; 
	
	if (dr<RadiusIso)
	  sumPhotonMiniIso += c.pt();
	
      }
      
    }
   
    return sumPhotonMiniIso;
    
  }
  
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
  
  float mass = defaultvalues::defaultFloat;
  
  const reco::Candidate* mcH = 0;
  
  // loop over gen particles
  for(size_t gp=0; gp<genParticles_.size();++gp){
    
    int pdgId = genParticles_[gp] -> pdgId();
    
    // Stop {1000006}
    if( (pdgId == 25 && genParticles_[gp]->isHardProcess()) ) {
      mcH = &(*(genParticles_[gp]));
      mass = mcH->mass();
      break;
    }
  } // loop over gen particles
  
  return mass;
}



const float reco::SkimEvent::getHiggsPt() const {
  
  float pt = defaultvalues::defaultFloat;
  
  const reco::Candidate* mcH = 0;
  
  // loop over gen particles
  for(size_t gp=0; gp<genParticles_.size();++gp){
    
    int pdgId = genParticles_[gp] -> pdgId();
    
    // Stop {1000006}
    if( (pdgId == 25 && genParticles_[gp]->isHardProcess()) ) {
      mcH = &(*(genParticles_[gp]));
      pt = mcH->pt();
      break;
    }
  } // loop over gen particles
  
  return pt;
}


const float reco::SkimEvent::getHiggsEta() const {
  
  float eta = defaultvalues::defaultFloat;
  
  const reco::Candidate* mcH = 0;
  
  // loop over gen particles
  for(size_t gp=0; gp<genParticles_.size();++gp){
    
    int pdgId = genParticles_[gp] -> pdgId();
    
    // Stop {1000006}
    if( (pdgId == 25 && genParticles_[gp]->isHardProcess()) ) {
      mcH = &(*(genParticles_[gp]));
      eta = mcH->eta();
      break;
    }
  } // loop over gen particles
  
  return eta;
}



const float reco::SkimEvent::getHiggsPhi() const {
  
  float phi = defaultvalues::defaultFloat;
  
  const reco::Candidate* mcH = 0;
  
  // loop over gen particles
  for(size_t gp=0; gp<genParticles_.size();++gp){
    
    int pdgId = genParticles_[gp] -> pdgId();
    
    // Stop {1000006}
    if( (pdgId == 25 && genParticles_[gp]->isHardProcess()) ) {
      mcH = &(*(genParticles_[gp]));
      phi = mcH->phi();
      break;
    }
  } // loop over gen particles
  
  return phi;
}



//---- Susy masses

const float reco::SkimEvent::getSusyStopMass() const {
  
  float mass = -1;
  
  const reco::Candidate* mcStop = 0;
  
  // std::cout << " genParticles_.size() = " << genParticles_.size() << std::endl;
  
  // loop over gen particles
  for(size_t gp=0; gp<genParticles_.size();++gp){
    
    int pdgId = genParticles_[gp] -> pdgId();
    //int status = genParticles_[gp] -> status();
    // std::cout << " pdgId = " << pdgId << " ~~ status = " << status << std::endl;
    
    // Stop1 {1000006} Stop2 {2000006}
    if( (abs(pdgId) == 1000006 || abs(pdgId) == 2000006) && (genParticles_[gp]->isHardProcess()) ) {
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
    //int status = genParticles_[gp] -> status();
    
    // LSP {1000022, 1000023, 1000025, 1000035}
    if( (abs(pdgId) == 1000022 || abs(pdgId) == 1000023 || abs(pdgId) == 1000025 || abs(pdgId) == 1000035) && (genParticles_[gp]->isHardProcess()) ) {
      mcChi = &(*(genParticles_[gp]));
      mass = mcChi->mass();
    }
  } // loop over gen particles
  
  return mass;
}


const float reco::SkimEvent::susyParticlePt(size_t index) const {
  
  int jndex = -1;

  // loop over gen particles
  for(size_t gp=0; gp<genParticles_.size();++gp) {
    
    int pdgId = genParticles_[gp] -> pdgId();
    //int status = genParticles_[gp] -> status();
    
    if (abs(pdgId) == 1000006 || abs(pdgId) == 2000006) { // Stop
      
      bool DecayToLSP = false;
      
      int ndaughters = genParticles_[gp]->numberOfDaughters();
     
      for (int id=0; id<ndaughters; id++) {
	
	int pdgIdDaughter = (genParticles_[gp]->daughter(id))->pdgId();
	
	if (abs(pdgIdDaughter)==1000022 || abs(pdgIdDaughter)==1000023 || abs(pdgIdDaughter)==1000025 || abs(pdgIdDaughter)==1000035) {
	  
	  DecayToLSP = true;
	  continue;
	  
	}
	
      }
      
      if (DecayToLSP) jndex++;
      
    } else if (abs(pdgId) == 1000022 || abs(pdgId) == 1000023 || abs(pdgId) == 1000025 || abs(pdgId) == 1000035) { // LSP
      
      if (genParticles_[gp]-> numberOfMothers() < 1) continue;
      int pdgIdMother = (genParticles_[gp]->mother())->pdgId();
      if (abs(pdgIdMother)==1000006 || abs(pdgIdMother)==2000006)
	jndex++;
      
    }
    
    if (jndex>=0 && abs(jndex)==index) {
      
      const reco::Candidate* mcP = &(*(genParticles_[gp]));
      return mcP->pt();
      
    }

  } // loop over gen particles
  
  if (jndex<0) return -999.;
  else return -100.*(jndex+1);
  
}


const float reco::SkimEvent::susyParticleEta(size_t index) const {
  
  int jndex = -1;

  // loop over gen particles
  for(size_t gp=0; gp<genParticles_.size();++gp) {
    
    int pdgId = genParticles_[gp] -> pdgId();
    //int status = genParticles_[gp] -> status();
    
    if (abs(pdgId) == 1000006 || abs(pdgId) == 2000006) { // Stop
      
      bool DecayToLSP = false;
      
      int ndaughters = genParticles_[gp]->numberOfDaughters();
     
      for (int id=0; id<ndaughters; id++) {
	
	int pdgIdDaughter = (genParticles_[gp]->daughter(id))->pdgId();
	
	if (abs(pdgIdDaughter)==1000022 || abs(pdgIdDaughter)==1000023 || abs(pdgIdDaughter)==1000025 || abs(pdgIdDaughter)==1000035) {
	  
	  DecayToLSP = true;
	  continue;
	  
	}
	
      }
      
      if (DecayToLSP) jndex++;
      
    } else if (abs(pdgId) == 1000022 || abs(pdgId) == 1000023 || abs(pdgId) == 1000025 || abs(pdgId) == 1000035) { // LSP
      
      if (genParticles_[gp]-> numberOfMothers() < 1) continue;
      int pdgIdMother = (genParticles_[gp]->mother())->pdgId();
      if (abs(pdgIdMother)==1000006 || abs(pdgIdMother)==2000006)
	jndex++;
      
    }
    
    if (jndex>=0 && abs(jndex)==index) {
      
      const reco::Candidate* mcP = &(*(genParticles_[gp]));
      return mcP->eta();
      
    }

  } // loop over gen particles
  
  if (jndex<0) return -999.;
  else return -100.*(jndex+1);
  
}


const float reco::SkimEvent::susyParticlePhi(size_t index) const {
  
  int jndex = -1;

  // loop over gen particles
  for(size_t gp=0; gp<genParticles_.size();++gp) {
    
    int pdgId = genParticles_[gp] -> pdgId();
    //int status = genParticles_[gp] -> status();
    
    if (abs(pdgId) == 1000006 || abs(pdgId) == 2000006) { // Stop
      
      bool DecayToLSP = false;
      
      int ndaughters = genParticles_[gp]->numberOfDaughters();
     
      for (int id=0; id<ndaughters; id++) {
	
	int pdgIdDaughter = (genParticles_[gp]->daughter(id))->pdgId();
	
	if (abs(pdgIdDaughter)==1000022 || abs(pdgIdDaughter)==1000023 || abs(pdgIdDaughter)==1000025 || abs(pdgIdDaughter)==1000035) {
	  
	  DecayToLSP = true;
	  continue;
	  
	}
	
      }
      
      if (DecayToLSP) jndex++;
      
    } else if (abs(pdgId) == 1000022 || abs(pdgId) == 1000023 || abs(pdgId) == 1000025 || abs(pdgId) == 1000035) { // LSP
      
      if (genParticles_[gp]-> numberOfMothers() < 1) continue;
      int pdgIdMother = (genParticles_[gp]->mother())->pdgId();
      if (abs(pdgIdMother)==1000006 || abs(pdgIdMother)==2000006)
	jndex++;
      
    }
    
    if (jndex>=0 && abs(jndex)==index) {
      
      const reco::Candidate* mcP = &(*(genParticles_[gp]));
      return mcP->phi();
      
    }

  } // loop over gen particles
  
  if (jndex<0) return -999.;
  else return -100.*(jndex+1);

}


const int reco::SkimEvent::susyParticleID(size_t index) const {
  
  int jndex = -1;

  // loop over gen particles
  for(size_t gp=0; gp<genParticles_.size();++gp) {
    
    int pdgId = genParticles_[gp] -> pdgId();
    //int status = genParticles_[gp] -> status();
    
    if (abs(pdgId) == 1000006 || abs(pdgId) == 2000006) { // Stop
      
      bool DecayToLSP = false;
      
      int ndaughters = genParticles_[gp]->numberOfDaughters();
     
      for (int id=0; id<ndaughters; id++) {
	
	int pdgIdDaughter = (genParticles_[gp]->daughter(id))->pdgId();
	
	if (abs(pdgIdDaughter)==1000022 || abs(pdgIdDaughter)==1000023 || abs(pdgIdDaughter)==1000025 || abs(pdgIdDaughter)==1000035) {
	  
	  DecayToLSP = true;
	  continue;
	  
	}
	
      }
      
      if (DecayToLSP) jndex++;
      
    } else if (abs(pdgId) == 1000022 || abs(pdgId) == 1000023 || abs(pdgId) == 1000025 || abs(pdgId) == 1000035) { // LSP
      
      if (genParticles_[gp]-> numberOfMothers() < 1) continue;
      int pdgIdMother = (genParticles_[gp]->mother())->pdgId();
      if (abs(pdgIdMother)==1000006 || abs(pdgIdMother)==2000006)
	jndex++;
      
    }
    
    if (jndex>=0 && abs(jndex)==index) {
      
      const reco::Candidate* mcP = &(*(genParticles_[gp]));
      return pdgId;
      
    }

  } // loop over gen particles
  
  if (jndex<0) return -999.;
  else return -100.*(jndex+1);

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
    if (LHEhepeup_.ISTUP.at (iPart) != 1) continue ; //incoming particle / beam, we only want intermediate or outgoing particles
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
  if (pt_ofIndex != -9999.9){
    // loop over particles in the event
    for (unsigned int iPart = 0 ; iPart < LHEhepeup_.IDUP.size (); ++iPart) {
      if (LHEhepeup_.ISTUP.at (iPart) != 1) continue ; //incoming particle / beam, we only want intermediate or outgoing particles
      int type = abs (LHEhepeup_.IDUP.at (iPart)) ;
      if ((type < 9 && type > 0) || type == 21) {
        float iPart_Pt = sqrt (LHEhepeup_.PUP.at (iPart) [0] * LHEhepeup_.PUP.at (iPart) [0] + // px
        LHEhepeup_.PUP.at (iPart) [1] * LHEhepeup_.PUP.at (iPart) [1]); // py
        if(iPart_Pt!=pt_ofIndex) continue;
        particleID = (float) type;
      }
    }
  }
  //---- now return ----
  return particleID;
}

const float reco::SkimEvent::leadingLHEJetPhi(size_t index) const {
  float pt_ofIndex = leadingLHEJetPt(index);
  float phi=-9999.9;
  if (pt_ofIndex != -9999.9){
    // loop over particles in the event
    for (unsigned int iPart = 0 ; iPart < LHEhepeup_.IDUP.size (); ++iPart) {
      if (LHEhepeup_.ISTUP.at (iPart) != 1) continue ; //incoming particle / beam, we only want intermediate or outgoing particles
      int type = abs (LHEhepeup_.IDUP.at (iPart)) ;
      if ((type < 9 && type > 0) || type == 21) {
        float iPart_Pt = sqrt (LHEhepeup_.PUP.at (iPart) [0] * LHEhepeup_.PUP.at (iPart) [0] + // px
        LHEhepeup_.PUP.at (iPart) [1] * LHEhepeup_.PUP.at (iPart) [1]); // py
        if(iPart_Pt!=pt_ofIndex) continue;
        TVector3 pt_ofIndex(LHEhepeup_.PUP.at (iPart) [0], LHEhepeup_.PUP.at (iPart) [1], LHEhepeup_.PUP.at (iPart) [2] ); // pass px, py, pz
        phi = pt_ofIndex.Phi() ;
      }
    }
  }
  //---- now return ----
  return phi;
}

const float reco::SkimEvent::leadingLHEJetEta(size_t index) const {
  float pt_ofIndex = leadingLHEJetPt(index);
  float eta=-9999.9;
  if (pt_ofIndex != -9999.9){
    // loop over particles in the event
    for (unsigned int iPart = 0 ; iPart < LHEhepeup_.IDUP.size (); ++iPart) {
      if (LHEhepeup_.ISTUP.at (iPart) != 1) continue ; //incoming particle / beam, we only want intermediate or outgoing particles
      int type = abs (LHEhepeup_.IDUP.at (iPart)) ;
      if ((type < 9 && type > 0) || type == 21) {
        float iPart_Pt = sqrt (LHEhepeup_.PUP.at (iPart) [0] * LHEhepeup_.PUP.at (iPart) [0] + // px
        LHEhepeup_.PUP.at (iPart) [1] * LHEhepeup_.PUP.at (iPart) [1]); // py
        if(iPart_Pt!=pt_ofIndex) continue;
        TVector3 pt_ofIndex(LHEhepeup_.PUP.at (iPart) [0], LHEhepeup_.PUP.at (iPart) [1], LHEhepeup_.PUP.at (iPart) [2] ); // pass px, py, pz
        eta = pt_ofIndex.PseudoRapidity() ;
      }
    }
  }
  //---- now return ----
  return eta;
}

const float reco::SkimEvent::leadingLHELeptonPt(size_t index) const {
  std::vector<float> v_jetsLHE_pt ;
  // loop over particles in the event
  for (unsigned int iPart = 0 ; iPart < LHEhepeup_.IDUP.size (); ++iPart) {
    if (LHEhepeup_.ISTUP.at (iPart) != 1) continue ; //incoming particle / beam, we only want intermediate or outgoing particles
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
  if (pt_ofIndex != -9999.9){
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
        particleID = (float) LHEhepeup_.IDUP.at (iPart);
        //std::cout << "particleID " << particleID << std::endl;
      }
    }
  }
  //---- now return ----
  return particleID;
}

const float reco::SkimEvent::leadingLHELeptonPhi(size_t index) const {
  float pt_ofIndex = leadingLHELeptonPt(index);
  float phi=-9999.9;
  if (pt_ofIndex != -9999.9){
    // loop over particles in the event
    for (unsigned int iPart = 0 ; iPart < LHEhepeup_.IDUP.size (); ++iPart) {
      if (LHEhepeup_.ISTUP.at (iPart) != 1) continue ; //incoming particle / beam, we only want intermediate or outgoing particles
      int type = abs (LHEhepeup_.IDUP.at (iPart)) ;
      if (type == 11 || type == 13 || type == 15) { //---- quarks or gluons
        float iPart_Pt = sqrt (LHEhepeup_.PUP.at (iPart) [0] * LHEhepeup_.PUP.at (iPart) [0] + // px
        LHEhepeup_.PUP.at (iPart) [1] * LHEhepeup_.PUP.at (iPart) [1]); // py
        if(iPart_Pt!=pt_ofIndex) continue;
        TVector3 pt_ofIndex(LHEhepeup_.PUP.at (iPart) [0], LHEhepeup_.PUP.at (iPart) [1], LHEhepeup_.PUP.at (iPart) [2] ); // pass px, py, pz
        phi = pt_ofIndex.Phi() ;
      }
    }
  }
  //---- now return ----
  return phi;
}

const float reco::SkimEvent::leadingLHELeptonEta(size_t index) const {
  float pt_ofIndex = leadingLHELeptonPt(index);
  float eta=-9999.9;
  if (pt_ofIndex != -9999.9){
    // loop over particles in the event
    for (unsigned int iPart = 0 ; iPart < LHEhepeup_.IDUP.size (); ++iPart) {
      if (LHEhepeup_.ISTUP.at (iPart) != 1) continue ; //incoming particle / beam, we only want intermediate or outgoing particles
      int type = abs (LHEhepeup_.IDUP.at (iPart)) ;
      if (type == 11 || type == 13 || type == 15) { //---- quarks or gluons
        float iPart_Pt = sqrt (LHEhepeup_.PUP.at (iPart) [0] * LHEhepeup_.PUP.at (iPart) [0] + // px
        LHEhepeup_.PUP.at (iPart) [1] * LHEhepeup_.PUP.at (iPart) [1]); // py
        if(iPart_Pt!=pt_ofIndex) continue;
        TVector3 pt_ofIndex(LHEhepeup_.PUP.at (iPart) [0], LHEhepeup_.PUP.at (iPart) [1], LHEhepeup_.PUP.at (iPart) [2] ); // pass px, py, pz
        eta = pt_ofIndex.PseudoRapidity() ;
      }
    }
  }
  //---- now return ----
  return eta;
}

const float reco::SkimEvent::leadingLHENeutrinoPt(size_t index) const {
  std::vector<float> v_jetsLHE_pt ;
  // loop over particles in the event
  for (unsigned int iPart = 0 ; iPart < LHEhepeup_.IDUP.size (); ++iPart) {
    if (LHEhepeup_.ISTUP.at (iPart) != 1) continue ; //incoming particle / beam, we only want intermediate or outgoing particles
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
  if (pt_ofIndex != -9999.9){
    // loop over particles in the event
    for (unsigned int iPart = 0 ; iPart < LHEhepeup_.IDUP.size (); ++iPart) {
      if (LHEhepeup_.ISTUP.at (iPart) != 1) continue ; //incoming particle / beam, we only want intermediate or outgoing particles
      int type = abs (LHEhepeup_.IDUP.at (iPart)) ;
      if (type == 12 || type == 14 || type == 16) {
        float iPart_Pt = sqrt (LHEhepeup_.PUP.at (iPart) [0] * LHEhepeup_.PUP.at (iPart) [0] + // px
        LHEhepeup_.PUP.at (iPart) [1] * LHEhepeup_.PUP.at (iPart) [1]); // py
        if(iPart_Pt!=pt_ofIndex) continue;
        particleID = (float) LHEhepeup_.IDUP.at (iPart);
      }
    }
  }
  //---- now return ----
  return particleID;
}

const float reco::SkimEvent::leadingLHENeutrinoPhi(size_t index) const {
  float pt_ofIndex = leadingLHENeutrinoPt(index);
  float phi=-9999.9;
  if (pt_ofIndex != -9999.9){
    // loop over particles in the event
    for (unsigned int iPart = 0 ; iPart < LHEhepeup_.IDUP.size (); ++iPart) {
      if (LHEhepeup_.ISTUP.at (iPart) != 1) continue ; //incoming particle / beam, we only want intermediate or outgoing particles
      int type = abs (LHEhepeup_.IDUP.at (iPart)) ;
      if (type == 12 || type == 14 || type == 16) {
        float iPart_Pt = sqrt (LHEhepeup_.PUP.at (iPart) [0] * LHEhepeup_.PUP.at (iPart) [0] + // px
        LHEhepeup_.PUP.at (iPart) [1] * LHEhepeup_.PUP.at (iPart) [1]); // py
        if(iPart_Pt!=pt_ofIndex) continue;
        TVector3 pt_ofIndex(LHEhepeup_.PUP.at (iPart) [0], LHEhepeup_.PUP.at (iPart) [1], LHEhepeup_.PUP.at (iPart) [2] ); // pass px, py, pz
        phi = pt_ofIndex.Phi() ;
      }
    }
  }
  //---- now return ----
  return phi;
}

const float reco::SkimEvent::leadingLHENeutrinoEta(size_t index) const {
  float pt_ofIndex = leadingLHENeutrinoPt(index);
  float eta=-9999.9;
  if (pt_ofIndex != -9999.9){
    // loop over particles in the event
    for (unsigned int iPart = 0 ; iPart < LHEhepeup_.IDUP.size (); ++iPart) {
      if (LHEhepeup_.ISTUP.at (iPart) != 1) continue ; //incoming particle / beam, we only want intermediate or outgoing particles
      int type = abs (LHEhepeup_.IDUP.at (iPart)) ;
      if (type == 12 || type == 14 || type == 16) {
        float iPart_Pt = sqrt (LHEhepeup_.PUP.at (iPart) [0] * LHEhepeup_.PUP.at (iPart) [0] + // px
        LHEhepeup_.PUP.at (iPart) [1] * LHEhepeup_.PUP.at (iPart) [1]); // py
        if(iPart_Pt!=pt_ofIndex) continue;
        TVector3 pt_ofIndex(LHEhepeup_.PUP.at (iPart) [0], LHEhepeup_.PUP.at (iPart) [1], LHEhepeup_.PUP.at (iPart) [2] ); // pass px, py, pz
        eta = pt_ofIndex.PseudoRapidity() ;
      }
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
    if (LHEhepeup_.ISTUP.at (iPart) != 1) continue ; //incoming particle / beam, we only want intermediate or outgoing particles
    int type = abs (LHEhepeup_.IDUP.at (iPart)) ;
    if (type == 12 || type == 14 || type == 16) { //---- neutrinos
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
    if (LHEhepeup_.ISTUP.at (iPart) != 1) continue ; //incoming particle / beam, we only want intermediate or outgoing particles
    int type = abs (LHEhepeup_.IDUP.at (iPart)) ;
    if (type == 12 || type == 14 || type == 16) { //---- neutrinos
      sum_px+=LHEhepeup_.PUP.at (iPart) [0];
      sum_py+=LHEhepeup_.PUP.at (iPart) [1];
      sum_pz+=LHEhepeup_.PUP.at (iPart) [2];
      number_neutrino++;
    }
  }
  //now return
  if(number_neutrino==0) return phi;
  TVector3 pt_ofIndex(sum_px, sum_py, sum_pz ); // pass px, py, pz
  if (pt_ofIndex.Pt() > 0) phi = pt_ofIndex.Phi() ;
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
    //   std::cout << " LHEhepeup_.ISTUP.at (" << iPart << ") = " << LHEhepeup_.ISTUP.at (iPart) << " LHEhepeup_.IDUP.at (" << iPart << ") = " << LHEhepeup_.IDUP.at (iPart)  << std::endl;
    if (LHEhepeup_.ISTUP.at (iPart) != 1) continue ; //incoming particle / beam, we only want intermediate or outgoing particles
    int type = abs (LHEhepeup_.IDUP.at (iPart)) ;
    if (type == 12 || type == 14 || type == 16) { //---- neutrinos
      sum_px+=LHEhepeup_.PUP.at (iPart) [0];
      sum_py+=LHEhepeup_.PUP.at (iPart) [1];
      sum_pz+=LHEhepeup_.PUP.at (iPart) [2];
      number_neutrino++;
    }
  }
  //---- now return ----
  if (number_neutrino==0) return eta;
  TVector3 pt_ofIndex(sum_px, sum_py, sum_pz ); // pass px, py, pz
  if (pt_ofIndex.Pt()>0) eta = pt_ofIndex.Eta() ;
  return eta;
}

const float reco::SkimEvent::higgsLHEPt() const {
  std::vector<float> v_jetsLHE_pt ;
  // loop over particles in the event
  for (unsigned int iPart = 0 ; iPart < LHEhepeup_.IDUP.size (); ++iPart) {
    //   if (LHEhepeup_.ISTUP.at (iPart) != 1) continue ;
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
    //   if (LHEhepeup_.ISTUP.at (iPart) != 1) continue ;
    int type = abs (LHEhepeup_.IDUP.at (iPart)) ;
    if (type ==25) { //---- Higgs
      TVector3 temp_vector(LHEhepeup_.PUP.at (iPart) [0], LHEhepeup_.PUP.at (iPart) [1], LHEhepeup_.PUP.at (iPart) [2] ); // pass px, py, pz
      if (temp_vector.Pt() > 0) v_particleLHE_eta.push_back(temp_vector.Eta());
      else v_particleLHE_eta.push_back(-9999.9);
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
    //   if (LHEhepeup_.ISTUP.at (iPart) != 1) continue ;
    int type = abs (LHEhepeup_.IDUP.at (iPart)) ;
    if (type ==25) { //---- Higgs
      TVector3 temp_vector(LHEhepeup_.PUP.at (iPart) [0], LHEhepeup_.PUP.at (iPart) [1], LHEhepeup_.PUP.at (iPart) [2] ); // pass px, py, pz
      if (temp_vector.Pt() > 0) v_particleLHE_phi.push_back(temp_vector.Phi());
      else v_particleLHE_phi.push_back(-9999.9);
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
    //   if (LHEhepeup_.ISTUP.at (iPart) != 1) continue ;
    int type = abs (LHEhepeup_.IDUP.at (iPart)) ;
    //   std::cout << " type = " << type << std::endl;
    if (type ==25) { //---- Higgs
      v_particleLHE_mass.push_back(LHEhepeup_.PUP.at (iPart) [4]); //---- mass
      //---- see http://home.thep.lu.se/~leif/LHEF/classLHEF_1_1HEPEUP.html
    }
  }
  //---- now return ----
  if ( 0 < v_particleLHE_mass.size() ) return v_particleLHE_mass.at(0);
  return -9999.9; //if no Higgs was found
}


//---- end LHE information


//------------------------------------------------------------------------------
// leadingGenJetPartonPt
//------------------------------------------------------------------------------
const float reco::SkimEvent::leadingGenJetPartonPt(size_t index) const
{
  std::vector<float> v_jets_pt;

  float pt = defaultvalues::defaultFloat;

  const reco::Candidate* mcH = 0;

  // Loop over gen particles
  for (size_t gp=0; gp<genParticles_.size();++gp) {

    int type = abs(genParticles_[gp]->pdgId());

    if (type < 1) continue;
    if (type > 8 && type != 21) continue;
    if (!genParticles_[gp]->isHardProcess() && !genParticles_[gp]->statusFlags().isPrompt()) continue;
    if (!genParticles_[gp]->isHardProcess() && type == 21) continue;
    if (!genParticles_[gp]->isHardProcess() && type  <  5) continue;

    int ndaughters = genParticles_[gp]->numberOfDaughters();

    bool rejectMe = false;

    if (!genParticles_[gp]->isHardProcess())
      {
        for (int id=0; id<ndaughters; id++)
          {
            if ((genParticles_[gp]->daughter(id))->pdgId() == genParticles_[gp]->pdgId()) rejectMe = true;
          }
      }

    if (rejectMe) continue;

    mcH = &(*(genParticles_[gp]));
    v_jets_pt.push_back(mcH->pt());
  }

  if (v_jets_pt.size () > 0) std::sort(v_jets_pt.rbegin(), v_jets_pt.rend());

  size_t count = 0;

  for (size_t i=0;i<v_jets_pt.size();++i) {
    if (++count > index) return v_jets_pt.at(i);
  }

  return pt;
}


//------------------------------------------------------------------------------
// leadingGenJetPartonPID
//------------------------------------------------------------------------------
const float reco::SkimEvent::leadingGenJetPartonPID(size_t index) const {
  float pt_ofIndex = leadingGenJetPartonPt(index);
  float particleID= defaultvalues::defaultFloat;
  const reco::Candidate* mcH = 0;
  // loop over gen particles
  for(size_t gp=0; gp<genParticles_.size();++gp){
    int type = abs( genParticles_[gp] -> pdgId() );

    if (type < 1) continue;
    if (type > 8 && type != 21) continue;
    if (!genParticles_[gp]->isHardProcess() && !genParticles_[gp]->statusFlags().isPrompt()) continue;
    if (!genParticles_[gp]->isHardProcess() && type == 21) continue;
    if (!genParticles_[gp]->isHardProcess() && type  <  5) continue;

    int ndaughters = genParticles_[gp]->numberOfDaughters();

    bool rejectMe = false;

    if (!genParticles_[gp]->isHardProcess())
      {
	for (int id=0; id<ndaughters; id++)
	  {
	    if ((genParticles_[gp]->daughter(id))->pdgId() == genParticles_[gp]->pdgId()) rejectMe = true;
	  }
      }

    if (rejectMe) continue;

    mcH = &(*(genParticles_[gp]));
    if( mcH->pt() != pt_ofIndex) continue;
    particleID = (float) genParticles_[gp]->pdgId();
    break;
  } // loop over gen particles
  return particleID;
}


//------------------------------------------------------------------------------
// leadingGenJetPartonEta
//------------------------------------------------------------------------------
const float reco::SkimEvent::leadingGenJetPartonEta(size_t index) const {
  float pt_ofIndex = leadingGenJetPartonPt(index);
  float particleEta= defaultvalues::defaultFloat;
  const reco::Candidate* mcH = 0;
  // loop over gen particles
  for(size_t gp=0; gp<genParticles_.size();++gp){
    int type = abs( genParticles_[gp] -> pdgId() );

    if (type < 1) continue;
    if (type > 8 && type != 21) continue;
    if (!genParticles_[gp]->isHardProcess() && !genParticles_[gp]->statusFlags().isPrompt()) continue;
    if (!genParticles_[gp]->isHardProcess() && type == 21) continue;
    if (!genParticles_[gp]->isHardProcess() && type  <  5) continue;

    int ndaughters = genParticles_[gp]->numberOfDaughters();

    bool rejectMe = false;

    if (!genParticles_[gp]->isHardProcess())
      {
	for (int id=0; id<ndaughters; id++)
	  {
	    if ((genParticles_[gp]->daughter(id))->pdgId() == genParticles_[gp]->pdgId()) rejectMe = true;
	  }
      }

    if (rejectMe) continue;

    mcH = &(*(genParticles_[gp]));
    if( mcH->pt() != pt_ofIndex) continue;
    particleEta = (float) mcH->eta();
    break;
  } // loop over gen particles
  return particleEta;
}


//------------------------------------------------------------------------------
// leadingGenJetPartonPhi
//------------------------------------------------------------------------------
const float reco::SkimEvent::leadingGenJetPartonPhi(size_t index) const {
  float pt_ofIndex = leadingGenJetPartonPt(index);
  float particlePhi= defaultvalues::defaultFloat;
  const reco::Candidate* mcH = 0;
  // loop over gen particles
  for(size_t gp=0; gp<genParticles_.size();++gp){
    int type = abs( genParticles_[gp] -> pdgId() );

    if (type < 1) continue;
    if (type > 8 && type != 21) continue;
    if (!genParticles_[gp]->isHardProcess() && !genParticles_[gp]->statusFlags().isPrompt()) continue;
    if (!genParticles_[gp]->isHardProcess() && type == 21) continue;
    if (!genParticles_[gp]->isHardProcess() && type  <  5) continue;

    int ndaughters = genParticles_[gp]->numberOfDaughters();

    bool rejectMe = false;

    if (!genParticles_[gp]->isHardProcess())
      {
	for (int id=0; id<ndaughters; id++)
	  {
	    if ((genParticles_[gp]->daughter(id))->pdgId() == genParticles_[gp]->pdgId()) rejectMe = true;
	  }
      }

    if (rejectMe) continue;

    mcH = &(*(genParticles_[gp]));
    if( mcH->pt() != pt_ofIndex) continue;
    particlePhi = (float) mcH->phi();
    break;
  } // loop over gen particles
  return particlePhi;
}


//------------------------------------------------------------------------------
// leadingGenJetPartonIsPrompt
//------------------------------------------------------------------------------
const float reco::SkimEvent::leadingGenJetPartonIsPrompt(size_t index) const {
  float pt_ofIndex = leadingGenJetPartonPt(index);
  float flag= defaultvalues::defaultFloat;
  const reco::Candidate* mcH = 0;
  // loop over gen particles
  for(size_t gp=0; gp<genParticles_.size();++gp){
    int type = abs( genParticles_[gp] -> pdgId() );

    if (type < 1) continue;
    if (type > 8 && type != 21) continue;
    if (!genParticles_[gp]->isHardProcess() && !genParticles_[gp]->statusFlags().isPrompt()) continue;
    if (!genParticles_[gp]->isHardProcess() && type == 21) continue;
    if (!genParticles_[gp]->isHardProcess() && type  <  5) continue;

    int ndaughters = genParticles_[gp]->numberOfDaughters();

    bool rejectMe = false;

    if (!genParticles_[gp]->isHardProcess())
      {
	for (int id=0; id<ndaughters; id++)
	  {
	    if ((genParticles_[gp]->daughter(id))->pdgId() == genParticles_[gp]->pdgId()) rejectMe = true;
	  }
      }

    if (rejectMe) continue;

    mcH = &(*(genParticles_[gp]));
    if( mcH->pt() != pt_ofIndex) continue;
    flag = (float) genParticles_[gp]->statusFlags().isPrompt();
    break;
  } // loop over gen particles
  return flag;
}


//------------------------------------------------------------------------------
// leadingGenJetPartonIsHardProcess
//------------------------------------------------------------------------------
const float reco::SkimEvent::leadingGenJetPartonIsHardProcess(size_t index) const {
  float pt_ofIndex = leadingGenJetPartonPt(index);
  float flag= defaultvalues::defaultFloat;
  const reco::Candidate* mcH = 0;
  // loop over gen particles
  for(size_t gp=0; gp<genParticles_.size();++gp){
    int type = abs( genParticles_[gp] -> pdgId() );

    if (type < 1) continue;
    if (type > 8 && type != 21) continue;
    if (!genParticles_[gp]->isHardProcess() && !genParticles_[gp]->statusFlags().isPrompt()) continue;
    if (!genParticles_[gp]->isHardProcess() && type == 21) continue;
    if (!genParticles_[gp]->isHardProcess() && type  <  5) continue;

    int ndaughters = genParticles_[gp]->numberOfDaughters();

    bool rejectMe = false;

    if (!genParticles_[gp]->isHardProcess())
      {
	for (int id=0; id<ndaughters; id++)
	  {
	    if ((genParticles_[gp]->daughter(id))->pdgId() == genParticles_[gp]->pdgId()) rejectMe = true;
	  }
      }

    if (rejectMe) continue;

    mcH = &(*(genParticles_[gp]));
    if( mcH->pt() != pt_ofIndex) continue;
    flag = (float) genParticles_[gp]->statusFlags().isHardProcess();
    break;
  } // loop over gen particles
  return flag;
}


//---- Vector Bosons

const float reco::SkimEvent::genVBosonPt(size_t index) const {
  
  std::vector<float> v_bosons_pt;
  
  float particlePt = defaultvalues::defaultFloat;
  
  const reco::Candidate* mcH = 0;
  
  // Loop over gen particles
  for (size_t gp=0; gp<genParticles_.size(); ++gp) {
    int type = abs(genParticles_[gp]->pdgId());
    
    if ( !((type == 23 || type == 24) && genParticles_[gp]->isHardProcess()) ) continue;
    
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
    
    if ( !((type == 23 || type == 24) && genParticles_[gp]->isHardProcess()) ) continue;
    
    mcH = &(*(genParticles_[gp]));
    if (mcH->pt() != pt_ofIndex) continue;
    particleStatus = (float) genParticles_[gp]->status();
    break;
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
    
    if ( !((abs(type) == 23 || abs(type) == 24) && genParticles_[gp]->isHardProcess()) ) continue;
    
    mcH = &(*(genParticles_[gp]));
    if (mcH->pt() != pt_ofIndex) continue;
    
    particleID = (float) type;
    break;
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
    
    if ( !((type == 23 || type == 24) && genParticles_[gp]->isHardProcess()) ) continue;
    
    mcH = &(*(genParticles_[gp]));
    if (mcH->pt() != pt_ofIndex) continue;
    particleEta = (float) mcH->eta();
    break;
  }
  
  return particleEta;
}


// Compatible with PYTHIA8
const float reco::SkimEvent::genVBosonMass(size_t index) const {
  
  float pt_ofIndex  = genVBosonPt(index);
  float particleMass = defaultvalues::defaultFloat;
  
  const reco::Candidate* mcH = 0;
  
  // Loop over gen particles
  for (size_t gp=0; gp<genParticles_.size(); ++gp) {
    int type = abs(genParticles_[gp]->pdgId());
    
    if ( !((type == 23 || type == 24) && genParticles_[gp]->isHardProcess()) ) continue;
    
    mcH = &(*(genParticles_[gp]));
    if (mcH->pt() != pt_ofIndex) continue;
    particleMass = (float) mcH->mass();
    break;
  }
  
  return particleMass;
}


const float reco::SkimEvent::genVBosonPhi(size_t index) const {
  
  float pt_ofIndex  = genVBosonPt(index);
  float particlePhi = defaultvalues::defaultFloat;
  
  const reco::Candidate* mcH = 0;
  
  // Loop over gen particles
  for (size_t gp=0; gp<genParticles_.size(); ++gp) {
    int type = abs(genParticles_[gp]->pdgId());
    
    if ( !((type == 23 || type == 24) && genParticles_[gp]->isHardProcess()) ) continue;
    
    mcH = &(*(genParticles_[gp]));
    if (mcH->pt() != pt_ofIndex) continue;
    particlePhi = (float) mcH->phi();
    break;
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


const float reco::SkimEvent::genLeptonPt(size_t index) const {
  if( index < leptonIndices.size() ) 
    return genParticles_[leptonIndices[index]]->pt();
  else 
    return defaultvalues::defaultFloat;
}

const float reco::SkimEvent::genLeptonStatus(size_t index) const {
  if( index < leptonIndices.size() ) 
    return genParticles_[leptonIndices[index]]->status();
  else 
    return defaultvalues::defaultFloat;
}

const float reco::SkimEvent::genLeptonIsPrompt (size_t index) const {
  if( index < leptonIndices.size() ) 
    return genParticles_[leptonIndices[index]]->statusFlags().isPrompt();
  else 
    return defaultvalues::defaultFloat;
}

const float reco::SkimEvent::genLeptonIsDirectPromptTauDecayProduct (size_t index) const {
  if( index < leptonIndices.size() ) 
    return genParticles_[leptonIndices[index]]->statusFlags().isDirectPromptTauDecayProduct();
  else 
    return defaultvalues::defaultFloat;
}

const float reco::SkimEvent::genLeptonIsTauDecayProduct (size_t index) const {
  if( index < leptonIndices.size() ) 
    return genParticles_[leptonIndices[index]]->statusFlags().isTauDecayProduct();
  else 
    return defaultvalues::defaultFloat;
}

const float reco::SkimEvent::genLeptonIsDirectHadronDecayProduct (size_t index) const {
  if( index < leptonIndices.size() ) 
    return genParticles_[leptonIndices[index]]->statusFlags().isDirectHadronDecayProduct();
  else 
    return defaultvalues::defaultFloat;
}

const float reco::SkimEvent::genLeptonFromHardProcess (size_t index) const
{
  if( index < leptonIndices.size() ) 
    return genParticles_[leptonIndices[index]]->statusFlags().fromHardProcess();
  else 
    return defaultvalues::defaultFloat;
}

// Compatible with PYTHIA8
const float reco::SkimEvent::genLeptonIndex(size_t index) const {
  
  float pt_ofIndex    = genLeptonPt(index);
  float particleIndex = defaultvalues::defaultFloat;
  
  const reco::Candidate* mcH = 0;
  
  // Loop over gen particles
  for (size_t gp=0; gp<genParticles_.size(); ++gp) {
    int type = abs(genParticles_[gp]->pdgId());
    
    if( !((type == 11 || type == 13) && genParticles_[gp]->status()==1 ) && !(type == 15 && genParticles_[gp]->isPromptDecayed()) ) continue;
    
    mcH = &(*(genParticles_[gp]));
    if (mcH->pt() != pt_ofIndex) continue;
    particleIndex = (float) gp;
    break;
  }
  
  return particleIndex;
}


// Compatible with PYTHIA8
const float reco::SkimEvent::genLeptonPID(size_t index) const {
  if( index < leptonIndices.size() ) 
    return genParticles_[leptonIndices[index]]->pdgId();
  else 
    return defaultvalues::defaultFloat;
}


// Compatible with PYTHIA8
const float reco::SkimEvent::genLeptonEta(size_t index) const {
  if( index < leptonIndices.size() ) 
    return genParticles_[leptonIndices[index]]->eta();
  else 
    return defaultvalues::defaultFloat;
}


// Compatible with PYTHIA8
const float reco::SkimEvent::genLeptonPhi(size_t index) const {
  if( index < leptonIndices.size() ) 
    return genParticles_[leptonIndices[index]]->phi();
  else 
    return defaultvalues::defaultFloat;
}


// Compatible with PYTHIA8
const float reco::SkimEvent::genLeptonMotherPID(size_t index) const {
  
  float pt_ofIndex = genLeptonPt(index);
  float motherPID  = defaultvalues::defaultFloat;
  int   motherPID_int    = defaultvalues::defaultInt;
  
  const reco::Candidate* mcH = 0;
  
  // Loop over gen particles
  for (size_t gp=0; gp<genParticles_.size(); ++gp) {
    // Set default
    //motherStatus_int = defaultvalues::defaultInt;
    motherPID_int    = defaultvalues::defaultInt;
    
    int type = genParticles_[gp]->pdgId();
    if( !(( abs(type) == 11 || abs(type) == 13) && genParticles_[gp]->status()==1 ) && !( abs(type) == 15 && genParticles_[gp]->isPromptDecayed()) ) continue;
    
    const reco::Candidate* pMother = 0;
    if (genParticles_[gp]-> numberOfMothers() < 1) continue;
    pMother       = genParticles_[gp]->mother();
    motherPID_int = pMother->pdgId();
    while (motherPID_int == type) {
      if (pMother -> numberOfMothers() < 1) break;
      pMother       = pMother->mother();
      motherPID_int = pMother->pdgId();
    }
    
    mcH = &(*(genParticles_[gp]));
    if (mcH->pt() != pt_ofIndex) continue;
    motherPID = (float) motherPID_int;
    break;
  }
  
  return motherPID;
}


// Compatible with PYTHIA8
const float reco::SkimEvent::genLeptonMotherStatus(size_t index) const {
  
  float pt_ofIndex       = genLeptonPt(index);
  float motherStatus     = defaultvalues::defaultFloat;
  int   motherStatus_int = defaultvalues::defaultInt;
  //float motherPID        = defaultvalues::defaultFloat;
  int   motherPID_int    = defaultvalues::defaultInt;
  
  const reco::Candidate* mcH = 0;
  
  // Loop over gen particles
  for (size_t gp=0; gp<genParticles_.size(); ++gp) {
    
    // Set default
    motherStatus_int = defaultvalues::defaultInt;
    motherPID_int    = defaultvalues::defaultInt;
    
    int type = genParticles_[gp]->pdgId();
    if( !(( abs(type) == 11 || abs(type) == 13) && genParticles_[gp]->status()==1 ) && !( abs(type) == 15 && genParticles_[gp]->isPromptDecayed()) ) continue;
    
    const reco::Candidate* pMother = 0;
    if (genParticles_[gp]-> numberOfMothers() < 1) continue;
    pMother          = genParticles_[gp]->mother();
    motherPID_int    = pMother->pdgId();
    motherStatus_int = pMother->status();
    
    while ( motherPID_int == type) {
      //motherOriginalStatus = originalStatus(pMother);
      if (pMother -> numberOfMothers() < 1) break;
      pMother          = pMother->mother();
      motherPID_int    = pMother->pdgId();
      motherStatus_int = pMother->status();
    }
    
    mcH = &(*(genParticles_[gp]));
    if (mcH->pt() != pt_ofIndex) continue;
    motherStatus = (float) motherStatus_int;
    //motherStatus = (float) motherOriginalStatus;
    break;
  }
  
  return motherStatus;
}

//---- neutrinos

const float reco::SkimEvent::leadingGenNeutrinoIsPrompt(size_t index) const {
  float pt_ofIndex   = leadingGenNeutrinoPt(index);
  const reco::Candidate* mcH = 0;
  
  // Loop over gen particles
  for (size_t gp=0; gp<genParticles_.size(); gp++) {
    int type = abs(genParticles_[gp]->pdgId());
    if( (type == 12 || type == 14 || type == 16) && genParticles_[gp]->status()==1 ) {
      
      mcH = &(*(genParticles_[gp]));
      if (mcH->pt() != pt_ofIndex) continue;
      
      return genParticles_[gp]->statusFlags().isPrompt();
    }
  } 
  
  return defaultvalues::defaultFloat;
}


const float reco::SkimEvent::leadingGenNeutrinoIsDirectPromptTauDecayProduct(size_t index) const {
  float pt_ofIndex   = leadingGenNeutrinoPt(index);
  const reco::Candidate* mcH = 0;
  
  // Loop over gen particles
  for (size_t gp=0; gp<genParticles_.size(); gp++) {
    int type = abs(genParticles_[gp]->pdgId());
    if( (type == 12 || type == 14 || type == 16) && genParticles_[gp]->status()==1 ) {
      
      mcH = &(*(genParticles_[gp]));
      if (mcH->pt() != pt_ofIndex) continue;
      
      return genParticles_[gp]->statusFlags().isDirectPromptTauDecayProduct();
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
    int status = genParticles_[gp] -> status();
    
    // Stop {1000006}
    if( (type == 12 || type == 14 || type == 16) && status==1 ) {
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
    int status = genParticles_[gp] -> status();
    if( (type == 12 || type == 14 || type == 16) && status==1 ) {
      //   if( (type == 12 || type == 14 || type == 16) && (status == 3) ) {
      mcH = &(*(genParticles_[gp]));
      if( mcH->pt() != pt_ofIndex) continue;
      particleID = (float) type;
      break;
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
    if( (type == 12 || type == 14 || type == 16) && status==1 ) {
      //   if( (type == 12 || type == 14 || type == 16) && (status == 3) ) {
      mcH = &(*(genParticles_[gp]));
      if( mcH->pt() != pt_ofIndex) continue;
      particleEta = (float) mcH->eta();
      break;
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
    if( (type == 12 || type == 14 || type == 16) && status==1 ) {
      //   if( (type == 12 || type == 14 || type == 16) && (status == 3) ) {
      mcH = &(*(genParticles_[gp]));
      if( mcH->pt() != pt_ofIndex) continue;
      particlePhi = (float) mcH->phi();
      break;
    }
  } // loop over gen particles
  return particlePhi;
}


const float reco::SkimEvent::leadingGenNeutrinoMotherPID(size_t index) const {
  
  float pt_ofIndex = leadingGenNeutrinoPt(index);
  float motherPID  = defaultvalues::defaultFloat;
  int   motherPID_int    = defaultvalues::defaultInt;
  
  const reco::Candidate* mcH = 0;
  
  // Loop over gen particles
  for (size_t gp=0; gp<genParticles_.size(); ++gp) {
    // Set default
    motherPID_int    = defaultvalues::defaultInt;
    
    int type = genParticles_[gp]->pdgId();
    if( !(( abs(type) == 12 || abs(type) == 14 || abs(type) == 16) && genParticles_[gp]->status()==1 ) ) continue;
    
    const reco::Candidate* pMother = 0;
    if (genParticles_[gp]-> numberOfMothers() < 1) continue;
    pMother       = genParticles_[gp]->mother();
    motherPID_int = pMother->pdgId();
    while ( motherPID_int == type) {
      if (pMother -> numberOfMothers() < 1) break;
      pMother       = pMother->mother();
      motherPID_int = pMother->pdgId();
    }
    
    mcH = &(*(genParticles_[gp]));
    if (mcH->pt() != pt_ofIndex) continue;
    motherPID = (float) motherPID_int;
    break;
  }
  return motherPID;
}

// Compatible with PYTHIA8 
const float reco::SkimEvent::leadingGenNeutrinoMotherStatus(size_t index) const {
  
  float pt_ofIndex       = leadingGenNeutrinoPt(index);
  float motherStatus     = defaultvalues::defaultFloat;
  int   motherStatus_int = defaultvalues::defaultInt;
  
  int   motherPID_int    = defaultvalues::defaultInt;
  
  const reco::Candidate* mcH = 0;
  
  // Loop over gen particles 
  for (size_t gp=0; gp<genParticles_.size(); ++gp) {
    // Set default
    motherStatus_int = defaultvalues::defaultInt;
    motherPID_int    = defaultvalues::defaultInt;
    
    int type = genParticles_[gp]->pdgId();
    if( !(( abs(type) == 12 || abs(type) == 14 || abs(type) == 16) && genParticles_[gp]->status()==1 ) ) continue;
    
    const reco::Candidate* pMother = 0;
    if (genParticles_[gp]-> numberOfMothers() < 1) continue;
    pMother          = genParticles_[gp]->mother();
    motherPID_int    = pMother->pdgId();
    motherStatus_int = pMother->status();
    while ( motherPID_int == type) {
      if (pMother -> numberOfMothers() < 1) break;
      pMother          = pMother->mother();
      motherPID_int    = pMother->pdgId();
      motherStatus_int = pMother->status();
    }
    
    mcH = &(*(genParticles_[gp]));
    if (mcH->pt() != pt_ofIndex) continue;
    motherStatus = (float) motherStatus_int;
    break;
  }
  return motherStatus;
}


//Photons

const float reco::SkimEvent::genPhotonPt(size_t index) const {
  std::vector<float> v_pho_pt ;
  float pt = -9999.9;
  const reco::Candidate* mcH = 0;
  
  // loop over gen particles
  for(size_t gp=0; gp<genParticles_.size();++gp){
    int type = abs( genParticles_[gp] -> pdgId() );
    int status = genParticles_[gp] -> status();
    
    if( type == 22 && status == 1 ) {
      mcH = &(*(genParticles_[gp]));
      v_pho_pt.push_back( mcH->pt() );
    }
  }
  
  if (v_pho_pt.size () > 0) {
    std::sort (v_pho_pt.rbegin (), v_pho_pt.rend ()) ;
  }
  //---- now return ----
  size_t count = 0;
  for(size_t i=0;i<v_pho_pt.size();++i) {
    if(++count > index) return v_pho_pt.at(i);
  }
  return pt;
}


const float reco::SkimEvent::genPhotonPhi(size_t index) const {
  float pt_ofIndex = genPhotonPt(index);
  float particlePhi=-9999.9;
  const reco::Candidate* mcH = 0;
  // loop over gen particles
  for(size_t gp=0; gp<genParticles_.size();++gp){
    int type = abs( genParticles_[gp] -> pdgId() );
    int status = genParticles_[gp] -> status();
    if( type == 22 && status == 1 ) {
      
      mcH = &(*(genParticles_[gp]));
      if( mcH->pt() != pt_ofIndex) continue;
      particlePhi = (float) mcH->phi();
      break;
    }
  } // loop over gen particles
  return particlePhi;
}


const float reco::SkimEvent::genPhotonEta(size_t index) const {
  float pt_ofIndex = genPhotonPt(index);
  float particleEta=-9999.9;
  const reco::Candidate* mcH = 0;
  // loop over gen particles
  for(size_t gp=0; gp<genParticles_.size();++gp){
    int type = abs( genParticles_[gp] -> pdgId() );
    int status = genParticles_[gp] -> status();
    if( type == 22 && status == 1 ) {
      
      mcH = &(*(genParticles_[gp]));
      if( mcH->pt() != pt_ofIndex) continue;
      particleEta = (float) mcH->eta();
      break;
    }
  } // loop over gen particles
  return particleEta;
}


const float reco::SkimEvent::genPhotonMotherPID(size_t index) const {
  
  float pt_ofIndex = genPhotonPt(index);
  float motherPID  = defaultvalues::defaultFloat;
  int   motherPID_int    = defaultvalues::defaultInt;
  
  const reco::Candidate* mcH = 0;
  
  // Loop over gen particles
  for (size_t gp=0; gp<genParticles_.size(); ++gp) {
    // Set default
    motherPID_int    = defaultvalues::defaultInt;
    
    int type = genParticles_[gp]->pdgId();
    if( !( abs(type) == 22 && genParticles_[gp]->status()==1 ) ) continue;
    
    const reco::Candidate* pMother = 0;
    if (genParticles_[gp]-> numberOfMothers() < 1) continue;
    pMother       = genParticles_[gp]->mother();
    motherPID_int = pMother->pdgId();
    while ( motherPID_int == type) {
      if (pMother -> numberOfMothers() < 1) break;
      pMother       = pMother->mother();
      motherPID_int = pMother->pdgId();
    }
    
    mcH = &(*(genParticles_[gp]));
    if (mcH->pt() != pt_ofIndex) continue;
    motherPID = (float) motherPID_int;
    break;
  }
  return motherPID;
}


const float reco::SkimEvent::genPhotonMotherStatus(size_t index) const {
  
  float pt_ofIndex       = genPhotonPt(index);
  float motherStatus     = defaultvalues::defaultFloat;
  int   motherStatus_int = defaultvalues::defaultInt;
  
  int   motherPID_int    = defaultvalues::defaultInt;
  
  const reco::Candidate* mcH = 0;
  
  // Loop over gen particles 
  for (size_t gp=0; gp<genParticles_.size(); ++gp) {
    // Set default
    motherStatus_int = defaultvalues::defaultInt;
    motherPID_int    = defaultvalues::defaultInt;
    
    int type = genParticles_[gp]->pdgId();
    if( !( abs(type) == 22 && genParticles_[gp]->status()==1 ) ) continue;
    
    const reco::Candidate* pMother = 0;
    if (genParticles_[gp]-> numberOfMothers() < 1) continue;
    pMother          = genParticles_[gp]->mother();
    motherPID_int    = pMother->pdgId();
    motherStatus_int = pMother->status();
    while ( motherPID_int == type) {
      if (pMother -> numberOfMothers() < 1) break;
      pMother          = pMother->mother();
      motherPID_int    = pMother->pdgId();
      motherStatus_int = pMother->status();
    }
    
    mcH = &(*(genParticles_[gp]));
    if (mcH->pt() != pt_ofIndex) continue;
    motherStatus = (float) motherStatus_int;
    break;
  }
  return motherStatus;
}


//Met

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
    break;
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
    break;
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

//---- B-tagging
//---- see: https://github.com/alefisico/cmssw/blob/jetToolbox_74X/RecoJets/JetProducers/python/jetToolbox_cff.py#L156

const float reco::SkimEvent::jettcheByPt(size_t i = 0) const {
  return leadingJetBtag(i,"pfTrackCountingHighEffBJetTags",              minPtForJets_,maxEtaForJets_,applyCorrectionForJets_,applyIDForJets_,dzCutForBtagJets_);
}
const float reco::SkimEvent::jettchpByPt(size_t i = 0) const {
  return leadingJetBtag(i,"pfTrackCountingHighPurBJetTags",              minPtForJets_,maxEtaForJets_,applyCorrectionForJets_,applyIDForJets_,dzCutForBtagJets_);
}
const float reco::SkimEvent::jetbjpbByPt(size_t i = 0) const {
  return leadingJetBtag(i,"pfJetBProbabilityBJetTags",                   minPtForJets_,maxEtaForJets_,applyCorrectionForJets_,applyIDForJets_,dzCutForBtagJets_);
}
const float reco::SkimEvent::jetbjpByPt(size_t i = 0) const {
  return leadingJetBtag(i,"pfJetProbabilityBJetTags",                   minPtForJets_,maxEtaForJets_,applyCorrectionForJets_,applyIDForJets_,dzCutForBtagJets_);
}
const float reco::SkimEvent::jetcsvv2ivfByPt(size_t i = 0) const {
  return leadingJetBtag(i,"pfCombinedInclusiveSecondaryVertexV2BJetTags",minPtForJets_,maxEtaForJets_,applyCorrectionForJets_,applyIDForJets_,dzCutForBtagJets_);
}
const float reco::SkimEvent::jetssvheByPt(size_t i = 0) const {
  return leadingJetBtag(i,"pfSimpleSecondaryVertexHighEffBJetTags",      minPtForJets_,maxEtaForJets_,applyCorrectionForJets_,applyIDForJets_,dzCutForBtagJets_);
}
const float reco::SkimEvent::jetssvhbByPt(size_t i = 0) const {
  return leadingJetBtag(i,"pfSimpleSecondaryVertexHighPurBJetTags",      minPtForJets_,maxEtaForJets_,applyCorrectionForJets_,applyIDForJets_,dzCutForBtagJets_);
}
const float reco::SkimEvent::jetpfcsvByPt(size_t i = 0) const {
  return leadingJetBtag(i,"pfCombinedSecondaryVertexBJetTags",           minPtForJets_,maxEtaForJets_,applyCorrectionForJets_,applyIDForJets_,dzCutForBtagJets_);
}
const float reco::SkimEvent::jetcmvav2ByPt(size_t i = 0) const {
  return leadingJetBtag(i,"pfCombinedMVAV2BJetTags",                         minPtForJets_,maxEtaForJets_,applyCorrectionForJets_,applyIDForJets_,dzCutForBtagJets_); 
}
const float reco::SkimEvent::jetpfCombinedCvsLJetTagsByPt(size_t i = 0) const {
  return leadingJetBtag(i,"pfCombinedCvsLJetTags",           minPtForJets_,maxEtaForJets_,applyCorrectionForJets_,applyIDForJets_,dzCutForBtagJets_);
}
const float reco::SkimEvent::jetpfCombinedCvsBJetTagsByPt(size_t i = 0) const {
  return leadingJetBtag(i,"pfCombinedCvsBJetTags",           minPtForJets_,maxEtaForJets_,applyCorrectionForJets_,applyIDForJets_,dzCutForBtagJets_);
}





//---- Photon
void reco::SkimEvent::setPhoton(const edm::Handle<edm::View<reco::RecoCandidate> > &h,size_t i){
  phos_.push_back( h->ptrAt(i) );
}
const pat::Photon * reco::SkimEvent::getPhoton(size_t i) const {
  return getPhoton(phos_[i]);
}
const pat::Photon * reco::SkimEvent::getPhoton(const refToCand &c) const {
  return static_cast<const pat::Photon*>(c.get());
}
const math::XYZTLorentzVector reco::SkimEvent::photon(size_t i) const {
  
  if(indexByPtPho (i) >= phos_.size()) return math::XYZTLorentzVector(0,0,0,0);
  return phos_[indexByPtPho (i)]->p4();
}

const float reco::SkimEvent::photon_ptByPt(size_t i) const
{
  if (indexByPtPho(i) >= phos_.size()) return defaultvalues::defaultFloat;
  
  return phos_[indexByPtPho(i)]->pt();
}

const float reco::SkimEvent::photon_etaByPt(size_t i) const
{
  if (indexByPtPho(i) >= phos_.size()) return defaultvalues::defaultFloat;
  
  return phos_[indexByPtPho(i)]->eta();
}

const float reco::SkimEvent::photon_phiByPt(size_t i) const
{
  if (indexByPtPho(i) >= phos_.size()) return defaultvalues::defaultFloat;
  
  return phos_[indexByPtPho(i)]->phi();
}

const float reco::SkimEvent::photonid_ptByPt(size_t i, int WP) const
{
  if (indexByPtPho(i) >= phos_.size()) return defaultvalues::defaultFloat;
  
  if (Pho_n_ID(WP) < 1) return defaultvalues::defaultFloat;
  
  for (unsigned int a=0; a<phos_.size(); a++)
  {
    if (Pho_IsIdIso(indexByPtPho(a), WP)) return phos_[indexByPtPho(a)]->pt();
  }
  
  return phos_[indexByPtPho(i)]->pt();
}

const float reco::SkimEvent::photonid_etaByPt(size_t i, int WP) const
{
  if (indexByPtPho(i) >= phos_.size()) return defaultvalues::defaultFloat;
  
  if (Pho_n_ID(WP) < 1) return defaultvalues::defaultFloat;
  
  for (unsigned int a=0; a<phos_.size(); a++)
  {
    if (Pho_IsIdIso(indexByPtPho(a), WP)) return phos_[indexByPtPho(a)]->eta();
  }
  
  return phos_[indexByPtPho(i)]->eta();
}

const float reco::SkimEvent::photonid_phiByPt(size_t i, int WP) const
{
  if (indexByPtPho(i) >= phos_.size()) return defaultvalues::defaultFloat;
  
  if (Pho_n_ID(WP) < 1) return defaultvalues::defaultFloat;
  
  for (unsigned int a=0; a<phos_.size(); a++)
  {
    if (Pho_IsIdIso(indexByPtPho(a), WP)) return phos_[indexByPtPho(a)]->phi();
  }
  
  return phos_[indexByPtPho(i)]->phi();
}

//dressed leptons
void reco::SkimEvent::setDressedLepton(const edm::Handle<edm::View<reco::Candidate> > &h,size_t i){
  dressedLeptons_.push_back( h->ptrAt(i) );
}
const reco::Candidate * reco::SkimEvent::getDressedLepton(size_t i) const {
  return getDressedLepton(dressedLeptons_[i]);
}
const reco::Candidate * reco::SkimEvent::getDressedLepton(const edm::Ptr<reco::Candidate> &c) const {
  return static_cast<const reco::Candidate*>(c.get());
}

const size_t reco::SkimEvent::dressed_indexByPt(size_t i) const {
  if( i >= dressedLeptons_.size() ) return 9999; //--> big number then it will fail other tests later! good!
  std::vector<indexValueStruct> a;
  
  for(size_t j=0;j<dressedLeptons_.size();++j) a.push_back(indexValueStruct(dressed_pt(j),j));
  std::sort(a.begin(),a.end(),highToLow);
  
  return a[i].index;
}

const float reco::SkimEvent::dressed_pt(size_t i) const {
  if(i >= dressedLeptons_.size()) return defaultvalues::defaultFloat; //---- -9999.0
  return dressedLeptons_[i]->pt();
}

const float reco::SkimEvent::dressed_eta(size_t i) const {
  if(i >= dressedLeptons_.size()) return defaultvalues::defaultFloat; //---- -9999.0
  return dressedLeptons_[i]->eta();
}

const float reco::SkimEvent::dressed_phi(size_t i) const {
  if(i >= dressedLeptons_.size()) return defaultvalues::defaultFloat; //---- -9999.0
  return dressedLeptons_[i]->phi();
}

const float reco::SkimEvent::dressed_pdgId(size_t i) const {
  if(i >= dressedLeptons_.size()) return defaultvalues::defaultFloat; //---- -9999.0
  return dressedLeptons_[i]->pdgId();
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

void reco::SkimEvent::InitEffectiveAreasElectrons()
{
  // See 2nd table in slide 14 of
  // https://indico.cern.ch/event/369239/contribution/4/attachments/1134761/1623262/talk_effective_areas_25ns.pdf
  
  // 50ns
  _eaElectronIso[0][1] = 0.1733;
  _eaElectronIso[1][1] = 0.1782;
  _eaElectronIso[2][1] = 0.1238;
  _eaElectronIso[3][1] = 0.1571;
  _eaElectronIso[4][1] = 0.2095;
  _eaElectronIso[5][1] = 0.2425;
  _eaElectronIso[6][1] = 0.2935;
  
  // 80X: https://github.com/ikrav/cmssw/blob/egm_id_80X_v1/RecoEgamma/ElectronIdentification/data/Summer16/effAreaElectrons_cone03_pfNeuHadronsAndPhotons_80X.txt
  // 25ns
  _eaElectronIso[0][0] = 0.1703;
  _eaElectronIso[1][0] = 0.1715;
  _eaElectronIso[2][0] = 0.1213;
  _eaElectronIso[3][0] = 0.1230;
  _eaElectronIso[4][0] = 0.1635;
  _eaElectronIso[5][0] = 0.1937;
  _eaElectronIso[6][0] = 0.2393;
}


const float reco::SkimEvent::GetElectronEffectiveArea(size_t i, bool apply50nsValues) const
{
  int use50ns = (apply50nsValues) ? 1 : 0;
  
  if (i >= leps_.size()) return defaultvalues::defaultFloat;
  
  if (isElectron(i))
  {
    float aeta = fabs(leps_.at(i)->eta());
    
    if      (aeta <  1.000)                 return _eaElectronIso[0][use50ns];
    else if (aeta >= 1.000 && aeta < 1.479) return _eaElectronIso[1][use50ns];
    else if (aeta >= 1.479 && aeta < 2.000) return _eaElectronIso[2][use50ns];
    else if (aeta >= 2.000 && aeta < 2.200) return _eaElectronIso[3][use50ns];
    else if (aeta >= 2.200 && aeta < 2.300) return _eaElectronIso[4][use50ns];
    else if (aeta >= 2.300 && aeta < 2.400) return _eaElectronIso[5][use50ns];
    else if (aeta >= 2.400 && aeta < 2.500) return _eaElectronIso[6][use50ns];
    else                                    return defaultvalues::defaultFloat; 
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

void reco::SkimEvent::InitEffectiveAreasPhoton()
{
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
