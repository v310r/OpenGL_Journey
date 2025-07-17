group "AdvancedLighting"

    group "AdvancedLighting.5.3-Shadows-ShadowMapping"

        project "5.3-Shadows-ShadowMapping-chapter"
            location "%{wks.location}/chapters/AdvancedLighting/5.3-Shadows-ShadowMapping/chapter"

            files
            {
                "%{wks.location}/common_src/**.h",
                "%{wks.location}/common_src/**.cpp"
            }

            setupCommonWindowsSettings()
            