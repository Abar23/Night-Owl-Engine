workspace "Night-Owl-Engine"  
    architecture "x86_64"
    startproject "Night-Owl-Engine"
    
    configurations { 
        "Debug", 
        "Release"
    } 

    flags {
        "MultiProcessorCompile"
    }

    outputDir = "%{cfg.buildcfg}_%{cfg.system}_%{cfg.architecture}"
    sourceDir = "./src"
    vendorDir = "./vendor"

    group "Dependencies"
        include "./vendor/Glad"
        include "./vendor/GLFW"
        include "./vendor/ImGui"
    group ""

    project "Night-Owl-Engine"  
        kind "ConsoleApp"   
        language "C++"   
        cppdialect "C++20"
        staticruntime "on"

        targetdir ("./build/bin/" .. outputDir .. "/%{prj.name}") 
        objdir ("./build/obj/" .. outputDir .. "/%{prj.name}")
        
        pchheader "NightOwlPch.h"
        pchsource "src/NightOwlPch.cpp"

        files 
        { 
            sourceDir .. "/**.h", 
            sourceDir .. "/**.cpp",
            sourceDir .. "/**.inl" 
        } 

        defines
        {
            "_USE_MATH_DEFINES",
            "_CRT_SECURE_NO_WARNINGS",
            "GLFW_INCLUDE_NONE",
            "STB_IMAGE_IMPLEMENTATION",
            "OPEN_GL",
            "ENGINE_LOG_FILE=\"./Logs/EngineLogs.txt\"",
            "CLIENT_LOG_FILE=\"./Logs/ClientLogs.txt\"",
            "LOG_FILE_DIR=\"./Logs/\"",
            "SERIALIZED_SCENE_DIR=\"./assets/Scenes\""
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
            vendorDir .. "/OpenAl/include",
            vendorDir .. "/libsndfile/include"   
        }

        links
        {
            "GLFW",
            "Glad",
            "ImGui",
            "opengl32.lib",
            vendorDir .. "/OpenAl/libs/Win64/OpenAl32.lib"
        }

        filter "system:windows"
            systemversion "latest"

            prebuildcommands 
            { 
                "copy \"$(ProjectDir)vendor\\OpenAl\\router\\64\\*.dll\" \"$(ProjectDir)$(OutDir)\"",
                "copy \"$(ProjectDir)vendor\\OpenAl\\bin\\64\\*.dll\" \"$(ProjectDir)$(OutDir)\""
            }

        filter "configurations:Debug"
            defines "DEBUG" 
            runtime "Debug"
            symbols "on" 

            links
            {
                vendorDir .. "/libsndfile/Debug/sndfile.lib"
            }

            prebuildcommands 
            { 
                "copy \"$(ProjectDir)vendor\\libsndfile\\Debug\\*.dll\" \"$(ProjectDir)$(OutDir)\""
            }
    
        filter "configurations:Release"  
            defines "RELEASE" 
            runtime "Release"
            optimize "on" 
    
            links
            {
                vendorDir .. "/libsndfile/Release/sndfile.lib"
            }

            prebuildcommands 
            { 
                "copy \"$(ProjectDir)vendor\\libsndfile\\Release\\*.dll\" \"$(ProjectDir)$(OutDir)\""
            }
    