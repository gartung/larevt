art_add_service( SpaceCharge_service 
     SpaceCharge_service.cc
)

install(TARGETS
     SpaceCharge_service
     EXPORT larevtLibraries
     RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
     LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
     ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
     COMPONENT Runtime
     )

install(FILES SpaceCharge.h DESTINATION 
    ${CMAKE_INSTALL_INCLUDEDIR}/SpaceCharge COMPONENT Development)


file(GLOB FHICL_FILES 
     [^.]*.fcl
)

install(FILES ${FHICL_FILES} DESTINATION job COMPONENT Runtime)
