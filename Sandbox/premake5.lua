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
		"GLFW_INCLUDE_NONE"
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
		"%{wks.location}/NightOwlEngine/src"
	}

	links
	{
		"NightOwlEngine"
	}

	filter "system:windows"
		systemversion "latest"

        prebuildcommands 
        { 
            "copy \"..\\NightOwlEngine\\vendor\\fmod\\lib\\x64\\*.dll\" \"$(ProjectDir)$(OutDir)\"" 
        }

	filter "configurations:Debug"
		defines "DEBUG" 
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		defines "RELEASE"
		runtime "Release"
		optimize "on"
		