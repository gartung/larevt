////////////////////////////////////////////////////////////////////////
//
// ContainmentFilter class:
// Algoritm to produce a filtered event file having
// events with user-defined containment
//
// Joseph Zennamo, UChicago, 2017
// jzennamo@uchicago.edu
//
////////////////////////////////////////////////////////////////////////

#ifndef CONTAINMENTFILTER_H
#define CONTAINMENTFILTER_H

//Framework Includes
#include "art/Framework/Core/EDFilter.h"
#include "art/Framework/Core/ModuleMacros.h" 
#include "art/Framework/Principal/Event.h" 
#include "fhiclcpp/ParameterSet.h" 
#include "art/Framework/Principal/Handle.h" 
#include "canvas/Persistency/Common/Ptr.h" 
#include "canvas/Persistency/Common/PtrVector.h" 
#include "art/Framework/Services/Registry/ServiceHandle.h" 
#include "art/Framework/Services/Optional/TFileService.h" 
#include "art/Framework/Services/Optional/TFileDirectory.h" 
#include "messagefacility/MessageLogger/MessageLogger.h" 


//Larsoft Includes
#include "larcore/CoreUtils/ServiceUtil.h" // lar::providerFrom<>()
#include "larcore/Geometry/Geometry.h"
#include "lardata/DetectorInfoServices/DetectorPropertiesService.h"
#include "larcore/Geometry/PlaneGeo.h"
#include "larcore/Geometry/WireGeo.h"
#include "lardata/Utilities/AssociationUtil.h"

//Data product Includes
#include "nusimdata/SimulationBase/MCNeutrino.h"
#include "nusimdata/SimulationBase/MCTruth.h"
#include "lardataobj/MCBase/MCShower.h"
#include "lardataobj/MCBase/MCTrack.h"
#include "nusimdata/SimulationBase/MCParticle.h"

#include "TH1.h"
#include "TH2.h"

namespace filt {

  class ContainmentFilter : public art::EDFilter  {
    
  public:
    
    explicit ContainmentFilter(fhicl::ParameterSet const& ); 
    virtual ~ContainmentFilter();
         
    
    bool filter(art::Event& evt);
    void reconfigure(fhicl::ParameterSet const& p);
    void beginJob();
   

  private: 
 
    std::string fGenieModuleLabel;
    std::string fMCRecoModuleLabel;

    bool fDebug;

    bool fisInclusiveCondition;

    bool fCheckNeutrinoMuon;
    bool fCheckNeutrinoElectron;
    bool fCheckNeutrinoPhotons;

    std::pair<bool,bool> fCheckNeutrinoVertex;
    std::pair<bool,bool> fCheckMuonFullContainment;
    std::pair<bool,bool> fCheckMuonFracLength;
    std::pair<bool,bool> fCheckMuonFracEnergy;
    std::pair<bool,bool> fCheckElectronFracEnergy;
    std::pair<bool,bool> fCheckPhotonConversionLocation;
    std::pair<bool,bool> fCheckPhotonFracEnergy;

    std::pair< bool, int > fExclusiveNumberOfPhotons;

    double fMinTrkL;
    double fMinTrkE;
    double fMinShwE;

    double   fFidEdgeX; // fiducial volume - x
    double   fFidEdgeY; // fiducial volume - y
    double   fFidEdgeZ; // fiducial volume - z

    double   fActiveEdgeX; // fiducial volume - x
    double   fActiveEdgeY; // fiducial volume - y
    double   fActiveEdgeZ; // fiducial volume - z
    
    TH1D* fSelectedEvents;
    TH1D* fTotalEvents;

    geo::GeometryCore const*            fGeometry; // pointer to the Geometry service
    detinfo::DetectorProperties const*  fDetector; // pointer to the detector properties

  protected: 

