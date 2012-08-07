cmake_minimum_required (VERSION 2.6)
project (libsem182 C)

set (LIB_VERSION_MAJOR "2")
set (LIB_VERSION_MINOR "1")
set (LIB_VERSION_PATCH "1")

include_directories (${CMAKE_SOURCE_DIR})

install (FILES "${PROJECT_BINARY_DIR}/libsem182.a" DESTINATION lib)
install (FILES sem182.h msem182.h DESTINATION include)
install (FILES sem182.3 msem182.3 DESTINATION share/man/man3)
install (SCRIPT "${PROJECT_SOURCE_DIR}/lnmanpages.txt")

ADD_LIBRARY( sem182 STATIC
   msem182.c
   sem182.c)

# build a CPack driven installer package
# generall:
set (CPACK_PACKAGE_NAME "osue-${CMAKE_PROJECT_NAME}")
set (CPACK_PACKAGE_CONTACT 
   "OSUE TEAM <osue-team@vmars.tuwien.ac.at>")
set (CPACK_PACKAGE_DESCRIPTION_SUMMARY 
   "Semaphor library for OSUE @ TU-WIEN")
set (CPACK_PACKAGE_VERSION_MAJOR "${LIB_VERSION_MAJOR}")
set (CPACK_PACKAGE_VERSION_MINOR "${LIB_VERSION_MINOR}")
set (CPACK_PACKAGE_VERSION_PATCH "${LIB_VERSION_PATCH}")

# RPM specific
set (CPACK_RPM_PACKAGE_ARCHITECTURE "x86_64")

include (CPack)
