project "Sandbox"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++20"
	staticruntime "on"

    targetdir ("./build/bin/" .. outputDir .. "/%{prj.name}") 
    objdir ("./build/obj/" .. outputDir .. "/%{prj.name}")

	files
	{
        sourceDir .. "/**.h", 
        sourceDir .. "/**.cpp",
        sourceDir .. "/**.inl" 
	}

	defines
	{
		"GLFW_INCLUDE_NONE",
	}

	includedirs
	{
		sourceDir,
		"%{wks.location}/NightOwlEngine/vendor/glad/include",
		"%{wks.location}/NightOwlEngine/vendor/GLFW/include",
		"%{wks.location}/NightOwlEngine/vendor/imgui/include",
		"%{wks.location}/NightOwlEngine/vendor/stb/include",
		"%{wks.location}/NightOwlEngine/vendor/rapidjson/include",
		"%{wks.location}/NightOwlEngine/vendor/nlohmann/single_include",
		"%{wks.location}/NightOwlEngine/vendor/OpenAL/include",
		"%{wks.location}/NightOwlEngine/vendor/libsndfile/include",
		"%{wks.location}/NightOwlEngine/vendor/Assimp/include",
		"%{wks.location}/NightOwlEngine/vendor/SPIRV-Cross/SPIRV-Cross-main/include",
		"%{wks.location}/NightOwlEngine/vendor/shaderc/shaderc-main/libshaderc/include",
		"%{wks.location}/NightOwlEngine/src"
	}

	links
	{
		"NightOwlEngine"
	}

	filter "system:windows"
		systemversion "latest"
		
		postbuildcommands 
		{ 
			"copy \"..\\NightOwlEngine\\vendor\\OpenAL\\bin\\Win64\\*.dll\" \"$(ProjectDir)$(OutDir)\"",
			"copy \"..\\NightOwlEngine\\vendor\\Assimp\\bin\\x64\\*.dll\" \"$(ProjectDir)$(OutDir)\""		
		}

	filter "configurations:Debug"
		defines "DEBUG" 
		runtime "Debug"
		symbols "on"

		postbuildcommands 
        { 
            "copy \"..\\NightOwlEngine\\vendor\\libsndfile\\Debug\\*.dll\" \"$(ProjectDir)$(OutDir)\""
        }

	filter "configurations:Release"
		defines "RELEASE"
		runtime "Release"
		optimize "on"

		postbuildcommands 
        { 
            "copy \"..\\NightOwlEngine\\vendor\\libsndfile\\Release\\*.dll\" \"$(ProjectDir)$(OutDir)\""
        }
		