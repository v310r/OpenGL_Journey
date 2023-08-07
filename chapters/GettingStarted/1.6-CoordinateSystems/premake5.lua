group "GettingStarted"

    group "GettingStarted.1.6-CoordinateSystems"

        project "1.6-CoordinateSystems-chapter"
            location "%{wks.location}/chapters/GettingStarted/1.6-CoordinateSystems/chapter"
            
            files
            {
                "%{wks.location}/common_src/Shader1/Shader.h",
                "%{wks.location}/common_src/Shader1/Shader.cpp"
            }

            setupCommonWindowsSettings()


        project "1.6-CoordinateSystems-task1"
            location "%{wks.location}/chapters/GettingStarted/1.6-CoordinateSystems/task1"

            files
            {
                "%{wks.location}/common_src/Shader1/Shader.h",
                "%{wks.location}/common_src/Shader1/Shader.cpp"
            }

            setupCommonWindowsSettings()


        project "1.6-CoordinateSystems-task2"
            location "%{wks.location}/chapters/GettingStarted/1.6-CoordinateSystems/task2"

            files
            {
                "%{wks.location}/common_src/Shader1/Shader.h",
                "%{wks.location}/common_src/Shader1/Shader.cpp"
            }

            setupCommonWindowsSettings()


        project "1.6-CoordinateSystems-task3"
            location "%{wks.location}/chapters/GettingStarted/1.6-CoordinateSystems/task3"

            files
            {
                "%{wks.location}/common_src/Shader1/Shader.h",
                "%{wks.location}/common_src/Shader1/Shader.cpp"
            }

            setupCommonWindowsSettings()

