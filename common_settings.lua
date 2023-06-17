function setupCommonWindowsSettings()
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++17"
    staticruntime "Off"

    debugdir "%{prj.location}"

    targetdir ("%{prj.location}/build/bin/" .. outputdir)
    objdir ("%{prj.location}/build/intermediate/" .. outputdir)

    files
    {
        "%{prj.location}/src/**.h",
        "%{prj.location}/src/**.cpp",
        "%{prj.location}/shaders/**",
        "%{wks.location}/common_src/**.c",
        "%{wks.location}/common_src/**.h",
        "%{wks.location}/common_src/**.cpp" 
    }

    includedirs
    {
        "%{prj.location}/src",
        "%{wks.location}/common_src",
        "%{wks.location}/vendor/glfw/include",
        "%{wks.location}/vendor/GLAD/include"
    }

    links
    {
        "glfw"
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
end