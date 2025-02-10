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
                "%{wks.location}/common_src/Buffers/**.h",
                "%{wks.location}/common_src/Buffers/**.cpp",
                "%{wks.location}/common_src/GUI/**.h",
                "%{wks.location}/common_src/GUI/**.cpp",
                "%{wks.location}/common_src/Input/**.h",
                "%{wks.location}/common_src/Input/**.cpp",
                "%{wks.location}/common_src/Shader1/**.h",
                "%{wks.location}/common_src/Shader1/**.cpp",
                "%{wks.location}/common_src/Utility/**.h",
                "%{wks.location}/common_src/Utility/**.cpp",
                "%{wks.location}/common_src/Data/**.h",
                "%{wks.location}/common_src/Data/**.cpp"
            }

            setupCommonWindowsSettings()

