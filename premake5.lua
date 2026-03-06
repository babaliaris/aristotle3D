workspace "Aristotle3D-Workspace"

    -- *****************|Global Configurations|****************** --

    platforms {
        "x64"
    }

    configurations {
        "debug", "profiling", "release"
    }

    objdir "obj/%{cfg.shortname}-%{prj.name}"
    targetdir "build/%{cfg.shortname}"

    warnings "Extra" -- -Wall & -Wextra (GCC/Clang) or /W4 (MSVC)

    startproject "Sandbox"

    -- ----------|All Platforms & Systems|---------- --
    -- DEBUG
    filter "configurations:debug"
        symbols "on"
        defines {
            "ARS3D_DEBUG"
        }
    
    -- PROFILING
    filter "configurations:profiling"
        symbols "on"
        defines {
            "ARS3D_PROFILING"
        }

    -- RELEASE
    filter "configurations:release"
        symbols "off"
        defines {
            "ARS3D_RELEASE"
        }

    filter {} -- Clear all filters
    -- ----------|All Platforms & Systems|---------- --

    -- ------|System Specific Configurations|------- --
    -- LINUX
    filter "system:linux"
        defines {
            "ARS3D_LINUX"
        }

    -- WINDOWS (Global Configurations)
    filter "system:windows"
        defines {
            "ARS3D_WINDOWS"
        }
        systemversion "latest" -- Latest Windows SDK
        staticruntime "off"    -- (Dynamic Link /MD) - Shared Libs

    -- WINDOWS + DEBUG
    filter { "system:windows", "configurations:debug" }
        runtime "Debug"

    -- WINDOWS + RELEASE/PROFILING
    filter { "system:windows", "configurations:release or configurations:profiling" }
        runtime "Release"

    filter {} -- Clear all filters
    -- ------|System Specific Configurations|------- --

    -- *****************|Global Configurations|****************** --


    -- ++++++++++++++++++++|Include Projects|++++++++++++++++++++ --
    group "Depedencies"
        include "Projects/Depedencies/GLAD/glad.lua"
    group ""

    include "Projects/Aristotle3D/aristotle3D.lua"
    include "Projects/Sandbox/sandbox.lua"
    -- ++++++++++++++++++++|Include Projects|++++++++++++++++++++ --
