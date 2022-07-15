1. Install drivers as described in `computer_setup.md`
2. Copy `spcm64.dll`, `dcc64.dll`, and `gvd64.dll` to the standard dll location `C:\Windows\System32\`
3. Save the C code folder contraining the source files, header files, and CMake files locally
4. Use Visual Studio 2022 to open up the code folder. If you are using a different version, you may want to start a new CMake project and just add the code files manually (or change the project properties to the correct version of VS and the Windows SDK).
5. Change `CMakeLists.txt` to point towards the location for the `spcm64`, `dcc64`, and `gvd64` `lib` files corresponding to the dlls.
