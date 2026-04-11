project("MicroUI")
  language("C")
  cdialect("C99")
  kind("StaticLib")
  pic("On")

  files({
    "src/**.c",
    "src/**.h",
    "include/**.h",
  })

  includedirs({
    "src/",
    "include/",
  })
