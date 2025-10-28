cmake_minimum_required (VERSION 3.8)

project ("NIDAQ_test")

add_executable (NIDAQ_test "main.cpp")

set(NIDAQMX_INCLUDE_DIR "C:/Program Files (x86)/National Instruments/Shared/ExternalCompilerSupport/C/include")
set(NIDAQMX_LIB_DIR "C:/Program Files (x86)/National Instruments/Shared/ExternalCompilerSupport/C/lib64/msvc")

find_path(NIDAQMX_INCLUDE_PATH NIDAQmx.h HINTS ${NIDAQMX_INCLUDE_DIR})
find_library(NIDAQMX_LIBRARY NIDAQmx HINTS ${NIDAQMX_LIB_DIR})

target_include_directories(NIDAQ_test PUBLIC "${NIDAQMX_INCLUDE_DIR}")
target_link_libraries(NIDAQ_test PUBLIC "${NIDAQMX_LIBRARY}")

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET NIDAQ_test PROPERTY CXX_STANDARD 20)
endif()