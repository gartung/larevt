art_add_module(CalWireAna_module 
       CalWireAna_module.cc
)

art_add_module(CalWire_module
       CalWire_module.cc
)

art_add_module(CalWireT962_module
       CalWireT962_module.cc
)

art_add_module(DumpWires_module
       DumpWires_module.cc
)

file(GLOB FHICL_FILES 
     [^.]*.fcl
)

install(FILES ${FHICL_FILES} DESTINATION job COMPONENT Runtime)


install(TARGETS
     CalWireAna_module
     CalWire_module
     CalWireT962_module
     DumpWires_module
     EXPORT larevtLibraries
     RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
     LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
     ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
     COMPONENT Runtime
     )

add_subdirectory(test)
