#include "LatinoTrees/DataFormats/interface/SkimEvent.h"
#include "DataFormats/Common/interface/Ref.h"
#include "DataFormats/Common/interface/Wrapper.h"
#include "DataFormats/Common/interface/ValueMap.h"
#include "DataFormats/Common/interface/RefVector.h"
#include <DataFormats/Common/interface/BaseHolder.h>

namespace
{
 struct dictionary {

  reco::SkimEvent dummy1;
  std::vector<reco::SkimEvent> dummy2;
  edm::Wrapper<reco::SkimEvent> dummy3;
  edm::Wrapper<std::vector<reco::SkimEvent> > dummy4;

  std::map<unsigned int,std::vector<std::pair<unsigned int,int> > > test;

 };
}




