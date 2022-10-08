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
        
        files { 
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
            "ENGINE_LOG_FILE=\"./src/NightOwl/Core/Utitlity/Logging/Logs/EngineLogs.txt\"",
            "CLIENT_LOG_FILE=\"./src/NightOwl/Core/Utitlity/Logging/Logs/ClientLogs.txt\""
        }

        includedirs
        {
            sourceDir,
            vendorDir .. "/glad/include",
            vendorDir .. "/GLFW/include",
            vendorDir .. "/imgui/include",
            vendorDir .. "/stb/include"
        }

        links
        {
            "GLFW",
            "Glad",
            "ImGui",
            "opengl32.lib"
        }

        filter "system:windows"
            systemversion "latest"

        filter "configurations:Debug"
            defines { 
                "DEBUG" 
            }  
            
            symbols "On" 

        filter "configurations:Release"  
            defines { 
                "NDEBUG" 
            }    
            
            optimize "On" 