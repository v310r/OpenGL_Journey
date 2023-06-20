group "GettingStarted"

    group "GettingStarted.1.2-HelloTriangle"

        project "1.2-HelloTriangle-chapter"
            location "%{wks.location}/chapters/GettingStarted/1.2-HelloTriangle/chapter"
            setupCommonWindowsSettings()

        project "1.2-HelloTriangle-task1"
            location "%{wks.location}/chapters/GettingStarted/1.2-HelloTriangle/task1"
            setupCommonWindowsSettings()

        project "1.2-HelloTriangle-task2"
            location "%{wks.location}/chapters/GettingStarted/1.2-HelloTriangle/task2"
            setupCommonWindowsSettings()

        project "1.2-HelloTriangle-task3"
            location "%{wks.location}/chapters/GettingStarted/1.2-HelloTriangle/task3"
            setupCommonWindowsSettings()