    bool isEfracContainedinAV(double MinEfrac, const art::Ptr<sim::MCShower> show) const;
    bool isEfracContainedinAV(double MinEfrac, const art::Ptr<sim::MCTrack> trk) const;
    bool isTrackFullyContained(art::Ptr<sim::MCTrack> trk) const;
    bool isTrackLengthContained(double MinLength, const art::Ptr<sim::MCTrack> trk) const;
    bool inFV(double x, double y, double z) const;
    bool inAV(double x, double y, double z) const;
    double TrackLength(art::Ptr<sim::MCTrack>) const;
    double TrackEnergy(art::Ptr<sim::MCTrack>) const;

    bool isFromNeutrinoVertex(art::Ptr<simb::MCTruth> mc, art::Ptr<sim::MCTrack> trk) const;
    bool isFromNeutrinoVertex(art::Ptr<simb::MCTruth> mc, art::Ptr<sim::MCShower> show) const;
        
  }; // class ContainmentFilter

}

namespace filt{

  //-------------------------------------------------
  ContainmentFilter::ContainmentFilter(fhicl::ParameterSet const & pset) :
    fGeometry(lar::providerFrom<geo::Geometry>()),
    fDetector(lar::providerFrom<detinfo::DetectorPropertiesService>())
  {   
    this->reconfigure(pset);
  }

  //-------------------------------------------------
  ContainmentFilter::~ContainmentFilter()
  {
  }
  
  //-------------------------------------------------
  void ContainmentFilter::reconfigure(fhicl::ParameterSet const& p)
  {
    fDebug             = p.get< bool >("IsVerbose"); 
    fGenieModuleLabel  = p.get< std::string      >("GenieModuleLabel");
    fMCRecoModuleLabel  = p.get< std::string      >("MCRecoModuleLabel");
    fFidEdgeX       = fGeometry->DetHalfWidth()   - p.get<double>("FidDistX",   10.);
    fFidEdgeY       = fGeometry->DetHalfHeight()  - p.get<double>("FidDistY",   10.);
    fFidEdgeZ       = fGeometry->DetLength() / 2. - p.get<double>("FidDistZ",   10.);

    fActiveEdgeX  = fGeometry->DetHalfWidth();
    fActiveEdgeY  = fGeometry->DetHalfHeight();
    fActiveEdgeZ  = fGeometry->DetLength() / 2.;

    fisInclusiveCondition  = p.get< bool >("InclusiveSelection");
    fCheckNeutrinoMuon     = p.get< bool >("CheckNeutrinoMuon");
    fCheckNeutrinoElectron = p.get< bool >("CheckNeutrinoElectron");
    fCheckNeutrinoPhotons  = p.get< bool >("CheckNeutrinoPhotons");

    fCheckNeutrinoVertex = p.get< std::pair<bool , bool> >("CheckNeutrinoVertex");
    fCheckMuonFullContainment = p.get< std::pair<bool , bool> >("CheckMuonFullContainment");
    fCheckMuonFracLength = p.get< std::pair<bool , bool> >("CheckMuonFracLength");
    fCheckMuonFracEnergy = p.get< std::pair<bool , bool> >("CheckMuonFracEnergy");
    fCheckElectronFracEnergy = p.get< std::pair<bool , bool> >("CheckElectronFracEnergy");
    fCheckPhotonConversionLocation = p.get< std::pair<bool , bool> >("CheckPhotonConversionLocation");
    fCheckPhotonFracEnergy = p.get< std::pair<bool , bool> >("CheckPhotonFracEnergy");

    fExclusiveNumberOfPhotons = p.get< std::pair<bool , int> >("OnlyExceptThisManyPhotons");
    
    fMinTrkL = p.get< double >("MinimumContainedTrackLength");
    fMinTrkE = p.get< double >("MinimumContainedTrackEnergy");
    fMinShwE = p.get< double >("MinimumContainedShowerEnergy");


  }

  //-------------------------------------------------
  void ContainmentFilter::beginJob()
  {
    art::ServiceHandle<art::TFileService> tfs;
    fSelectedEvents = tfs->make<TH1D>("fSelectedEvents", "Number of Selected Events", 3, 0, 3); //counts the number of selected events 
    fTotalEvents = tfs->make<TH1D>("fTotalEvents", "Total Events", 3, 0, 3); //counts the initial number of events in the unfiltered root input file
  }

