-- Nultima

projectsettings("Nultima")	
platforms{"x32", "x64"}
configuration { "Debug or Release" }
kind "ConsoleApp"

files { "source/*.cpp", "source/*.h" }
flags { "ExtraWarnings", "FatalWarnings" }

configuration { "windows" }
defines { "GLUT_NO_LIB_PRAGMA" }
links { "glut", "opengl32", "glu32" }
includedirs { "external/glut/include" }

configuration { "macosx" }
links { "GLUT.framework", "OpenGL.framework" }
