/* paths.test.cpp
 * Copyright © 2024 Saul D. Beniquez
 * License: Mozilla Public License v. 2.0
 *
 * This Source Code Form is subject to the terms of the Mozilla Public License,
 * v.2.0. If a copy of the MPL was not distributed with this file, You can
 * obtain one at https://mozilla.org/MPL/2.0/.
 */

#include "types/legible_ctypes.hpp"

#include "Exception.hpp"
#include "sys/paths.hpp"
#include "sys/platform.hpp"

#include "test-utils/common.hpp"

#include <catch2/catch_test_macros.hpp>
#include <filesystem>

// Define the test suite
BEGIN_TEST_SUITE("elemental::paths")
{
	namespace fs = std::filesystem;
	using namespace elemental::paths;

	TEST("elemental::paths::GetHome() returns expected path")
	{
#if defined(__unix__) || (defined(__APPLE__) && defined(__MACH__))
		fs::path home_path = GetHome();
		REQUIRE(home_path == getenv("HOME"));
#else
		WARN("Test is skipped on non-POSIX systems.");
		REQUIRE(true);
#endif
	}

	TEST("elemental::paths::GetAppConfigRoot() returns expected path")
	{
#if (defined(__APPLE__) && defined(__MACH__))
		fs::path app_config_root = GetAppConfigRoot();
		REQUIRE(app_config_root ==
		        (GetHome() / "Library" / "Application Support"));
#elif defined(__unix__)

		fs::path appConfigRoot = GetAppConfigRoot();
		REQUIRE(appConfigRoot == (GetHome() / ".config"));
#else
		WARN("Test is skipped on non-POSIX systems.");
		REQUIRE(true);
#endif
	}

	TEST("elemental::paths::ExpandPath() expands variables and ~ ")
	{

		using namespace elemental::paths;

		elemental::c::string home_var = getenv("HOME");
		elemental::c::string user_name = getenv("USER");
		SECTION("resolves home directory")
		{

			if (home_var) {
				REQUIRE(ExpandPath("~") ==
				        fs::canonical(home_var));
				REQUIRE(ExpandPath("$HOME") ==
				        fs::canonical(home_var));
			}
		}
		SECTION("substitutes other variables")
		{
			if ((home_var && user_name) &&
			    (std::string(home_var) != "/")) {
				REQUIRE(ExpandPath("~/../$USER") ==
				        fs::canonical(home_var));
			}
		}
#if !defined(__unix__) && !(defined(__APPLE__) && defined(__MACH__))
		WARN("Test is skipped on non-POSIX systems.");
		REQUIRE(true);
#endif
	}
}

// clang-format off
// vim: set foldmethod=syntax textwidth=80 ts=8 sts=0 sw=8 foldlevel=99 noexpandtab ft=cpp.doxygen :
