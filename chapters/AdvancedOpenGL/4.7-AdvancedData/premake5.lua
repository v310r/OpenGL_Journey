group "AdvancedOpenGL"

    group "AdvancedOpenGL.4.7-AdvancedData"

        project "4.7-AdvancedData-chapter"
            location "%{wks.location}/chapters/AdvancedOpenGL/4.7-AdvancedData/chapter"

            files
            {
                "%{wks.location}/common_src/**.h",
                "%{wks.location}/common_src/**.cpp"
            }

            setupCommonWindowsSettings()
            