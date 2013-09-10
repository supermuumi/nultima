-- Nultima

projectsettings("Nultima")	
platforms{"native", "x32", "x64"}
configuration {"Debug or Release"}
kind "ConsoleApp"
--flags { "ExtraWarnings", "FatalWarnings" }

configuration { "windows" }
defines { "GLUT_BUILDING_LIB", "GLUT_NO_LIB_PRAGMA", "GLEW_STATIC" }
files { "source/*.cpp", "source/*.h" }
libdirs { "external/bass" }
links { "glut", "opengl32", "glu32", "glew", "bass"}
includedirs { "external/glut/include", "external/glew/include", "external/rapidjson", "external/stb", "external/bass" }
postbuildcommands { "copy ..\\..\\external\\bass\\bass.dll ..\\..\\bin\\windows" }

configuration { "macosx" }
files { "source/*.cpp", "source/*.h" }
libdirs { "external/bass" }
links { "GLUT.framework", "OpenGL.framework", "glew", "bass"}
includedirs { "external/glut/include", "external/glew/include", "external/rapidjson", "external/stb", "external/bass" }
