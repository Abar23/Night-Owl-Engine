require "PremakeActions/clean"
require "PremakeActions/generate"
require "PremakeHelpers/BuildDependencies"

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

        defines
        {
            "_USE_MATH_DEFINES",
            "_CRT_SECURE_NO_WARNINGS",
            "GLFW_INCLUDE_VULKAN"
        }
        
        outputDir = "%{cfg.buildcfg}_%{cfg.system}_%{cfg.architecture}"

        group "Dependencies"
            include "NightOwlEngine/vendor/Glad"
        group ""

        group "NightOwlEngine"
            include "NightOwlEngine"
        group ""

        group "Sandbox"
            include "Sandbox"
        group ""
 end