group "AdvancedOpenGL"

    group "AdvancedOpenGL.4.9-GeometryShader"

        project "4.9-GeometryShader-chapter"
            location "%{wks.location}/chapters/AdvancedOpenGL/4.9-GeometryShader/chapter"

            files
            {
                "%{wks.location}/common_src/**.h",
                "%{wks.location}/common_src/**.cpp"
            }

            setupCommonWindowsSettings()
            