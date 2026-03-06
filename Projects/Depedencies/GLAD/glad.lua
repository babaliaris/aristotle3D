project "GLAD"
    language "C"
    cdialect "C99"
    kind "StaticLib"

    files {
        "src/**.c",
        "src/**.h",
        "include/**.h"
    }

    includedirs {
        "src/",
        "include/"
    }
