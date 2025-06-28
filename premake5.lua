workspace "OpenGL_Journey"
    startproject "OpenGL_Journey"
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


    -- Fake stub, Actually it is CMake that builds everything, this premake-made project is used just for checking source code
    dofile("vendor/premake5_assimp.lua")

    dofile("vendor/premake5_glfw.lua")

    dofile("vendor/premake5_spdlog.lua")

    dofile("vendor/premake5_glm.lua")

    dofile("vendor/premake5_imgui.lua")

    dofile("vendor/GLAD/premake5.lua")

    dofile("vendor/stb_image/premake5.lua")

    -----------------------------------------------------------------------------------

    -- GettingStarted
    -----------------------------------------------------------------------------------
    dofile("chapters/GettingStarted/1.1-HelloWindow/premake5.lua")

    dofile("chapters/GettingStarted/1.2-HelloTriangle/premake5.lua")

    dofile("chapters/GettingStarted/1.3-Shaders/premake5.lua")

    dofile("chapters/GettingStarted/1.4-Textures/premake5.lua")

    dofile("chapters/GettingStarted/1.5-Transformations/premake5.lua")

    dofile("chapters/GettingStarted/1.6-CoordinateSystems/premake5.lua")

    dofile("chapters/GettingStarted/1.7-Camera/premake5.lua")
    -----------------------------------------------------------------------------------

    -- Lighting
    -----------------------------------------------------------------------------------
    dofile("chapters/Lighting/2.1-Colors/premake5.lua")

    dofile("chapters/Lighting/2.2-BasicLighting/premake5.lua")

    dofile("chapters/Lighting/2.3-Materials/premake5.lua")

    dofile("chapters/Lighting/2.4-LightingMaps/premake5.lua")

    dofile("chapters/Lighting/2.5-LightCasters/premake5.lua")

    dofile("chapters/Lighting/2.6-MultipleLights/premake5.lua")
    -----------------------------------------------------------------------------------

    -- ModelLoading
    -----------------------------------------------------------------------------------
    dofile("chapters/ModelLoading/3.1-Assimp/premake5.lua")

    dofile("chapters/ModelLoading/3.2-Mesh/premake5.lua")

    dofile("chapters/ModelLoading/3.3-Model/premake5.lua")
    -----------------------------------------------------------------------------------

    -- AdvancedOpenGL
    -----------------------------------------------------------------------------------
    dofile("chapters/AdvancedOpenGL/4.1-DepthTesting/premake5.lua")

    dofile("chapters/AdvancedOpenGL/4.2-StencilTesting/premake5.lua")

    dofile("chapters/AdvancedOpenGL/4.3-Blending/premake5.lua")

    dofile("chapters/AdvancedOpenGL/4.4-FaceCulling/premake5.lua")

    dofile("chapters/AdvancedOpenGL/4.5-Framebuffers/premake5.lua")

    dofile("chapters/AdvancedOpenGL/4.6-Cubemaps/premake5.lua")
