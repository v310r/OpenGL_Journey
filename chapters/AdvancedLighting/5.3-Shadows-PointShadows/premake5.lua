group "AdvancedLighting"

    group "AdvancedLighting.5.3-Shadows-PointShadows"

        project "5.3-Shadows-PointShadows-chapter"
            location "%{wks.location}/chapters/AdvancedLighting/5.3-Shadows-PointShadows/chapter"

            files
            {
                "%{wks.location}/common_src/**.h",
                "%{wks.location}/common_src/**.cpp"
            }

            setupCommonWindowsSettings()
            