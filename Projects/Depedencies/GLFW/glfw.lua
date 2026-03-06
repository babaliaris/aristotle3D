project "GLFW"
    targetname "GLFW_TRIGGER"
    kind "StaticLib"
    language "C"
    enablepch "Off"

    files { "force_cmake_build.c" }


    local src_dir = _ARS3D_EXTERNAL_DIR .. "/glfw"

    -- files:force_cmake_build.c force premake to skip compiling and use our own custom buildcommands.
    filter { "system:linux", "files:force_cmake_build.c" }

        -- These two variables MUST be adjusted based on the specific names Cmake produces!!!
        local cmakeOut = _ARS3D_OBJECT_DIR .. "/src/libglfw3.a"
        local target   = _ARS3D_TARGET_DIR .. "/libglfw3.a"

        buildcommands {
            "{MKDIR} " .. _ARS3D_OBJECT_DIR,
            "cmake -S " .. src_dir .. " -B " .. _ARS3D_OBJECT_DIR .. " -DGLFW_BUILD_EXAMPLES=OFF -DGLFW_BUILD_TESTS=OFF -DGLFW_BUILD_DOCS=OFF",
            "cmake --build " .. _ARS3D_OBJECT_DIR .. " --config %{cfg.buildcfg}",
            "{COPY} " .. cmakeOut .. " " .. target
        }
        buildoutputs { target }
    
    -- files:force_cmake_build.c force premake to skip compiling and use our own custom buildcommands.
    filter { "system:windows", "files:force_cmake_build.c" }

        -- These two variables MUST be adjusted based on the specific names Cmake produces!!!
        local cmakeOut = _ARS3D_OBJECT_DIR .. "/src/%{cfg.buildcfg}/glfw3.lib"
        local target   = _ARS3D_TARGET_DIR .. "/glfw3.lib"

        buildcommands {
            "{MKDIR} " .. _ARS3D_OBJECT_DIR,
            'cmake -S "' .. src_dir .. '" -B "' .. _ARS3D_OBJECT_DIR .. '" -DGLFW_BUILD_EXAMPLES=OFF -DGLFW_BUILD_TESTS=OFF -DGLFW_BUILD_DOCS=OFF',
            'cmake --build "' .. _ARS3D_OBJECT_DIR .. '" --config %{cfg.buildcfg}',
            '{COPY} "' .. cmakeOut .. '" "' .. target .. '"'
        }
        buildoutputs { target }
