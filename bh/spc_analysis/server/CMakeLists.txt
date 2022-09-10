
cmake_minimum_required (VERSION 3.8)

project ("spc_analysis")

add_executable (spc_analysis "main.cpp")

target_include_directories(spc_analysis PUBLIC "..\\include")
target_include_directories(spc_analysis PUBLIC "C:\\Users\\TCSPC\\Documents\\GitHub\\2pscopecontrol\\bh\\include")

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET spc_analysis PROPERTY CXX_STANDARD 20)
endif()