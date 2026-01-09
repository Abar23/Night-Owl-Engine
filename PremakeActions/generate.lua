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
    vendorDir = absolutePath .. "/../NightOwlEngine/vendor/",
    cmakeExecutable = absolutePath .. "/../NightOwlEngine/vendor/cmake/Windows/bin/cmake.exe"
}

-- Define dependency-specific parameters
local dependencies = {
    {
        relativeSourceDirectory = "GLFWLatest",
        customOptions = {
            BUILD_SHARED_LIBS = OFF,
            GLFW_LIBRARY_TYPE = "STATIC",
            GLFW_BUILD_EXAMPLES = OFF,
            GLFW_BUILD_TESTS = OFF,
            GLFW_BUILD_DOCS = OFF
        }
    },
    {
        relativeSourceDirectory = "Assimp",
        customOptions = {
            ASSIMP_HUNTER_ENABLED = OFF,
            BUILD_SHARED_LIBS = OFF, 
            ASSIMP_BUILD_FRAMEWORK = OFF,
            ASSIMP_DOUBLE_PRECISION = OFF,
            ASSIMP_OPT_BUILD_PACKAGES = OFF,
            ASSIMP_ANDROID_JNIIOSYSTEM = OFF,
            ASSIMP_NO_EXPORT = OFF,
            ASSIMP_BUILD_ZLIB = ON,
            ASSIMP_BUILD_ALL_EXPORTERS_BY_DEFAULT = ON,
            ASSIMP_BUILD_ALL_IMPORTERS_BY_DEFAULT = ON,
            ASSIMP_BUILD_ASSIMP_TOOLS = OFF,
            ASSIMP_BUILD_SAMPLES = OFF,
            ASSIMP_BUILD_TESTS = ON,
            ASSIMP_COVERALLS = OFF,
            ASSIMP_INSTALL = ON,
            ASSIMP_WARNINGS_AS_ERRORS = ON,
            ASSIMP_ASAN = OFF,
            ASSIMP_UBSAN = OFF,
            ASSIMP_BUILD_DOCS = OFF,
            ASSIMP_INJECT_DEBUG_POSTFIX = ON,
            ASSIMP_IGNORE_GIT_HASH = OFF,
            ASSIMP_INSTALL_PDB = ON,
            USE_STATIC_CRT = OFF,
            ASSIMP_BUILD_DRACO = OFF,
            ASSIMP_BUILD_ASSIMP_VIEW = ON,
            ASSIMP_BUILD_USD_IMPORTER = OFF
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
        local buildDirectory = sourceDirectory .. "/build"

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