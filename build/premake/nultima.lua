-- Nultima

projectsettings("Nultima")	
platforms{"native", "x32", "x64"}
configuration {"Debug or Release"}
kind "ConsoleApp"


configuration { "windows" }
        defines { "GLUT_BUILDING_LIB", "GLUT_NO_LIB_PRAGMA", "GLEW_STATIC" }
        files { "source/*.cpp", "source/*.h" }
        links { "opengl32", "glu32", "bass", "SDL2" }
        libdirs { "external/bass" }
        includedirs { "external" }
        postbuildcommands { "copy ..\\..\\external\\bass\\bass.dll ..\\..\\bin\\windows" }
        flags { "ExtraWarnings", "FatalWarnings" }

configuration { "macosx" }              
        files { "source/*.cpp", "source/*.h" }
        links { "OpenGL.framework", "bass", "SDL2" }
        libdirs { "external/bass", "/usr/local/lib" }
        includedirs { "external", "/usr/local/include" }
        flags { "ExtraWarnings" }
        buildoptions { "-stdlib=libstdc++" }
        linkoptions { "-stdlib=libstdc++" }        
