group "GettingStarted"

    group "GettingStarted.1.3-Shaders"

        project "1.3-Shaders-chapter"
            location "%{wks.location}/chapters/GettingStarted/1.3-Shaders/chapter"

            files
            {
                "%{wks.location}/common_src/**.h",
                "%{wks.location}/common_src/**.cpp"
            }

            setupCommonWindowsSettings()


        project "1.3-Shaders-task1"
            location "%{wks.location}/chapters/GettingStarted/1.3-Shaders/task1"

            files
            {
                "%{wks.location}/common_src/**.h",
                "%{wks.location}/common_src/**.cpp"
            }

            setupCommonWindowsSettings()


        project "1.3-Shaders-task2"
            location "%{wks.location}/chapters/GettingStarted/1.3-Shaders/task2"

            files
            {
                "%{wks.location}/common_src/**.h",
                "%{wks.location}/common_src/**.cpp"
            }

            setupCommonWindowsSettings()


        project "1.3-Shaders-task3"
            location "%{wks.location}/chapters/GettingStarted/1.3-Shaders/task3"
            
            files
            {
                "%{wks.location}/common_src/**.h",
                "%{wks.location}/common_src/**.cpp"
            }

            setupCommonWindowsSettings()

