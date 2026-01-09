project "Sandbox"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++20"
	staticruntime "off"

    targetdir ("./build/bin/" .. outputDir .. "/%{prj.name}")
    objdir ("./build/obj/" .. outputDir .. "/%{prj.name}")

	files
	{
        "./src/**.h",
        "./src/**.cpp",
        "./src/**.inl"
	}

	defines
	{
		"GLFW_INCLUDE_NONE",
	}

	includedirs
	{
		"./src",
		"%{IncludeDirs.EngineSource}",
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
		"NightOwlEngine"
	}

	filter "system:windows"
		systemversion "latest"

		postbuildcommands 
		{
			"copy \"..\\NightOwlEngine\\vendor\\OpenAL\\bin\\Win64\\*.dll\" \"$(ProjectDir)$(OutDir)\"",
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
