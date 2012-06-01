solution "tinystl"

	platforms {
		"x32",
		"x64",
	}

	configurations {
		"debug",
		"release",
	}

	language "C++"

	flags {
		"StaticRuntime",
		"NoMinimalRebuild",
		"NoPCH",
		"NoRTTI",
		"NoExceptions",
		"NoEditAndContinue",
		"ExtraWarnings",
		"FatalWarnings",
		"Symbols",
	}

	includedirs {
		"../include/"
	}

	location "../.build/projects/"
	objdir "../.build/obj/"
	targetdir "../bin/"

project "test_vector"
	kind "ConsoleApp"

	files {
		"../test/**.cpp"
	}
