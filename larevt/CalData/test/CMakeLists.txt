cet_enable_asserts()

simple_plugin(FFTTest "module"
              lardata_Utilities_LArFFT_service
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
              ${ROOT_MathCore_LIBRARY}
              ${ROOT_FFTW_LIBRARY}
              ${ROOT_Hist_LIBRARY}
              ${ROOT_RIO_LIBRARY}
)

install_headers()
install_fhicl()
install_source()

