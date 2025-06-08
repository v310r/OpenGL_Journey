group "GettingStarted"

    group "GettingStarted.1.7-Camera"

        project "1.7-Camera-chapter"
            location "%{wks.location}/chapters/GettingStarted/1.7-Camera/chapter"

            files
            {
                "%{wks.location}/common_src/**.h",
                "%{wks.location}/common_src/**.cpp"
            }

            setupCommonWindowsSettings()


        project "1.7-Camera-task1"
            location "%{wks.location}/chapters/GettingStarted/1.7-Camera/task1"

            files
            {
                "%{wks.location}/common_src/**.h",
                "%{wks.location}/common_src/**.cpp"
            }

            setupCommonWindowsSettings()


        project "1.7-Camera-task2"
            location "%{wks.location}/chapters/GettingStarted/1.7-Camera/task2"

            files
            {
                "%{prj.location}/src/Camera/**.h",
                "%{prj.location}/src/Camera/**.cpp",
                "%{wks.location}/common_src/**.h",
                "%{wks.location}/common_src/**.cpp"
            }

            setupCommonWindowsSettings()

