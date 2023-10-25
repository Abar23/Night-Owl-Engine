premake.modules.lua = {}
local m = premake.modules.lua
local p = premake

local CMAKE_DEPENDENCY_DIRS = {
    "./NightOwlEngine/vendor/shaderc",
    "./NightOwlEngine/vendor/SPIRV-Cross"
}

newaction {
    trigger = "clean",
    description = "Clean build directories",

    onStart = function()
        print("Cleaning project")
    end,

    execute = function()
        for i = 1, #CMAKE_DEPENDENCY_DIRS do
            cleanBuildDirectory(CMAKE_DEPENDENCY_DIRS[i] .. "/build")
        end
    end,

    onEnd = function()
        print("Clean finished")
    end
}

-- Check if the build directory exists
function cleanBuildDirectory(buildDirectory)
    if os.isdir(buildDirectory) then
        -- Delete the contents of the build directory (excluding CMakeCache.txt)
        os.execute('rd /s/q "'..buildDirectory..'"')

        print(buildDirectory .. " cleaned")
    end
end

return m