  //-------------------------------------------------
  bool ContainmentFilter::filter(art::Event &evt)
  { 
        
    art::Handle< std::vector<simb::MCTruth> > mclistHandle;
    std::vector<art::Ptr<simb::MCTruth> > mclist;
    if(evt.getByLabel(fGenieModuleLabel,mclistHandle))
      art::fill_ptr_vector(mclist,mclistHandle);

    art::Handle< std::vector<sim::MCShower> > mcslistHandle;
    std::vector<art::Ptr<sim::MCShower> > mcslist;
    if(evt.getByLabel(fMCRecoModuleLabel,mcslistHandle))
      art::fill_ptr_vector(mcslist,mcslistHandle);

    art::Handle< std::vector<sim::MCTrack> > mctlistHandle;
    std::vector<art::Ptr<sim::MCTrack> > mctlist;
    if(evt.getByLabel(fMCRecoModuleLabel,mctlistHandle))
      art::fill_ptr_vector(mctlist,mctlistHandle);


    fTotalEvents->Fill(1);

    bool nuPasses = false;

    bool muFullPasses = false;
    bool muLenPasses  = false;
    bool muEngPasses  = false;

    bool elEngPasses = false;

    bool gammaConvPasses = false;
    bool gammeEngPasses  = false;
    
    for(auto const mc : mclist){

      //If Check Neutrino Vertex
      // >> Check Neutrino Vertex 
      
      if(fCheckNeutrinoVertex.first){

	std::cout << "Checking nu vertex: " << std::endl;

	//Neutrino interaction vertex
	double x = mc->GetNeutrino().Nu().Trajectory().Position(0).X(); 
	double y = mc->GetNeutrino().Nu().Trajectory().Position(0).Y();
	double z = mc->GetNeutrino().Nu().Trajectory().Position(0).Z();
	
	if( inFV(x,y,z) ){
	  nuPasses = true;
	  std::cout << "\t :::  Passed " << std::endl; 
	}
	
      }//fCheckNeutrinoVertex
      
      
      //If Check Neutrino Muon
      // >> If Check Fully Contain 
      // >> >> Check if Fully Contained
      // >> If Check Fractionally Contained
      // >> >> If Check length or energy
      // >> >> >> Check 
      
      if(fCheckNeutrinoMuon){
	
	for(auto const mct : mctlist){
	  
	  //Look for direct neutrino induced muons		    
	  if(abs(mct->PdgCode()) == 13 && isFromNeutrinoVertex(mc, mct)){	      
	    
	    if(fCheckMuonFullContainment.first){	   
	      
	      std::cout << "Checking muon is fully contained: " << std::endl;
	      
	      if(isTrackFullyContained(mct)){
		muFullPasses = true;
		std::cout << "\t :::  Passed " << std::endl; 
	      }	      
	    }
	    
	    if(fCheckMuonFracLength.first){

	      std::cout << "Checking muon contained length : " << std::endl;
	      
	      if(isTrackLengthContained(fMinTrkL, mct)){
		muLenPasses = true;
		std::cout << "\t :::  Passed " << std::endl; 
	      }
	    }	
	    
	    if(fCheckMuonFracEnergy.first){

	      std::cout << "Checking muon contained energy : " << std::endl;

	      if(isEfracContainedinAV(fMinTrkE, mct)){
		muEngPasses = true;
		std::cout << "\t :::  Passed " << std::endl; 
	      }
	    }
	    
	  }//Check if it is a neutrino induced muon
	}//iterate through tracks	
      }//fCheckNeutrinoMuon
      
      //If Check Neutrino Electron 
      // >> Check fractional energy
      
      if(fCheckNeutrinoElectron){

	if(int(mcslist.size()) == 0){ std::cout << "No Showers To Check?!" << std::endl; }
	
	for(auto const mcs : mcslist){
	  
	  if(abs(mcs->PdgCode()) == 11  && isFromNeutrinoVertex(mc, mcs)){	      
	    
	    if(fCheckElectronFracEnergy.first){

	      std::cout << "Checking electron contained energy : " << std::endl;

	      if(isEfracContainedinAV(fMinShwE, mcs)){
		elEngPasses = true;
		std::cout << "\t :::  Passed " << std::endl; 
	      }
	    }
	  }  
	}//iterate through the showers 
      }//fCheckNeutrinoElectron
      
      
      //If Check neutrino photons (inclusive or exclusive)
      // >> If Check Conversion Location
      // >> >> Check Conversion Location 
      // >> If Check fractional energy contained
      // >> >> check energy 
      
      if(fCheckNeutrinoPhotons){
	
	int photonConvNum = 0;
	int photonEngNum = 0;
	
	for(auto const mcs : mcslist){
	  
	  std::cout << "Shower PDG : " << abs(mcs->PdgCode()) << ", vertex : " << isFromNeutrinoVertex(mc, mcs) << std::endl; 	  
	  if(abs(mcs->PdgCode()) == 22  && isFromNeutrinoVertex(mc, mcs) ){	      
	    	    
	    if(fCheckPhotonConversionLocation.first){
	      
	      std::cout << "Checking photon conversion location : " << std::endl;

	      if(inFV(mcs->End().X(), mcs->End().Y(), mcs->End().Z())){
		
		gammaConvPasses = true;
		std::cout << "\t :::  Passed " << std::endl; 
		photonConvNum++;
		
		if(fExclusiveNumberOfPhotons.first){
		  
		  if(photonConvNum > fExclusiveNumberOfPhotons.second){
		    std::cout << ">>>>> Too Many Photons! Failed" << std::endl; 
		    gammaConvPasses = false;
		  }
		  
		}//Photon Exclusivity 
		
	      }//isFV
	      
	    }//fCheckPhotonConversionLocation
	    
	    if(fCheckPhotonFracEnergy.first){

	      std::cout << "Checking photon contained energy : " << std::endl;
       	      
	      if(isEfracContainedinAV(fMinShwE, mcs)){

		gammeEngPasses = true;
		std::cout << "\t :::  Passed " << std::endl; 
		photonEngNum++;
		
		if(fExclusiveNumberOfPhotons.first){	 
		  if(photonEngNum > fExclusiveNumberOfPhotons.second){
		    std::cout << ">>>>> Too Many Photons! Failed" << std::endl; 
		    gammeEngPasses = false;
		  }
		}//Photon Exclusivity
	      }// Check Energy Contained
	    }//fCheckPhotonConversionLocation
	    
	  }//check if photon is from neutrino 
	  
	}//iterate through photons 
	
      }//fCheckNeutrinoPhotons
      
    }//Iterate through neutrino interactions
    
    
    bool EventPasses = false;

    if(fisInclusiveCondition){
      if(nuPasses || muFullPasses || muLenPasses || muEngPasses || elEngPasses || gammaConvPasses || gammeEngPasses){
	EventPasses = true;
      }
    }
    else{
      
      bool ExclusiveFail = true;
      
      if(fCheckNeutrinoVertex.first && ExclusiveFail){
	if(fCheckNeutrinoVertex.second == nuPasses){
	  EventPasses = true;
	    }
	else{
	  EventPasses = false;
	  ExclusiveFail = false;
	}
      }

      if(fCheckMuonFullContainment.first && ExclusiveFail){
	if(fCheckMuonFullContainment.second == muFullPasses){
	  EventPasses = true;
	    }
	else{
	  EventPasses = false;
	  ExclusiveFail = false;
	}
      }
      
      if(fCheckMuonFracLength.first && ExclusiveFail){
	if(fCheckMuonFracLength.second == muLenPasses){
	  EventPasses = true;
	    }
	else{
	  EventPasses = false;
	  ExclusiveFail = false;
	}
      }

      if(fCheckMuonFracEnergy.first && ExclusiveFail){
	if(fCheckMuonFracEnergy.second == muEngPasses){
	  EventPasses = true;
	    }
	else{
	  EventPasses = false;
	  ExclusiveFail = false;
	}
      }
    
      if(fCheckElectronFracEnergy.first && ExclusiveFail){
	if(fCheckElectronFracEnergy.second == elEngPasses){
	  EventPasses = true;
	    }
	else{
	  EventPasses = false;
	  ExclusiveFail = false;
	}
      }

      if(fCheckPhotonConversionLocation.first && ExclusiveFail){
	if(fCheckPhotonConversionLocation.second == gammaConvPasses){
	  EventPasses = true;
	    }
	else{
	  EventPasses = false;
	  ExclusiveFail = false;
	}
      }

      if(fCheckPhotonFracEnergy.first && ExclusiveFail){
	if(fCheckPhotonFracEnergy.second == gammeEngPasses){
	  EventPasses = true;
	    }
	else{
	  EventPasses = false;
	  ExclusiveFail = false;
	}
      }

      


    }

    if(!EventPasses) std::cout << "\n EVENT FAILED! \n " << std::endl;

    return EventPasses;
    
    
  
  } // bool  
  //} // namespace

