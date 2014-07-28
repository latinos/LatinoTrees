// #include "LatinoTrees/DataFormats/interface/SkimEvent.h"
#include "DataFormats/Common/interface/Ref.h"
#include "DataFormats/HepMCCandidate/interface/GenParticle.h"
#include "DataFormats/HepMCCandidate/interface/GenParticleFwd.h"
#include "DataFormats/Common/interface/Wrapper.h"
#include "DataFormats/Common/interface/ValueMap.h"
#include "DataFormats/VertexReco/interface/Vertex.h"
#include "DataFormats/VertexReco/interface/VertexFwd.h"
#include "RecoMET/METAlgorithms/interface/PFSpecificAlgo.h"
#include "DataFormats/Common/interface/RefVector.h"
#include <DataFormats/Common/interface/BaseHolder.h>
// #include "CMGTools/External/interface/PileupJetIdentifier.h"

namespace
{
 struct dictionary {

//   reco::SkimEvent dummy1;
//   std::vector<reco::SkimEvent> dummy2;
//   edm::Wrapper<reco::SkimEvent> dummy3;
//   edm::Wrapper<std::vector<reco::SkimEvent> > dummy4;

//   edm::ValueMap<reco::PFMET> mwlDummy01;
//   edm::Wrapper<edm::ValueMap<reco::PFMET> > mwlDummy02;
//   edm::RefVector<std::vector<reco::Vertex> > mwlDummy03;
//   edm::Wrapper<edm::RefVector<std::vector<reco::Vertex> > > mwlDummy04;
  //std::vector<edm::RefToBase<reco::RecoCandidate> > reftobaseRecoCand;
//   edm::RefToBase<reco::RecoCandidate> refToBaseRecoCandidateDummy;
//   edm::RefToBaseVector<reco::RecoCandidate> refToBaseVectorRecoCandidateDummy;
//   edm::Ptr<reco::RecoCandidate> ptrRecoCandidateDummy;
//   std::vector<edm::Ptr<reco::RecoCandidate> > vectorPtrRecoCandidateDummy;
  //edm::reftobase::BaseVectorHolder<reco::RecoCandidate> reftobaseBaseVectorHolderRecoCandidateDummy;
  //edm::reftobase::BaseHolder<reco::RecoCandidate> reftobaseBaseHolderRecoCandidateDummy;

//   edm::Wrapper<edm::ValueMap<StoredPileupJetIdentifier> > DummyStoredPileupJetIdentifier ;
//   std::vector<StoredPileupJetIdentifier> vectorDummyStoredPileupJetIdentifier ;

  std::map<unsigned int,std::vector<std::pair<unsigned int,int> > > test;

 };
}




