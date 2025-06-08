project "assimp"
    location "%{wks.location}/vendor/assimp"
    kind "StaticLib"
    language "C++"
    staticruntime "Off"

    debugdir "%{prj.location}"

    targetdir ("%{prj.location}/build/bin/" .. outputdir)
    objdir ("%{prj.location}/build/intermediate/" .. outputdir)

    files
    {
        "%{prj.location}/code/**.h",
        "%{prj.location}/code/**.cpp",
        "%{prj.location}/include.assimp/**.h",
        "%{prj.location}/include.assimp/**.inl"

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