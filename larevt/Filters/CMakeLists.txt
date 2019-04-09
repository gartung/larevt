
art_make( LIB_LIBRARIES 
                        lardataobj_RecoBase
                        larcorealg_Geometry
                        larcore_Geometry_Geometry_service
                        lardata_Utilities
                        ${ART_FRAMEWORK_CORE}
                        ${ART_FRAMEWORK_PRINCIPAL}
                        ${ART_FRAMEWORK_SERVICES_REGISTRY}
                        ${ART_ROOT_IO_TFILE_SUPPORT}
                        ${ART_ROOT_IO_TFILESERVICE_SERVICE}
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
                        ${ROOT_Physics_LIBRARY}
                        ${Boost_system_LIBRARY}
          MODULE_LIBRARIES larevt_Filters
                           lardataobj_RawData
                           nusimdata_SimulationBase
                           ${ROOT_Core_LIBRARY}
                           ${ROOT_Hist_LIBRARY}
                           ${ROOT_RIO_LIBRARY}
                           ${ROOT_Physics_LIBRARY}
          SERVICE_LIBRARIES larevt_Filters
                            ${ART_FRAMEWORK_PRINCIPAL}
                            ${ART_FRAMEWORK_SERVICES_REGISTRY}
                            ${MF_MESSAGELOGGER}
                            ${FHICLCPP}
                            cetlib cetlib_except
                            ${ROOT_Core_LIBRARY}
                            ${ROOT_Hist_LIBRARY}
                            ${ROOT_RIO_LIBRARY}
                            ${ROOT_Physics_LIBRARY}
        )

install_headers()
install_fhicl()
install_source()
