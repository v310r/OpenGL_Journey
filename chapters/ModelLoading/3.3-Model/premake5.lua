group "ModelLoading"

    group "ModelLoading.3.3-Model"

        project "3.3-Model-chapter"
            location "%{wks.location}/chapters/ModelLoading/3.3-Model/chapter"

            files
            {
                "%{wks.location}/common_src/**.h",
                "%{wks.location}/common_src/**.cpp"
            }

            setupCommonWindowsSettings()
            