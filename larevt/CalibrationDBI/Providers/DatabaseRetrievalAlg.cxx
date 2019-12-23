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
   // std::string  OverrideDate = p.get<std::string>("OverrideDate","");
    DBTimeStamp_t  OverrideTimeStamp = p.get<std::uint64_t>("OverrideStamp",0);
    (void)applyScaling;
    (void)Modifier;
    //if(applyScaling && !OverrideDate.length() )
    if(applyScaling && OverrideTimeStamp==0 ) 
         fFolder.reset(new DBFolder(foldername, url, tag,applyScaling,Modifier));
    //else if(!applyScaling && OverrideDate.length() ) 
    else if(!applyScaling && OverrideTimeStamp!=0 ) 
         //fFolder.reset(new DBFolder(foldername, url, tag,OverrideDate));
        fFolder.reset(new DBFolder(foldername, url, tag,OverrideTimeStamp));
    //else if( applyScaling && OverrideDate.length() )
    else if( applyScaling && OverrideTimeStamp!=0 )
         std::cout << "----- error, can't do both ----- "  << std::endl;
    else
        fFolder.reset(new DBFolder(foldername, url, tag));
  }
}
#endif
