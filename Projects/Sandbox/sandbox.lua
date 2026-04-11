project "Sandbox"
    targetname "sandbox"
    language "C"
    cdialect "C99"
    kind "ConsoleApp"

    files {
        "src/**.c",
        "src/**.h"
    }

    includedirs {
        "src/",
        "%{wks.location}/Projects/Aristotle3D/include/",
        "%{wks.location}/external/cglm/include",
        "%{wks.location}/external/microui/src"
    }

    dependson {
        "MicroUI",
        "Aristotle3D"
    }

    links {
        "Aristotle3D",
        "MicroUI"
    }

    filter "system:linux"
        linkoptions {
            "-Wl,-rpath,'$$ORIGIN'"
        }

        links {
            "m"
        }
