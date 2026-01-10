project "NightOwlEngine"
    kind "StaticLib"
    language "C++"
    cppdialect "C++20"
    staticruntime "off"

    targetdir ("./build/bin/" .. outputDir .. "/%{prj.name}")
    objdir ("./build/obj/" .. outputDir .. "/%{prj.name}")

    pchheader "NightOwlPch.h"
    pchsource "src/NightOwlPch.cpp"

    defines
    {
        "_USE_MATH_DEFINES",
        "_CRT_SECURE_NO_WARNINGS",
        "GLFW_INCLUDE_NONE",
        "OPEN_GL",
        "ENGINE_LOG_FILE=\"./Logs/EngineLogs.txt\"",
        "CLIENT_LOG_FILE=\"./Logs/ClientLogs.txt\"",
        "LOG_FILE_DIR=\"./Logs/\"",
        "SERIALIZED_SCENE_DIR=\"./assets/Scenes\"",
        "ENGINE_TEXTURE_DIRECTORY=\"../NightOwlEngine/assets/Textures\"",
        "ENGINE_SHADER_DIRECTORY=\"../NightOwlEngine/assets/Shaders\"",
        "ENGINE_MODEL_DIRECTORY=\"../NightOwlEngine/assets/Models\"",
        "ENGINE_AUDIO_DIRECTORY=\"../NightOwlEngine/assets/Audio\"",
        "ENGINE_ANIMATION_DIRECTORY=\"../NightOwlEngine/assets/Animations\"",
        "ENGINE_NAME=\"NightOwlEngine\""
    }

    files
    {
        "./src/**.h",
        "./src/**.cpp",
        "./src/**.inl"
    }

    includedirs
    {
        "./src",
        "%{IncludeDirs.Glad}",
        "%{IncludeDirs.GLFW}",
        "%{IncludeDirs.Stb}",
        "%{IncludeDirs.Imgui}",
        "%{IncludeDirs.Nlohmann}",
        "%{IncludeDirs.RapidJson}",
        "%{IncludeDirs.OpenAL}",
        "%{IncludeDirs.LibSndFile}",
        "%{IncludeDirs.Assimp}",
        "%{IncludeDirs.AssimpBuild}",
        "%{IncludeDirs.VMA}",
        "%{IncludeDirs.VulkanSDK}"
    }

    links
    {
        "Glad",
        "opengl32.lib",
        "%{Libraries.OpenAL}",
        "%{Libraries.Vulkan}"
    }

    filter "system:windows"
        systemversion "latest"

    filter "configurations:Debug"
        defines "DEBUG" 
        runtime "Debug"
        symbols "on"

        links
        {
            "%{Libraries.LibSndFileDebug}",
            "%{Libraries.SpirvCrossCppDebug}",
            "%{Libraries.SpirvCrossCoreDebug}",
            "%{Libraries.SpirvCrossGlslDebug}",
            "%{Libraries.SpirvCrossReflectDebug}",
            "%{Libraries.SpirvCrossUtilDebug}",
            "%{Libraries.ShaderCCombinedDebug}",
            "%{Libraries.GLFWDebug}",
            "%{Libraries.AssimpDebug}",
            "%{Libraries.ZlibDebug}"
        }

    filter "configurations:Release"
        defines "RELEASE" 
        runtime "Release"
        optimize "on"

        links
        {
            "%{Libraries.LibSndFile}",
            "%{Libraries.SpirvCrossCpp}",
            "%{Libraries.SpirvCrossCore}",
            "%{Libraries.SpirvCrossGlsl}",
            "%{Libraries.SpirvCrossReflect}",
            "%{Libraries.SpirvCrossUtil}",
            "%{Libraries.ShaderCCombined}",
            "%{Libraries.GLFW}",
            "%{Libraries.Assimp}",
            "%{Libraries.Zlib}"
        }

