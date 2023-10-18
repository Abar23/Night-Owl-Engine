@echo on

:: Specify the path to the CMake executable (replace with your actual path)
set CMAKE_EXECUTABLE=..\..\cmake\Windows\bin\cmake.exe

:: Specify the source directory of your CMake project
set SOURCE_DIR=..\SPIRV-Cross-main

:: Specify the build directory (create it if it doesn't exist)
set BUILD_DIR=.\build

:: Specity generator name
set GENERATOR_NAME="Visual Studio 17 2022"

:: Custom build options
set CUSTOM_OPTIONS= ^
    -DSPIRV_CROSS_ENABLE_CPP=1 ^
    -DSPIRV_CROSS_ENABLE_GLSL=1 ^
    -DSPIRV_CROSS_ENABLE_REFLECT=1 ^
    -DSPIRV_CROSS_ENABLE_UTIL=1 ^
    -DSPIRV_CROSS_STATIC=1 ^
    -DSPIRV_CROSS_CLI=0 ^
    -DSPIRV_CROSS_ENABLE_C_API=0 ^
    -DSPIRV_CROSS_ENABLE_HLSL=0 ^
    -DSPIRV_CROSS_ENABLE_MSL=0 ^
    -DSPIRV_CROSS_ENABLE_TESTS=0 ^
    -DSPIRV_CROSS_EXCEPTIONS_TO_ASSERTIONS=0 ^
    -DSPIRV_CROSS_FORCE_PIC=0 ^
    -DSPIRV_CROSS_FORCE_STL_TYPES=0 ^
    -DSPIRV_CROSS_MISC_WARNINGS=0 ^
    -DSPIRV_CROSS_NAMESPACE_OVERRIDE=0 ^
    -DSPIRV_CROSS_SANITIZE_ADDRESS=0 ^
    -DSPIRV_CROSS_SANITIZE_MEMORY=0 ^
    -DSPIRV_CROSS_SANITIZE_THREADS=0 ^
    -DSPIRV_CROSS_SANITIZE_UNDEFINED=0 ^
    -DSPIRV_CROSS_SHARED=0 ^
    -DSPIRV_CROSS_SKIP_INSTALL=1 ^
    -DSPIRV_CROSS_WERROR=0

:: Create the build directory if it doesn't exist
if not exist "%BUILD_DIR%" (
    mkdir "%BUILD_DIR%"
)

:: Change to the build directory
cd /d "%BUILD_DIR%"

:: Generate CMake project build files with custom options
"%CMAKE_EXECUTABLE%" -G %GENERATOR_NAME% %CUSTOM_OPTIONS% "%SOURCE_DIR%"

:: Build the project using the generated build files
"%CMAKE_EXECUTABLE%" --build "." --config Release
"%CMAKE_EXECUTABLE%" --build "." --config Debug