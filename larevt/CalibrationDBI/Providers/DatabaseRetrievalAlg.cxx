#ifndef DATABASERETRIEVALALG_CXX
#define DATABASERETRIEVALALG_CXX 1

#include "DatabaseRetrievalAlg.h"

namespace lariov {

  /// Configure using fhicl::ParameterSet
  void DatabaseRetrievalAlg::Reconfigure(fhicl::ParameterSet const& p) {

    std::string foldername = p.get<std::string>("DBFolderName");
    std::string url        = p.get<std::string>("DBUrl");
    std::string tag        = p.get<std::string>("DBTag", "");
    bool applyScaling      = p.get<bool>("ApplyScaling",false);
    std::uint64_t Modifier   = p.get<std::uint64_t>("Scaling",1);
    if(applyScaling) 
        fFolder.reset(new DBFolder(foldername, url, tag,applyScaling,Modifier));
    else
        fFolder.reset(new DBFolder(foldername, url, tag));
  }
}
#endif