  //------------------------------------------------   
  bool ContainmentFilter::isEfracContainedinAV(double MinEfrac, art::Ptr<sim::MCShower>show) const
  {
    
    double Edep = show->DetProfile().Momentum().E();
    double Etot = show->Start().E();

    std::cout << "Edep : " << Edep << ", Etot : " << Etot << std::endl; 
    
    double Edepfrac = Edep/Etot;
    
    if(Edepfrac > 1 || !(std::isfinite(Edepfrac))){
      std::cout << "You're stupid : isEfracContainedinAV" << std::endl; 
      Edepfrac = 0;
    }
    
    if(Edepfrac > MinEfrac){ return true;}
    else{ return false;}
    
    
  }

  bool ContainmentFilter::isEfracContainedinAV(double MinEfrac, art::Ptr<sim::MCTrack>trk) const
  {
    
    double Edep = TrackEnergy(trk); 
    double Etot = trk->Start().E();
    
    double Edepfrac = Edep/Etot;
    
   if(Edepfrac > 1 || !(std::isfinite(Edepfrac))){
     std::cout << "You're stupid : isEfracContainedinAV" << std::endl; 
     Edepfrac = 0;
   }
   
   if(Edepfrac > MinEfrac){ return true;}
   else{ return false;}
   
   
  }
  
