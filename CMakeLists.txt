cmake_minimum_required(VERSION 3.27)
project(mockcoin)

set(CMAKE_CXX_STANDARD 17)

# Use pkg-config to find Crypto++
find_package(PkgConfig REQUIRED)
pkg_check_modules(CRYPTOPP REQUIRED libcrypto++)

# Include Crypto++ headers
include_directories(${CRYPTOPP_INCLUDE_DIRS})

add_executable(mockcoin main.cpp)

# Link against Crypto++ libraries
target_link_libraries(mockcoin ${CRYPTOPP_LIBRARIES})
