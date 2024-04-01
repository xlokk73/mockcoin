cmake_minimum_required(VERSION 3.0)
project(mockcoin)

# Add your source files here
add_executable(mockcoin_node main.cpp)

if(APPLE)
    include_directories(/opt/local/include)
    target_link_libraries(mockcoi_node /opt/local/lib/libcryptopp.a)
else()
    # Use pkg-config to find Crypto++
    find_package(PkgConfig REQUIRED)
    pkg_check_modules(CRYPTOPP REQUIRED libcrypto++)

    # Include Crypto++ headers
    include_directories(${CRYPTOPP_INCLUDE_DIRS})

    # Link against Crypto++ libraries
    target_link_libraries(mockcoin_node ${CRYPTOPP_LIBRARIES})
endif()