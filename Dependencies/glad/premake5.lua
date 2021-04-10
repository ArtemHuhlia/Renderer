project "glad"
    kind "StaticLib"
    language "C"

    systemversion "8.1"

    targetdir ("Binaries/" .. outputdir)
    objdir ("Intermediate/" .. outputdir)

    files
    {
        "include/glad/glad.h",
        "include/KHR/khrplatform.h",
        "src/glad.c"
    }

    includedirs
    {
        "include/"
    }