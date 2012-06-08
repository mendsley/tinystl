project "UnitTest++"
	kind "StaticLib"

	flags {
		"ExtraWarnings",
		"FatalWarnings",
	}

	files {
		"../3rdparty/UnitTest++/src/*.cpp",
		"../3rdparty/UnitTest++/src/*.h",
	}

	configuration { "linux" }
		files {
			"../3rdparty/UnitTest++/src/Posix/**.cpp",
			"../3rdparty/UnitTest++/src/Posix/**.h",
		}

	configuration {}
