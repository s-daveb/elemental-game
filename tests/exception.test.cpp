/* exceptions-test.cpp
 * Copyright © 2023 Saul D. Beniquez
 * License: Mozilla Public License v. 2.0
 *
 * This Source Code Form is subject to the terms of the Mozilla Public License,
 * v.2.0. If a copy of the MPL was not distributed with this file, You can
 * obtain one at https://mozilla.org/MPL/2.0/.
 */

#include "exception.hpp"

#include "./common.hpp"

#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_string.hpp>

#include <exception>
#include <iostream>
#include <optional>
#include <stdexcept>

namespace Match = Catch::Matchers;
using Catch::CaseSensitive;

void
throw_wrapped_stl_exceptiong()
{
	auto local = std::logic_error("This is a stack variable");

	throw elemental::exception(local);
}

BEGIN_TEST_SUITE("elemental::exception")
{
	auto throw_an_exception = []() {
		throw elemental::exception("An error occurred!!");
	};

	TEST("elemental::exception -  Can throw new exception type")
	{
		REQUIRE_THROWS(throw_an_exception());
	}

	TEST("elemental::exception -  Can construct exception various ways")
	{
		SECTION("1. Blank constructor")
		{
			elemental::exception obj;
		}
		SECTION("2a. With cstring parameter")
		{
			elemental::exception obj("Sample Error");
		}
		SECTION("2b. With std::string parameter")
		{
			elemental::exception obj(std::string("Sample Error"));
		}

		SECTION("3. With STL exception")
		{
			elemental::exception obj(
			    std::runtime_error("Sample Error"));
		}
		SECTION("4. With destroyed stack")
		{
			auto nested_function_call = []() {
				throw_wrapped_stl_exceptiong();
			};
			try {
				nested_function_call();
			} catch (std::exception& e) {
				REQUIRE_THAT(e.what(),
				             Match::ContainsSubstring(
						 "This is a stack variable"));
			}
		}
	}

	TEST("elemental::exception -  what() message reflects error")
	{

		SECTION("1. Unspecified error or exception")
		{
			elemental::exception obj;

			REQUIRE_THAT(obj.what(),
			             Match::ContainsSubstring(
					 elemental::exception::default_error,
					 CaseSensitive::Yes));
		}

		SECTION("2. custom error or exception")
		{
			constexpr auto test_message = "This is a test.";
			elemental::exception test_object_one(test_message);
			elemental::exception test_object_two(
			    std::logic_error("Makes no sense"));
			SECTION(" a: what() does not contain default message")
			{
				REQUIRE_THAT(
				    test_object_one.what(),
				    !Match::ContainsSubstring(
					elemental::exception::default_error));
			}
			SECTION(" b: what() displays custom message")
			{
				REQUIRE_THAT(
				    test_object_one.what(),
				    Match::ContainsSubstring(test_message));
			}
			SECTION(" c: what() contains inner exception message")
			{
				REQUIRE_THAT(
				    test_object_two.what(),
				    Match::ContainsSubstring("Makes no sense"));
			}
		}
	}

	TEST("elemental::exception - what() contains stacktrace with Catch2 "
	     "runtime methods")
	{
		elemental::exception test_object("Test");
		SECTION(" a: what() does not contain default message")
		{
			REQUIRE_THAT(
			    test_object.what(),
			    Match::ContainsSubstring("Catch::RunContext"));
			SUCCEED(test_object.what());
		}
	}
}

// clang-format off
// vim: set foldmethod=syntax foldlevel=99 textwidth=80 ts=4 sts=0 sw=4  noexpandtab ft=cpp.doxygen :
