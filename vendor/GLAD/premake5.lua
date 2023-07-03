project "GLAD"
    location "%{wks.location}/vendor/GLAD"
    kind "StaticLib"
    language "C"
    cdialect "C17"
    staticruntime "Off"

    targetdir ("%{prj.location}/build/bin/" .. outputdir)
    objdir ("%{prj.location}/build/intermediate/" .. outputdir)

    files
    {
        "%{prj.location}/include/glad/glad.h",
        "%{prj.location}/include/KHR/khrplatform.h",
        "%{prj.location}/glad.c"
    }

    includedirs
    {
        "%{prj.location}/include"
    }

    filter "system:windows"
        systemversion = "latest"

        filter "configurations:Debug"
            symbols "On"
            defines
            {
                "_DEBUG"
            }
        
        filter "configurations:Release"
            optimize "On"
            defines
            {
                "NDEBUG"
            }