simple_plugin(SpaceChargeServiceStandard "service"
              larevt_SpaceCharge
              ${ART_FRAMEWORK_CORE}
              ${ART_FRAMEWORK_PRINCIPAL}
              ${ART_FRAMEWORK_SERVICES_REGISTRY}
              art_Persistency_Common
              art_Persistency_Provenance
              ${ART_FRAMEWORK_SERVICES_BASIC}
              ${ART_ROOT_IO_TFILE_SUPPORT}
              ${ART_ROOT_IO_TFILESERVICE_SERVICE}
              ${ART_FRAMEWORK_SERVICES_SYSTEM_FILECATALOGMETADATA_SERVICE}
              art_Utilities
              canvas
              ${MF_MESSAGELOGGER}
              ${ROOT_Core_LIBRARY}
              ${ROOT_RIO_LIBRARY}
              ${FHICLCPP}
              cetlib cetlib_except
)

install_headers()
install_fhicl()
install_source()
