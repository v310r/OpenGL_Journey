group "AdvancedLighting"

    group "AdvancedLighting.5.1-AdvancedLighting"

        project "5.1-AdvancedLighting-chapter"
            location "%{wks.location}/chapters/AdvancedLighting/5.1-AdvancedLighting/chapter"

            files
            {
                "%{wks.location}/common_src/**.h",
                "%{wks.location}/common_src/**.cpp"
            }

            setupCommonWindowsSettings()
            