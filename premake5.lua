workspace "LearnOpenGL"
    startproject "LearnOpenGL"
    architecture "x86_64"

    configurations
    {
        "Debug",
        "Release"
    }

    flags
    {
        "MultiProcessorCompile"
    }

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

project "LearnOpenGL"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++17"
    staticruntime "Off"

    targetdir ("build/%{prj.name}/bin/" .. outputdir)
    objdir ("build/%{prj.name}/intermediate/" .. outputdir)

    files
    {
        "%{prj.location}/src/**.h",
        "%{prj.location}/src/**.cpp",
        "%{prj.location}/src/**.c"
    }

    includedirs
    {
        "%{prj.location}/src",
        "%{prj.location}/vendor/glfw/include",
        "%{prj.location}/vendor/GLAD/include"
    }

    filter "system:windows"
        systemversion = "latest"

        defines
        {
            "_CONSOLE",
        }
    
        links
        {
            "opengl32.lib",
            "kernel32.lib",
            "user32.lib",
            "gdi32.lib"
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

    externalproject "glfw"
        location "%{wks.location}/vendor/glfw/build/src"
        kind "StaticLib"
        language "C++"
