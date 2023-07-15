group "GettingStarted"

    group "GettingStarted.1.7-Camera"

        project "1.7-Camera-chapter"
            location "%{wks.location}/chapters/GettingStarted/1.7-Camera/chapter"
            setupCommonWindowsSettings()

        project "1.7-Camera-task1"
            location "%{wks.location}/chapters/GettingStarted/1.7-Camera/task1"
            setupCommonWindowsSettings()

        project "1.7-Camera-task2"
            location "%{wks.location}/chapters/GettingStarted/1.7-Camera/task2"
            setupCommonWindowsSettings()
