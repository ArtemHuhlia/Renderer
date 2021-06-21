workspace "RenderingEngine"
    configurations 
    {
        "Debug",
        "Release"
    }

    architecture "x64"

outputdir = "%{prj.name}-%{cfg.buildcfg}-%{cfg.architecture}"

--include "Dependencies/ImGui"

project "RenderingEngine"

    kind "ConsoleApp"

    language "C++"
    
    targetdir ("Binaries/" .. outputdir)
    objdir ("Intermediate/" .. outputdir)

    systemversion "latest"
    
    --pchheader "PCH.h"
    --pchsource "Source/PCH.cpp"

    files 
    {
        "Source/**.h",
        "Source/**.cpp",
    }

    IncludeDirs = {}
    IncludeDirs["ImGui"] = "Dependencies/ImGui/src"

    includedirs
    {
        "Source",
        "Dependencies/SDL2/include",
        "Dependencies/Math",
        --"%{IncludeDirs.ImGui}",
        "Dependencies/stb",
        "Dependencies/tinyobjloader"
    }

    libdirs
    {
        "Dependencies/SDL2/lib/x64"
    }

    links
    {
        "SDL2test",
        "SDL2main",
        "SDL2",
        "user32",
        "d3d11.lib",
        "dxgi.lib",
        "d3dcompiler.lib",
        "dxguid.lib"
        --"ImGui",
    }

    filter {"configurations:Debug"}
        defines {"RE_DEBUG"}
        symbols "On"

    filter {"configurations:Release"}
        defines {"RE_RELEASE"}
        optimize "On"

    prebuildcommands
    {
        ("{COPY} Dependencies/SDL2/lib/x64/SDL2.dll Binaries/" .. outputdir)
    }
    