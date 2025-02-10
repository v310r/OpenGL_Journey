group "Lighting"

    group "Lighting.2.3-Materials"

        project "2.3-Materials-chapter"
            location "%{wks.location}/chapters/Lighting/2.3-Materials/chapter"

            files
            {
                "%{wks.location}/common_src/**.h",
                "%{wks.location}/common_src/**.cpp"
            }

            setupCommonWindowsSettings()


        --project "2.2-BasicLighting-task1"
        --    location "%{wks.location}/chapters/Lighting/2.2-BasicLighting/task1"

        --   files
        --   {
        --        "%{wks.location}/common_src/**.h",
        --        "%{wks.location}/common_src/**.cpp"
        --    }

        --    setupCommonWindowsSettings()


