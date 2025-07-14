group "AdvancedLighting"

    group "AdvancedLighting.5.2-GammaCorrection"

        project "5.2-GammaCorrection-chapter"
            location "%{wks.location}/chapters/AdvancedLighting/5.2-GammaCorrection/chapter"

            files
            {
                "%{wks.location}/common_src/**.h",
                "%{wks.location}/common_src/**.cpp"
            }

            setupCommonWindowsSettings()
            