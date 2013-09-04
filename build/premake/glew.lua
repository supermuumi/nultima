projectsettings("glew")
platforms { "native", "x32", "x64" }
configuration {"Debug or Release"}
kind "StaticLib"
includedirs { "external/glew/include" }
links { "opengl32" }
defines { "GLEW_STATIC", "_CRT_SECURE_NO_WARNINGS" }

files { "external/glew/include/GL/*.h" }
files { "external/glew/src/*.c" }
excludes {
	"external/glew/src/visualinfo.c",
	"external/glew/src/glewinfo.c"
}

configuration { "macosx" }
files { "external/glew/include/GL/*.h" }
files { "external/glew/src/*.c" }
excludes {
	"external/glew/src/visualinfo.c",
	"external/glew/src/glewinfo.c"
}
links { "OpenGL.framework" }