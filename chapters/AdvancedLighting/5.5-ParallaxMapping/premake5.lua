group "AdvancedLighting"

    group "AdvancedLighting.5.5-ParallaxMapping"

        project "5.5-ParallaxMapping-chapter"
            location "%{wks.location}/chapters/AdvancedLighting/5.5-ParallaxMapping/chapter"

            files
            {
                "%{wks.location}/common_src/**.h",
                "%{wks.location}/common_src/**.cpp"
            }

            setupCommonWindowsSettings()
            