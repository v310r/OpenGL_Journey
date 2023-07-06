group "GettingStarted"

    group "GettingStarted.1.5-Transformations"

        project "1.5-Transformations-chapter"
            location "%{wks.location}/chapters/GettingStarted/1.5-Transformations/chapter"
            setupCommonWindowsSettings()

        project "1.5-Transformations-task1"
            location "%{wks.location}/chapters/GettingStarted/1.5-Transformations/task1"
            setupCommonWindowsSettings()

        project "1.5-Transformations-task2"
            location "%{wks.location}/chapters/GettingStarted/1.5-Transformations/task2"
            setupCommonWindowsSettings()
