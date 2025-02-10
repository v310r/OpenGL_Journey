project "stb_image"
    location "%{wks.location}/vendor/stb_image"
    kind "StaticLib"
    language "C++"
    cppdialect "C++17"
    staticruntime "On"

    targetdir ("%{prj.location}/build/bin/" .. outputdir)
    objdir ("%{prj.location}/build/intermediate/" .. outputdir)

    files
    {
        "%{prj.location}/stb_image.h",
        "%{prj.location}/stb_image.cpp"
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