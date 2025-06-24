group "AdvancedOpenGL"

    group "AdvancedOpenGL.4.2-StencilTesting"

        project "4.2-StencilTesting-chapter"
            location "%{wks.location}/chapters/AdvancedOpenGL/4.2-StencilTesting/chapter"

            files
            {
                "%{wks.location}/common_src/**.h",
                "%{wks.location}/common_src/**.cpp"
            }

            setupCommonWindowsSettings()
            