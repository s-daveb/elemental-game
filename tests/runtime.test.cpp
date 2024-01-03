/* runtime-tests.cpp
 * Copyright © 2023 Saul D. Beniquez
 * License: Mozilla Public License v. 2.0
 *
 * This Source Code Form is subject to the terms of the Mozilla Public License,
 * v.2.0. If a copy of the MPL was not distributed with this file, You can
 * obtain one at https://mozilla.org/MPL/2.0/.
 */

#include "Exception.hpp"

#include "test-utils/common.hpp"

#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_all.hpp>

#include <exception>
#include <iostream>
#include <optional>
#include <stdexcept>

namespace Match = Catch::Matchers;

/* A simple test suite checking that the Catch2 API works the way I think it
 * does */
BEGIN_TEST_SUITE("Catch2")
{
	TEST("Catch2 - Basic Functionality")
	{
		auto throw_something = []() {
			throw std::runtime_error("ERROR");
		};
		try {
			CHECK_THROWS(throw_something());
			CHECK(true == true);
			REQUIRE_FALSE(true == false);
			CHECK_NOFAIL(false == true);

		} catch (std::exception& e) {
			FAIL("Manual exception handler!\nCatch2 failed to catch"
			     " an exception!");
		}
	}
	TEST("Catch2 -Matchers")
	{
		REQUIRE_THAT("Test string", Match::EndsWith("string"));
		REQUIRE_THAT("LOREM IPSUM",
		             Match::ContainsSubstring("REM IPS"));
	}
	///! \todo: Test FakeIt - create a simple Mock class specification
}

// clang-format off
// vim: set foldmethod=syntax textwidth=80 ts=8 sts=0 sw=8 foldlevel=99 noexpandtab ft=cpp.doxygen :
