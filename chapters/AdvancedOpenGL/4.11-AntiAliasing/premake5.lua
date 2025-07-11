group "AdvancedOpenGL"

    group "AdvancedOpenGL.4.11-AntiAliasing"

        project "4.11-AntiAliasing-chapter"
            location "%{wks.location}/chapters/AdvancedOpenGL/4.11-AntiAliasing/chapter"

            files
            {
                "%{wks.location}/common_src/**.h",
                "%{wks.location}/common_src/**.cpp"
            }

            setupCommonWindowsSettings()
            