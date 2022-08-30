
cmake_minimum_required (VERSION 3.8)

project ("Checkdll_project")

# Add source to this project's executable.
add_executable (Checkdll_project "checkdll.c")

target_include_directories(Checkdll_project PUBLIC "C:\\Program Files (x86)\\BH\\SPCM\\DLL\\LIB\\MSVC64")
target_include_directories(Checkdll_project PUBLIC "C:\\Program Files (x86)\\BH\\SPCM\\DLL")



find_library(SPCM64_LIB spcm64 "C:\\Program Files (x86)\\BH\\SPCM\\DLL\\LIB\\MSVC64")
target_link_libraries(Checkdll_project PUBLIC "${SPCM64_LIB}")

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET Checkdll_project PROPERTY CXX_STANDARD 20)
endif()

# TODO: Add tests and install targets if needed.
