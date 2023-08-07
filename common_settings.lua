function setupCommonWindowsSettings()
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++17"
    staticruntime "On"

    debugdir "%{prj.location}"

    targetdir ("%{prj.location}/build/bin/" .. outputdir)
    objdir ("%{prj.location}/build/intermediate/" .. outputdir)

    files
    {
        "%{prj.location}/src/**.h",
        "%{prj.location}/src/**.cpp",
        "%{prj.location}/shaders/**"
    }

    includedirs
    {
        "%{prj.location}/src",
        "%{wks.location}/common_src",
        "%{wks.location}/vendor/glfw/include",
        "%{wks.location}/vendor/GLAD/include",
        "%{wks.location}/vendor/stb_image",
        "%{wks.location}/vendor/glm/glm",
        "%{wks.location}/vendor/imgui_includes"
    }

    links
    {
        "glfw",
        "stb_image",
        "GLAD",
        "imgui"
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