group "ModelLoading"

    group "ModelLoading.3.1-Assimp"

        project "3.1-Assimp-chapter"
            location "%{wks.location}/chapters/ModelLoading/3.1-Assimp/chapter"

            files
            {
                "%{wks.location}/common_src/**.h",
                "%{wks.location}/common_src/**.cpp"
            }

            setupCommonWindowsSettings()
            