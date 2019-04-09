cet_find_library( LIBWDA NAMES wda  )

include_directories( ${LIBWDA_INC_DIR} )

art_make( 
         LIB_LIBRARIES
         larevt_CalibrationDBI_IOVData
         ${LIBWDA}
         ${MF_MESSAGELOGGER}
         ${MF_UTILITIES}
         ${FHICLCPP}
         cetlib cetlib_except
         larcorealg_Geometry
         larcore_Geometry_Geometry_service
         ${Boost_system_LIBRARY}
         ${ART_FRAMEWORK_SERVICES_REGISTRY}
         ${ROOT_Core_LIBRARY}
         art_Utilities canvas
        )

install_headers()
install_source()
