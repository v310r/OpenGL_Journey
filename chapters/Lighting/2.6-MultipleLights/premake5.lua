group "Lighting"

    group "Lighting.2.6-MultipleLights"

        project "2.6-MultipleLights-chapter"
            location "%{wks.location}/chapters/Lighting/2.6-MultipleLights/chapter"

            files
            {
                "%{wks.location}/common_src/**.h",
                "%{wks.location}/common_src/**.cpp"
            }

            setupCommonWindowsSettings()
            