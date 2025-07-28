group "AdvancedLighting"

    group "AdvancedLighting.5.6-HDR"

        project "5.6-HDR-chapter"
            location "%{wks.location}/chapters/AdvancedLighting/5.6-HDR/chapter"

            files
            {
                "%{wks.location}/common_src/**.h",
                "%{wks.location}/common_src/**.cpp"
            }

            setupCommonWindowsSettings()
            