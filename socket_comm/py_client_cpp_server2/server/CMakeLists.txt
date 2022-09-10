
cmake_minimum_required (VERSION 3.8)

project ("winsock_test_server")

# Add source to this project's executable.
add_executable (winsock_test_server "main.cpp")

target_link_libraries(winsock_test_server PUBLIC "${SPCM64_LIB}" "${DCC64_LIB}" "${GVD64_LIB}")

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET winsock_test_server PROPERTY CXX_STANDARD 20)
endif()

# TODO: Add tests and install targets if needed.
