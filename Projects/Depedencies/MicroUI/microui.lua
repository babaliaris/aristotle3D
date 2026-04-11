project "MicroUI"
  language "C"
  cdialect "C99"
  kind "StaticLib"
  pic "On"

  files {
    "%{wks.location}/external/microui/src/*.c",
    "%{wks.location}/external/microui/src/*.h"
  }

  includedirs {
    "%{wks.location}/external/microui/src/"
  }