  bool ContainmentFilter::isTrackFullyContained(art::Ptr<sim::MCTrack>trk) const
  {
    
    bool fullCont = true;
    
    for(auto const step : *trk){
      if(!inAV(step.X(),step.Y(), step.Z())) fullCont = false;
    }
    
    return fullCont;
    
  }
  
  bool ContainmentFilter::isFromNeutrinoVertex(art::Ptr<simb::MCTruth> mc, art::Ptr<sim::MCShower> show) const
  {
    
    double x_truth = mc->GetNeutrino().Nu().Trajectory().Position(0).X();
    double y_truth = mc->GetNeutrino().Nu().Trajectory().Position(0).Y();
    double z_truth = mc->GetNeutrino().Nu().Trajectory().Position(0).Z();

    double x_obj   = show->Start().X();
    double y_obj   = show->Start().Y();
    double z_obj   = show->Start().Z();

    double x_diff = x_truth - x_obj;
    double y_diff = y_truth - y_obj;
    double z_diff = z_truth - z_obj;
    
    double dist = sqrt(x_diff*x_diff+ 
		       y_diff*y_diff+
		       z_diff*z_diff);
    
    if(dist < 0.01){return true;}

    return false;
  }

  bool ContainmentFilter::isFromNeutrinoVertex(art::Ptr<simb::MCTruth> mc, art::Ptr<sim::MCTrack> trk) const
  {
    
    double x_truth = mc->GetNeutrino().Nu().Trajectory().Position(0).X();
    double y_truth = mc->GetNeutrino().Nu().Trajectory().Position(0).Y();
    double z_truth = mc->GetNeutrino().Nu().Trajectory().Position(0).Z();

    double x_obj   = trk->Start().X();
    double y_obj   = trk->Start().Y();
    double z_obj   = trk->Start().Z();

    double x_diff = x_truth - x_obj;
    double y_diff = y_truth - y_obj;
    double z_diff = z_truth - z_obj;
    
    double dist = sqrt(x_diff*x_diff+ 
		       y_diff*y_diff+
		       z_diff*z_diff);
    
    if(dist < 0.01){return true;}

    return false;
  }

  

