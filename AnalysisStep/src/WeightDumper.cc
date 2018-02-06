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
#include "FWCore/Framework/interface/Run.h"
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
// LHE Run
#include "SimDataFormats/GeneratorProducts/interface/LHERunInfoProduct.h"
#include "GeneratorInterface/LHEInterface/interface/LHERunInfo.h"

#include "SimDataFormats/GeneratorProducts/interface/LHEEventProduct.h"
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
 edm::InputTag mcGenEventInfoTag_;
 edm::InputTag mcLHERunInfoTag_;
 bool dumpWeights_;
 bool _debug;
 edm::EDGetTokenT<LHEEventProduct> productLHET_ ;
 edm::EDGetTokenT<GenEventInfoProduct> GenInfoT_ ;
 edm::EDGetTokenT<LHERunInfoProduct> mcLHERunInfoT_ ;
 
 TTree* myTree_;
 
 TH1F* _mcWeightExplained; 
 TH1F* _mcWeightExplainedOrdered;
 TH1F* _mcWeightPos;
 TH1F* _mcWeightNeg;
 TH1F* _list_vectors_weights;
 
 int _MAXWEIGHTS;
 
  //---- MC qcd scale
  std::vector <double> _weightsLHE;
  //  std::vector <std::string> _weightsLHEID;
  std::vector <int> _weightsLHEID;
  // std::vector <std::string> _weightsLHEIDExplained;
  int list_size;
  double _weightNominalLHE;
  std::vector <double> _weights;
  double _weightSM;
  std::vector<TString> list;
 
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
 mcGenEventInfoTag_      = iConfig.getParameter<edm::InputTag>("genEvtInfoTag");
 mcLHERunInfoTag_        = iConfig.getParameter<edm::InputTag>("mcLHERunInfoTag"); //---- "externalLHEProducer"
 _debug                  = iConfig.getUntrackedParameter< bool >("debug",false);
 
 productLHET_   = consumes<LHEEventProduct>     (mcLHEEventInfoTag_);
 GenInfoT_      = consumes<GenEventInfoProduct> (mcGenEventInfoTag_);
 mcLHERunInfoT_ = consumes<LHERunInfoProduct, edm::InRun>   (mcLHERunInfoTag_);
 
 edm::Service<TFileService> fs ;
 _MAXWEIGHTS = 200;
 _mcWeightExplainedOrdered = fs -> make <TH1F>("mcWeightExplainedOrdered", "mcWeightExplainedOrdered", 2000, 0, 2000);
 _mcWeightExplained = fs -> make <TH1F>("mcWeightExplained", "mcWeightExplained", 10000, 0, 10000);
 _mcWeightPos = fs -> make <TH1F>("mcWeightPos","mcWeightPos", 1, 0, 1);
 _mcWeightNeg = fs -> make <TH1F>("mcWeightNeg","mcWeightNeg", 1, 0, 1);
 
