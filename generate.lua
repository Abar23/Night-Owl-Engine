premake.modules.generate = {}
local module = premake.modules.generate

local command = io.popen("cd")
local absolutePath = command:read("*a"):gsub("\\", "/"):gsub("\n", "")
command:close()

newaction {
    trigger = "generate",
    description = "Generate dependencies",

    onStart = function()
        print("Generating dependecies")
    end,

    execute = function()
        GenerateDependencies()
    end,

    onEnd = function()
        print("Finished generating dependencies")
    end
}

local ON = "1"
local OFF = "0"

-- Define default values for generator, CMake executable, and vendor directory
local defaults = {
    generator = "Visual Studio 17 2022",
    vendorDir = absolutePath .. "/NightOwlEngine/vendor/",
    cmakeExecutable = absolutePath .. "/NightOwlEngine/vendor/cmake/Windows/bin/cmake.exe"
}

-- Define dependency-specific parameters
local dependencies = {
    {
        relativeSourceDirectory = "SPIRV-Cross/SPIRV-Cross-main",
        customOptions = {
            SPIRV_CROSS_ENABLE_CPP = ON,
            SPIRV_CROSS_ENABLE_GLSL = ON,
            SPIRV_CROSS_ENABLE_REFLECT = ON,
            SPIRV_CROSS_ENABLE_UTIL = ON,
            SPIRV_CROSS_STATIC = ON,
            SPIRV_CROSS_CLI = OFF,
            SPIRV_CROSS_ENABLE_C_API = OFF,
            SPIRV_CROSS_ENABLE_HLSL = OFF,
            SPIRV_CROSS_ENABLE_MSL = OFF,
            SPIRV_CROSS_ENABLE_TESTS = OFF,
            SPIRV_CROSS_EXCEPTIONS_TO_ASSERTIONS = OFF,
            SPIRV_CROSS_FORCE_PIC = OFF,
            SPIRV_CROSS_FORCE_STL_TYPES = OFF,
            SPIRV_CROSS_MISC_WARNINGS = OFF,
            SPIRV_CROSS_NAMESPACE_OVERRIDE = OFF,
            SPIRV_CROSS_SANITIZE_ADDRESS = OFF,
            SPIRV_CROSS_SANITIZE_MEMORY = OFF,
            SPIRV_CROSS_SANITIZE_THREADS = OFF,
            SPIRV_CROSS_SANITIZE_UNDEFINED = OFF,
            SPIRV_CROSS_SHARED = OFF,
            SPIRV_CROSS_SKIP_INSTALL = ON,
            SPIRV_CROSS_WERROR = OFF
        }
    },
    {
        relativeSourceDirectory = "shaderc/shaderc-main",
        customOptions = {
            SHADERC_ENABLE_WERROR_COMPILE = ON,
            SHADERC_SKIP_EXAMPLES = ON,
            SHADERC_SKIP_TESTS = ON,
            SHADERC_SKIP_INSTALL = ON,
            SPIRV_CHECK_CONTEXT = ON,
            SPIRV_SKIP_TESTS = ON,
            SPIRV_TOOLS_BUILD_STATIC = ON,
            SPIRV_WERROR = ON,
            SHADERC_THIRD_PARTY_ROOT_DIR = defaults.vendorDir,
            SHADERC_GLSLANG_DIR = defaults.vendorDir .. "glslang",
            SHADERC_SPIRV_HEADERS_DIR = defaults.vendorDir .. "SPIRV-Headers",
            SHADERC_SPIRV_TOOLS_DIR = defaults.vendorDir .. "SPIRV-Tools",
            SPIRV_TOOLS_INSTALL_EMACS_HELPERS = OFF,
            gtest_force_shared_crt = OFF,
            SPIRV_SKIP_EXECUTABLES = ON,
            SPIRV_LOG_DEBUG = OFF,
            SPIRV_CROSS_EXCEPTIONS_TO_ASSERTIONS = OFF,
            SPIRV_COLOR_TERMINAL= OFF,
            SPIRV_BUILD_LIBFUZZER_TARGETS = OFF,
            SPIRV_BUILD_FUZZER = OFF,
            SPIRV_BUILD_COMPRESSION = OFF,
            SPIRV_ALLOW_TIMERS = OFF,
            SHADERC_SKIP_COPYRIGHT_CHECK = ON,
            SHADERC_ENABLE_WGSL_OUTPUT = OFF,
            SHADERC_ENABLE_SHARED_CRT = OFF,
            ENABLE_RTTI = OFF,
            ENABLE_GLSLANG_JS = OFF,
            ENABLE_GLSLANG_BINARIES = OFF,
            ENABLE_EXCEPTIONS = OFF,
            ENABLE_CTEST = OFF,
            DISABLE_RTTI = OFF,
            DISABLE_EXCEPTIONS = OFF,
            BUILD_TESTING = OFF,
            BUILD_SHARED_LIBS = OFF,
            ALLOW_EXTERNAL_SPIRV_TOOLS= OFF
       }
    }
}

-- Check if the build directory exists
function GenerateDependencies()
    for _, dependency in ipairs(dependencies) do
        if not dependency.relativeSourceDirectory then
            print("Relative source directory is a required parameter.")
            return
        end

        -- Create concatenated and CMake formatted options from the list of options provided by the dependency
        local formattedOptions = ""
        for key, value in pairs(dependency.customOptions) do
            formattedOptions = formattedOptions .. "-D" .. key .. "=\"" .. value .. "\" "
        end
        
        -- Create the source and build directories
        local sourceDirectory = defaults.vendorDir .. dependency.relativeSourceDirectory
        local buildDirectory = sourceDirectory .. "/../build"

        -- Create the build directory if it doesn't exist
        local mkdirCommand = 'mkdir "' .. buildDirectory .. '"'
        os.execute(mkdirCommand)

        -- Change to the build directory
        local cdCommand = 'cd /d "' .. buildDirectory .. '" &&'

        -- Generate CMake project build files with custom options
        local cmakeCommand = '"' .. defaults.cmakeExecutable .. '" -G "' .. defaults.generator .. '" ' .. formattedOptions .. ' "' .. sourceDirectory .. '"'
        os.execute(cdCommand .. cmakeCommand)

        -- Build the project using the generated build files
        local buildCommand = '"' .. defaults.cmakeExecutable .. '" --build . --config Release'
        os.execute(cdCommand .. buildCommand)

        -- Build the project for Debug configuration
        local buildDebugCommand = '"' .. defaults.cmakeExecutable .. '" --build . --config Debug'
        os.execute(cdCommand .. buildDebugCommand)
    end
end

return module