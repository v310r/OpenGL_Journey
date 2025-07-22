group "AdvancedLighting"

    group "AdvancedLighting.5.4-NormalMapping"

        project "5.4-NormalMapping-chapter"
            location "%{wks.location}/chapters/AdvancedLighting/5.4-NormalMapping/chapter"

            files
            {
                "%{wks.location}/common_src/**.h",
                "%{wks.location}/common_src/**.cpp"
            }

            setupCommonWindowsSettings()
            