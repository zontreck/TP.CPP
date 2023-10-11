
find_package(PkgConfig REQUIRED)

pkg_check_modules(LibOpenSSL REQUIRED openssl)

# pkg_check_modules(LIBCPPREST REQUIRED libcpprest2.10)

# Add your source files
set(SOURCES
    HTTP.cpp
    ZHash.cpp
    Tools.cpp
)

set(HEADERS
    HTTP.h
    ZHash.h
    Tools.h
)

set(INCLUDE_OUTPUT_DIR ${CMAKE_BINARY_DIR}/includes)
set(LIB_OUTPUT_DIR ${CMAKE_BINARY_DIR}/lib)

file(COPY ${HEADERS} DESTINATION ${INCLUDE_OUTPUT_DIR})

add_library(TP.CPP ${SOURCES})


set_target_properties(TP.CPP PROPERTIES ARCHIVE_OUTPUT_DIRECTORY ${LIB_OUTPUT_DIR})
export(TARGETS TP.CPP FILE TP.CPPConfig.cmake)

install (TARGETS TP.CPP DESTINATION /usr/local/lib)
install (FILES ${HEADERS} DESTINATION /usr/local/include)