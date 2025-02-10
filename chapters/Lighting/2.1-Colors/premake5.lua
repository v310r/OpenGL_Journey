group "Lighting"

    group "Lighting.2.1-Colors"

        project "2.1-Colors-chapter"
            location "%{wks.location}/chapters/Lighting/2.1-Colors/chapter"

            files
            {
                "%{wks.location}/common_src/**.h",
                "%{wks.location}/common_src/**.cpp"
            }

            setupCommonWindowsSettings()


        --project "1.7-Camera-task1"
        --    location "%{wks.location}/chapters/GettingStarted/1.7-Camera/task1"
--
        --    files
        --    {
        --        "%{wks.location}/common_src/**.h",
        --        "%{wks.location}/common_src/**.cpp"
        --    }
--
         --   setupCommonWindowsSettings()
--
--
        --project "1.7-Camera-task2"
        --    location "%{wks.location}/chapters/GettingStarted/1.7-Camera/task2"

            --setupCommonWindowsSettings()

