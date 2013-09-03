toolset = _ACTION or "gmake"

function projectsettings(_name)
	project(_name)
	targetname(_name)
	location("build/" .. toolset)
	objdir("intermediates/" .. _name)

	configuration "Debug"
	defines { "DEBUG", "_DEBUG" }
	targetsuffix "_d"

	configuration "Release"
	flags "OptimizeSpeed"
	defines "NDEBUG"

	-- because of osx dylib path
	configuration { "macosx", "SharedLib" }
	targetdir("bin/Frameworks")
	
	configuration("windows", "not StaticLib")
	targetdir("bin/windows/") 
	
	configuration("macosx", "not StaticLib")
	targetdir("bin/osx/")
	
	configuration { "windows" }
	defines { "_CRT_SECURE_NO_WARNINGS" }
end

-- Solution settings
function solutionsettings(_name)
	solution(_name)
	location "."
	configurations { "Debug", "Release" }
	language "C++"
	flags { "Symbols", "NoPCH", "NoEditAndContinue",
	        "NoMinimalRebuild", "NoRTTI", "NoManifest" }
	
	configuration { "Debug", "x32" }
	targetsuffix "32_d"
	configuration { "Debug", "x64" }
	targetsuffix "64_d"

	configuration { "Release", "x32" }
	targetsuffix "32"
	configuration { "Release", "x64" }
	targetsuffix "64"
end
