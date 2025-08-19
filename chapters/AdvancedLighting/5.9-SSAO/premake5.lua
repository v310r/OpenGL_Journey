group "AdvancedLighting"

    group "AdvancedLighting.5.9-SSAO"

        project "5.9-SSAO-chapter"
            location "%{wks.location}/chapters/AdvancedLighting/5.9-SSAO/chapter"

            files
            {
                "%{wks.location}/common_src/**.h",
                "%{wks.location}/common_src/**.cpp"
            }

            setupCommonWindowsSettings()
            