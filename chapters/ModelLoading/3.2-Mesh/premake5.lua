group "ModelLoading"

    group "ModelLoading.3.2-Mesh"

        project "3.2-Mesh-chapter"
            location "%{wks.location}/chapters/ModelLoading/3.2-Mesh/chapter"

            files
            {
                "%{wks.location}/common_src/**.h",
                "%{wks.location}/common_src/**.cpp"
            }

            setupCommonWindowsSettings()
            