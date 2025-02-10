group "Lighting"

    group "Lighting.2.5-LightCasters"

        project "2.5-LightCasters-chapter"
            location "%{wks.location}/chapters/Lighting/2.5-LightCasters/chapter"

            files
            {
                "%{wks.location}/common_src/**.h",
                "%{wks.location}/common_src/**.cpp"
            }

            setupCommonWindowsSettings()
            