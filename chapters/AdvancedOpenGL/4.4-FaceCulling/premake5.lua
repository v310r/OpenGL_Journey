group "AdvancedOpenGL"

    group "AdvancedOpenGL.4.4-FaceCulling"

        project "4.4-FaceCulling-chapter"
            location "%{wks.location}/chapters/AdvancedOpenGL/4.4-FaceCulling/chapter"

            files
            {
                "%{wks.location}/common_src/**.h",
                "%{wks.location}/common_src/**.cpp"
            }

            setupCommonWindowsSettings()
            