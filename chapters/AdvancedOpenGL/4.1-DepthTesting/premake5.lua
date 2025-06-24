group "AdvancedOpenGL"

    group "AdvancedOpenGL.4.1-DepthTesting"

        project "4.1-DepthTesting-chapter"
            location "%{wks.location}/chapters/AdvancedOpenGL/4.1-DepthTesting/chapter"

            files
            {
                "%{wks.location}/common_src/**.h",
                "%{wks.location}/common_src/**.cpp"
            }

            setupCommonWindowsSettings()
            