project "UnitTest++"
	kind "StaticLib"

	flags {
		"ExtraWarnings",
		"FatalWarnings",
	}

	files {
		"../3rdparty/unittest-cpp/UnitTest++/*.cpp",
		"../3rdparty/unittest-cpp/UnitTest++/*.h",
	}

	configuration { "linux or macosx" }
		files {
			"../3rdparty/unittest-cpp/UnitTest++/Posix/**.cpp",
			"../3rdparty/unittest-cpp/UnitTest++/Posix/**.h",
		}

	configuration { "windows" }
		defines {
			"_CRT_SECURE_NO_WARNINGS",
		}
		files {
			"../3rdparty/unittest-cpp/UnitTest++/Win32/**.cpp",
			"../3rdparty/unittest-cpp/UnitTest++/Win32/**.h",
		}

	configuration {}
