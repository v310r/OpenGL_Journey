group "AdvancedLighting"

    group "AdvancedLighting.5.8-DeferredShading"

        project "5.8-DeferredShading-chapter"
            location "%{wks.location}/chapters/AdvancedLighting/5.8-DeferredShading/chapter"

            files
            {
                "%{wks.location}/common_src/**.h",
                "%{wks.location}/common_src/**.cpp"
            }

            setupCommonWindowsSettings()
            