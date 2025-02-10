group "Lighting"

    group "Lighting.2.4-LightingMaps"

        project "2.4-LightingMaps-chapter"
            location "%{wks.location}/chapters/Lighting/2.4-LightingMaps/chapter"

            files
            {
                "%{wks.location}/common_src/**.h",
                "%{wks.location}/common_src/**.cpp"
            }

            setupCommonWindowsSettings()


        project "2.4-LightingMaps-task2"
           location "%{wks.location}/chapters/Lighting/2.4-LightingMaps/task2"

            files
            {
               "%{wks.location}/common_src/**.h",
               "%{wks.location}/common_src/**.cpp"
            }

           setupCommonWindowsSettings()

        project "2.4-LightingMaps-task3"
            location "%{wks.location}/chapters/Lighting/2.4-LightingMaps/task3"

            files
            {
               "%{wks.location}/common_src/**.h",
               "%{wks.location}/common_src/**.cpp"
            }

            setupCommonWindowsSettings()

        project "2.4-LightingMaps-task4"
            location "%{wks.location}/chapters/Lighting/2.4-LightingMaps/task4"

            files
            {
               "%{wks.location}/common_src/**.h",
               "%{wks.location}/common_src/**.cpp"
            }

            setupCommonWindowsSettings()

