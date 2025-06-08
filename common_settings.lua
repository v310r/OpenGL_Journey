function setupCommonWindowsSettings()
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++17"
    staticruntime "Off"

    debugdir "%{prj.location}"

    targetdir ("%{prj.location}/build/bin/" .. outputdir)
    objdir ("%{prj.location}/build/intermediate/" .. outputdir)

    buildoptions
    {
        "/utf-8"
    }

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
        "%{wks.location}/vendor/spdlog/include",
        "%{wks.location}/vendor/stb_image",
        "%{wks.location}/vendor/glm/glm",
        "%{wks.location}/vendor/imgui",
        "%{wks.location}/vendor/imgui/backends",
        "%{wks.location}/vendor/assimp/include",

        -- kinda limited to x64, but who cares)
        "%{wks.location}/vendor/assimp/build/x64/include"
    }

    libdirs
    {
        "%{wks.location}/vendor/glfw/build/bin/" .. outputdir,
        "%{wks.location}/vendor/GLAD/build/bin/" .. outputdir,
        "%{wks.location}/vendor/stb_image/build/bin/" .. outputdir,
        "%{wks.location}/vendor/imgui/build/bin/" .. outputdir
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
            "winspool.lib",
            "shell32.lib",
            "ole32.lib",
            "oleaut32.lib",
            "uuid.lib",
            "comdlg32.lib",
            "advapi32.lib",

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

            libdirs
            {
                "%{wks.location}/vendor/assimp/build/x64/lib/Debug/",
                "%{wks.location}/vendor/assimp/build/x64/contrib/zlib/Debug"
            }

            links
            {
                "assimp-vc143-mtd",
                "zlibstaticd"
            }
        
        filter "configurations:Release"
            optimize "On"
            defines
            {
                "NDEBUG"
            }

            libdirs
            {
                "%{wks.location}/vendor/assimp/build/x64/lib/Release/",
                "%{wks.location}/vendor/assimp/build/x64/contrib/zlib/Release"
            }

            links
            {
                "assimp-vc143-mt",
                "zlibstatic"
            }
end