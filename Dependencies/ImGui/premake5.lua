project "ImGui"
    kind "StaticLib"
    language "C++"

    systemversion "8.1"

    targetdir ("Binaries/" .. outputdir)
    objdir ("Intermediate/" .. outputdir)

    files
    {
       "src/**.cpp",
       "src/**.h"
    }
