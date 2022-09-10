
cmake_minimum_required (VERSION 3.8)

project ("spc_analysis")

# Add source to this project's executable.
add_executable (spc_analysis "main.cpp")

target_include_directories(spc_analysis PUBLIC "C:\\Users\\TCSPC\\Documents\\GitHub\\2pscopecontrol\\bh\\include")

target_link_libraries(spc_analysis PUBLIC "${SPCM64_LIB}" "${DCC64_LIB}" "${GVD64_LIB}")

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET spc_analysis PROPERTY CXX_STANDARD 20)
endif()

# TODO: Add tests and install targets if needed.