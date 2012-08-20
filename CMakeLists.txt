cmake_minimum_required (VERSION 2.6)
project (libsem182 C)

set (LIB_VERSION_MAJOR "2")
set (LIB_VERSION_MINOR "3")
set (LIB_VERSION_PATCH "1")

set (COMMON_FLAGS "-std=c99 -Wall -pedantic -D_XOPEN_SOURCE=500")
if (CMAKE_BUILD_TYPE STREQUAL "debug")
   set (DEBUG_FLAGS "-O0 -g")
else()
   set (CMAKE_BUILD_TYPE "release")
endif()
message ("-- Building target: ${CMAKE_BUILD_TYPE}")

set (CMAKE_C_FLAGS_RELEASE "${COMMON_FLAGS}")
set (CMAKE_C_FLAGS_DEBUG "${COMMON_FLAGS} ${DEBUG_FLAGS}")

include_directories (${CMAKE_SOURCE_DIR})

install (FILES "${PROJECT_BINARY_DIR}/${CMAKE_PROJECT_NAME}.a" DESTINATION lib)
install (FILES sem182.h msem182.h DESTINATION include)
install (FILES sem182.3 msem182.3 DESTINATION share/man/man3)
install (SCRIPT "${PROJECT_SOURCE_DIR}/lnmanpages.txt")

ADD_LIBRARY(sem182 STATIC
   msem182.c
   sem182.c)

# enable testing in debug mode
if (CMAKE_BUILD_TYPE STREQUAL "debug")
   ENABLE_TESTING()
   
   # sem182
   add_test(sem ${CMAKE_CURRENT_BINARY_DIR}/client_server_sem)
   set_tests_properties (sem
      PROPERTIES PASS_REGULAR_EXPRESSION "c0s1c2s3c4s5c6s7c8s9" TIMEOUT 5)
   
   add_executable(client_server_sem tests/client_server_sem.c tests/common.c)
   target_link_libraries(client_server_sem sem182)
   
   # msem182
   add_test(msem ${CMAKE_CURRENT_BINARY_DIR}/client_server_msem)
   set_tests_properties (msem
      PROPERTIES PASS_REGULAR_EXPRESSION "c0s1c2s3c4s5c6s7c8s9" TIMEOUT 5)
   
   add_executable(client_server_msem tests/client_server_msem.c tests/common.c)
   target_link_libraries(client_server_msem sem182)
   
endif() # enable testing in debug mode

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
