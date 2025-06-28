group "AdvancedOpenGL"

    group "AdvancedOpenGL.4.6-Cubemaps"

        project "4.6-Cubemaps-chapter"
            location "%{wks.location}/chapters/AdvancedOpenGL/4.6-Cubemaps/chapter"

            files
            {
                "%{wks.location}/common_src/**.h",
                "%{wks.location}/common_src/**.cpp"
            }

            setupCommonWindowsSettings()
            