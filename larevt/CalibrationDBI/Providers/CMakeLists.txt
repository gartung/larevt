cet_find_library( LIBWDA NAMES wda  )

include_directories( ${LIBWDA_INC_DIR} )

art_make(LIB_LIBRARIES
           larevt_CalibrationDBI_IOVData
           ${LIBWDA}
           ${MF_MESSAGELOGGER}
           ${FHICLCPP}
           canvas
           cetlib
           cetlib_except
           larcorealg_Geometry
           ${ART_FRAMEWORK_SERVICES_REGISTRY}
           ROOT::Core
           ${ART_UTILITIES}
        )

install_headers()
install_source()
