require("export-compile-commands")

_ARS3D_TARGET_DIR = "%{wks.location}/build/%{cfg.shortname}"
_ARS3D_OBJECT_DIR = "%{wks.location}/obj/%{cfg.shortname}-%{prj.name}"
_ARS3D_EXTERNAL_DIR = "%{wks.location}/external"

workspace("Aristotle3D-Workspace")

-- *****************|Global Configurations|****************** --

platforms({
	"x64",
})

configurations({
	"Debug",
	"Profiling",
	"Release",
})

-- Use clang
toolset("clang")

-- Set build and object directories.
targetdir(_ARS3D_TARGET_DIR)
objdir(_ARS3D_OBJECT_DIR)

-- Append this to all projects, so they can find custom build libs.
libdirs(_ARS3D_TARGET_DIR)

-- Global Compilation Options
warnings("Extra") -- -Wall & -Wextra (GCC/Clang) or /W4 (MSVC)

-- Good to have options.
startproject("Sandbox")

-- --------|All Systems & Configurations|------- --
filter("platforms:x64")
defines({
	"ARS3D_X64",
})

filter({}) -- Clear all filters
-- --------|All Systems & Configurations|------- --

-- ----------|All Platforms & Systems|---------- --
-- DEBUG
filter("configurations:Debug")
symbols("on")
defines({
	"ARS3D_DEBUG",
})

-- PROFILING
filter("configurations:Profiling")
symbols("on")
defines({
	"ARS3D_PROFILING",
})

-- RELEASE
filter("configurations:Release")
symbols("off")
defines({
	"ARS3D_RELEASE",
})

filter({}) -- Clear all filters
-- ----------|All Platforms & Systems|---------- --

-- ------|System Specific Configurations|------- --
-- LINUX
filter("system:linux")
defines({
	"ARS3D_LINUX",
})

-- WINDOWS (Global Configurations)
filter("system:windows")
defines({
	"ARS3D_WINDOWS",
})
systemversion("latest") -- Latest Windows SDK
staticruntime("off") -- (Dynamic Link /MD) - Shared Libs

-- WINDOWS + DEBUG
filter({ "system:windows", "configurations:Debug" })
runtime("Debug")

-- WINDOWS + RELEASE/PROFILING
filter({ "system:windows", "configurations:Release or configurations:Profiling" })
runtime("Release")

filter({}) -- Clear all filters
-- ------|System Specific Configurations|------- --

-- *****************|Global Configurations|****************** --

-- ++++++++++++++++++++|Include Projects|++++++++++++++++++++ --
group("Depedencies")
include("Projects/Depedencies/GLAD/glad.lua")
include("Projects/Depedencies/GLFW/glfw.lua")
group("")

include("Projects/Aristotle3D/aristotle3D.lua")
include("Projects/Sandbox/sandbox.lua")
include("Projects/UnitTests/unit-tests.lua")
-- ++++++++++++++++++++|Include Projects|++++++++++++++++++++ --
