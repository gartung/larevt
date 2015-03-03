add_library(Filters SHARED
	ChannelFilter.h	
	ChannelFilter.cxx
)

target_link_libraries(Filters
     larsoft::RecoBase
     larsoft::Geometry
     larsoft::Utilities
     ${ROOT_BASIC_LIB_LIST}
     art::art_Framework_Core
     art::art_Framework_Principal
     art::art_Persistency_Provenance
     art::art_Utilities
     art::art_Framework_Services_Registry
     FNALCore::FNALCore	
)

art_add_module(ADCFilter_module 
	ADCFilter_module.cc
)

 
art_add_module(EmptyFilter_module 
	EmptyFilter_module.cc
)

 
art_add_module(EventFilter_module 
	EventFilter_module.cc
)

art_add_module(FinalStateParticleFilter_module 
	FinalStateParticleFilter_module.cc
)

 
art_add_module(MuonFilter_module 
	MuonFilter_module.cc
)


art_add_module(SmallClusterFilter_module 
	SmallClusterFilter_module.cc
)


install(TARGETS
     Filters
     ADCFilter_module
     EmptyFilter_module
     EventFilter_module
     FinalStateParticleFilter_module
     MuonFilter_module
     SmallClusterFilter_module
     EXPORT larevtLibraries
     RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
     LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
     ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
     COMPONENT Runtime
     )

install(FILES ChannelFilter.h DESTINATION 
    ${CMAKE_INSTALL_INCLUDEDIR}/Filters COMPONENT Development)


file(GLOB FHICL_FILES 
     [^.]*.fcl
)

install(FILES ${FHICL_FILES} DESTINATION job COMPONENT Runtime)
