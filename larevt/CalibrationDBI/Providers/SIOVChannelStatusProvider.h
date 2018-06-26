/**
 * @file   SIOVChannelStatusProvider.h
 * @brief  Channel quality provider with information from configuration file
 * @author Brandon Eberly (eberly@fnal.gov)
 * @date   August 24, 2015
 * @see    ChannelStatusService.h SIOVChannelStatusProvider.cpp
 */


#ifndef SIOVCHANNELSTATUSPROVIDER_H
#define SIOVCHANNELSTATUSPROVIDER_H 1

// LArSoft libraries
#include "larevt/CalibrationDBI/Interface/ChannelStatusProvider.h"
#include "larevt/CalibrationDBI/Providers/DatabaseRetrievalAlg.h"
#include "larevt/CalibrationDBI/IOVData/ChannelStatus.h"
#include "larevt/CalibrationDBI/IOVData/Snapshot.h"
#include "larcorealg/Geometry/GeometryCore.h"

// Utility libraries
#include "fhiclcpp/ParameterSet.h"


/// Filters for channels, events, etc
namespace lariov {


  /** **************************************************************************
   * @brief Class providing information about the quality of channels
   *
   * This class serves information read from a FHiCL configuration file and/or a database.
   * 
   * LArSoft interface to this class is through the service
   * SIOVChannelStatusService.
   */
  class SIOVChannelStatusProvider: public DatabaseRetrievalAlg, public ChannelStatusProvider {
    
    public:
    
      /// Constructor
      SIOVChannelStatusProvider(fhicl::ParameterSet const& pset, const geo::GeometryCore* geo);
    
      ///Default destructor
      virtual ~SIOVChannelStatusProvider() = default;

      //
      // non-interface methods
      //
      /// Returns Channel Status
      const ChannelStatus& GetChannelStatus(raw::ChannelID_t channel) const;

      //
      // interface methods
      //

      /// @name Single channel queries
      /// @{
      /// Returns whether the specified channel is physical and connected to wire
      bool IsPresent(raw::ChannelID_t channel) const override {
        return GetChannelStatus(channel).IsPresent();
      }

      /// Returns whether the specified channel is bad in the current run
      bool IsBad(raw::ChannelID_t channel) const override {
        return GetChannelStatus(channel).IsDead() || GetChannelStatus(channel).IsLowNoise() || !IsPresent(channel);
      }

      /// Returns whether the specified channel is noisy in the current run
      bool IsNoisy(raw::ChannelID_t channel) const override {
        return GetChannelStatus(channel).IsNoisy();
      }
      
      /// Returns whether the specified channel is physical and good
      bool IsGood(raw::ChannelID_t channel) const override {
        return GetChannelStatus(channel).IsGood(); 
      }
      /// @}
      
      Status_t Status(raw::ChannelID_t channel) const override {
        return (Status_t) this->GetChannelStatus(channel).Status();
      }

      /// @name Global channel queries
      /// @{
      /// Returns a copy of set of good channel IDs for the current run
      ChannelSet_t GoodChannels() const override;

      /// Returns a copy of set of bad channel IDs for the current run
      ChannelSet_t BadChannels() const override;

      /// Returns a copy of set of noisy channel IDs for the current run
      ChannelSet_t NoisyChannels() const override;
      /// @}


      /// @name Configuration functions
      /// @{
      /// Prepares the object to provide information about the specified time
      bool Update(DBTimeStamp_t);

      /// Allows a service to add to the list of noisy channels
      void AddNoisyChannel(raw::ChannelID_t ch);

      ///@}
      
      
      /// Converts LArSoft channel ID in the one proper for the DB
      static DBChannelID_t rawToDBChannel(raw::ChannelID_t channel)
        { return DBChannelID_t(channel); }
      
    private:
    
      DataSource::ds fDataSource;
      Snapshot<ChannelStatus> fData;
      Snapshot<ChannelStatus> fNewNoisy;
      
      ChannelSet_t GetChannelsWithStatus(chStatus status) const;
    
  }; // class SIOVChannelStatusProvider
  
  
} // namespace lariov


#endif // SIOVCHANNELSTATUSPROVIDER_H
