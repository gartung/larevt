cmake_minimum_required(VERSION 2.8.12)

if(POLICY CMP0025)
	cmake_policy(SET CMP0025 OLD)
endif()

if(POLICY CMP0042)
	cmake_policy(SET CMP0042 NEW)
endif()


project(larevt)


set(larevt_VERSION "04.00.01")
set(larevt_VERSION_MAJOR 04)
set(larevt_VERSION_MINOR 00)
set(larevt_VERSION_PATCH 00)

set(larevt_SOVERSION "1.0.0")


set(larevt_DEBUG_POSTFIX "d")

set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_LIST_DIR}/Modules ${CMAKE_MODULE_PATH})

include(CMakePackageConfigHelpers)
include(GNUInstallDirs)
include(CheckCXXCompilerFlag)

set(BASE_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/BuildProducts")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${BASE_OUTPUT_DIRECTORY}/${CMAKE_INSTALL_BINDIR}")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${BASE_OUTPUT_DIRECTORY}/${CMAKE_INSTALL_LIBDIR}")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${BASE_OUTPUT_DIRECTORY}/${CMAKE_INSTALL_LIBDIR}")

find_package(nutools 1.07.00 REQUIRED)

find_package(art 1.13.01 REQUIRED)

find_package(FNALCore 0.1.0 REQUIRED)

find_package( larcore 1.0.0 )

find_package( lardata 1.0.0 )

find_package( Boost 1.53.0 
     REQUIRED
      serialization
      date_time
     )

find_package(GCCXML 0.9.0 REQUIRED)

find_package(CLHEP 2.2.0.3 REQUIRED)

find_package(SQLite3 3.8.5 REQUIRED)

find_package(ROOT 5.34.20 REQUIRED
     Core
     Cint
     Cintex
     Hist
     Matrix
     Reflex
     RIO
     Thread
     Tree
)

if(NOT ROOT_python_FOUND)
     message(FATAL_ERROR "art requires ROOT with Python support")
endif()

find_package(TBB 4.1.0 REQUIRED)

set(PQ ${PostgreSQL_LIBRARIES} )

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${FNALCore_CXX_FLAGS} -O3 -g -DNDEBUG -fno-omit-frame-pointer -pedantic -Wno-unused-local-typedefs")

include_directories(${PROJECT_SOURCE_DIR})
include_directories(${PROJECT_BINARY_DIR})
include_directories(${art_INCLUDE_DIRS})
include_directories(${FNALCore_INCLUDE_DIRS})
include_directories(${Boost_INCLUDE_DIR})
include_directories(${PostgreSQL_INCLUDE_DIRS})
include_directories(${larcore_INCLUDE_DIRS})
include_directories(${lardata_INCLUDE_DIRS})
include_directories(${ROOT_INCLUDE_DIRS})
include_directories(${CLHEP_INCLUDE_DIRS})
include_directories ( ${nutools_INCLUDE_DIRS} )


# source
add_subdirectory(CalData)
add_subdirectory(Filters)
add_subdirectory(SpaceCharge)

configure_package_config_file(
  Modules/larevtConfig.cmake.in
  ${CMAKE_CURRENT_BINARY_DIR}/larevtConfig.cmake
  INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/larevt-${lardata_VERSION}
  PATH_VARS
    CMAKE_INSTALL_INCLUDEDIR
    CMAKE_INSTALL_LIBDIR
  )

write_basic_package_version_file(
  ${CMAKE_CURRENT_BINARY_DIR}/larevtConfigVersion.cmake
  VERSION ${larevt_VERSION}
  COMPATIBILITY AnyNewerVersion
  )

install(FILES
  ${CMAKE_CURRENT_BINARY_DIR}/larevtConfig.cmake
  ${CMAKE_CURRENT_BINARY_DIR}/larevtConfigVersion.cmake
  DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/larevt-${larevt_VERSION}
  COMPONENT Development
  )

install(EXPORT larevtLibraries
  DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/larevt-${larevt_VERSION}
  NAMESPACE larsoft::
  COMPONENT Development
  )

include(ArtCPack)