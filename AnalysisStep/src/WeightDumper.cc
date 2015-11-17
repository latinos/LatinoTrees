// -*- C++ -*-
//
// Package:    WeightDumper
// Class:      WeightDumper
// 
/**\class WeightDumper WeightDumper.cc
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

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"


//---- for auto-tree
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"

#include "TTree.h"



//---- for GenParticles
#include "DataFormats/HepMCCandidate/interface/GenParticle.h"
// #include "DataFormats/Candidate/interface/Candidate.h"

//---- for GenJets
#include "DataFormats/JetReco/interface/GenJet.h" 

//---- for DeltaR
#include "Math/VectorUtil.h"
//---- for DeltaPhi
#include "DataFormats/Math/interface/deltaPhi.h"

//---- for LHE information
#include "SimDataFormats/GeneratorProducts/interface/LHEEventProduct.h"
#include "SimDataFormats/GeneratorProducts/interface/LHERunInfoProduct.h"
#include "SimDataFormats/GeneratorProducts/interface/LHEXMLStringProduct.h"
#include <fstream> 

//---- TLorentzVector
#include "TLorentzVector.h"

//---- to get weights
#include "SimDataFormats/GeneratorProducts/interface/GenEventInfoProduct.h"

//
// class declaration
//

class WeightDumper : public edm::EDAnalyzer {
public:
 explicit WeightDumper(const edm::ParameterSet&);
 ~WeightDumper();
 
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
 edm::InputTag mcLHEEventInfoTag_;
 edm::InputTag genEvtInfoTag_;
 bool dumpWeights_;
 bool _debug;
 
 
 TTree* myTree_;
 
 TH1F* _mcWeightPos;
 TH1F* _mcWeightNeg;
 TH1F* _list_vectors_weights;
 int _MAXWEIGHTS;
 
 //---- MC qcd scale
 std::vector <double> _weightsLHE;
 double _weightNominalLHE;
 std::vector <double> _weights;
 double _weightSM;
 
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
WeightDumper::WeightDumper(const edm::ParameterSet& iConfig)

{
 //now do what ever initialization is needed
 mcLHEEventInfoTag_      = iConfig.getParameter<edm::InputTag>("mcLHEEventInfoTag");
 genEvtInfoTag_          = iConfig.getParameter<edm::InputTag>("genEvtInfoTag");
 _debug                  = iConfig.getUntrackedParameter< bool >("debug",false);
 
 
 edm::Service<TFileService> fs ;
 _MAXWEIGHTS = 200;
 _mcWeightPos = fs -> make <TH1F>("mcWeightPos","mcWeightPos", 1, 0, 1);
 _mcWeightNeg = fs -> make <TH1F>("mcWeightNeg","mcWeightNeg", 1, 0, 1);
 _list_vectors_weights = fs -> make <TH1F>("list_vectors_weights","list_vectors_weights", _MAXWEIGHTS, 0, _MAXWEIGHTS);
 
 
 myTree_ = fs -> make <TTree>("myTree","myTree");
 
 myTree_ -> Branch("weightsLHE", "std::vector<double>", &_weightsLHE);
//  myTree_ -> Branch("weightsLHE", &_weightsLHE);
 myTree_ -> Branch("weightNominalLHE", &_weightNominalLHE, "weightNominalLHE/D");
 myTree_ -> Branch("weights", "std::vector<double>", &_weights);
//  myTree_ -> Branch("weights", &_weights);
 myTree_ -> Branch("weightSM", &_weightSM, "weightSM/D");
 
}


WeightDumper::~WeightDumper()
{
 
 // do anything here that needs to be done at desctruction time
 // (e.g. close files, deallocate resources etc.)
 
}


//
// member functions
//

// ------------ method called for each event  ------------
void WeightDumper::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup) {
 
 edm::Handle<GenEventInfoProduct> genEvtInfo;
 iEvent.getByLabel( genEvtInfoTag_, genEvtInfo );
 
 edm::Handle<LHEEventProduct> productLHEHandle;
 iEvent.getByLabel(mcLHEEventInfoTag_, productLHEHandle);
 
 //---- See https://twiki.cern.ch/twiki/bin/view/CMSPublic/SWGuideDataFormatGeneratorInterface
 //---- weights: from EventInfo and from LHE
 
 std::vector<double> evtWeights = genEvtInfo->weights();
 _weightSM = genEvtInfo->weight();
 
 _weights.clear();
 if (_debug) std::cout << " evtWeights.size() = " << evtWeights.size() << std::endl;
 for (unsigned int iWeight = 0; iWeight < evtWeights.size(); iWeight++) {
//   if (_debug) std::cout << " evtWeights[" << iWeight << "] = " << evtWeights.at(iWeight) << std::endl;
  _weights.push_back(evtWeights.at(iWeight));
 }
 if (_debug) std::cout << " weightSM = " << _weightSM << std::endl;
 
 _weightsLHE.clear();
 unsigned int num_whichWeight = productLHEHandle->weights().size();
 for (unsigned int iWeight = 0; iWeight < num_whichWeight; iWeight++) {
  _weightsLHE.push_back( productLHEHandle->weights()[iWeight].wgt/productLHEHandle->originalXWGTUP() ); 
  if (_debug) std::cout << " weightLHE[" << iWeight << "] = " << productLHEHandle->weights()[iWeight].wgt << std::endl;
 }
 _weightNominalLHE = productLHEHandle->originalXWGTUP();
 
 if (_debug) std::cout << " weightNominalLHE = " << _weightNominalLHE << std::endl;
 if (_debug) std::cout << " ---------- " << std::endl; 
 
 myTree_->Fill();

 
 if (_weightSM > 0) {
//   _mcWeightPos->Fill(0.5);
  _mcWeightPos->Fill(0.5, _weightSM);
 }
 if (_weightSM < 0) {
//   _mcWeightNeg->Fill(0.5);
  _mcWeightNeg->Fill(0.5, -_weightSM);
 }
 
 unsigned int min_num;
 if (int(num_whichWeight) > _MAXWEIGHTS) min_num = _MAXWEIGHTS;
 else  min_num = num_whichWeight;
  
 for (unsigned int iWeight = 0; iWeight < min_num ; iWeight++) {
  _list_vectors_weights->Fill( iWeight+0.5, productLHEHandle->weights()[iWeight].wgt/productLHEHandle->originalXWGTUP() );
 }
 
}



// ------------ method called once each job just before starting event loop  ------------
void 
WeightDumper::beginJob()
{
}

// ------------ method called once each job just after ending the event loop  ------------
void 
WeightDumper::endJob() 
{
}

// ------------ method called when starting to processes a run  ------------
void WeightDumper::beginRun(edm::Run const& iRun, edm::EventSetup const&) {
 edm::Handle<LHERunInfoProduct> run;
 //  LHERunInfoProduct        "externalLHEProducer"   ""                "LHE"     
 //  edmDumpEventContent  /tmp/amassiro/180BFD9B-CDD0-E411-9330-0CC47A13D09C.root --run 
 
 typedef std::vector<LHERunInfoProduct::Header>::const_iterator headers_const_iterator;
 
 //iRun.getByLabel( "externalLHEProducer", run );
 iRun.getByLabel( mcLHEEventInfoTag_, run );
 
 LHERunInfoProduct myLHERunInfoProduct = *(run.product());
 
 for (headers_const_iterator iter=myLHERunInfoProduct.headers_begin(); iter!=myLHERunInfoProduct.headers_end(); iter++){
  std::cout << iter->tag() << std::endl;
  std::vector<std::string> lines = iter->lines();
  for (unsigned int iLine = 0; iLine<lines.size(); iLine++) {
   std::cout << lines.at(iLine);
  }
 }
 
 
 const lhef::HEPRUP thisHeprup_ = run->heprup();
 std::cout << "HEPRUP \n" << std::endl;
 std::cout << "IDBMUP " << std::setw(14) << std::fixed << thisHeprup_.IDBMUP.first;
 std::cout << std::setw(14) << std::fixed << thisHeprup_.IDBMUP.second << std::endl;
 std::cout << "EBMUP " << std::setw(14) << std::fixed << thisHeprup_.EBMUP.first;
 std::cout << std::setw(14) << std::fixed << thisHeprup_.EBMUP.second << std::endl;
 std::cout << "PDFGUP " << std::setw(14) << std::fixed << thisHeprup_.PDFGUP.first;
 std::cout << std::setw(14) << std::fixed << thisHeprup_.PDFGUP.second << std::endl;
 std::cout << "PDFSUP " << std::setw(14) << std::fixed << thisHeprup_.PDFSUP.first;
 std::cout << std::setw(14) << std::fixed << thisHeprup_.PDFSUP.second << std::endl;
 std::cout << "IDWTUP " << std::setw(14) << std::fixed << thisHeprup_.IDWTUP << std::endl;
 std::cout << "NPRUP " << std::setw(14) << std::fixed << thisHeprup_.NPRUP << std::endl;
 std::cout << " XSECUP " << std::setw(14) << std::fixed;
 std::cout << " XERRUP " << std::setw(14) << std::fixed;
 std::cout << " XMAXUP " << std::setw(14) << std::fixed;
 std::cout << " LPRUP " << std::setw(14) << std::fixed << std::endl;
 for ( unsigned int iSize = 0 ; iSize < thisHeprup_.XSECUP.size() ; iSize++ ) {
  std::cout << std::setw(14) << std::fixed << thisHeprup_.XSECUP[iSize];
  std::cout << std::setw(14) << std::fixed << thisHeprup_.XERRUP[iSize];
  std::cout << std::setw(14) << std::fixed << thisHeprup_.XMAXUP[iSize];
  std::cout << std::setw(14) << std::fixed << thisHeprup_.LPRUP[iSize];
  std::cout << std::endl;
 }
 std::cout << " " << std::endl;
 
 
}

// ------------ method called when ending the processing of a run  ------------
void 
WeightDumper::endRun(edm::Run const&, edm::EventSetup const&)
{
}

// ------------ method called when starting to processes a luminosity block  ------------
void 
WeightDumper::beginLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&)
{
}

// ------------ method called when ending the processing of a luminosity block  ------------
void 
WeightDumper::endLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&)
{
}

// ------------ method fills 'descriptions' with the allowed parameters for the module  ------------
void
WeightDumper::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
 //The following says we do not know what parameters are allowed so do no validation
 // Please change this to state exactly what you do use, even if it is no parameters
 edm::ParameterSetDescription desc;
 desc.setUnknown();
 descriptions.addDefault(desc);
}

//define this as a plug-in
DEFINE_FWK_MODULE(WeightDumper);