  bool ContainmentFilter::isTrackLengthContained(double MinLength, art::Ptr<sim::MCTrack>trk) const
  {    
    
    if(TrackLength(trk) > MinLength){ return true;}
    else{return false;}
  }

  bool ContainmentFilter::inFV(double x, double y, double z) const
  {
    double distInX = x - fGeometry->DetHalfWidth();
    double distInY = y;
    double distInZ = z - 0.5 * fGeometry->DetLength();
    
    if (std::abs(distInX) < fFidEdgeX && std::abs(distInY) < fFidEdgeY && std::abs(distInZ) < fFidEdgeZ) return true;
    
    return false;
  }
  
  bool ContainmentFilter::inAV(double x, double y, double z) const
  {
    double distInX = x - fGeometry->DetHalfWidth();
    double distInY = y;
    double distInZ = z - 0.5 * fGeometry->DetLength();
    
    if (std::abs(distInX) < fActiveEdgeX && std::abs(distInY) < fActiveEdgeY && std::abs(distInZ) < fActiveEdgeZ) return true;
    
    return false;
  }
  
  
  double ContainmentFilter::TrackLength(art::Ptr<sim::MCTrack>trk) const{
    // This checks the CONTINUOUS length of a track inside the TPC,
    // if the track exits the AV then the length of the track doesn't
    // take into account any hits if the track re-enter
    
    double Length = 0.;
    
    if(trk->size() == 0) return 0.;
    
    for(int step = 1; step < int(trk->size()); step++ ){

      if(inAV(trk->at(step).X(),  trk->at(step).Y(),   trk->at(step).Z()) && 
	 inAV(trk->at(step-1).X(),trk->at(step-1).Y(), trk->at(step-1).Z())){
	
	Length += sqrt((trk->at(step).X() - trk->at(step-1).X())*(trk->at(step).X() - trk->at(step-1).X()) + 
		       (trk->at(step).Y() - trk->at(step-1).Y())*(trk->at(step).Y() - trk->at(step-1).Y()) + 
		       (trk->at(step).Z() - trk->at(step-1).Z())*(trk->at(step).Z() - trk->at(step-1).Z()));	  	
      }
      else{
	break;
      }
    }
    
    return Length;
    
  
  }

 
  double ContainmentFilter::TrackEnergy(art::Ptr<sim::MCTrack>trk) const{
    // This checks the CONTINUOUS energy deposited of a track inside the TPC,
    // if the track exits the AV then the length of the track doesn't
    // take into account any hits if the track re-enter
    
    double Energy = 0.; 

    if(trk->size() == 0) return 0.;
    
    double E_start = trk->at(0).Momentum().E();
    double E_end = 0;
       
    for(auto const step : *trk){
      if(inAV(step.X(), step.Y(), step.Z())){       
	E_end =  step.Momentum().E();
	  }
      else{
	break;
      }
    }
  
    Energy = E_start - E_end;

    return Energy;
    
  
  }



}

//--------------------------------------------------

namespace filt {

  DEFINE_ART_MODULE(ContainmentFilter)

} //namespace filt

#endif // CONTAINMENTFILTER_H
