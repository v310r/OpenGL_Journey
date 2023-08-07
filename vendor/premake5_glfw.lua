project "glfw"
    location "%{wks.location}/vendor/glfw"
    kind "StaticLib"
    language "C"
    cdialect "C17"
    staticruntime "On"

    debugdir "%{prj.location}"

    targetdir ("%{prj.location}/build/bin/" .. outputdir)
    objdir ("%{prj.location}/build/intermediate/" .. outputdir)

    files
    {
        "%{prj.location}/src/**.h",
        "%{prj.location}/src/**.c"
    }

    includedirs
    {
        "%{wks.location}/vendor/glfw/include"
    }

    filter "system:windows"
        systemversion = "latest"

        defines
        {
            "WIN32",
            "_WINDOWS",
            "_GLFW_WIN32",
            "UNICODE",
            "_UNICODE",
            "_CRT_SECURE_NO_WARNINGS"
        }

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