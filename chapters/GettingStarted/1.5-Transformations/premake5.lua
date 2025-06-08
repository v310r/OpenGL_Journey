group "GettingStarted"

    group "GettingStarted.1.5-Transformations"

        project "1.5-Transformations-chapter"
            location "%{wks.location}/chapters/GettingStarted/1.5-Transformations/chapter"

            files
            {
                "%{wks.location}/common_src/**.h",
                "%{wks.location}/common_src/**.cpp"
            }

            setupCommonWindowsSettings()


        project "1.5-Transformations-task1"
            location "%{wks.location}/chapters/GettingStarted/1.5-Transformations/task1"

            files
            {
                "%{wks.location}/common_src/**.h",
                "%{wks.location}/common_src/**.cpp"
            }

            setupCommonWindowsSettings()


        project "1.5-Transformations-task2"
            location "%{wks.location}/chapters/GettingStarted/1.5-Transformations/task2"
            
            files
            {
                "%{wks.location}/common_src/**.h",
                "%{wks.location}/common_src/**.cpp"
            }

            setupCommonWindowsSettings()

