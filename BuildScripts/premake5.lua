workspace "RenderEngine"
    location "../"
    
    architecture "x64"

    configurations { "Debug", "Release" }
    
    filter { "configurations:Debug" }
        optimize "Off"
        symbols "On"
        defines { "RE_DEBUG "}

    filter { "configurations:Release" }
        optimize "On"
        symbols "Off"
        defines { "RE_RELEASE "}

    filter { }

    --intrumentals
    outputdir = "%{prj.name}-%{cfg.buildcfg}-%{cfg.architecture}"

    project "Engine"

        kind "ConsoleApp"

        language "C++"

        targetdir ("../Binaries/" .. outputdir)
        objdir  ("../Intermediate/" .. outputdir)

        systemversion "latest"

        files { "../Source/**" }