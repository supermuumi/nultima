toolset = _ACTION or "gmake"

function projectsettings(_name)
	project(_name)
	targetname(_name)
	location("build/" .. toolset)
	objdir("intermediates/" .. _name)

	-- because of osx dylib path
	configuration { "macosx", "SharedLib" }
	targetdir("bin/Frameworks")
	
	-- TODO: get these from somewhere instead of assuming libumbraXYZ(32|64)(_d).dylib
	configuration { "macosx", "SharedLib", "Release", "x32" }
	linkoptions { "-install_name @executable_path/../Frameworks/libumbra" .. _name .."32.dylib" }
	configuration { "macosx", "SharedLib", "Release", "x64" }
	linkoptions { "-install_name @executable_path/../Frameworks/libumbra" .. _name .."64.dylib" }
	configuration { "macosx", "SharedLib", "Debug", "x32" }
	linkoptions { "-install_name @executable_path/../Frameworks/libumbra" .. _name .."32_d.dylib" }
	configuration { "macosx", "SharedLib", "Debug", "x64" }
	linkoptions { "-install_name @executable_path/../Frameworks/libumbra" .. _name .."64_d.dylib" }

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
	configuration { "x32 or x64", "macosx" }
		local sdk_path = "/Developer/SDKs/MacOSX10.6.sdk/"
		buildoptions { "-isysroot " .. sdk_path, "-mmacosx-version-min=10.6"}
		linkoptions {  "-isysroot " .. sdk_path, "-mmacosx-version-min=10.6"}
	configuration "Debug"
	defines { "DEBUG", "_DEBUG" }
	targetsuffix "_d"
	configuration { "Debug", "x32" }
	targetsuffix "32_d"
	configuration { "Debug", "x64" }
	targetsuffix "64_d"

	configuration "Release"
	flags "OptimizeSpeed"
	defines "NDEBUG"
	configuration { "Release", "x32" }
	targetsuffix "32"
	configuration { "Release", "x64" }
	targetsuffix "64"
end
