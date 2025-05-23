require "PremakeActions/clean"
require "PremakeActions/generate"

if _ACTION == "vs2022" then
    workspace "NightOwlEngine"  
        architecture "x86_64"
        startproject "Sandbox"
        
        configurations 
        { 
            "Debug", 
            "Release"
        } 

        flags  
        {
            "MultiProcessorCompile"
        }

        sourceDir = "./src"
        vendorDir = "./vendor"

        defines
        {
            "_USE_MATH_DEFINES",
            "_CRT_SECURE_NO_WARNINGS",
        }

        outputDir = "%{cfg.buildcfg}_%{cfg.system}_%{cfg.architecture}"

        group "Dependencies"
            include "NightOwlEngine/vendor/Glad"
            include "NightOwlEngine/vendor/GLFW"
            include "NightOwlEngine/vendor/ImGui"
        group ""

        group "NightOwlEngine"
            include "NightOwlEngine"
        group ""

        group "Sandbox"
            include "Sandbox"
        group ""
 end