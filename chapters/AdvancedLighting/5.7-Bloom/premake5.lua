group "AdvancedLighting"

    group "AdvancedLighting.5.7-Bloom"

        project "5.7-Bloom-chapter"
            location "%{wks.location}/chapters/AdvancedLighting/5.7-Bloom/chapter"

            files
            {
                "%{wks.location}/common_src/**.h",
                "%{wks.location}/common_src/**.cpp"
            }

            setupCommonWindowsSettings()
            