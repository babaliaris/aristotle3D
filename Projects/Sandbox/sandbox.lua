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
        "%{_WORKING_DIR}/Projects/Aristotle3D/src/include/"
    }

    links {
        "Aristotle3D"
    }

    filter "system:linux"
        linkoptions {
            "-Wl,-rpath,'$$ORIGIN'"
        }

    filter "system:windows"

        defines {
            "ARS3D_IMPORT_DLL"
        }
