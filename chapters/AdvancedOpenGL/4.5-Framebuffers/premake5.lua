group "AdvancedOpenGL"

    group "AdvancedOpenGL.4.5-Framebuffers"

        project "4.5-Framebuffers-chapter"
            location "%{wks.location}/chapters/AdvancedOpenGL/4.5-Framebuffers/chapter"

            files
            {
                "%{wks.location}/common_src/**.h",
                "%{wks.location}/common_src/**.cpp"
            }

            setupCommonWindowsSettings()

        project "4.5-Framebuffers-task1"
            location "%{wks.location}/chapters/AdvancedOpenGL/4.5-Framebuffers/task1"

            files
            {
                "%{wks.location}/common_src/**.h",
                "%{wks.location}/common_src/**.cpp"
            }

            setupCommonWindowsSettings()

        project "4.5-Framebuffers-task2"
            location "%{wks.location}/chapters/AdvancedOpenGL/4.5-Framebuffers/task2"

            files
            {
                "%{wks.location}/common_src/**.h",
                "%{wks.location}/common_src/**.cpp"
            }

            setupCommonWindowsSettings()
            