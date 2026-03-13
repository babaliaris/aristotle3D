# Aristotle3D

```text
_______________
  |  ___________  |    Aristotle3D Framework v1.0
  | |           | |    [Logged in as: Dev]
  | |   (⌐■_■)  | |    Status: Logic & Vertices... OK
  | |___________| |    
  |_______________|    "Know thy Shaders."
         | |
  _______| |_______
```

## First Principles of Rendering

"The whole is something over and above its parts, and not just the sum of them all." — Aristotle

Aristotle3D is a modern C99/OpenGL 3.3 -> (configurable) framework built from the ground up for the Aristotle University of Thessaloniki Graphics (any) curriculum.

## Cloning

```bash
git clone --recursive https://github.com/babaliaris/aristotle3D.git
```

## Updating

```bash
git pull origin main
git submodule update --init --recursive
```

## Building

### First, generate the project files.
```bash
premake5 <action>
```

**Available Actions**:
```text
Action    |   Description
=========================
codelite      CodeLite projects
gmake         Generate GNU Makefiles (including Cygwin and MinGW)
ninja	        Ninja projects
vs2026	      Generate Visual Studio 2026 project files
vs2022	      Generate Visual Studio 2022 project files
vs2019	      Generate Visual Studio 2019 project files
vs2017	      Generate Visual Studio 2017 project files
vs2015	      Generate Visual Studio 2015 project files
vs2013	      Generate Visual Studio 2013 project files
vs2012	      Generate Visual Studio 2012 project files
vs2010	      Generate Visual Studio 2010 project files
vs2008	      Generate Visual Studio 2008 project files
vs2005	      Generate Visual Studio 2005 project files
xcode4	      XCode projects
```

Then compile using the appropriate method for the generated project files.
