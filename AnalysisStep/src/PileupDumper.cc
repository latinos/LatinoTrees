// -*- C++ -*-
//
// Package:    PileUpDumper
// Class:      PileUpDumper
// 
/**\class PileUpDumper PileUpDumper.cc
 * 
 * Description: [one line class summary]
 * 
 * Implementation:
 *     [Notes on implementation]
 */
//
// Original Author:  Andrea Massironi,42 2-027,+41227662346,
//         Created:  lun feb 10 17:03:36 CET 2014
// $Id$
//
//


// system include files
#include <memory>
#include <vector>
#include <string>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Framework/interface/ConsumesCollector.h"
#include "SimDataFormats/PileupSummaryInfo/interface/PileupSummaryInfo.h"

//---- for auto-tree
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"

#include "TTree.h"


//
// class declaration
//

class PileUpDumper : public edm::EDAnalyzer {
public:
 explicit PileUpDumper(const edm::ParameterSet&);
 ~PileUpDumper();
 
 static void fillDescriptions(edm::ConfigurationDescriptions& descriptions);
 
 
private:
 virtual void beginJob() ;
 virtual void analyze(const edm::Event&, const edm::EventSetup&);
 virtual void endJob() ;
 
 virtual void beginRun(edm::Run const&, edm::EventSetup const&);
 virtual void endRun(edm::Run const&, edm::EventSetup const&);
 virtual void beginLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&);
 virtual void endLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&);
 
 // ----------member data ---------------------------
 edm::InputTag _puTag;
 edm::EDGetTokenT<std::vector<PileupSummaryInfo> > _puSummaryT ;
 bool _debug;
 
 TTree* myTree_;
 
 //---- MC qcd scale
 std::vector <double> _puOccupancy;
 //----    -10, -9, ..., -1, 0, 1, 2, ... , 9, 10
 double _trpu;
 
};

//
// constants, enums and typedefs
//

//
// static data member definitions
//

//
// constructors and destructor
//
PileUpDumper::PileUpDumper(const edm::ParameterSet& iConfig)

{
 //now do what ever initialization is needed
 _puTag       = iConfig.getParameter<edm::InputTag>( "puLabel" );
 _debug       = iConfig.getUntrackedParameter< bool >("debug",false);
 _puSummaryT  = consumes<std::vector<PileupSummaryInfo> >(_puTag);
 
 edm::Service<TFileService> fs ;
 myTree_ = fs -> make <TTree>("myTree","myTree");
 
 myTree_ -> Branch("puOccupancy", "std::vector<double>", &_puOccupancy);
 myTree_ -> Branch("trpu", &_trpu, "trpu/D");
 
 
}


PileUpDumper::~PileUpDumper()
{
 
 // do anything here that needs to be done at desctruction time
 // (e.g. close files, deallocate resources etc.)
 
}


//
// member functions
//

// ------------ method called for each event  ------------
void PileUpDumper::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup) {
 
 edm::Handle<std::vector<PileupSummaryInfo> > puInfoH;
 iEvent.getByToken(_puSummaryT,puInfoH);
 
 _puOccupancy.clear();
 for (int j=0; j<20; j++) {
  _puOccupancy.push_back(-1.0);
 }
  
 for(size_t i=0;i<puInfoH->size();++i) {
  for (int j=0; j<20; j++) {
   if( puInfoH->at(i).getBunchCrossing() == (-10 + j) ) {
    _puOccupancy.at(j) = puInfoH->at(i).getPU_NumInteractions();
   }
   if( puInfoH->at(i).getBunchCrossing()==-10 ) _trpu = puInfoH->at(i).getTrueNumInteractions(); 
  }
 }
 
 myTree_->Fill();
}



// ------------ method called once each job just before starting event loop  ------------
void 
PileUpDumper::beginJob()
{
}

// ------------ method called once each job just after ending the event loop  ------------
void 
PileUpDumper::endJob() 
{
}

// ------------ method called when starting to processes a run  ------------
void PileUpDumper::beginRun(edm::Run const& iRun, edm::EventSetup const&)
{
}

// ------------ method called when ending the processing of a run  ------------
void 
PileUpDumper::endRun(edm::Run const&, edm::EventSetup const&)
{
}

// ------------ method called when starting to processes a luminosity block  ------------
void 
PileUpDumper::beginLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&)
{
}

// ------------ method called when ending the processing of a luminosity block  ------------
void 
PileUpDumper::endLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&)
{
}

// ------------ method fills 'descriptions' with the allowed parameters for the module  ------------
void
PileUpDumper::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
 //The following says we do not know what parameters are allowed so do no validation
 // Please change this to state exactly what you do use, even if it is no parameters
 edm::ParameterSetDescription desc;
 desc.setUnknown();
 descriptions.addDefault(desc);
}

//define this as a plug-in
DEFINE_FWK_MODULE(PileUpDumper);
