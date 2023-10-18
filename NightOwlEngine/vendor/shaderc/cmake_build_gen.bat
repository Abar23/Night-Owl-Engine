@echo off

:: Specify the path to the CMake executable (replace with your actual path)
set CMAKE_EXECUTABLE=..\..\cmake\Windows\bin\cmake.exe

:: Specify the source directory of your CMake project
set SOURCE_DIR=..\shaderc-main

:: Specify the build directory (create it if it doesn't exist)
set BUILD_DIR=.\build

:: Specity generator name
set GENERATOR_NAME="Visual Studio 17 2022"

:: Set absolute path so shaderc can find third party dependencies
set ABSOLUTE_PATH=%CD%

:: Custom build options
set CUSTOM_OPTIONS= ^
    -DSHADERC_ENABLE_WERROR_COMPILE=1 ^
    -DSHADERC_SKIP_EXAMPLES=1 ^
    -DSHADERC_SKIP_TESTS=1 ^
    -DSHADERC_SKIP_INSTALL=1 ^
    -DSPIRV_CHECK_CONTEXT=1 ^
    -DSPIRV_SKIP_TESTS=1 ^
    -DSPIRV_TOOLS_BUILD_STATIC=1 ^
    -DSPIRV_WERROR=1 ^
    -DSHADERC_THIRD_PARTY_ROOT_DIR="%ABSOLUTE_PATH%\.." ^
    -DSHADERC_GLSLANG_DIR="%ABSOLUTE_PATH%\..\glslang" ^
    -DSHADERC_SPIRV_HEADERS_DIR="%ABSOLUTE_PATH%\..\SPIRV-Headers" ^
    -DSHADERC_SPIRV_TOOLS_DIR="%ABSOLUTE_PATH%\..\SPIRV-Tools" ^
    -DSPIRV_TOOLS_INSTALL_EMACS_HELPERS=0 ^
    -Dgtest_force_shared_crt=0 ^
    -DSPIRV_SKIP_TESTS=1 ^
    -DSPIRV_SKIP_EXECUTABLES=1 ^
    -DSPIRV_LOG_DEBUG=0 ^
    -DSPIRV_CROSS_EXCEPTIONS_TO_ASSERTIONS=0 ^
    -DSPIRV_COLOR_TERMINAL=0 ^
    -DSPIRV_BUILD_LIBFUZZER_TARGETS=0 ^
    -DSPIRV_BUILD_FUZZER=0 ^
    -DSPIRV_BUILD_COMPRESSION=0 ^
    -DSPIRV_ALLOW_TIMERS=0 ^
    -DSHADERC_SKIP_COPYRIGHT_CHECK=1 ^
    -DSHADERC_ENABLE_WGSL_OUTPUT=0 ^
    -DSHADERC_ENABLE_SHARED_CRT=0 ^
    -DENABLE_RTTI=0 ^
    -DENABLE_GLSLANG_JS=0 ^
    -DENABLE_GLSLANG_BINARIES=0 ^
    -DENABLE_EXCEPTIONS=0 ^
    -DENABLE_CTEST=0 ^
    -DDISABLE_RTTI=0 ^
    -DDISABLE_EXCEPTIONS=0 ^
    -DBUILD_TESTING=0 ^
    -DBUILD_SHARED_LIBS=0 ^
    -DALLOW_EXTERNAL_SPIRV_TOOLS=0

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