solution "tinystl"

	local ROOT_DIR = path.join(path.getdirectory(_SCRIPT), "..") .. "/"
	local THIRDPARTY_DIR = ROOT_DIR .. "3rdparty/"

	platforms {
		"x32",
		"x64",
	}

	configurations {
		"debug",
		"release",
	}

	language "C++"
	rtti "Off"
	editandcontinue "Off"
	warnings "Extra"
	symbols "On"

	flags {
		"StaticRuntime",
		"NoMinimalRebuild",
		"NoPCH",
		"FatalWarnings",
	}

	includedirs {
		"../include/"
	}

	location (ROOT_DIR .. ".build/projects")
	objdir (ROOT_DIR .. ".build/obj/")
	targetdir (ROOT_DIR .. "bin/")

dofile "unittest++.lua"

project "test_tinystl"
	kind "ConsoleApp"

	files {
		ROOT_DIR .. "test/**.cpp",
		ROOT_DIR .. "include/**.h",
	}

	includedirs {
		ROOT_DIR .. "include/",
		THIRDPARTY_DIR .. "unittest-cpp/UnitTest++/",
	}

	links {
		"UnitTest++"
	}

	postbuildcommands {
		ROOT_DIR .. "bin/test_tinystl"
	}

	configuration { "windows" }
		defines {
			"_SCL_SECURE_NO_WARNINGS",
			"_CRT_NONSTDC_NO_WARNINGS",
		}
