group "AdvancedOpenGL"

    group "AdvancedOpenGL.4.3-Blending"

        project "4.3-Blending-chapter"
            location "%{wks.location}/chapters/AdvancedOpenGL/4.3-Blending/chapter"

            files
            {
                "%{wks.location}/common_src/**.h",
                "%{wks.location}/common_src/**.cpp"
            }

            setupCommonWindowsSettings()
            