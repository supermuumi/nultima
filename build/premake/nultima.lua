-- Nultima

projectsettings("Nultima")	
platforms{"native","x32", "x64"}
configuration {"Debug or Release"}
kind "ConsoleApp"
files { "source/*.cpp", "source/*.h" }
flags { "ExtraWarnings", "FatalWarnings" }

configuration { "windows" }
defines { "GLUT_NO_LIB_PRAGMA", "GLEW_STATIC" }
links { "glut", "opengl32", "glu32", "glew" }
includedirs { "external/glut/include", "external/glew/include" }

configuration { "macosx" }
files { "source/*.cpp", "source/*.h" }
links { "GLUT.framework", "OpenGL.framework", "glew"}
includedirs { "external/glut/include", "external/glew/include" }