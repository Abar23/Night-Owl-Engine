project "NightOwlEngine"  
    kind "StaticLib"   
    language "C++"   
    cppdialect "C++20"
    staticruntime "on"

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
        "SERIALIZED_SCENE_DIR=\"./assets/Scenes\""
    }

    files 
    { 
        sourceDir .. "/**.h", 
        sourceDir .. "/**.cpp",
        sourceDir .. "/**.inl" 
    } 

    includedirs
    {
        sourceDir,
        vendorDir .. "/glad/include",
        vendorDir .. "/GLFW/include",
        vendorDir .. "/imgui/include",
        vendorDir .. "/stb/include",
        vendorDir .. "/rapidjson/include",
        vendorDir .. "/nlohmann/single_include",
        vendorDir .. "/OpenAL/include",
        vendorDir .. "/libsndfile/include"   
    }

    libdirs 
    {
        vendorDir .. "/OpenAL/libs/Win64"
    }

    links
    {
        "GLFW",
        "Glad",
        "ImGui",
        "opengl32.lib",
        "OpenAl32.lib"
    }

    filter "system:windows"
        systemversion "latest"

    filter "configurations:Debug"
        defines "DEBUG" 
        runtime "Debug"
        symbols "on" 

    filter "configurations:Release"  
        defines "RELEASE" 
        runtime "Release"
        optimize "on" 