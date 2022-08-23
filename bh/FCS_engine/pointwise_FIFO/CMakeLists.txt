
cmake_minimum_required (VERSION 3.8)

project ("FCS_engine")

# Add source to this project's executable.
add_executable (FCS_engine "main.cpp")

target_include_directories(FCS_engine PUBLIC "C:\\Program Files (x86)\\BH\\SPCM\\DLL\\LIB\\MSVC64")
target_include_directories(FCS_engine PUBLIC "C:\\Program Files (x86)\\BH\\SPCM\\DLL")

target_include_directories(FCS_engine PUBLIC "C:\\Program Files (x86)\\BH\\DCC\\DLL\\LIB\\MSVC64")
target_include_directories(FCS_engine PUBLIC "C:\\Program Files (x86)\\BH\\DCC\\DLL")

target_include_directories(FCS_engine PUBLIC "C:\\Program Files (x86)\\BH\\GVD\\DLL\\LIB\\MSVC64")
target_include_directories(FCS_engine PUBLIC "C:\\Program Files (x86)\\BH\\GVD\\DLL")



find_library(SPCM64_LIB spcm64 "C:\\Program Files (x86)\\BH\\SPCM\\DLL\\LIB\\MSVC64")
find_library(DCC64_LIB dcc64 "C:\\Program Files (x86)\\BH\\DCC\\DLL\\LIB\\MSVC64")
find_library(GVD64_LIB gvd64 "C:\\Program Files (x86)\\BH\\GVD\\DLL\\LIB\\MSVC64")
target_link_libraries(FCS_engine PUBLIC "${SPCM64_LIB}" "${DCC64_LIB}" "${GVD64_LIB}")

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET FCS_engine PROPERTY CXX_STANDARD 20)
endif()

# TODO: Add tests and install targets if needed.
