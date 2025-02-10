project "imgui"
    location "%{wks.location}/vendor/imgui"
    kind "StaticLib"
    language "C++"
    cppdialect "C++17"
    staticruntime "Off"

    targetdir ("%{prj.location}/build/bin/" .. outputdir)
    objdir ("%{prj.location}/build/intermediate/" .. outputdir)

    files
    {
        "%{prj.location}/*.h",
        "%{prj.location}/*.cpp",
        "%{prj.location}/backends/imgui_impl_glfw.cpp",
        "%{prj.location}/backends/imgui_impl_glfw.h",
        "%{prj.location}/backends/imgui_impl_opengl3.cpp",
        "%{prj.location}/backends/imgui_impl_opengl3.h",
        "%{prj.location}/backends/imgui_impl_opengl3_loader.h"
    }

    includedirs
    {
        "%{wks.location}/vendor/imgui",
        "%{wks.location}/vendor/glfw/include"
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
