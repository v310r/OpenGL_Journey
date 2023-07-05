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

    
    include "common_settings.lua"
    
    outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"
    

    defines { "ASSETS_PATH=\"%{wks.location}/assets\"" }

    externalproject "glfw"
        location "%{wks.location}/vendor/glfw/build/src"
        kind "StaticLib"
        language "C++"

    dofile("vendor/premake5_glm.lua")

    dofile("vendor/premake5_imgui.lua")

    dofile("vendor/GLAD/premake5.lua")

    dofile("vendor/stb_image/premake5.lua")

    -----------------------------------------------------------------------------------

    dofile("chapters/GettingStarted/1.1-HelloWindow/premake5.lua")

    dofile("chapters/GettingStarted/1.2-HelloTriangle/premake5.lua")

    dofile("chapters/GettingStarted/1.3-Shaders/premake5.lua")

    dofile("chapters/GettingStarted/1.4-Textures/premake5.lua")

    dofile("chapters/GettingStarted/1.5-Transformations/premake5.lua")



