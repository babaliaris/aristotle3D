project "Aristotle3D"
    language "C"
    cdialect "C99"
    kind "SharedLib"

    files {
        "src/**.c",
        "src/**.h",
        "include/**.h"
    }

    includedirs {
        "include/",
        "%{wks.location}/Projects/Depedencies/GLAD/include",
        "%{wks.location}/external/glfw/include"
    }

    dependson {
        "GLAD",
        "GLFW"
    }

    links {
        "GLAD",
        "glfw3" -- NO GLFW (Project name), because this is the fake CMAKE wrapper
    }

    filter "system:linux"
        links {
            "GL",       -- OpenGL
            "pthread",  -- Threading support
            "dl",       -- Dynamic Linker for depedencies like GLAD/GLEW
            "m"         -- Math library
        }

    filter "system:windows"
        links {
            "opengl32", -- OpenGL
            "gdi32",    -- Device Context
            "user32",   -- Windows/Input
            "shell32"   -- GLFW or other libs depending on it
        }

        defines {
            "ARS3D_EXPORT_DLL"
        }