//  _mcWeightPDF = fs -> make <TH1F>("mcWeightPDF","mcWeightPDF", 200, 0, 200);
//  _mcWeightQCD = fs -> make <TH1F>("mcWeightQCD","mcWeightQCD", 9, 0, 1);
 
 _list_vectors_weights = fs -> make <TH1F>("list_vectors_weights","list_vectors_weights", _MAXWEIGHTS, 0, _MAXWEIGHTS);
 
 
 myTree_ = fs -> make <TTree>("myTree","myTree");
 
 myTree_ -> Branch("weightsLHE", "std::vector<double>", &_weightsLHE);
 // myTree_ -> Branch("weightsLHEID", "std::vector<std::string>", &_weightsLHEID);
 myTree_ -> Branch("weightsLHEID", &_weightsLHEID);
 myTree_ -> Branch("list_size", &list_size);
 myTree_ -> Branch("weightNominalLHE", &_weightNominalLHE, "weightNominalLHE/D");
 myTree_ -> Branch("weights", "std::vector<double>", &_weights);
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
 //  iEvent.getByLabel( mcGenEventInfoTag_, genEvtInfo );
 iEvent.getByToken( GenInfoT_, genEvtInfo );
 
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
  float weight_to_be_saved = productLHEHandle->weights()[iWeight].wgt/productLHEHandle->originalXWGTUP() ;
  _mcWeightExplainedOrdered -> Fill(iWeight,weight_to_be_saved);
  _weightsLHE.push_back( weight_to_be_saved ); 
  if (_debug) std::cout << " weightLHE[" << iWeight << "] = " << productLHEHandle->weights()[iWeight].wgt << std::endl;
 }
 _weightNominalLHE = productLHEHandle->originalXWGTUP();
 
 _weightsLHEID.clear();
 unsigned int num_whichWeightID = productLHEHandle->weights().size();
 for (unsigned int iWeight = 0; iWeight < num_whichWeightID; iWeight++) {
   _weightsLHEID.push_back( TString(productLHEHandle->weights()[iWeight].id).Atoi() ); 
  if (_debug) std::cout << " weightLHEID[" << iWeight << "] = " << productLHEHandle->weights()[iWeight].id << std::endl;
 }
 
 /*
  _ weightsLHEIDExplained.clear();*
  for (unsigned int iWeight = 0; iWeight < num_whichWeightID; iWeight++) {
   int found = 0;
   for (unsigned int j = 0; j < list.size(); ++j){
    if (list.at(j).Contains(productLHEHandle->weights()[iWeight].id)){
     _weightsLHEIDExplained.push_back(std::string(list.at(j))); 
     found = 1;
}
}
if (found == 0) _weightsLHEIDExplained.push_back("Currently Unknown");
}
*/
 
 list_size = list.size();
 
 if (_debug) std::cout << " weightNominalLHE = " << _weightNominalLHE << std::endl;
 if (_debug) std::cout << " ---------- " << std::endl; 
 
 myTree_->Fill();
  
 if (_weightSM > 0) {
  _mcWeightPos->Fill(0.5); //---> histogram is filled with +1 event
  //   _mcWeightPos->Fill(0.5, _weightSM);
 }
 if (_weightSM < 0) {
  _mcWeightNeg->Fill(0.5); //---> histogram is filled with +1 event
  //   _mcWeightNeg->Fill(0.5, -_weightSM);
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
 
 typedef std::vector<LHERunInfoProduct::Header>::const_iterator headers_const_iterator;
 
//  std::cout << " >>> WeightDumper::beginRun " << std::endl;
 
 if (!(mcLHERunInfoTag_ == edm::InputTag(""))) {
  
  iRun.getByLabel( mcLHERunInfoTag_, run );
  //     iRun.getByToken( mcLHERunInfoT_, run );
  
  LHERunInfoProduct myLHERunInfoProduct = *(run.product()); 
  
//   std::cout << " >>> WeightDumper::beginRun after mcLHERunInfoTag check " << std::endl;
  
  for (headers_const_iterator iter = myLHERunInfoProduct.headers_begin(); iter != myLHERunInfoProduct.headers_end(); iter++){
   std::vector<std::string> lines = iter->lines();
   int counter = 0;
   for (unsigned int iLine = 0; iLine<lines.size(); iLine++) {
//     std::cout << " lines[" << iLine << "] = " << lines.at(iLine) << std::endl;
    if (iter->tag() == "initrwgt"){
     TString dumpLine = lines.at(iLine);
//      std::cout << " dumpLine = " << dumpLine << std::endl;
     list.push_back(dumpLine);
     if (dumpLine.IsAlpha()) continue;
     if (_debug) std::cout << dumpLine << std::endl;
     if (dumpLine.Contains("<weight id=")){
      int IDint = 0;
      TString shortLine = dumpLine;
      shortLine.Remove(0,12);
      shortLine.Remove(4,shortLine.Length()-4);
      if (_debug) std::cout << "shortLine = " << shortLine << std::endl;
      if (shortLine.IsDigit()){
	IDint = shortLine.Atoi();
	if (_debug) std::cout << "IDint = " << IDint <<std::endl;     
	_mcWeightExplained -> GetXaxis() -> SetBinLabel(IDint, dumpLine);
	_mcWeightExplained -> SetBinContent(IDint,1);
       
	_mcWeightExplainedOrdered -> GetXaxis() -> SetBinLabel(counter+1, dumpLine);
// 	_mcWeightExplainedOrdered -> Fill(counter,1);
	counter++;
      }
     }
    }
   }
  }
 }  
 
 
 /*  
  c onst lhef::HEPRUP thisH*eprup = run->heprup();
  
  std::cout << "HEPRUP \n" << std::endl;
  std::cout << "IDBMUP " << std::setw(14) << std::fixed << thisHeprup.IDBMUP.first;
  std::cout << std::setw(14) << std::fixed << thisHeprup.IDBMUP.second << std::endl;
  std::cout << "EBMUP " << std::setw(14) << std::fixed << thisHeprup.EBMUP.first;
  std::cout << std::setw(14) << std::fixed << thisHeprup.EBMUP.second << std::endl;
  std::cout << "PDFGUP " << std::setw(14) << std::fixed << thisHeprup.PDFGUP.first;
  std::cout << std::setw(14) << std::fixed << thisHeprup.PDFGUP.second << std::endl;
  std::cout << "PDFSUP " << std::setw(14) << std::fixed << thisHeprup.PDFSUP.first;
  std::cout << std::setw(14) << std::fixed << thisHeprup.PDFSUP.second << std::endl;
  std::cout << "IDWTUP " << std::setw(14) << std::fixed << thisHeprup.IDWTUP << std::endl;
  std::cout << "NPRUP " << std::setw(14) << std::fixed << thisHeprup.NPRUP << std::endl;
  std::cout << " XSECUP " << std::setw(14) << std::fixed;
  std::cout << " XERRUP " << std::setw(14) << std::fixed;
  std::cout << " XMAXUP " << std::setw(14) << std::fixed;
  std::cout << " LPRUP " << std::setw(14) << std::fixed << std::endl;
  for ( unsigned int iSize = 0 ; iSize < thisHeprup.XSECUP.size() ; iSize++ ) {
   std::cout << std::setw(14) << std::fixed << thisHeprup.XSECUP[iSize];
   std::cout << std::setw(14) << std::fixed << thisHeprup.XERRUP[iSize];
   std::cout << std::setw(14) << std::fixed << thisHeprup.XMAXUP[iSize];
   std::cout << std::setw(14) << std::fixed << thisHeprup.LPRUP[iSize];
   std::cout << std::endl;
}
std::cout << " " << std::endl;
*/  
 //} 
}

// ------------ method called when ending the processing of a run  ------------
void 
WeightDumper::endRun(edm::Run const& iRun, edm::EventSetup const&)
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
