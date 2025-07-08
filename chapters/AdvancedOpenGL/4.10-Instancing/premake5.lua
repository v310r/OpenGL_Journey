group "AdvancedOpenGL"

    group "AdvancedOpenGL.4.10-Instancing"

        project "4.10-Instancing-chapter"
            location "%{wks.location}/chapters/AdvancedOpenGL/4.10-Instancing/chapter"

            files
            {
                "%{wks.location}/common_src/**.h",
                "%{wks.location}/common_src/**.cpp"
            }

            setupCommonWindowsSettings()
            