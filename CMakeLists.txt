cmake_minimum_required(VERSION 3.12)

set(SPREAD_VERSION_MAJOR 1)
set(SPREAD_VERSION_MINOR 0)
set(SPREAD_VERSION_MICRO 0)
set(SPREAD_VERSION ${SPREAD_VERSION_MAJOR}.${SPREAD_VERSION_MINOR}.${SPREAD_VERSION_MICRO})

project(sportreader VERSION ${SPREAD_VERSION})

list(APPEND CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake)

find_package(LibXml2 REQUIRED)
find_package(PCRE REQUIRED)

include_directories(${CMAKE_SOURCE_DIR})
include_directories(${CMAKE_SOURCE_DIR}/config)
include_directories(${LIBXML2_INCLUDE_DIRS})
include_directories(${PCRE_INCLUDE_DIRS})
include_directories(${CMAKE_PREFIX_PATH}/include)
include_directories(${CMAKE_BINARY_DIR})

include(CheckSymbolExists)
include(CheckTypeSize)

check_symbol_exists(printf_s stdio.h HAVE_PRINTF_S)
check_symbol_exists(vprintf_s stdio.h HAVE_VPRINTF_S)
check_symbol_exists(sscanf_s stdio.h HAVE_SSCANF_S)
CHECK_TYPE_SIZE(time_t SIZE_OF_TIME_T)
check_symbol_exists(min stdlib.h HAVE_MIN)
check_symbol_exists(_mkgmtime time.h HAVE__MKGMTIME)
check_symbol_exists(timegm time.h HAVE_TIMEGM)
check_symbol_exists(gmtime_s time.h HAVE_GMTIME_S)
check_symbol_exists(localtime_s time.h HAVE_LOCALTIME_S)

configure_file(config.h.in config.h)

if (MSVC)
    add_definitions(/DHAVE_CONFIG_H)
    add_compile_options(/wd4267 /wd4244)
else()
    add_definitions(-DHAVE_CONFIG_H)
    add_compile_options("-Wno-incompatible-pointer-types")
    add_compile_options("-fms-extensions")
    if (${CMAKE_BUILD_TYPE} STREQUAL "Release")
        add_link_options(-s)
    endif()
endif()

if (MSVC)
    configure_file(res/sportreader.rc.in ${CMAKE_SOURCE_DIR}/res/sportreader.rc)
    set(SPORTREADER_RC res/sportreader.rc res/sportreader_resource.h)
    add_compile_definitions(/DHAVE_RESOURCE)
    include_directories(${CMAKE_SOURCE_DIR}/msvc)
    set(OPT_SOU msvc/getopt_core.h msvc/getopt_ext.h msvc/getopt_int.h msvc/getopt.h msvc/getopt.c msvc/getopt1.c)
endif()

set(UTILS_OBJS utils/i18n.h utils/i18n.c)

set(UTILS_PUBLIC_HEADERS utils/i18n.h)

add_library(libsportreader-utils STATIC ${UTILS_OBJS})

set(CONFIG_OBJS config/stdio_config.h config/stdlib_config.h)

set(LIB_OBJS ${CONFIG_OBJS} lib/public.h lib/xml.h lib/xml.c lib/iso8601.h lib/iso8601.c 
lib/util.h lib/util.c lib/time_config.h lib/time_config.c lib/activity.h lib/activity.c
lib/activityLap.h lib/activityLap.c lib/trackpoint.h lib/trackpoint.c lib/position.h 
lib/position.c lib/track.h lib/track.c lib/tcd.h lib/tcd.c lib/activityList.h
lib/activityList.c lib/numparse.h lib/numparse.c lib/mss.h lib/mss.c lib/flags.h lib/flags.c
lib/log.h lib/log.c)

set(PUBLIC_HEADERS lib/activity.h lib/activityLap.h lib/activityList.h lib/flags.h
lib/log.h lib/iso8601.h lib/mss.h lib/position.h lib/public.h lib/tcd.h lib/track.h
lib/trackpoint.h lib/xml.h)
list(TRANSFORM PUBLIC_HEADERS PREPEND ${CMAKE_SOURCE_DIR}/)

option(BUILD_SHARED_LIBS "Build shared library." ON)
option(BUILD_STATIC_LIBS "Build static library." ON)
option(PRO_LINK_STATIC "Let program link static library first." OFF)

if (NOT BUILD_SHARED_LIBS AND NOT BUILD_STATIC_LIBS)
    message(FATAL_ERROR "At least build one type of library.")
else()
    include_directories(${CMAKE_SOURCE_DIR}/lib)
endif()

if (BUILD_SHARED_LIBS)
    if(MSVC)
        set(LIB_OUTPUT_NAME libsportreader)
    else()
        set(LIB_OUTPUT_NAME sportreader)
    endif()
    add_library(libsportreader SHARED ${LIB_OBJS})
    list(APPEND ALL_TARGETS libsportreader)
    set_target_properties(libsportreader PROPERTIES OUTPUT_NAME ${LIB_OUTPUT_NAME})
    target_link_libraries(libsportreader LibXml2::LibXml2)
    target_link_libraries(libsportreader Pcre::Pcre)
    target_link_libraries(libsportreader libsportreader-utils)
endif()
if (BUILD_STATIC_LIBS)
    if (MSVC AND BUILD_SHARED_LIBS)
        set(LIB_OUTPUT_NAME libsportreader-static)
    elseif(MSVC)
        set(LIB_OUTPUT_NAME libsportreader)
    else()
        set(LIB_OUTPUT_NAME sportreader)
    endif()
    add_library(libsportreader-static STATIC ${LIB_OBJS})
    list(APPEND ALL_TARGETS libsportreader-utils)
    list(APPEND ALL_TARGETS libsportreader-static)
    set_target_properties(libsportreader-static PROPERTIES OUTPUT_NAME ${LIB_OUTPUT_NAME})
    target_link_libraries(libsportreader-static LibXml2::LibXml2)
    target_link_libraries(libsportreader-static Pcre::Pcre)
    target_link_libraries(libsportreader-static libsportreader-utils)
endif()

add_executable(sportreader ${CONFIG_OBJS} ${OPT_SOU} src/strlist.h src/strlist.c src/main.c ${SPORTREADER_RC})
list(APPEND ALL_TARGETS sportreader)

if (BUILD_SHARED_LIBS OR BUILD_STATIC_LIBS)
    if (PRO_LINK_STATIC)
        if (BUILD_STATIC_LIBS)
            set(LINK_TARGET libsportreader-static)
        else()
            set(LINK_TARGET libsportreader)
        endif()
    else()
        if (BUILD_SHARED_LIBS)
            set(LINK_TARGET libsportreader)
        else()
            set(LINK_TARGET libsportreader-static)
        endif()
    endif()
endif()

target_link_libraries(sportreader ${LINK_TARGET})
target_link_libraries(sportreader libsportreader-static)

if (NOT CMAKE_INSTALL_INCLUDEDIR)
    set(CMAKE_INSTALL_INCLUDEDIR include/libsportreader)
endif()

install(TARGETS ${ALL_TARGETS})

if (BUILD_SHARED_LIBS OR BUILD_STATIC_LIBS)
    install(FILES ${PUBLIC_HEADERS} DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
    if (BUILD_STATIC_LIBS)
        install(FILES ${UTILS_PUBLIC_HEADERS} DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
    endif()
endif()
