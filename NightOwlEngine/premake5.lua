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
        "SERIALIZED_SCENE_DIR=\"./assets/Scenes\"",
        "ENGINE_TEXTURE_DIRECTORY=\"../NightOwlEngine/assets/Textures\"",
        "ENGINE_SHADER_DIRECTORY=\"../NightOwlEngine/assets/Shaders\"",
        "ENGINE_MODEL_DIRECTORY=\"../NightOwlEngine/assets/Models\"",
        "ENGINE_AUDIO_DIRECTORY=\"../NightOwlEngine/assets/Audio\"",
        "ENGINE_ANIMATION_DIRECTORY=\"../NightOwlEngine/assets/Animations\""
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
        vendorDir .. "/libsndfile/include",
        vendorDir .. "/Assimp/include",
        vendorDir .. "/shaderc/include",
        vendorDir .. "/SPIRV-Cross/SPIRV-Cross-main/include",
        vendorDir .. "/shaderc/shaderc-main/libshaderc/include",
    }

    links
    {
        "GLFW",
        "Glad",
        "ImGui",
        "opengl32.lib",
        "OpenAL32.lib",
        "sndfile.lib",
        "assimp-vc143-mt.lib",
        "shaderc_combined.lib"
    }

    libdirs 
    {
        vendorDir .. "/OpenAL/libs/Win64",
        vendorDir .. "/Assimp/lib/x64"
    }

    filter "system:windows"
        systemversion "latest"

    filter "configurations:Debug"
        defines "DEBUG" 
        runtime "Debug"
        symbols "on" 

        libdirs 
        {
            vendorDir .. "/libsndfile/Debug",
            vendorDir .. "/Assimp/lib/x64",
            vendorDir .. "/SPIRV-Cross/build/Debug",
            vendorDir .. "/shaderc/build/libshaderc/Debug"
        }

        links
        {
            "spirv-cross-cored.lib",
            "spirv-cross-cppd.lib",
            "spirv-cross-glsld.lib",
            "spirv-cross-reflectd.lib",
            "spirv-cross-utild.lib",
        }

    filter "configurations:Release"  
        defines "RELEASE" 
        runtime "Release"
        optimize "on" 

        libdirs 
        {
            vendorDir .. "/libsndfile/Release",
            vendorDir .. "/shaderc/lib/Release",
            vendorDir .. "/SPIRV-Cross/build/Release",
            vendorDir .. "/shaderc/build/libshaderc/Release"
        }

        links
        {
            "spirv-cross-core.lib",
            "spirv-cross-cpp.lib",
            "spirv-cross-glsl.lib",
            "spirv-cross-reflect.lib",
            "spirv-cross-util.lib"
        }
    
