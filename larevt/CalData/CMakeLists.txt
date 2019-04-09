
simple_plugin(CalWireAna "module"
                        larevt_Filters
                        lardataobj_RawData
                        lardata_Utilities_LArFFT_service
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
                        cetlib
                        cetlib_except
                        ${ROOT_Core_LIBRARY}
                        ${ROOT_MathCore_LIBRARY}
                        ${ROOT_Geom_LIBRARY}
                        ${ROOT_XMLIO_LIBRARY}
                        ${ROOT_Gdml_LIBRARY}
)

simple_plugin(CalWire "module"
                        larevt_Filters
                        lardataobj_RawData
                        lardataobj_RecoBase
                        lardata_ArtDataHelper
                        ${ART_FRAMEWORK_CORE}
                        ${ART_FRAMEWORK_SERVICES_REGISTRY}
                        ${ART_ROOT_IO_TFILE_SUPPORT}
                        ${ART_ROOT_IO_TFILESERVICE_SERVICE}
                        art_Persistency_Common
                        art_Persistency_Provenance
                        art_Utilities
                        canvas
                        ${MF_MESSAGELOGGER}  
                        ${FHICLCPP}
                        cetlib
                        cetlib_except
                        ${ROOT_Core_LIBRARY}
                        ${ROOT_MathCore_LIBRARY}
                        ${ROOT_Geom_LIBRARY}
                        ${ROOT_XMLIO_LIBRARY}
                        ${ROOT_Gdml_LIBRARY}
)

simple_plugin(CalWireT962 "module"
                        larevt_Filters
                        lardataobj_RawData
                        lardataobj_RecoBase
                        lardata_ArtDataHelper
                        larcore_Geometry_Geometry_service
                        larcorealg_Geometry
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
                        cetlib
                        cetlib_except
                        ${ROOT_Geom_LIBRARY}
                        ${ROOT_XMLIO_LIBRARY}
                        ${ROOT_Gdml_LIBRARY}
                        ${ROOT_Core_LIBRARY}
                        ${ROOT_MathCore_LIBRARY}
)


install_headers()
install_fhicl()
install_source()

add_subdirectory(test)
