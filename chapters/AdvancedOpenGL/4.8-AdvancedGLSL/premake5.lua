group "AdvancedOpenGL"

    group "AdvancedOpenGL.4.8-AdvancedGLSL"

        project "4.8-AdvancedGLSL-chapter"
            location "%{wks.location}/chapters/AdvancedOpenGL/4.8-AdvancedGLSL/chapter"

            files
            {
                "%{wks.location}/common_src/**.h",
                "%{wks.location}/common_src/**.cpp"
            }

            setupCommonWindowsSettings()
            