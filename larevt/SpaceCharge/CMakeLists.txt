art_make( NO_PLUGINS
          LIB_LIBRARIES
          lardata_Utilities
          larcorealg_Geometry
          nusimdata_SimulationBase
          ${ART_FRAMEWORK_CORE}
          ${ART_FRAMEWORK_PRINCIPAL}
          ${ART_FRAMEWORK_SERVICES_REGISTRY}
          art_Persistency_Common
          art_Persistency_Provenance
          art_Utilities
          canvas
          ${MF_MESSAGELOGGER}
          ${FHICLCPP}
          cetlib cetlib_except
          ${ROOT_Core_LIBRARY}
          ${ROOT_Hist_LIBRARY}
          ${ROOT_RIO_LIBRARY}
        )

install_headers()
install_fhicl()
install_source()
